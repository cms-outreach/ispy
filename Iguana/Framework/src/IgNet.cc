#include "Iguana/Framework/interface/IgNet.h"
#include "Iguana/Framework/interface/IgNetError.h"
#include "classlib/sysapi/InetSocket.h" // for completing InetAddress
#include "classlib/iobase/Filename.h"
#include "classlib/utils/TimeInfo.h"
#include "classlib/utils/StringList.h"
#include "classlib/utils/StringFormat.h"
#include "classlib/utils/StringOps.h"
#include "classlib/utils/SystemError.h"
#include "classlib/utils/Regexp.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <cassert>

#define MESSAGE_SIZE_LIMIT (8*1024*1024)
#define SOCKET_BUF_SIZE (8*1024*1024)
#define SOCKET_READ_SIZE (SOCKET_BUF_SIZE/8)
#define SOCKET_READ_GROWTH (SOCKET_BUF_SIZE)

using namespace lat;

//////////////////////////////////////////////////////////////////////
// Generate log prefix.
std::ostream &
IgNet::logme(void)
{
  return std::cerr
    << Time::current().format(true, "%Y-%m-%d %H:%M:%S")
    << " " << appname_ << "[" << pid_ << "]: ";
}

// Append data into a bucket.
void
IgNet::copydata(Bucket *b, const void *data, size_t len)
{
  b->data.insert(b->data.end(),
                 (const unsigned char *)data,
                 (const unsigned char *)data + len);
}

// Discard a bucket chain.
void
IgNet::discard(Bucket *&b)
{
  while (b)
  {
    Bucket *next = b->next;
    delete b;
    b = next;
  }
}

//////////////////////////////////////////////////////////////////////
/** Handle errors with a peer socket.  Zaps the socket send queue,
    the socket itself, detaches the socket from the selector, and
    purges any pending wait requests linked to the socket.  */
bool
IgNet::losePeer(const char *reason,
                Peer *peer,
                IOSelectEvent *ev,
                Error *err)
{
  if (reason)
    logme()
      << reason << peer->peeraddr
      << (err ? "; error was: " + err->explain() : std::string(""))
      << std::endl;

  Socket *s = peer->socket;

  for (WaitList::iterator i = waiting_.begin(), e = waiting_.end(); i != e; )
    if (i->peer == peer)
      waiting_.erase(i++);
    else
      ++i;

  if (ev)
    ev->source = 0;

  discard(peer->sendq);
  if (peer->automatic)
    peer->automatic->peer = 0;

  sel_.detach(s);
  s->close();
  removePeer(peer, s);
  delete s;
  return true;
}

/// Queue an object request to the data server.
void
IgNet::requestObject(Peer *p, const char *name, size_t len)
{
  Bucket **msg = &p->sendq;
  while (*msg)
    msg = &(*msg)->next;
  *msg = new Bucket;
  (*msg)->next = 0;

  uint32_t words[3];
  words[0] = sizeof(words) + len;
  words[1] = VIS_MSG_GET_OBJECT;
  words[2] = len;
  (*msg)->data.reserve((*msg)->data.size() + words[0]);
  copydata(*msg, words, sizeof(words));
  copydata(*msg, name, len);
}

/// Queue a request for an object and put a peer into the mode of
/// waiting for object data to appear.
void
IgNet::waitForData(Peer *p, const std::string &name, const std::string &info, Peer *owner)
{
  // FIXME: Should we automatically record which exact peer the waiter
  // is expecting to deliver data so we know to release the waiter if
  // the other peer vanishes?  The current implementation stands a
  // chance for the waiter to wait indefinitely -- although we do
  // force terminate the wait after a while.
  requestObject(owner, name.size() ? &name[0] : 0, name.size());
  WaitObject wo = { Time::current(), name, info, p };
  waiting_.push_back(wo);
  p->waiting++;
}

// Once an object has been updated, this is invoked for all waiting
// peers.  Send the object back to the peer in suitable form.
void
IgNet::releaseFromWait(WaitList::iterator i, Object *o)
{
  Bucket **msg = &i->peer->sendq;
  while (*msg)
    msg = &(*msg)->next;
  *msg = new Bucket;
  (*msg)->next = 0;

  releaseFromWait(*msg, *i, o);

  assert(i->peer->waiting > 0);
  i->peer->waiting--;
  waiting_.erase(i);
}

// Release everyone waiting for the object @a o.
void
IgNet::releaseWaiters(Object *o)
{
  for (WaitList::iterator i = waiting_.begin(), e = waiting_.end(); i != e; )
    if (i->name == o->name)
      releaseFromWait(i++, o);
    else
      ++i;
}

//////////////////////////////////////////////////////////////////////
// Check if the network layer should stop.
bool
IgNet::shouldStop(void)
{
  return shutdown_;
}

// Once an object has been updated, this is invoked for all waiting
// peers.  Send the requested object to the waiting peer.
void
IgNet::releaseFromWait(Bucket *msg, WaitObject &w, Object *o)
{
  if (o)
    sendObjectToPeer(msg, *o, true);
  else
  {
    uint32_t words[3];
    words[0] = sizeof(words) + w.name.size();
    words[1] = VIS_REPLY_NONE;
    words[2] = w.name.size();

    msg->data.reserve(msg->data.size() + words[0]);
    copydata(msg, &words[0], sizeof(words));
    copydata(msg, &w.name[0], w.name.size());
  }
}

// Send an object to a peer.  If not @a data, only sends a summary
// without the object data, except the data is always sent for scalar
// objects.
void
IgNet::sendObjectToPeer(Bucket *msg, Object &o, bool data)
{
  uint32_t flags = o.flags & ~VIS_FLAG_ZOMBIE;
  DataBlob objdata;

  if (data ||(flags & VIS_FLAG_SCALAR))
    objdata.insert(objdata.end(),
                   &o.rawdata[0],
                   &o.rawdata[0] + o.rawdata.size());

  uint32_t words[7];
  uint32_t namelen = o.name.size();
  uint32_t datalen = objdata.size();

  words[0] = sizeof(words) + namelen + datalen;
  words[1] = VIS_REPLY_OBJECT;
  words[2] = flags;
  words[3] = (o.version >> 0 ) & 0xffffffff;
  words[4] = (o.version >> 32) & 0xffffffff;
  words[5] = namelen;
  words[6] = datalen;

  msg->data.reserve(msg->data.size() + words[0]);
  copydata(msg, &words[0], sizeof(words));
  if (namelen)
    copydata(msg, &o.name[0], namelen);
  if (datalen)
    copydata(msg, &objdata[0], datalen);
}

//////////////////////////////////////////////////////////////////////
// Handle peer messages.
bool
IgNet::onMessage(Bucket *msg, Peer *p, unsigned char *data, size_t len)
{
  // Decode and process this message.
  uint32_t type;
  memcpy(&type, data + sizeof(uint32_t), sizeof(type));
  switch (type)
  {
  case VIS_MSG_UPDATE_ME:
    {
      if (len != 2*sizeof(uint32_t))
      {
        logme()
          << "ERROR: corrupt 'UPDATE_ME' message of length " << len
          << " from peer " << p->peeraddr << std::endl;
        return false;
      }

      if (debug_)
        logme()
          << "DEBUG: received message 'UPDATE ME' from peer "
          << p->peeraddr << std::endl;

      p->update = true;
    }
    return true;

  case VIS_MSG_LIST_OBJECTS:
    {
      if (debug_)
        logme()
          << "DEBUG: received message 'LIST OBJECTS' from peer "
          << p->peeraddr << std::endl;

      // Send over current status: list of known objects.
      lock();
      sendObjectListToPeer(msg, true, false);
      unlock();
    }
    return true;

  case VIS_MSG_GET_OBJECT:
    {
      if (debug_)
        logme()
          << "DEBUG: received message 'GET OBJECT' from peer "
          << p->peeraddr << std::endl;

      if (len < 3*sizeof(uint32_t))
      {
        logme()
          << "ERROR: corrupt 'GET IMAGE' message of length " << len
          << " from peer " << p->peeraddr << std::endl;
        return false;
      }

      uint32_t namelen;
      memcpy(&namelen, data + 2*sizeof(uint32_t), sizeof(namelen));
      if (len != 3*sizeof(uint32_t) + namelen)
      {
        logme()
          << "ERROR: corrupt 'GET OBJECT' message of length " << len
          << " from peer " << p->peeraddr
          << ", expected length " << (3*sizeof(uint32_t))
          << " + " << namelen << std::endl;
        return false;
      }

      lock();
      std::string name((char *) data + 3*sizeof(uint32_t), namelen);
      Peer *owner = 0;
      Object *o = findObject(0, name, &owner);
      if (o)
      {
        o->lastreq = Time::current();
        if (o->rawdata.empty())
          waitForData(p, name, "", owner);
        else
          sendObjectToPeer(msg, *o, true);
      }
      else
      {
        uint32_t words[3];
        words[0] = sizeof(words) + name.size();
        words[1] = VIS_REPLY_NONE;
        words[2] = name.size();

        msg->data.reserve(msg->data.size() + words[0]);
        copydata(msg, &words[0], sizeof(words));
        copydata(msg, &name[0], name.size());
      }
      unlock();
    }
    return true;

  case VIS_REPLY_LIST_BEGIN:
    {
      if (len != 4*sizeof(uint32_t))
      {
        logme()
          << "ERROR: corrupt 'LIST BEGIN' message of length " << len
          << " from peer " << p->peeraddr << std::endl;
        return false;
      }

      if (debug_)
        logme()
          << "DEBUG: received message 'LIST BEGIN' from "
          << p->peeraddr << std::endl;

      // Get the update status: whether this is a full update.
      uint32_t flags;
      memcpy(&flags, data + 3*sizeof(uint32_t), sizeof(uint32_t));

      // If we are about to receive a full list of objects, flag all
      // objects dead.  Subsequent object notifications will undo this
      // for the live objects.  This tells us the object has been
      // removed, but we can keep making it available for a while if
      // there continues to be interest in it.
      if (flags)
      {
        lock();
        markObjectsZombies(p);
        unlock();
      }
    }
    return true;

  case VIS_REPLY_LIST_END:
    {
      if (len != 4*sizeof(uint32_t))
      {
        logme()
          << "ERROR: corrupt 'LIST END' message of length " << len
          << " from peer " << p->peeraddr << std::endl;
        return false;
      }

      // Get the update status: whether this is a full update.
      uint32_t flags;
      memcpy(&flags, data + 3*sizeof(uint32_t), sizeof(uint32_t));

      // If we received a full list of objects, flag all zombie objects
      // now dead. We need to do this in two stages in case we receive
      // updates in many parts, and end up sending updates to others in
      // between; this avoids us lying live objects are dead.
      if (flags)
      {
        lock();
        markObjectsDead(p);
        unlock();
      }

      if (debug_)
        logme()
          << "DEBUG: received message 'LIST END' from "
          << p->peeraddr << std::endl;

      // Indicate we have received another update from this peer.
      // Also indicate we should flush to our clients.
      flush_ = true;
      p->updates++;
    }
    return true;

  case VIS_REPLY_OBJECT:
    {
      uint32_t words[7];
      if (len < sizeof(words))
      {
        logme()
          << "ERROR: corrupt 'OBJECT' message of length " << len
          << " from peer " << p->peeraddr << std::endl;
        return false;
      }

      memcpy(&words[0], data, sizeof(words));
      uint32_t &namelen = words[5];
      uint32_t &datalen = words[6];

      if (len != sizeof(words) + namelen + datalen)
      {
        logme()
          << "ERROR: corrupt 'OBJECT' message of length " << len
          << " from peer " << p->peeraddr
          << ", expected length " << sizeof(words)
          << " + " << namelen
          << " + " << datalen
          << std::endl;
        return false;
      }

      unsigned char *namedata = data + sizeof(words);
      unsigned char *objdata = namedata + namelen;
      unsigned char *enddata = objdata + datalen;
      std::string name((char *) namedata, namelen);
      assert(enddata == data + len);

      if (debug_)
        logme()
          << "DEBUG: received message 'OBJECT " << name
          << "' from " << p->peeraddr << std::endl;

      // Mark the peer as a known object source.
      p->source = true;

      // Initialise or update an object entry.
      lock();
      Object *o = findObject(p, name);
      if (! o)
        o = makeObject(p, name);

      bool hadobject = ! o->rawdata.empty();
      o->flags = words[2] | VIS_FLAG_RECEIVED;
      o->version = ((uint64_t) words[4] << 32 | words[3]);
      o->rawdata.clear();
      o->rawdata.insert(o->rawdata.end(), objdata, enddata);

      // If we had an object for this one already and this is a list
      // update without data, issue an immediate data get request.
      if (hadobject && ! datalen)
        requestObject(p,(namelen ? &name[0] : 0), namelen);

      // If we have the object data, release from wait.
      if (datalen)
        releaseWaiters(o);
      unlock();
    }
    return true;

  case VIS_REPLY_NONE:
    {
      uint32_t words[3];
      if (len < sizeof(words))
      {
        logme()
          << "ERROR: corrupt 'NONE' message of length " << len
          << " from peer " << p->peeraddr << std::endl;
        return false;
      }

      memcpy(&words[0], data, sizeof(words));
      uint32_t &namelen = words[2];

      if (len != sizeof(words) + namelen)
      {
        logme()
          << "ERROR: corrupt 'NONE' message of length " << len
          << " from peer " << p->peeraddr
          << ", expected length " << sizeof(words)
          << " + " << namelen << std::endl;
        return false;
      }

      unsigned char *namedata = data + sizeof(words);
      unsigned char *enddata = namedata + namelen;
      std::string name((char *) namedata, namelen);
      assert(enddata == data + len);

      if (debug_)
        logme()
          << "DEBUG: received message 'NONE " << name
          << "' from " << p->peeraddr << std::endl;

      // Mark the peer as a known object source.
      p->source = true;

      // If this was a known object, update its entry.
      lock();
      Object *o = findObject(p, name);
      if (o)
        o->flags |= VIS_FLAG_DEAD;

      // If someone was waiting for this, let them go.
      releaseWaiters(o);
      unlock();
    }
    return true;

  default:
    logme()
      << "ERROR: unrecognised message of length " << len
      << " and type " << type << " from peer " << p->peeraddr
      << std::endl;
    return false;
  }
}

//////////////////////////////////////////////////////////////////////
/// Handle communication to a particular client.
bool
IgNet::onPeerData(IOSelectEvent *ev, Peer *p)
{
  assert(getPeer(dynamic_cast<Socket *>(ev->source)) == p);

  // If there is a problem with the peer socket, discard the peer
  // and tell the selector to stop prcessing events for it.  If
  // this is a server connection, we will eventually recreate
  // everything if/when the data server comes back.
  if (ev->events & IOUrgent)
  {
    if (p->automatic)
    {
      logme()
        << "WARNING: connection to the server at " << p->peeraddr
        << " lost(will attempt to reconnect in 15 seconds)\n";
      return losePeer(0, p, ev);
    }
    else
      return losePeer("WARNING: lost peer connection ", p, ev);
  }

  // If we can write to the peer socket, pump whatever we can into it.
  if (ev->events & IOWrite)
  {
    while (Bucket *b = p->sendq)
    {
      IOSize len = b->data.size() - p->sendpos;
      const void *data = (len ? (const void *)&b->data[p->sendpos]
                         : (const void *)&data);
      IOSize done;

      try
      {
        done = (len ? ev->source->write(data, len) : 0);
        if (debug_ && len)
          logme()
            << "DEBUG: sent " << done << " bytes to peer "
            << p->peeraddr << std::endl;
      }
      catch(Error &e)
      {
        return losePeer("WARNING: unable to write to peer ",
                        p, ev, &e);
      }

      p->sendpos += done;
      if (p->sendpos == b->data.size())
      {
        Bucket *old = p->sendq;
        p->sendq = old->next;
        p->sendpos = 0;
        old->next = 0;
        discard(old);
      }

      if (! done && len)
        // Cannot write any more.
        break;
    }
  }

  // If there is data to be read from the peer, first receive what we
  // can get out the socket, the process all complete requests.
  if (ev->events & IORead)
  {
    // First build up the incoming buffer of data in the socket.
    // Remember the last size returned by the socket; we need
    // it to determine if the remote end closed the connection.
    IOSize sz;
    try
    {
      std::vector<unsigned char> buf(SOCKET_READ_SIZE);
      do
        if ((sz = ev->source->read(&buf[0], buf.size())))
        {
          if (debug_)
            logme()
              << "DEBUG: received " << sz << " bytes from peer "
              << p->peeraddr << std::endl;
          DataBlob &data = p->incoming;
          if (data.capacity() < data.size() + sz)
            data.reserve(data.size() + SOCKET_READ_GROWTH);
          data.insert(data.end(), &buf[0], &buf[0] + sz);
        }
      while (sz == sizeof(buf));
    }
    catch(Error &e)
    {
      SystemError *next = dynamic_cast<SystemError *>(e.next());
      if (next && next->portable() == SysErr::ErrTryAgain)
        sz = 1; // Ignore it, and fake no end of data.
      else
        // Houston we have a problem.
        return losePeer("WARNING: failed to read from peer ",
                        p, ev, &e);
    }

    // Process fully received messages as long as we can.
    size_t consumed = 0;
    DataBlob &data = p->incoming;
    while (data.size()-consumed >= sizeof(uint32_t)
           && p->waiting < MAX_PEER_WAITREQS)
    {
      uint32_t msglen;
      memcpy(&msglen, &data[0]+consumed, sizeof(msglen));

      if (msglen >= MESSAGE_SIZE_LIMIT)
        return losePeer("WARNING: excessively large message from ", p, ev);

      if (data.size()-consumed >= msglen)
      {
        bool valid = true;
        if (msglen < 2*sizeof(uint32_t))
        {
          logme()
            << "ERROR: corrupt peer message of length " << msglen
            << " from peer " << p->peeraddr << std::endl;
          valid = false;
        }
        else
        {
          // Decode and process this message.
          Bucket msg;
          msg.next = 0;
          valid = onMessage(&msg, p, &data[0]+consumed, msglen);

          // If we created a response, chain it to the write queue.
          if (! msg.data.empty())
          {
            Bucket **prev = &p->sendq;
            while (*prev)
              prev = &(*prev)->next;

            *prev = new Bucket;
            (*prev)->next = 0;
            (*prev)->data.swap(msg.data);
          }
        }

        if (! valid)
          return losePeer("WARNING: data stream error with ", p, ev);

        consumed += msglen;
      }
      else
        break;
    }

    data.erase(data.begin(), data.begin()+consumed);

    // If the client has closed the connection, shut down our end.  If
    // we have something to send back still, leave the write direction
    // open.  Otherwise close the shop for this client.
    if (sz == 0)
      sel_.setMask(p->socket, p->mask &= ~IORead);
  }

  // Yes, please keep processing events for this socket.
  return false;
}

/** Respond to new connections on the server socket.  Accepts the
    connection and creates a new socket for the peer, and sets it up
    for further communication.  Returns @c false always to tell the
    IOSelector to keep processing events for the server socket.  */
bool
IgNet::onPeerConnect(IOSelectEvent *ev)
{
  // Recover the server socket.
  assert(ev->source == server_);

  // Accept the connection.
  Socket *s = server_->accept();
  assert(s);
  assert(! s->isBlocking());

  // Record it to our list of peers.
  Peer *p = createPeer(s);
  InetAddress peeraddr = ((InetSocket *) s)->peername();
  InetAddress myaddr = ((InetSocket *) s)->sockname();
  p->peeraddr = StringFormat("%1:%2")
                .arg(peeraddr.hostname())
                .arg(peeraddr.port());
  p->mask = IORead|IOUrgent;
  p->socket = s;

  // Report the new connection.
  if (debug_)
    logme()
      << "INFO: new peer " << p->peeraddr << " is now connected to "
      << myaddr.hostname() << ":" << myaddr.port() << std::endl;

  // Attach it to the listener.
  sel_.attach(s, p->mask, CreateHook(this, &IgNet::onPeerData, p));

  // We are never done.
  return false;
}

/** React to notifications from the app thread.  This is a simple
    message to tell this thread to wake up and send unsollicited
    updates to the peers when new app data appears.  We don't send the
    updates here, but just set a flag to tell the main event pump to
    send a notification later.  This avoids sending unnecessarily
    frequent object updates.  */
bool
IgNet::onLocalNotify(IOSelectEvent *ev)
{
  // Discard the data in the pipe, we care only about the wakeup.
  try
  {
    IOSize sz;
    unsigned char buf [1024];
    while ((sz = ev->source->read(buf, sizeof(buf))))
      ;
  }
  catch(Error &e)
  {
    SystemError *next = dynamic_cast<SystemError *>(e.next());
    if (next && next->portable() == SysErr::ErrTryAgain)
      ; // Ignore it
    else
      logme()
        << "WARNING: error reading from notification pipe: "
        << e.explain() << std::endl;
  }

  // Tell the main event pump to send an update in a little while.
  flush_ = true;

  // We are never done, always keep going.
  return false;
}

/// Update the selector mask for a peer based on data queues.  Close
/// the connection if there is no reason to maintain it open.
void
IgNet::updateMask(Peer *p)
{
  if (! p->socket)
    return;

  // Listen to writes iff we have data to send.
  unsigned oldmask = p->mask;
  if (! p->sendq &&(p->mask & IOWrite))
    sel_.setMask(p->socket, p->mask &= ~IOWrite);

  if (p->sendq && !(p->mask & IOWrite))
    sel_.setMask(p->socket, p->mask |= IOWrite);

  if (debug_ && oldmask != p->mask)
    logme()
      << "DEBUG: updating mask for " << p->peeraddr << " to "
      << p->mask << " from " << oldmask << std::endl;

  // If we have nothing more to send and are no longer listening
  // for reads, close up the shop for this peer.
  if (p->mask == IOUrgent && ! p->waiting)
  {
    assert(! p->sendq);
    if (debug_)
      logme() << "INFO: connection closed to " << p->peeraddr << std::endl;
    losePeer(0, p, 0);
  }
}

//////////////////////////////////////////////////////////////////////
IgNet::IgNet(const std::string &appname /* = "" */)
  : debug_(false),
    appname_(appname.empty() ? "IgNet" : appname.c_str()),
    pid_(getpid()),
    server_(0),
    version_(Time::current()),
    communicate_((pthread_t) -1),
    shutdown_(0),
    delay_(1000),
    flush_(false)
{
  // Create a pipe for the local apps to tell the communicator
  // thread that local app data has changed and that the peers
  // should be notified.
  fcntl(wakeup_.source()->fd(), F_SETFL, O_RDONLY | O_NONBLOCK);
  sel_.attach(wakeup_.source(), IORead, CreateHook(this, &IgNet::onLocalNotify));

  // Initialise the upstream and downstream to empty.
  upstream_.peer   = downstream_.peer   = 0;
  upstream_.next   = downstream_.next   = 0;
  upstream_.port   = downstream_.port   = 0;
  upstream_.update = downstream_.update = false;
  upstream_.warned = downstream_.warned = false;

  local_ = createPeer((Socket *) -1);
}

IgNet::~IgNet(void)
{
  // FIXME
}

/// Enable or disable verbose debugging.  Must be called before
/// calling run() or start().
void
IgNet::debug(bool doit)
{
  debug_ = doit;
}

/// Set the I/O dispatching delay.  Must be called before calling
/// run() or start().
void
IgNet::delay(int delay)
{
  delay_ = delay;
}

/// Start a server socket for accessing this node remotely.  Must
/// be called before calling run() or start().  May throw an Exception
/// if the server socket cannot be initialised.
void
IgNet::startLocalServer(int port)
{
  if (server_)
  {
    logme() << "ERROR: server was already started.\n";
    return;
  }

  try
  {
    server_ = new InetServerSocket(InetAddress(port), 10);
    server_->setopt(lat::SocketConst::OptSockSendBuffer, SOCKET_BUF_SIZE);
    server_->setopt(lat::SocketConst::OptSockReceiveBuffer, SOCKET_BUF_SIZE);
    server_->setBlocking(false);
    sel_.attach(server_, IOAccept, CreateHook(this, &IgNet::onPeerConnect));
  }
  catch(Error &e)
  {
    // FIXME: Do we need to do this when we throw an exception anyway?
    // FIXME: Abort instead?
    logme()
      << "ERROR: Failed to start server at port " << port << ": "
      << e.explain() << std::endl;

    throw IgNetError("Failed to start server at port ", e.clone());

    // FIXME: Throw something simpler that removes the dependency?
    //     throw cms::Exception("IgNet::startLocalServer")
    //       << "Failed to start server at port " << port << ": "
    //       << e.explain();
  }

  logme() << "INFO: Shared memory server started at port " << port << std::endl;
}

/// Tell the network layer to connect to @a host and @a port and
/// automatically send updates whenever local data changes.  Must
/// be called before calling run() or start().
void
IgNet::updateToCollector(const std::string &host, int port)
{
  if (! downstream_.host.empty())
  {
    logme()
      << "ERROR: Already updating another collector at "
      << downstream_.host << ":" << downstream_.port << std::endl;
    return;
  }

  downstream_.update = true;
  downstream_.host = host;
  downstream_.port = port;
}

/// Tell the network layer to connect to @a host and @a port and
/// automatically receive updates from upstream source(s).  Must be
/// called before calling run() or start().
void
IgNet::listenToSource(const std::string &host, int port)
{
  if (! upstream_.host.empty())
  {
    logme()
      << "ERROR: Already receiving data from another collector at "
      << upstream_.host << ":" << upstream_.port << std::endl;
    return;
  }

  upstream_.update = false;
  upstream_.host = host;
  upstream_.port = port;
}

/// Stop the network layer and wait it to finish.
void
IgNet::shutdown(void)
{
  shutdown_ = 1;
  if (communicate_ !=(pthread_t) -1)
    pthread_join(communicate_, 0);
}

/** A thread to communicate with the distributed memory cache peers.
    All this does is run the loop to respond to new connections.
    Much of the actual work is done when a new connection is
    received, and in pumping data around in response to actual
    requests.  */
static void *communicate(void *obj)
{
  sigset_t sigs;
  sigfillset(&sigs);
  pthread_sigmask(SIG_BLOCK, &sigs, 0);
  ((IgNet *)obj)->run();
  return 0;
}

/// Acquire a lock on the net layer.
void
IgNet::lock(void)
{
  if (communicate_ !=(pthread_t) -1)
    pthread_mutex_lock(&lock_);
}

/// Release the lock on the net layer.
void
IgNet::unlock(void)
{
  if (communicate_ !=(pthread_t) -1)
    pthread_mutex_unlock(&lock_);
}

/// Start running the network layer in a new thread.  This is an
/// exclusive alternative to the run() method, which runs the network
/// layer in the caller's thread.
void
IgNet::start(void)
{
  if (communicate_ !=(pthread_t) -1)
  {
    logme()
      << "ERROR: Shared memory networking thread has already been started\n";
    return;
  }

  pthread_mutex_init(&lock_, 0);
  pthread_create(&communicate_, 0, &communicate, this);
}

/** Run the actual I/O processing loop. */
void
IgNet::run(void)
{
  Time now;
  AutoPeer *automatic[2] = { &upstream_, &downstream_ };

  // Perform I/O.  Every once in a while flush updates to peers.
  while (! shouldStop())
  {
    for (int i = 0; i < 2; ++i)
    {
      AutoPeer *ap = automatic[i];

      // If we need a server connection and don't have one yet,
      // initiate asynchronous connection creation.  Swallow errors
      // in case the server won't talk to us.
      if (! ap->host.empty()
          && ! ap->peer
          &&(now = Time::current()) > ap->next)
      {
        ap->next = now + TimeSpan(0, 0, 0, 15 /* seconds */, 0);
        InetSocket *s = 0;
        try
        {
          s = new InetSocket(SocketConst::TypeStream);
          s->setBlocking(false);
          s->connect(InetAddress(ap->host.c_str(), ap->port));
          s->setopt(lat::SocketConst::OptSockSendBuffer, SOCKET_BUF_SIZE);
          s->setopt(lat::SocketConst::OptSockReceiveBuffer, SOCKET_BUF_SIZE);
        }
        catch(Error &e)
        {
          SystemError *sys = dynamic_cast<SystemError *>(e.next());
          if (! sys || sys->portable() != SysErr::ErrOperationInProgress)
          {
            // "In progress" just means the connection is in progress.
            // The connection is ready when the socket is writeable.
            // Anything else is a real problem.
            if (! ap->warned)
            {
              logme()
                << "NOTE: server at " << ap->host << ":" << ap->port
                << " is unavailable.  Connection will be established"
                << " automatically on the background once the server"
                << " becomes available.  Error from the attempt was: "
                << e.explain() << '\n';
              ap->warned = true;
            }

            if (s)
              s->abort();
            delete s;
            s = 0;
          }
        }

        // Set up with the selector if we were successful.  If this is
        // the upstream collector, queue a request for updates.
        if (s)
        {
          lock();
          Peer *p = createPeer(s);
          ap->peer = p;
          ap->warned = false;
          unlock();

          InetAddress peeraddr = ((InetSocket *) s)->peername();
          InetAddress myaddr = ((InetSocket *) s)->sockname();
          p->peeraddr = StringFormat("%1:%2")
                        .arg(peeraddr.hostname())
                        .arg(peeraddr.port());
          p->mask = IORead|IOWrite|IOUrgent;
          p->update = ap->update;
          p->automatic = ap;
          p->socket = s;
          sel_.attach(s, p->mask, CreateHook(this, &IgNet::onPeerData, p));
          if (ap == &upstream_)
          {
            uint32_t words[4] = { 2*sizeof(uint32_t), VIS_MSG_LIST_OBJECTS,
                                  2*sizeof(uint32_t), VIS_MSG_UPDATE_ME };
            p->sendq = new Bucket;
            p->sendq->next = 0;
            copydata(p->sendq, words, sizeof(words));
          }

          // Report the new connection.
          if (debug_)
            logme()
              << "INFO: now connected to " << p->peeraddr << " from "
              << myaddr.hostname() << ":" << myaddr.port() << std::endl;
        }
      }
    }

    // Pump events for a while.
    sel_.dispatch(delay_);
    now = Time::current();

    // Check if flush is required.  Flush only if one is needed.
    // Always sends the full object list.  Compact objects no longer
    // in active use before sending out the update.
    if (flush_)
    {
      flush_ = false;

      lock();
      purgeDeadObjects(now - TimeSpan(0, 0, 2 /* minutes */, 0, 0),
                       now - TimeSpan(0, 0, 20 /* minutes */, 0, 0));
      sendObjectListToPeers(true);
      unlock();
    }

    // Update the data server and peer selection masks.  If we
    // have no more data to send and listening for writes, remove
    // the write mask.  If we have something to write and aren't
    // listening for writes, start listening so we can send off
    // the data.
    updatePeerMasks();

    // Release peers that have been waiting for data for too long.
    lock();
    Time waitold = now - TimeSpan(0, 0, 2 /* minutes */, 0, 0);
    for (WaitList::iterator i = waiting_.begin(), e = waiting_.end(); i != e; )
    {
      // If the peer has waited for too long, send something.
      if (i->time < waitold)
        releaseFromWait(i++, findObject(0, i->name));

      // Keep it for now.
      else
        ++i;
    }
    unlock();
  }
}

// Tell the network cache that there have been local changes that
// should be advertised to the downstream listeners.
void
IgNet::sendLocalChanges(void)
{
  char byte = 0;
  wakeup_.sink()->write(&byte, 1);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgNet::Object *
IgNet::findObject(Peer *p, const std::string &name, Peer **owner)
{
  ObjectMap::iterator pos;
  PeerMap::iterator i, e;
  if (owner)
    *owner = 0;
  if (p)
  {
    pos = p->objs.find(name);
    if (pos == p->objs.end())
      return 0;
    else
    {
      if (owner) *owner = p;
      return &pos->second;
    }
  }
  else
  {
    for (i = peers_.begin(), e = peers_.end(); i != e; ++i)
    {
      pos = i->second.objs.find(name);
      if (pos != i->second.objs.end())
      {
        if (owner) *owner = &i->second;
        return &pos->second;
      }
    }
    return 0;
  }
}

IgNet::Object *
IgNet::makeObject(Peer *p, const std::string &name)
{
  Object *o = &p->objs[name];
  o->version = 0;
  o->name = name;
  o->flags = 0;
  o->lastreq = 0;
  return o;
}

// Mark all the objects as zombies.  This is intended to be used
// when starting to process a complete list of objects, in order
// to flag the objects that need to be killed at the end.  After
// call to this method, revive all live objects by removing the
// VIS_FLAG_ZOMBIE flag, then call markObjectsDead() at the end
// to flag dead as all remaining zombies.
void
IgNet::markObjectsZombies(Peer *p)
{
  ObjectMap::iterator i, e;
  for (i = p->objs.begin(), e = p->objs.end(); i != e; ++i)
    i->second.flags |= VIS_FLAG_ZOMBIE;
}

// Mark remaining zombie objects as dead.  See markObjectsZombies().
void
IgNet::markObjectsDead(Peer *p)
{
  ObjectMap::iterator i, e;
  for (i = p->objs.begin(), e = p->objs.end(); i != e; ++i)
    if (i->second.flags & VIS_FLAG_ZOMBIE)
      i->second.flags = (i->second.flags & ~VIS_FLAG_ZOMBIE) | VIS_FLAG_DEAD;
}

// Purge all old and dead objects.
void
IgNet::purgeDeadObjects(lat::Time oldobj, lat::Time deadobj)
{
  PeerMap::iterator pi, pe;
  ObjectMap::iterator oi, oe;
  for (pi = peers_.begin(), pe = peers_.end(); pi != pe; ++pi)
    for (oi = pi->second.objs.begin(), oe = pi->second.objs.end(); oi != oe; )
    {
      Object &o = oi->second;

      // Compact non-scalar objects that are unused.  We send scalar
      // objects to the web server so we keep them around.
      if (o.lastreq < oldobj && ! o.rawdata.empty() && !(o.flags & VIS_FLAG_SCALAR))
      {
        if (debug_)
          logme()
            << "DEBUG: compacting idle '" << o.name
            << "' from " << pi->second.peeraddr << std::endl;
      }

      // Remove if dead, old and unused.
      if (o.lastreq < deadobj
          && o.version < deadobj
          &&(o.flags & VIS_FLAG_DEAD))
      {
        if (debug_)
          logme()
            << "DEBUG: removing dead '" << o.name
            << "' from " << pi->second.peeraddr << std::endl;

        pi->second.objs.erase(oi++);
      }
      else
        ++oi;
    }
}

IgNet::Peer *
IgNet::getPeer(lat::Socket *s)
{
  PeerMap::iterator pos = peers_.find(s);
  return pos == peers_.end() ? 0 : &pos->second;
}

IgNet::Peer *
IgNet::createPeer(lat::Socket *s)
{
  Peer *p = &peers_[s];
  p->socket = 0;
  p->sendq = 0;
  p->sendpos = 0;
  p->mask = 0;
  p->source = false;
  p->update = false;
  p->updated = false;
  p->updates = 0;
  p->waiting = 0;
  p->automatic = 0;
  return p;
}

void
IgNet::removePeer(Peer *p, lat::Socket *s)
{
  bool needflush = ! p->objs.empty();

  p->objs.clear();
  peers_.erase(s);

  // If we removed a peer with objects, our list of objects
  // has changed and we need to update downstream peers.
  if (needflush)
    sendLocalChanges();
}

/// Send all objects to a peer and optionally mark sent objects old.
void
IgNet::sendObjectListToPeer(Bucket *msg, bool all, bool clear)
{
  PeerMap::iterator pi, pe;
  ObjectMap::iterator oi, oe;
  uint32_t numobjs = 0;
  for (pi = peers_.begin(), pe = peers_.end(); pi != pe; ++pi)
    numobjs += pi->second.objs.size();

  msg->data.reserve(msg->data.size() + 300*numobjs);

  uint32_t nupdates = 0;
  uint32_t words[4];
  words[0] = sizeof(words);
  words[1] = VIS_REPLY_LIST_BEGIN;
  words[2] = numobjs;
  words[3] = all;
  copydata(msg, &words[0], sizeof(words));

  for (pi = peers_.begin(), pe = peers_.end(); pi != pe; ++pi)
    for (oi = pi->second.objs.begin(), oe = pi->second.objs.end(); oi != oe; ++oi)
      if (all ||(oi->second.flags & VIS_FLAG_NEW))
      {
        sendObjectToPeer(msg, oi->second, false);
        if (clear)
          oi->second.flags &= ~VIS_FLAG_NEW;
        ++nupdates;
      }

  words[1] = VIS_REPLY_LIST_END;
  words[2] = nupdates;
  copydata(msg, &words[0], sizeof(words));
}

void
IgNet::sendObjectListToPeers(bool all)
{
  PeerMap::iterator i, e;
  for (i = peers_.begin(), e = peers_.end(); i != e; ++i)
  {
    Peer &p = i->second;
    if (! p.update)
      continue;

    if (debug_)
      logme()
        << "DEBUG: notifying " << p.peeraddr << std::endl;

    Bucket msg;
    msg.next = 0;
    sendObjectListToPeer(&msg, !p.updated || all, true);

    if (! msg.data.empty())
    {
      Bucket **prev = &p.sendq;
      while (*prev)
        prev = &(*prev)->next;

      *prev = new Bucket;
      (*prev)->next = 0;
      (*prev)->data.swap(msg.data);
    }
    p.updated = true;
  }
}

void
IgNet::updatePeerMasks(void)
{
  PeerMap::iterator i, e;
  for (i = peers_.begin(), e = peers_.end(); i != e; )
    updateMask(&(i++)->second);
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int
IgNet::receive(void(*callback)(void *arg, uint32_t reason, Object &obj), void *arg)
{
  int updates = 0;

  lock();
  PeerMap::iterator pi, pe;
  ObjectMap::iterator oi, oe;
  for (pi = peers_.begin(), pe = peers_.end(); pi != pe; ++pi)
  {
    Peer &p = pi->second;
    if (&p == local_)
      continue;

    updates += p.updates;

    for (oi = p.objs.begin(), oe = p.objs.end(); oi != oe; )
    {
      Object &o = oi->second;
      if (o.flags & VIS_FLAG_DEAD)
      {
        callback(arg, VIS_FLAG_DEAD, o);
        p.objs.erase(oi++);
      }
      else if (o.flags & VIS_FLAG_RECEIVED)
      {
        callback(arg, VIS_FLAG_RECEIVED, o);
        o.flags &= ~VIS_FLAG_RECEIVED;
        ++oi;
      }
      else
        ++oi;
    }
  }
  unlock();

  return updates;
}

/// Update the network cache for an object.  The caller must call
/// sendLocalChanges() later to push out the changes.
void
IgNet::updateLocalObject(Object &o)
{
  ObjectMap::iterator pos = local_->objs.find(o.name);
  if (pos == local_->objs.end())
    local_->objs.insert(ObjectMap::value_type(o.name, o));
  else
  {
    std::swap(pos->second.version,   o.version);
    std::swap(pos->second.flags,     o.flags);
    std::swap(pos->second.rawdata,   o.rawdata);
    pos->second.lastreq = 0;
  }
}

/// Delete the local object.  The caller must call sendLocalChanges()
/// later to push out the changes.
void
IgNet::removeLocalObject(const std::string &path)
{
  local_->objs.erase(path);
}
