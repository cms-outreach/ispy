#ifndef IGUANA_IG_NET_H
# define IGUANA_IG_NET_H

# include "classlib/iobase/InetServerSocket.h"
# include "classlib/iobase/IOSelector.h"
# include "classlib/iobase/Pipe.h"
# include "classlib/utils/Signal.h"
# include "classlib/utils/Error.h"
# include "classlib/utils/Time.h"
# include <pthread.h>
# include <stdint.h>
# include <iostream>
# include <vector>
# include <string>
# include <list>
# include <map>

class IgNet
{
public:
  static const uint32_t VIS_MSG_HELLO           = 0;
  static const uint32_t VIS_MSG_UPDATE_ME       = 1;
  static const uint32_t VIS_MSG_LIST_OBJECTS    = 2;
  static const uint32_t VIS_MSG_GET_OBJECT      = 3;

  static const uint32_t VIS_REPLY_LIST_BEGIN    = 101;
  static const uint32_t VIS_REPLY_LIST_END      = 102;
  static const uint32_t VIS_REPLY_NONE          = 103;
  static const uint32_t VIS_REPLY_OBJECT        = 104;

  static const uint32_t VIS_FLAG_SCALAR         = 0x1;
  static const uint32_t VIS_FLAG_RECEIVED       = 0x10000000;
  static const uint32_t VIS_FLAG_NEW            = 0x20000000;
  static const uint32_t VIS_FLAG_DEAD           = 0x40000000;
  static const uint32_t VIS_FLAG_ZOMBIE         = 0x80000000;

  static const uint32_t MAX_PEER_WAITREQS       = 128;

  struct Peer;
  struct Object;
  struct WaitObject;

  typedef std::vector<unsigned char>    DataBlob;
  typedef std::list<WaitObject>         WaitList;
  typedef std::map<std::string, Object> ObjectMap;
  typedef std::map<lat::Socket *, Peer> PeerMap;

  struct Object
  {
    uint64_t            version;
    std::string         name;
    DataBlob            rawdata;
    uint32_t            flags;
    lat::Time           lastreq;
  };

  struct Bucket
  {
    Bucket              *next;
    DataBlob            data;
  };

  struct WaitObject
  {
    lat::Time           time;
    std::string         name;
    std::string         info;
    Peer                *peer;
  };

  struct AutoPeer;
  struct Peer
  {
    std::string         peeraddr;
    lat::Socket         *socket;
    DataBlob            incoming;
    Bucket              *sendq;
    size_t              sendpos;

    unsigned            mask;
    bool                source;
    bool                update;
    bool                updated;
    size_t              updates;
    size_t              waiting;
    AutoPeer            *automatic;

    ObjectMap           objs;
  };

  struct AutoPeer
  {
    Peer                *peer;
    lat::Time           next;
    std::string         host;
    int                 port;
    bool                update;
    bool                warned;
  };

  IgNet(const std::string &appname = "");
  virtual ~IgNet(void);

  void                  debug(bool doit);
  void                  delay(int delay);
  void                  startLocalServer(int port);
  void                  updateToCollector(const std::string &host, int port);
  void                  listenToSource(const std::string &host, int port);
  void                  shutdown(void);
  void                  lock(void);
  void                  unlock(void);

  void                  start(void);
  void                  run(void);

  virtual int           receive(void(*callback)(void *arg, uint32_t reason, Object &obj), void *arg);
  virtual void          updateLocalObject(Object &o);
  virtual void          removeLocalObject(const std::string &name);
  void                  sendLocalChanges(void);

protected:
  std::ostream &        logme(void);
  static void           copydata(Bucket *b, const void *data, size_t len);
  void                  sendObjectToPeer(Bucket *msg, Object &o, bool data);

  virtual bool          shouldStop(void);
  void                  waitForData(Peer *p, const std::string &name, const std::string &info, Peer *owner);
  virtual void          releaseFromWait(Bucket *msg, WaitObject &w, Object *o);
  virtual bool          onMessage(Bucket *msg, Peer *p, unsigned char *data, size_t len);

  virtual Object *      findObject(Peer *p, const std::string &name, Peer **owner = 0);
  virtual Object *      makeObject(Peer *p, const std::string &name);
  virtual void          markObjectsZombies(Peer *p);
  virtual void          markObjectsDead(Peer *p);
  virtual void          purgeDeadObjects(lat::Time oldobj, lat::Time deadobj);

  virtual Peer *        getPeer(lat::Socket *s);
  virtual Peer *        createPeer(lat::Socket *s);
  virtual void          removePeer(Peer *p, lat::Socket *s);
  virtual void          sendObjectListToPeer(Bucket *msg, bool all, bool clear);
  virtual void          sendObjectListToPeers(bool all);

  void                  updateMask(Peer *p);
  virtual void          updatePeerMasks(void);

  bool                  debug_;

private:
  static void           discard(Bucket *&b);
  bool                  losePeer(const char *reason,
                                 Peer *peer,
                                 lat::IOSelectEvent *event,
                                 lat::Error *err = 0);
  void                  requestObject(Peer *p, const char *name, size_t len);
  void                  releaseFromWait(WaitList::iterator i, Object *o);
  void                  releaseWaiters(Object *o);

  bool                  onPeerData(lat::IOSelectEvent *ev, Peer *p);
  bool                  onPeerConnect(lat::IOSelectEvent *ev);
  bool                  onLocalNotify(lat::IOSelectEvent *ev);

  std::string           appname_;
  int                   pid_;

  lat::IOSelector       sel_;
  lat::InetServerSocket *server_;
  lat::Pipe             wakeup_;
  lat::Time             version_;

  PeerMap               peers_;
  AutoPeer              upstream_;
  AutoPeer              downstream_;
  WaitList              waiting_;
  Peer                  *local_;

  pthread_mutex_t       lock_;
  pthread_t             communicate_;
  sig_atomic_t          shutdown_;

  int                   delay_;
  bool                  flush_;

  // copying is not available
  IgNet(const IgNet &);
  IgNet &operator=(const IgNet &);
};

#endif // IGUANA_IG_NET_H
