#ifndef IGUANA_ISPY_APPLICATION_H
# define IGUANA_ISPY_APPLICATION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <QObject>
# include <QStringList>
# include <QTimer>
# include <QUrl>
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgCollection;
class IgAssociationSet;
class ISpyMainWindow;
class IgDataStorage;
class IgCollectionTableModel;
class IgMultiStorageTreeModel;
class Ig3DBaseModel;
class QTreeWidget;
class QTreeWidgetItem;
class SoSwitch;
class SoSeparator;
class ISpySplashScreen;
class QNetworkReply;
class QNetworkAccessManager;
class IgNetworkReplyHandler;
class QNetworkReply;
class QProgressDialog;

namespace lat
{
  class ZipArchive;
  class ZipMember;
}

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class ISpyApplication : public QObject
{
  Q_OBJECT
public:
  ISpyApplication(void);
  ~ISpyApplication(void);

  int                   run(int argc, char *argv[]);
  int                   argc(void) const;
  char **               argv(void) const;
  const char *          appname(void) const;
  QNetworkReply *       getUrl(const QUrl &link);

public slots:
  void                  openFileDialog(void);
  void                  openUrlDialog(void);
  void                  open(const QString &fileName);
  void                  connect(void);
  void                  autoEvents(void);
  void                  nextEvent(void);
  void                  previousEvent(void);
  void                  showAbout(void);

signals:
  void                  showMessage(const QString &fileName);
  void                  firstEvent(void);
  void                  lastEvent(void);
  void                  splashDone(void);
  void                  save(void);
  void                  print(void);

protected:
  int                   usage(void);
  int                   version(void);
  void                  setupMainWindow(void);
  void                  setupSplashScreen(void);

private slots:
  void                  cleanSplash(void);
  void                  setCurrentItem(QTreeWidgetItem *current, QTreeWidgetItem *previous);
  void                  itemActivated(QTreeWidgetItem *current, int);
  void                  onExit(void);
  void                  exit(void);
  void                  rewind(void);
  void                  handleWizardLinks(const QUrl &link);
  void                  fileDownloaded(IgNetworkReplyHandler *handler);
  void                  handleAbortedDownload(IgNetworkReplyHandler *handler);
  void                  setProgress(qint64 current, qint64 final);
  void                  handleDownloadError(IgNetworkReplyHandler *handler);

private:
  typedef void(*Make3D)(IgCollection **, IgAssociationSet **, SoSeparator *);

  struct CollectionSpec
  {
    std::string                 friendlyName;
    std::string                 collection;
    std::string                 otherCollection;
    std::string                 otherAssociation;
    std::vector<std::string>    requiredFields;
    std::vector<std::string>    otherRequiredFields;
    Make3D                      make3D;
  };

  struct Collection
  {
    CollectionSpec              *spec;
    IgCollection                *data[2];
    IgAssociationSet            *assoc;
    QTreeWidgetItem             *item;
    SoSwitch                    *node;
    SoSeparator                 *sep;
    int                         visibility;
  };

  struct Event
  {
    size_t                      index;
    lat::ZipArchive             *archive;
    lat::ZipMember              *contents;
  };

  typedef std::vector<CollectionSpec>   CollectionSpecs;
  typedef std::vector<Collection>       Collections;
  typedef std::vector<Event>            Events;

  int                   doRun(void);
  void                  defaultSettings(void);
  void                  restoreSettings(void);

  void                  collection(const char *friendlyName,
                                   const char *collectionSpec,
                                   const char *otherCollectionSpec,
                                   const char *associationSpec,
                                   Make3D make3D);
  void                  displayCollection(Collection &c);
  void                  updateCollections(void);
  lat::ZipArchive *     loadFile(const QString &fileName);
  void                  readData(IgDataStorage *to,
                                 lat::ZipArchive *archive,
                                 lat::ZipMember *source);
  void                  newEvent(void);
  void                  downloadFile(const QUrl &url);

  int                   m_argc;
  char                  **m_argv;
  char                  *m_appname;
  lat::ZipArchive       *m_archives[2];
  IgDataStorage         *m_storages[2];
  CollectionSpecs       m_specs;
  Collections           m_collections;
  Events                m_events;
  size_t                m_eventIndex;

  IgCollectionTableModel *m_tableModel;
  Ig3DBaseModel         *m_3DModel;

  ISpyMainWindow        *m_mainWindow;
  QTreeWidget           *m_treeWidget;
  ISpySplashScreen      *m_splash;

  bool                  m_autoEvents;
  bool                  m_exiting;
  QTimer                *m_timer;
  QNetworkAccessManager *m_networkManager;
  QProgressDialog       *m_progressDialog;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_ISPY_APPLICATION_H
