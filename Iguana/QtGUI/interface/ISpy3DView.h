#ifndef QT_GUI_ISPY_3D_VIEW_H
# define QT_GUI_ISPY_3D_VIEW_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
# include <QObject>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QToolBar;
class QWidget;
class Ig3DBaseModel;
class SoFieldSensor;
class QAction;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class ISpy3DView : public QObject,
                   public SoQtExaminerViewer
{
  Q_OBJECT
public:
  ISpy3DView(Ig3DBaseModel *model, QWidget *parent);
  // implicit copy constructor
  // implicit assignment operator
  // implicit destructor
  ~ISpy3DView(void);

  virtual Ig3DBaseModel *       model(void) const;
  virtual QWidget *             parent(void) const;
  virtual QToolBar *            toolBar(void) const;
  virtual void                  initCamera(void);

  virtual void   printBitmap(QString file, float ppi,
                             float dpi, QString format);

  static bool    saveNode(SoNode *node, const QString& title,
                          QWidget *parent = 0, const char* file = 0);
  static bool    writeNode(SoNode *node, const QString& file, bool binary,
                           QWidget *parent = 0);

public slots:
  void         save(void);
  void         print(void);
  void         zoomIn(void);
  void         zoomOut(void);
  void         zoom(const float diffvalue);
  void         resetToHomePosition(void);
  void         saveHomePosition(void);
  void         setCameraType(QAction *action);
  void         viewAll(void);
  void         seek(void);
  void         setFeedbackVisibility(bool enable);
  virtual void setGridVisibility(bool enable);
  void         view(void);
  void         pick(void);
  void         setWhatsThisPicking(bool enable = true);
  virtual void autoPrint(void);
  virtual void autoPrint(const std::string text);
  virtual void viewPlaneX(void);
  virtual void viewPlaneY(void);
  virtual void viewPlaneZ(void);
  virtual void toggleCameraType(void);
  virtual void invertCamera(void);

signals:
  void  cameraToggled(void);

protected:
  virtual void    initWidget(void);

  static SbBool   eventCallback(void *closure, QEvent *event);

private:
  //FIXME: remove these Callback method once the SoQt fixes the continuous
  //rendering problem
  static void           farDistanceSensorCB(void *me, SoSensor *sensor);
  static void           nearDistanceSensorCB(void *me, SoSensor *sensor);

  virtual void          drawGrid(const bool enable);
  void                  setupActions(void);
  SoNode *              findGroup(SoNode *node, const char* name);

  QWidget              *m_parent;
  QToolBar             *m_toolBar;
  Ig3DBaseModel        *m_model;
  bool                  m_whatsThisPicking;
  bool                  m_grid;
  bool                  m_oldView;
  bool                  m_oldSeek;

  // undefined semantics
  ISpy3DView(const ISpy3DView &);
  ISpy3DView &operator=(const ISpy3DView &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // QT_GUI_ISPY_3D_VIEW_H
