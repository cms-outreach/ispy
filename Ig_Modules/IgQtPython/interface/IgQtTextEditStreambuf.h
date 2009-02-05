#ifndef IG_QT_PYTHON_IG_QT_TEXT_EDIT_STREAMBUF_H
# define IG_QT_PYTHON_IG_QT_TEXT_EDIT_STREAMBUF_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtPython/interface/config.h"
# include <sstream>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QTextEdit;
class QColor;
class IgQtDecorator;
class IgPythonInterpreter;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/**\brief std::streambuf which flushes to a wanted QTextEditWidget.
   \warning I need to create a python interpreter for the creation of the decoration class,
   this is because there is not a global python interpreter...We must think about this...
 */
class IG_QT_PYTHON_API IgQtTextEditStreambuf : public std::stringbuf
{
private:
    QTextEdit *m_widget;
    int m_streamStyle;    
    bool m_waitToSync;
    IgQtDecorator *m_decorator;
    IgPythonInterpreter *m_decoratorInterpreter;
public:
    /** 
	\param widget QTextWidget to which append streamed contents.
	\param streamStyle style in which to decorate the streamed contents. For the Python shell: 0 input, 1 output, 2 error.
	\param waitToSync is used to avoid syncing on stream::flush() but waits for realSync. This is necessary because Python
	uses std::endl in place of \n forcing a flush for each line.
     */
    IgQtTextEditStreambuf(QTextEdit *widget, const int streamStyle, const bool waitToSync = false);
    /**syncs() the streambuf before deletion. 
     */
    ~IgQtTextEditStreambuf();
  
    /**If waitToSync==false syncs the streamed contents to the widget. Otherwise does nothing.
     */
    int sync();
    /** Syncs the streamed contents to the widget
     */
    void realSync();    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_PYTHON_IG_QT_TEXT_EDIT_STREAMBUF_H
