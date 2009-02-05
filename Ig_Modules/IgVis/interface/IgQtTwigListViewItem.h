#ifndef IG_CMSCAN_IG_QT_TWIG_LIST_VIEW_ITEM_H
# define IG_CMSCAN_IG_QT_TWIG_LIST_VIEW_ITEM_H

#include "Ig_Modules/IgVis/interface/config.h"
# include "Ig_Extensions/IgQt/interface/AdvListViewItem.h"

class IgQtTwig;
class IgQtTwigController;

/*!	\class  IgQtTwigListViewItem IgQtTwigListViewItem.h Ig_Modules/IgVis/interface/IgQtTwigListViewItem.h
 *
 *  \brief  Items controlled by a IgQtTwigController
 *
 *  \author I. Gaponenko, Northeastern University
 *
 */

class IG_VIS_API IgQtTwigListViewItem : public AdvListViewItem
{
    friend class IgQtTwigController;
public:
    IgQtTwigListViewItem (IgQtTwig *t, IgQtTwigController *parent);
    IgQtTwigListViewItem (IgQtTwig *t, IgQtTwigController *parent,
			  QString string,
			  QString s2=QString::null, QString s3=QString::null, 
			  QString s4=QString::null, QString s5=QString::null, 
			  QString s6=QString::null, QString s7=QString::null,
			  QString s8=QString::null);
    IgQtTwigListViewItem (IgQtTwig *t, IgQtTwigListViewItem *parent);
    IgQtTwigListViewItem (IgQtTwig *t, IgQtTwigListViewItem *parent,
			  QString string,  
			  QString s2=QString::null, QString s3=QString::null, 
			  QString s4=QString::null, QString s5=QString::null, 
			  QString s6=QString::null, QString s7=QString::null,
			  QString s8=QString::null);

    void		setParentTwig (IgQtTwig *pTwig);

private:
    IgQtTwig *		parentTwig;
};       


inline
IgQtTwigListViewItem::IgQtTwigListViewItem (IgQtTwig *t,
					    IgQtTwigController *parent)
    : AdvListViewItem (parent),
      parentTwig(t)
{}

inline
IgQtTwigListViewItem::IgQtTwigListViewItem (IgQtTwig *t,
					    IgQtTwigController *parent,
					    QString string,
					    QString s2, QString s3, 
					    QString s4, QString s5, 
					    QString s6, QString s7, QString s8)
    : AdvListViewItem (parent, string, s2, s3, s4, s5, s6, s7, s8),
      parentTwig(t)
{}

inline
IgQtTwigListViewItem::IgQtTwigListViewItem (IgQtTwig *t,
					    IgQtTwigListViewItem *parent)
    : AdvListViewItem(parent), parentTwig(t)
{}

inline
IgQtTwigListViewItem::IgQtTwigListViewItem (IgQtTwig *t,
					    IgQtTwigListViewItem *parent,
					    QString string,  
					    QString s2, QString s3, 
					    QString s4, QString s5, 
					    QString s6, QString s7, QString s8)
    : AdvListViewItem (parent, string, s2, s3, s4, s5, s6, s7, s8),
      parentTwig(t)
{}

inline void
IgQtTwigListViewItem::setParentTwig (IgQtTwig *pTwig)
{ parentTwig = pTwig; }

#endif // IG_CMSCAN_IG_QT_TWIG_LIST_VIEW_ITEM_H

