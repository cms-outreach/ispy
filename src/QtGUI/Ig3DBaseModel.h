#ifndef QT_GUI_IG3DBASE_MODEL_H
# define QT_GUI_IG3DBASE_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <string>
# include <QObject>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SbString;
class SbName;
class SoGroup;
class SoCamera;
class SoPath;
class SoNode;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class Ig3DBaseModel: public QObject
{
Q_OBJECT
public:
  Ig3DBaseModel(void);
  ~Ig3DBaseModel(void);

  SoGroup *             sceneGraph(void) const;
  SoGroup *             contents(void) const;
  SoGroup *             selection(void) const;
  
  void                  setCamera(SoCamera *camera);

  static SbString     encode(const std::string &name);
  static std::string  decode(const std::string &name);
  static std::string  decode(const SbName &name);

	void update();
	void updateSelection(SoNode* node);

signals:
	void elementSelected(QString name);

private:
  void          initScene(SoGroup *root);

  SoGroup               *m_sceneGraph;
  SoGroup               *m_contents;
  SoGroup               *m_selection;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // QT_GUI_IG3DBASE_MODEL_H
