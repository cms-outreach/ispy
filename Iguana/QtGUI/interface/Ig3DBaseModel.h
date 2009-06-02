#ifndef QT_GUI_IG3DBASE_MODEL_H
# define QT_GUI_IG3DBASE_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SbString;
class SbName;
class SoGroup;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class Ig3DBaseModel
{
public:
    Ig3DBaseModel (void);
    ~Ig3DBaseModel (void);
    
    SoGroup *		sceneGraph (void) const;
    SoGroup *		contents (void) const;
    SoGroup *		selection (void) const;

    static SbString     encode (const std::string &name);
    static std::string  decode (const std::string &name);
    static std::string  decode (const SbName &name);

private:
    void 		initScene (SoGroup *root);
    
    SoGroup		*m_sceneGraph;
    SoGroup		*m_contents;
    SoGroup		*m_selection;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // QT_GUI_IG3DBASE_MODEL_H
