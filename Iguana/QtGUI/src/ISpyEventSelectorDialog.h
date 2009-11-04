#ifndef QT_GUI_ISPY_EVENT_SELECTOR_DIALOG_H
# define QT_GUI_ISPY_EVENT_SELECTOR_DIALOG_H

# include "ui/ui_ISpyEventSelectorDialog.h"

class IgCollectionListModel;
class IgCollection;
class QTreeView;
class QSortFilterProxyModel;
class ISpyListView;

class ISpyEventSelectorDialog : public QDialog,
				private Ui::ISpyEventSelectorDialog
{
  Q_OBJECT
public:
  ISpyEventSelectorDialog (QWidget *parent = 0);

  void			setModel(QAbstractItemModel * model);
  const QString 	filterText(const QString &page);

public slots:
  void 			updateDialog(void);
  
signals:
  void			pageChanged(QString title);

private slots:
  void			updateSelection(void);
  void			updatePage(int);
  void			updateFilterText(const QString& text);
  void			enableButtons(void);

private:
  int			pageIndex(const QString& name);
  const QString 	pageTitle(int index);

  QTreeView 	       	*m_currentPage;
  QSortFilterProxyModel *m_proxyModel;
  QItemSelectionModel 	*m_selectionModel;
  QStringList		m_filterText;

  QTreeView		*m_l1ListView;
  QTreeView		*m_hltListView;
};

#endif // QT_GUI_ISPY_EVENT_SELECTOR_DIALOG_H
