#include "Iguana/QtGUI/src/ISpyEventSelectorDialog.h"
#include "Iguana/QtGUI/src/IgCollectionListModel.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include <QtGui>

ISpyEventSelectorDialog::ISpyEventSelectorDialog (QWidget *parent)
  : QDialog(parent),
    m_proxyModel(new QSortFilterProxyModel(this))
{
  setupUi(this);
  toolBox->clear();
    
  m_l1ListView = new QTreeView(l1MenuPage);
  m_l1ListView->setRootIsDecorated(false);
  m_l1ListView->setAlternatingRowColors(true);
  m_l1ListView->setModel(m_proxyModel);
  m_l1ListView->setSortingEnabled(true);

  toolBox->addTab(m_l1ListView, "Provenance/L1 Triggers");
  
  // FIXME IVO: Get a default/previously stored value
  // for the first page
  m_filterText << "";
  m_hltListView = new QTreeView(hltMenuPage);
  m_hltListView->setRootIsDecorated(false);
  m_hltListView->setAlternatingRowColors(true);
  m_hltListView->setModel(m_proxyModel);
  m_hltListView->setSortingEnabled(true);

  toolBox->addTab(m_hltListView, "Provenance/HLT Trigger Paths");
  
  // FIXME IVO: Get a default/previously stored value
  // for the second page
  m_filterText << "";
  m_selectionModel = m_l1ListView->selectionModel();

  QObject::connect(toolBox, SIGNAL(currentChanged(int)), this, SLOT(updatePage(int)));
  QObject::connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(updateSelection()));
  QObject::connect(lineEdit, SIGNAL(textEdited(QString)), this, SLOT(updateFilterText(QString)));
  QObject::connect(m_l1ListView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(updateSelection()));
  QObject::connect(m_hltListView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(updateSelection()));
  QObject::connect(m_l1ListView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(enableButtons()));
  QObject::connect(m_hltListView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(enableButtons()));
  QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateDialog()));
  buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Add"));
  buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);  
  buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Close"));
  checkBox->hide();
}

const QString
ISpyEventSelectorDialog::pageTitle(int index)
{
  return toolBox->tabText(index);
}

const QString
ISpyEventSelectorDialog::filterText(const QString& name)
{
  int i = pageIndex(name);
  if(! m_filterText.empty() && i != -1) 
    return m_filterText.at(i);

  return QString();
}

void
ISpyEventSelectorDialog::setModel(QAbstractItemModel * sourceModel)
{
  m_proxyModel->setSourceModel(sourceModel);
}

void
ISpyEventSelectorDialog::updateDialog(void)
{
  m_currentPage = dynamic_cast<QTreeView *>(toolBox->widget(toolBox->currentIndex()));
  lineEdit->setText(m_filterText.at(toolBox->currentIndex()));
  if(comboBox->currentIndex() == 0)
  {    
    lineEdit->setReadOnly(false);
    lineEdit->setText(m_filterText.at(toolBox->currentIndex()));
  }  
  else
  {
    lineEdit->setReadOnly(true);
    lineEdit->setText(m_filterText.join(")&(").append(")").prepend("(")); 
  }
  
  m_selectionModel  = m_currentPage->selectionModel();
  
  emit pageChanged(pageTitle(toolBox->currentIndex()));
}

void
ISpyEventSelectorDialog::updatePage(int /* index */)
{
  updateDialog();
}

void
ISpyEventSelectorDialog::updateSelection(void)
{
  QModelIndexList indexes = m_selectionModel->selectedIndexes();
  QModelIndex index;

  QString current(m_filterText.at(toolBox->currentIndex()));
  
  foreach(index, indexes) {
    QString selected(m_proxyModel->data(index, Qt::DisplayRole).toString());
    if(!current.contains(selected))
    {
      if(!current.isEmpty())
	current.append("|");
      current.append(selected);
    }    
  }
  m_filterText.replace(toolBox->currentIndex(),current);
  buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);  

  updateDialog();
}

void
ISpyEventSelectorDialog::updateFilterText(const QString& text)
{
  m_filterText.replace(toolBox->currentIndex(),text);
}

void
ISpyEventSelectorDialog::enableButtons(void)
{
  buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);  
}

int
ISpyEventSelectorDialog::pageIndex(const QString& name)
{
  for(int i = 0; i < toolBox->count(); ++i)
    if(toolBox->tabText(i) == name)
      return i;
  
  return -1;
}
