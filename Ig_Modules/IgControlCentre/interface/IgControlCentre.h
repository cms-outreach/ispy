/****************************************************************************
** Form interface generated from reading ui file 'IgControlCentre.ui'
**
** Created: Fri Oct 31 14:22:32 2003
**      by: The User Interface Compiler ($Id: IgControlCentre.h,v 1.12 2005/02/07 09:16:07 muzaffar Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef IGCONTROLCENTRE_H
#define IGCONTROLCENTRE_H

# include "Ig_Modules/IgControlCentre/interface/config.h"
# include <qvariant.h>
# include <qdialog.h>
# include <qstring.h>
# include <map>
# include <vector>

class IgControlCategory;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QIconView;
class QIconViewItem;
class QPushButton;
class IgState;
class QFocusEvent;

class IG_CONTROL_CENTRE_API IgControlCentre : public QDialog
{
    Q_OBJECT

public:
    IgControlCentre (QWidget* parent = 0, 
		     const char* name = 0, 
		     bool modal = FALSE, 
		     WFlags fl = 0);
    ~IgControlCentre (void);

    void		addCategory (IgControlCategory *category);
    void      		removeCategory (IgControlCategory *category);
    void       		selectCategory (IgControlCategory *category);
    void       		selectCategory (const QString& name);

    QIconView		*iconView (void);
    
    // Overrides from base class.
    virtual void	show (void);
    virtual void	hide (void);
    void		updateOnWindowActive (void);
    
public slots:
    virtual void 	apply (void);
    virtual void 	revert (void);
    virtual void 	help (void);
    virtual void 	back (void);
    virtual void	enableCategory (IgControlCategory *category,
                                        bool state = true);
					
private slots:
    void       		selectIconItem (QIconViewItem*);
    void    		dirtyCategory (void);
    void		cleanCategory (void);
    void		showCategory (IgControlCategory *category);
    void		hideCategory (void);
    void		addIconViewItems (QString name);   
    void		showIconView (QString name);   
    void		hideIconView (void);

protected:
    void 		windowActivationChange  (bool oldActive);
    
    QGridLayout		*IgControlCentreLayout;
    QHBoxLayout		*m_layout1;
    QGridLayout		*m_layout2;

protected slots:
    virtual void 	languageChange();

private:
    typedef std::map<QString, std::pair<IgControlCategory *, bool> > CategoryMap;
    typedef std::vector<QIconViewItem*> IconViewList;
    typedef std::map<QString, std::pair<void*, bool> > IconViewArch;
    void		updateControlCentre (QString name);
    void 		removeCategory (QString name, IconViewArch *toplevel);
    bool 		hasCategory (const IgControlCategory *category);
    void		deleteIconViewArch (IconViewArch *toplevel);
    void		enableCategory (QString name,
                                        IconViewArch *toplevel,
					bool state = true);
					  
    IconViewArch	m_iconViewArch;
    CategoryMap		m_categories;
    IconViewList	m_iconViewList;
    IgControlCategory	*m_current;

    QIconView		*m_iconView;
    QPushButton		*m_close;
    QPushButton		*m_revert;
    QPushButton		*m_apply;
    QPushButton		*m_help;
    QPushButton		*m_back;
    QString		m_name;
    QString		m_baseName;
    bool		m_updateOnWindowActive;
    bool		m_focusChanged;
};

#endif // IGCONTROLCENTRE_H
