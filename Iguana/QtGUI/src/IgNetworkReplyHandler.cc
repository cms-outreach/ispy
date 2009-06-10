//<<<<<< INCLUDES                                                       >>>>>>
#include "Iguana/QtGUI/interface/IgNetworkReplyHandler.h"
#include <QtNetwork>
#include <QtCore>
#include <QIODevice>
#include <qglobal.h>
#include <iostream>

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>


IgNetworkReplyHandler::IgNetworkReplyHandler (QNetworkReply *reply, QIODevice *device)
    : m_reply (reply),
      m_device (device)
{
    Q_CHECK_PTR(m_reply);
    Q_CHECK_PTR(m_device);
    connect(m_reply, SIGNAL(readyRead()), this, SLOT(downloadMore()));
    connect(m_reply, SIGNAL(finished()), this, SLOT(finished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(error(QNetworkReply::NetworkError)));
    m_device->open (QFile::WriteOnly);
}

IgNetworkReplyHandler::~IgNetworkReplyHandler (void)
{
    delete m_device;
}

void
IgNetworkReplyHandler::downloadMore (void) 
{
    m_device->write (m_reply->readAll ());
}

void
IgNetworkReplyHandler::finished (void) 
{
    m_device->write (m_reply->readAll ());
    m_device->close ();
    if (m_reply->error())
        return;
    emit done (this);
}

QIODevice * 
IgNetworkReplyHandler::device (void)
{
    return m_device;
}

QNetworkReply::NetworkError
IgNetworkReplyHandler::error (void)
{
    return m_reply->error();
}

QNetworkReply *
IgNetworkReplyHandler::reply (void)
{
    return m_reply;
}

void
IgNetworkReplyHandler::abort (void)
{
    m_reply->abort();
    emit downloadAborted(this);
}

void
IgNetworkReplyHandler::error (QNetworkReply::NetworkError)
{
    emit downloadError(this);
}