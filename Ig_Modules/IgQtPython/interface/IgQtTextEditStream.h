#ifndef IG_QT_PYTHON_IG_QT_TEXT_EDIT_STREAM_H
# define IG_QT_PYTHON_IG_QT_TEXT_EDIT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtPython/interface/config.h"
# include "Ig_Modules/IgQtPython/interface/IgQtTextEditStreambuf.h"
# include <qtextedit.h>
# include <iostream>
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgQtTextEditStreambuf;
class QWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
class IG_QT_PYTHON_API IgQtTextEditStream : public QTextEdit, public std::ostream
{
private:
    QWidget *m_parent;    
    IgQtTextEditStreambuf m_streamBuf;
    int m_streamStyle;
    bool m_waitToFlush;
public:
    
    IgQtTextEditStream (QWidget *parent, const int streamStyle,const bool waitToFlush = false);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    void realFlush(void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_PYTHON_IG_QT_TEXT_EDIT_STREAM_H
