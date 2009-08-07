#ifndef IGUANA_ISPY_APPLICATION_H
# define IGUANA_ISPY_APPLICATION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <QObject>
# include <QStringList>
# include <QTimer>
# include <QUrl>
# include <QCheckBox>
# include <QHeaderView>
# include <vector>
# include <map>

# include "Iguana/QtGUI/interface/ISpyConsumerThread.h"

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
class ISpy3DView;
class QTreeWidget;
class QTreeWidgetItem;
class QToolBar;
class QActionGroup;
class SoSwitch;
class SoSeparator;
class ISpySplashScreen;
class QNetworkReply;
class QNetworkAccessManager;
class IgNetworkReplyHandler;
class QNetworkReply;
class QProgressDialog;
class SoCamera;

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
  QNetworkReply *	getUrl(const QUrl &link);

public slots:
  void                  openFileDialog(void);
  void                  openUrlDialog(void);
  void                  open(const QString &fileName);
  void                  connect(void);
  void                  autoEvents(void);
  void                  animateViews(void);
  void                  nextEvent(void);
  void                  previousEvent(void);
  void                  showAbout(void);
  void                  newEvent(void);

signals:
  void                  showMessage(const QString &fileName);
  void                  firstEvent(void);
  void                  lastEvent(void);
  void                  save(void);
  void                  print(void);
  void                  resetCounter(void);

protected:
  int                   usage(void);
  int                   version(void);
  void                  setupMainWindow(void);
  void                  setupSplashScreen(void);

private slots:
  void                  setCurrentItem(QTreeWidgetItem *current, QTreeWidgetItem *previous);
  void                  itemActivated(QTreeWidgetItem *current, int);
  void                  handleGroupsClicking(QTreeWidgetItem *current);
  void                  onExit(void);
  void                  exit(void);
  void                  rewind(void);
  void                  handleWizardLinks(const QUrl &link);
  void                  fileDownloaded(IgNetworkReplyHandler *handler);
  void                  setProgress(qint64 current, qint64 final);
  void                  handleDownloadError(IgNetworkReplyHandler *handler);
  void                  switchView(int i);
  void                  cameraToggled(void);
  void                  resetToHomePosition(void);
  void                  restartPlay(void);

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
    int                         defaultVisibility;
    size_t                      visibilityIndex;
  };

  struct CameraSpec
  {
    float                       position[3];
    float                       pointAt[3];
    float                       scale;
    bool                        orthographic;
    bool                        rotating;
  };
  
  struct ViewSpec
  {
    std::string                 name;
    size_t                      cameraIndex;
    size_t                      startCollIndex;
    size_t                      endCollIndex;
    bool                        specialized;
  };

  struct Camera
  {
    CameraSpec                  *spec;
    SoCamera                    *node;
  };

  struct View
  {
    ViewSpec                    *spec;
    Camera                      *camera;
  };
  
  struct Collection
  {
    CollectionSpec              *spec;
    IgCollection                *data[2];
    IgAssociationSet            *assoc;
    QTreeWidgetItem             *item;
    SoSwitch                    *node;
    SoSeparator                 *sep;
    std::string                 collectionName;
    int                         groupIndex;
  };

  struct Group
  {
    std::string                 name;
    bool                        expanded;
    std::vector<size_t>         children;
    QTreeWidgetItem             *item;
  };

  struct Event
  {
    size_t                      index;
    lat::ZipArchive             *archive;
    lat::ZipMember              *contents;
  };

  typedef std::vector<CollectionSpec>   CollectionSpecs;
  typedef std::vector<CameraSpec>       CameraSpecs;
  typedef std::vector<ViewSpec>         ViewSpecs;
  typedef std::vector<Collection>       Collections;
  typedef std::vector<View>             Views;
  typedef std::vector<Camera>           Cameras;
  typedef std::vector<Event>            Events;
  typedef std::vector<Group>            Groups;
  typedef std::vector<size_t>           GroupIndex;
  typedef std::vector<Qt::CheckState>   Visibilities;
  typedef std::map<std::string, size_t> VisibilityGroupMap;

  struct SortBySpecAndName
  {
    bool operator()(const Collection &a, const Collection &b)
    {
      if (!a.spec && !b.spec)
        return a.collectionName < b.collectionName;
      if (!a.spec)
        return false;
      if (!b.spec)
        return true;
      return a.spec < b.spec;
    }
  };

  int                   doRun(void);
  void                  defaultSettings(void);
  void                  restoreSettings(void);
  void                  onlineConfig(const char* server);

  int                   getCollectionIndex(QTreeWidgetItem *item);
  void                  collection(const char *friendlyName,
                                   const char *collectionSpec,
                                   const char *otherCollectionSpec,
                                   const char *associationSpec,
                                   Make3D make3D,
                                   Qt::CheckState visibility);
  void                  view(const char *name,
                             bool specialized);

  void                  camera(float *pos,
                               float *pointAt,
                               float scale,
                               bool orthographic,
                               bool rotating);

  void                  visibilityGroup(void);

  void                  displayCollection(Collection &c);
  void                  updateCollections(void);
  lat::ZipArchive *     loadFile(const QString &fileName);
  void                  readData(IgDataStorage *to,
                                 lat::ZipArchive *archive,
                                 lat::ZipMember *source);
  void                  downloadFile(const QUrl &url);
  void                  setupActions(void);
  void                  restoreCameraFromSpec(CameraSpec *spec, Camera &camera);
  
  int                   m_argc;
  char                  **m_argv;
  char                  *m_appname;
  lat::ZipArchive       *m_archives[2];
  IgDataStorage         *m_storages[2];
  CollectionSpecs       m_specs;
  ViewSpecs             m_viewSpecs;
  CameraSpecs           m_cameraSpecs;
  Collections           m_collections;
  Views                 m_views;
  Cameras               m_cameras;
  Groups                m_groups;
  GroupIndex            m_groupIndex;
  Events                m_events;
  Visibilities          m_visibility;
  VisibilityGroupMap    m_visibilityGroupMap;
  size_t                m_eventIndex;
  size_t                m_currentViewIndex;

  IgCollectionTableModel *m_tableModel;
  Ig3DBaseModel         *m_3DModel;
  ISpy3DView            *m_viewer;
  ISpyMainWindow        *m_mainWindow;
  QTreeWidget           *m_treeWidget;
  ISpySplashScreen      *m_splash;

  ISpyConsumerThread    m_consumer;
  
  bool                  m_online;
  bool                  m_autoEvents;
  bool                  m_exiting;
  QTimer                *m_animationTimer;
  QTimer                *m_timer;
  QTimer                *m_idleTimer;
  QNetworkAccessManager *m_networkManager;
  QProgressDialog       *m_progressDialog;
  QToolBar              *m_3DToolBar;
  QAction               *m_actionCameraPerspective;
  QAction               *m_actionCameraOrthographic;
  QFont                 *m_groupFont;
  QFont                 *m_itemFont;
  QActionGroup          *m_viewPlaneGroup;
  QActionGroup          *m_viewModeGroup;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_ISPY_APPLICATION_H
