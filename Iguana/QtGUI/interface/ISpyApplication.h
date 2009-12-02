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
# include <Inventor/nodes/SoMarkerSet.h>
# include <Inventor/nodes/SoDrawStyle.h>
# include <Inventor/nodes/SoText2.h>
# include <Inventor/SbViewportRegion.h>

# include "Iguana/QtGUI/interface/ISpyConsumerThread.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgCollection;
class IgAssociationSet;
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
class SoDrawStyle;
class SoFont;
class ISpyPicturePublishingDialog;
class SoTexture2;

namespace lat
{
  class ZipArchive;
  class ZipMember;
}

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

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

enum ISPY_ANNOTATION_LEVEL {
  ISPY_ANNOTATION_LEVEL_NONE,
  ISPY_ANNOTATION_LEVEL_PRESS,
  ISPY_ANNOTATION_LEVEL_NORMAL,
  ISPY_ANNOTATION_LEVEL_FULL
};

class ISpyApplication : public QObject
{
  Q_OBJECT
public:

  // The style structure is used to keep track of the context used to render
  // a collection. It includes stuff that is graphics related (like the font
  // and the material) and stuff which is physics related (like min-energy, 
  // max-pt, etc).
  struct Style
  {
    size_t                      spec;
    SoMaterial                  *material;
    SoDrawStyle                 *drawStyle;
    SoFont                      *font;
    SoMarkerSet::MarkerType     markerType;
    SbViewportRegion            viewport;
    SoText2::Justification      textAlign;
    double                      minEnergy;
    double                      maxEnergy;
    double                      energyScale;
    SoTexture2                  *background;
    ISPY_ANNOTATION_LEVEL       annotationLevel;
    double                      left;
    double                      top;
  };

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
  void                  openWithFallbackGeometry(const QString &fileName);
  void                  connect(void);
  void                  autoEvents(void);
  void                  animateViews(void);
  void                  nextEvent(void);
  void                  previousEvent(void);
  void                  showAbout(void);
  void                  newEvent(void);
  void 			autoPrint(void);

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
  bool                  backgroundFileDownloaded(IgNetworkReplyHandler *handler);
  void                  setProgress(qint64 current, qint64 final);
  void                  handleDownloadError(IgNetworkReplyHandler *handler);
  void                  switchView(int i);
  void                  cameraToggled(void);
  void                  resetToHomePosition(void);
  void                  restartPlay(void);
  void			updateFilterListModel(const QString& title);
  void			showPublish(void);
  void			stopFiltering(void);

private:
  typedef void(*Make3D)(IgCollection **, IgAssociationSet **,
                        SoSeparator *, Style *);

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

  enum ISPY_DRAW_STYLE {
    ISPY_SOLID_DRAW_STYLE = SoDrawStyle::FILLED,
    ISPY_LINES_DRAW_STYLE = SoDrawStyle::LINES,
    ISPY_POINTS_DRAW_STYLE = SoDrawStyle::POINTS
  };

  static const size_t ISPY_MARKER_STYLES = 2;

  enum ISPY_MARKER_STYLE {
    ISPY_OUTLINE_MARKER_STYLE = 0,
    ISPY_FILLED_MARKER_STYLE  = 1
  };

  static const size_t ISPY_MARKER_SIZES  = 3;

  enum ISPY_MARKER_SIZE {
    ISPY_NORMAL_MARKER_SIZE = 0,
    ISPY_BIG_MARKER_SIZE    = 1 * ISPY_MARKER_STYLES,
    ISPY_HUGE_MARKER_SIZE   = 2 * ISPY_MARKER_STYLES,
  };

  enum ISPY_MARKER_SHAPE {
    ISPY_SQUARE_MARKER_SHAPE = 0 * (ISPY_MARKER_SIZES * ISPY_MARKER_STYLES),
    ISPY_CROSS_MARKER_SHAPE  = 1 * (ISPY_MARKER_SIZES * ISPY_MARKER_STYLES),
    ISPY_PLUS_MARKER_SHAPE   = 2 * (ISPY_MARKER_SIZES * ISPY_MARKER_STYLES),
    ISPY_CIRCLE_MARKER_SHAPE = 3 * (ISPY_MARKER_SIZES * ISPY_MARKER_STYLES)
  };

  enum ISPY_TEXT_ALIGN {
    ISPY_TEXT_ALIGN_LEFT,
    ISPY_TEXT_ALIGN_RIGHT,
    ISPY_TEXT_ALIGN_CENTER
  };
  
  struct StyleSpec
  {
    std::string                 viewName;
    std::string                 collectionName;
    std::string                 background;
    float                       diffuseColor[3];
    float                       transparency;
    float                       lineWidth;
    unsigned int                linePattern;
    float                       fontSize;
    std::string                 fontFamily;
    ISPY_DRAW_STYLE             drawStyle;
    ISPY_MARKER_SHAPE           markerShape;
    ISPY_MARKER_SIZE            markerSize;
    ISPY_MARKER_STYLE           markerStyle;
    ISPY_TEXT_ALIGN             textAlign;
    ISPY_ANNOTATION_LEVEL       annotationLevel;
    double                      minEnergy;
    double                      maxEnergy;
    double                      energyScale;
    double                      left;
    double                      top;
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
    lat::ZipArchive             *archive;
    lat::ZipMember              *contents;
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
  typedef std::vector<Qt::CheckState>   Visibilities;
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
  void                      parseCss(const char *css);
  bool                      parseCssFile(const char *filename);
  size_t                    findStyle(const char *pattern);
  
  // Helper methods to handle views layouts.
  void                      parseViewsDefinition(QByteArray &data);
  bool                      parseViewsDefinitionFile(const char *filename);
  void                      registerDrawFunctions(void);

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
  typedef std::map<size_t, size_t> StylesMap;


  // Storage for all the available styles.
  StyleSpecs            m_styleSpecs;
  // Actually active styles.
  Styles                m_styles;
  // Mapping between a StyleSpec and an active style, so that we avoid
  // creating the latter more than once.
  StylesMap             m_stylesMap;
  // Name of the css file to use to read style information from. Since
  // it's cascading it's contents get appended at the end of the default ones.
  std::string           m_cssFilename;
  
  //
  // Data concerning the view layout definition.
  //
  typedef std::map<std::string, Make3D> DrawFunctionsRegistry;
  
  // Lookup table for the available draw functions.
  DrawFunctionsRegistry   m_drawFunctions;
  // Name of the view layout file to read. The file has to contain the full 
  // view description.
  std::string             m_viewsLayoutFilename;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_ISPY_APPLICATION_H
