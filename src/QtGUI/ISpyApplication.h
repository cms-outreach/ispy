#ifndef IGUANA_ISPY_APPLICATION_H
# define IGUANA_ISPY_APPLICATION_H

# include <QObject>
# include <QStringList>
# include <QTimer>
# include <QUrl>
# include <QCheckBox>
# include <QHeaderView>
# include <vector>
# include <map>
# include <Inventor/nodes/SoMarkerSet.h>

# include "QtGUI/Style.h"
# include "QtGUI/Projectors.h"
# include "QtGUI/DrawHelpers.h"


class IgCollection;
class IgAssociations;
class ISpyEventSelectorDialog;
class ISpyMainWindow;
class IgDataStorage;
class IgCollectionListModel;
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
class QSortFilterProxyModel;
class SoCamera;
class SoMaterial;
class SoFont;
class ISpyPicturePublishingDialog;
class SoImage;
class QFileSystemWatcher;
class IgArchive;
class IgMember;

struct ViewSpecParseError
{
  ViewSpecParseError(size_t lineNumber = 0)
  :lineNumber(lineNumber)
  {
  }
  size_t lineNumber;
};

struct CssParseError
{
  CssParseError(const char *why, const std::string &what)
  :why(why), what(what)
  {}
  
  std::string why;
  std::string what;
};

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

  void                  collection(const char *friendlyName,
                                   const char *collectionSpec,
                                   const char *otherCollectionSpec,
                                   const char *associationSpec,
                                   const char *make3DName,
                                   bool visibility);
  void                  view(const char *name,
                             bool specialized,
                             bool autoplay);
  void                  camera(float *pos,
                               float *pointAt,
                               float scale,
                               bool orthographic,
                               bool rotating,
                               const std::string &projection);
  void                  visibilityGroup(void);

public slots:
  void                  openFileDialog(void);
  void                  openUrlDialog(void);
  void                  openWithFallbackGeometry(const QString &fileName);
  void                  connect(void);
  void                  autoEvents(void);
  void                  animateViews(void);
  void                  nextEvent(void);
  void                  previousEvent(void);
  void                  showAbout(void);
  void                  newEvent(void);
  void                  updateCollections(void);
  void                  openCss(const QString &filename);
  void                  cssDirChanged(const QString &cssDir);
  void                  checkCss(void);
  void                  autoPrint(void);

signals:
  void                  showMessage(const QString &fileName);
  void                  firstEvent(void);
  void                  lastEvent(void);
  void                  save(void);
  void                  print(void);
  void                  resetCounter(void);
  void			getNewEvent(void);
  void                  styleChanged(void);

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
  bool                  backgroundFileDownloaded(IgNetworkReplyHandler *handler);
  void                  setProgress(qint64 current, qint64 final);
  void                  handleDownloadError(IgNetworkReplyHandler *handler);
  void                  switchView(int i);
  void                  cameraToggled(void);
  void                  resetToHomePosition(void);
  void                  restartPlay(void);
  void                  updateFilterListModel(const QString& title);
  void                  showPublish(void);
  void                  stopFiltering(void);

private:
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
    Projectors                  projectors;
  };
  
  struct ViewSpec
  {
    std::string                 name;
    size_t                      cameraIndex;
    size_t                      startCollIndex;
    size_t                      endCollIndex;
    bool                        specialized;
    bool                        autoplay;
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
    IgAssociations            *assoc;
    QTreeWidgetItem             *item;
    SoSwitch                    *node;
    SoSeparator                 *sep;
    std::string                 collectionName;
    int                         groupIndex;
    size_t                      style;
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
    IgArchive                   *archive;
    IgMember                    *contents;
  };

  struct FilterSpec
  {
    std::string                 friendlyName;
    std::string                 collection;
    std::vector<std::string>    requiredFields;
  };
  
  struct Filter
  {
    FilterSpec                  *spec;
    IgCollection                *data[2];
    std::string                 collectionName;
    bool                      	result;
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
  typedef std::vector<bool>             Visibilities;
  typedef std::map<std::string, size_t> VisibilityGroupMap;
  typedef std::vector<Filter>           Filters;
  typedef std::vector<FilterSpec>       FilterSpecs;


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

  int                   getCollectionIndex(QTreeWidgetItem *item);

  void                  displayCollection(Collection &c);
  void                  createStats(void);
  IgArchive *           loadFile(const char *fileName);
  void                  readData(IgDataStorage *to, IgArchive *archive,
                                 IgMember *source);
  void                  downloadFile(const QUrl &url);
  void                  downloadGeometry(void);
  bool                  simpleOpen(const QString &fileName);
  void                  setupActions(void);
  void                  restoreCameraFromSpec(CameraSpec *spec, Camera &camera);
  void                  filter(const char *friendlyName,
			       const char *collectionSpec);
  bool                  filter(void);
  void			filterEvent(void);
  bool                  doFilterCollection(const Collection &collection, const char *algoName, const char *result);
  void                  doUpdateFilterListModel(const Collection &collection);
  // Helper methods to handle rendering styles.
  SoMarkerSet::MarkerType   getMarkerType(enum ISPY_MARKER_STYLE style,
                                          enum ISPY_MARKER_SIZE size,
                                          enum ISPY_MARKER_SHAPE shape);
  void                      style(const char *rule, const char *css);
  void                      resetStyleStack(size_t level);
  void                      parseCss(const char *css);
  bool                      parseCssFile(const char *filename);
  size_t                    findStyle(const char *pattern);
  // Helper methods to handle views layouts.
  void                      parseViewsDefinition(const char *data);
  bool                      parseViewsDefinitionFile(const char *filename);

  void                      registerDrawFunction(const char *name, Make3D func);
  void                      registerDrawFunctions(void);
  
  // Tells the viewer that the label to be used for autoprinting is changed.
  void                      updateEventMessage(void);

  int                   m_argc;
  char                  **m_argv;
  char                  *m_appname;
  IgArchive             *m_archives[2];
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
  FilterSpecs         	m_filterSpecs;
  Filters             	m_filters;

  IgCollectionListModel *m_listModel;
  IgCollectionTableModel *m_tableModel;
  QSortFilterProxyModel *m_tableProxyModel;
  Ig3DBaseModel         *m_3DModel;
  ISpy3DView            *m_viewer;
  ISpyMainWindow        *m_mainWindow;
  QTreeWidget           *m_treeWidget;
  ISpySplashScreen      *m_splash;
  ISpyEventSelectorDialog *m_selector;
  bool                   m_nextEvent;
    
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
  QFileSystemWatcher    *m_fileWatcher;
  ISpyPicturePublishingDialog *m_pubDialog;
  QTimer		*m_printTimer;
  QString		m_metaData;
  QProgressDialog       *m_filterProgressDialog;
  int			m_counter;

  class MatchByName {
  public:
    MatchByName(std::string name) : m_name(name) {}
    bool operator()(Collection &collection) const { return (m_name.compare(collection.collectionName) == 0); }
  private:
    std::string m_name;
  };
 
  class MatchByFriendlyName {
  public:
    MatchByFriendlyName(std::string name) : m_name(name) {}
    bool operator()(CollectionSpec &spec) const { return (m_name.compare(spec.friendlyName) == 0); }
  private:
    std::string m_name;
  };

  //
  // Data concerning rendering style handling.
  //
  typedef std::vector<StyleSpec> StyleSpecs;
  typedef std::vector<Style>     Styles;
  typedef std::vector<size_t>    StylesMap;


  // Storage for all the available styles.
  StyleSpecs            m_styleSpecs;
  // Actually active styles.
  Styles                m_styles;
  // Mapping between a StyleSpec and an active style, so that we avoid
  // creating the latter more than once.
  // We use MAX_SIZE to indicate that there is no mapping yet.
  StylesMap             m_stylesMap;

  // Name of the css file to use to read style information from. Since
  // it's cascading it's contents get appended at the end of the default ones.
  std::string           m_cssFilename;
  // The index of the last StyleSpec which was created by the default style
  // sheet. This is used to revert to defaults in case the style passed on 
  // command line is not correct.
  // Reverting to defaults only implies resizing m_styleSpecs to this
  // value. All the rest is handled automatically in findStyle.
  size_t                m_defaultStyleLevel;
  
  
  //
  // Data concerning the view layout definition.
  //
  typedef std::map<std::string, Make3D> DrawFunctionsRegistry;
  
  // Lookup table for the available draw functions.
  DrawFunctionsRegistry   m_drawFunctions;
  // Name of the view layout file to read. The file has to contain the full 
  // view description.
  std::string             m_viewsLayoutFilename;
  
  // Hold the name of the current event.
  QString                 m_eventName;
};

#endif // IGUANA_ISPY_APPLICATION_H
