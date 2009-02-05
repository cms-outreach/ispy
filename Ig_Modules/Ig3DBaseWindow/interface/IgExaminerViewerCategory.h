#ifndef IG_3D_BASE_WINDOW_IG_EXAMINER_VIEWER_CATEGORY_H
# define IG_3D_BASE_WINDOW_IG_EXAMINER_VIEWER_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DBaseWindow/interface/IgCommonViewCategory.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;
class QCheckBox;
class QComboBox;
class IgQtRangeControlFloat;
class IgQtRangeControlUInt;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_BASE_WINDOW_API IgExaminerViewerCategory
    : public IgCommonViewCategory,
      public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgExaminerViewerCategory);    
    Q_OBJECT
public:
    IgExaminerViewerCategory (IgState *state);
    ~IgExaminerViewerCategory ();

    virtual void		browserChanged 	(Ig3DBaseBrowser *browser);    

private slots:
    virtual void		setStillDrawStyle	(int style);
    virtual void		updateStillDrawStyle	(int style);
    virtual void		setInteractiveDrawStyle	(int style);
    virtual void		updateInteractiveDrawStyle(int style);
    virtual void		setBufferType		(int type);
    virtual void		updateBufferType	(int type);
    virtual void		setSeekTime		(float time);
    virtual void		setStereoOffset		(float offset);
    virtual void		setFeedbackSize		(unsigned int size);
    virtual void		setAutoClipping		(bool option);
    virtual void		setAnimation		(bool option);
    virtual void		setStereoViewing	(bool option);
    virtual void		setDetailSeek		(bool option);
    virtual void		setCursor		(bool option);
    
private:
    void			addExaminerViewerTab 	(void);
    void			updateStillDrawStyleHelp(void);
    void			updateInteractiveDrawStyleHelp(void);
    void			updateBufferTypeHelp	(void);
    
    QComboBox			*m_stillDrawStyle;
    QComboBox			*m_interactiveDrawStyle;
    QComboBox			*m_bufferType;
    IgQtRangeControlFloat	*m_seekTime;
    IgQtRangeControlFloat	*m_stereoOffset;
    IgQtRangeControlUInt	*m_feedbackSize;
    QCheckBox			*m_autoClipping;
    QCheckBox			*m_animation;
    QCheckBox			*m_stereoViewing;
    QCheckBox			*m_detailSeek;
    QCheckBox			*m_cursor;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_BASE_WINDOW_IG_EXAMINER_VIEWER_CATEGORY_H
