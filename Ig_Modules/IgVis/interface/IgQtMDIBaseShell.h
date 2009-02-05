#ifndef IG_CMSCAN_IG_QT_MDIBASESHELL_H
#define IG_CMSCAN_IG_QT_MDIBASESHELL_H

#include "Ig_Modules/IgVis/interface/config.h"
#include "Ig_Modules/IgVis/interface/IgQtMDIAbsShell.h"
#include "qmainwindow.h"
#include <list>

/*!
* \class IgQtMDIBaseShell IgQtMDIBaseShell.h Ig_Modules/IgVis/interface/IgQtMDIBaseShell.h
*
*  \brief Generate a Qt-type Multiple Document Interface wrapper for
*  the IgQtBrowser.
*
*  Maintains a list of widgets (IgQtMDITwigs) containing graphical representations
*  of the scene structure exhibited in the IgQtMDIBaseShell. Selecting a node in the widget
*  will highlight it in the scene and vice versa. (Caution: code to implement the resetting
*  of the selected item on reselect has not been completed... presently you need to select another
*  node _from the same_ IgQtTwigs widget for proper functioning in this case.)
*
*  \author G. Alverson, Northeastern University Computing Group
*  \date 22-Aug-2000
*
*/


class SoGroup;
class SoInput;
class IgQtMDITwigs;
class IgQtInfoStream;
class IgQtTwig;
class SoQtFullViewer;
class IgQtClipCtl;
class IgQtVisCtl;
class IgQtAnimCtl;
class IgQtViewpointCtl;
class IgQtEventCtl;
class IgQtMiscCtl;
class QPrinter;
class QPopupMenu;
class IgQtPhysTabs;


class IG_VIS_API IgQtMDIBaseShell : public IgQtMDIAbsShell {

    Q_OBJECT
public:
    //! Create an MDI Shell 
    explicit IgQtMDIBaseShell(IgQtMDI *parent=0, SoQtFullViewer *aViewer=0);
    //! d-tor
    virtual ~IgQtMDIBaseShell();
    //! returns the className
    virtual const QString shellName();
    //! add a Twig to the scene: if a TwigController doesn't exist, it will be built
    void addTwig(IgQtTwig *twig);
    //! add a volatile Twig to the scene. Upon a new event signal, a volatile twig is rebuilt
    void addVolatileTwig(IgQtTwig *twig, const QString &f=QString::null);
    //! add a static Twig 
    void addStaticTwig(IgQtTwig *twig, const QString &f=QString::null);
    //! store parameters into XML
    virtual QDomElement shellToXML( void ) const;
    //! restore parameters from XML
    virtual bool xmlToShell( QDomElement& de );
public slots:
    //! slot to open IV format files
    void openIv();
    //! save current scene slot
    void saveIv();
    //! close the widget
    void closeSlot();
    //! slot to call make a new viewer and attach to this scene
    void cloneSlot();
    //! slot to get a new event. if adorned, create twigWidget & infoWidget
    virtual void eventsSlot();
    //! slot to receive external notice that the request for a new slot 
    //! has been serviced
    virtual void eventNotifySlot();
    //! slot to continuously get new events
    virtual void autoEventsSlot(bool);
    //! slot to continuously get new events
    virtual void autoPrintViewsSlot(bool);
    //! slot to create InfoStream and tree widget
    void adornSlot();
    //! slot to make static objects wireframe style
    void setStaticWireframe(bool);
    //! slot to cutaway a sector of the static objects
    void setCutAway(bool);
    //! slot to _try_ to optimise static objects
    void setOptimiseStatics(bool);
    //! slot to pop-up physics controls
    virtual void generalPhysicsSlot();
    //! slot to pop-up preferences tabs
    virtual void preferencesSlot();
signals:
    void newEvent();

protected:
    //! code to build central widget
    void buildWidget(QWidget *parent);
    //! hidden c-tor with buildnow argument to defer widget creation until the parent is ready
    IgQtMDIBaseShell(IgQtMDI *parent, SoQtFullViewer *aViewer, bool buildnow);
    //! add a sub-scene: 
    void addScene(IgQtTwig *twig, const QString &f=QString::null);
    //! add a TwigWidget to describe the twig structure; a NULL title sets to the present caption
    IgQtMDITwigs* addTwigWidget(IgQtTwig *twig, QString title = NULL, QWidget *ws = NULL);
    //! add an InfoStream text view; a NULL title sets to the present caption
    IgQtInfoStream* addInfoStreamWidget(IgQtTwig *twig, QString title=NULL, QWidget *ws = NULL);
    //! clear the contents of the volatile twigs
    void clearVolatileTwigs(void);
    //! clear and delete the volatile twigs
    void killVolatileTwigs(void);
    //! handler for drag entry (over-ridden from QWidget)
    virtual void dragEnterEvent(QDragEnterEvent* /* event */);
    //! handler for drop after drag (over-ridden from QWidget)
    virtual void dropEvent(QDropEvent* /* event */);
    //! internal routine to open iv files
    void openIvP(const QString f); 
    //! similar to the def within Twig
    typedef std::list<IgQtTwig*> List_of_Twigs; 
    //! list of IgQtTwigs which are rebuilt each event (only one class of event at present... 
    //! one could envisage varing degrees of volatility)
    List_of_Twigs volatileTwigs;
    //! there is only one static twig
    IgQtTwig *staticTwig;
    typedef std::list<IgQtMDITwigs*> List_of_MDITwigs;
    //! list of twigWidgets we are responsible for
    List_of_MDITwigs twigWidgets;
    //! use  IgQtInfostreams to output status messages
    std::list<IgQtInfoStream *> infoList;
    //! this is the upper-most twig that the outside should know about
    IgQtTwig *topTwig;
    //! this twig holds the topTwig plus additional environmental objects
    IgQtTwig *sceneTwig;
    //! file level controls: save as IV, etc.
    QPopupMenu *file;
    //! Physics process controls
    QPopupMenu *physCtl;
    //! environment: clip planes
    IgQtClipCtl *clips;
    //! environment: lights
    IgQtVisCtl *lights;
    //! environment: animators
    IgQtAnimCtl *anim;
    //! environment: viewpoints
    IgQtViewpointCtl *views;
    //! event controls
    IgQtEventCtl *evt;
    //! miscellaneous controls
    IgQtMiscCtl *misc;
    //! the physics process control widget
    IgQtPhysTabs *tabs;
    //! the preferences widget
    IgQtPhysTabs *thePrefWidget;
    //! generate with or without info & TwigWidget
    bool adorn;
    //! saved adornment state during auto event
    bool saved_adorn;
    //! the name
    static const QString theShellName;
    //! the identifier for the print item on the file menu... may need to over-ride later
    int printID;
private:
    //! common construction code
    void constructor(bool build);
};
#endif // IG_CMSCAN_IG_QT_MDIBASESHELL_H



