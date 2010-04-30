#ifndef IGUANA_QTGUI_IG_NETWORK_REPLY_HANDLER_H
# define IGUANA_QTGUI_IG_NETWORK_REPLY_HANDLER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <QObject>
# include <QString>
# include <QList>
# include <QtNetwork>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QIODevice;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgNetworkReplyHandler : public QObject
{
  Q_OBJECT
public:
  IgNetworkReplyHandler(QNetworkReply *reply, QIODevice *device = 0);
  // implicit copy constructor
  // implicit assignment operator
  // implicit destructor
  virtual ~IgNetworkReplyHandler(void);

  QIODevice *device(void);
  QNetworkReply::NetworkError error(void);
  QNetworkReply *reply(void);
  bool aborted(void);
public slots:
  void abort(void);
  void error(QNetworkReply::NetworkError error);
signals:
  void        done(IgNetworkReplyHandler *handler);
  void        downloadError(IgNetworkReplyHandler *handler);
private slots:
  void        downloadMore(void);
  void        finished(void);
private:
  QNetworkReply   *m_reply;
  QIODevice       *m_device;
  bool            m_aborted;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_QTGUI_IG_NETWORK_REPLY_HANDLER_H
