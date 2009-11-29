#ifndef QT_GUI_ISPY_LIST_VIEW_H
# define QT_GUI_ISPY_LIST_VIEW_H

# include "ui/ui_ISpyListView.h"
# include <QTreeView>
# include <QObject>

class ISpyListView : public QTreeView, private Ui::ISpyListView
{
public:
  ISpyListView (QWidget *parent = 0);
};

#endif // QT_GUI_ISPY_LIST_VIEW_H
