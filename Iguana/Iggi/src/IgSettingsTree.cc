//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Iggi/interface/IgSettingsTree.h"
#include "Iguana/Iggi/interface/IgVariantDelegate.h"
#include <QtGui>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgSettingsTree::IgSettingsTree (QWidget *parent)
    : QTreeWidget(parent)
{
    setItemDelegate(new IgVariantDelegate(this));

    QStringList labels;
    labels << tr("Setting") << tr("Type") << tr("Value");
    setHeaderLabels(labels);
    header()->setResizeMode(0, QHeaderView::Stretch);
    header()->setResizeMode(2, QHeaderView::Stretch);

    settings = 0;
    refreshTimer.setInterval(2000);
    autoRefresh = false;

    groupIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
			QIcon::Normal, QIcon::Off);
    groupIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
			QIcon::Normal, QIcon::On);
    keyIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

    connect(&refreshTimer, SIGNAL(timeout()), this, SLOT(maybeRefresh()));
}

void 
IgSettingsTree::setSettingsObject(QSettings *settings)
{
    if (! this->settings)
	delete this->settings;
    this->settings = settings;
    clear();

    if (settings) {
	settings->setParent(this);
	refresh();
	if (autoRefresh)
	    refreshTimer.start();
    } else {
	refreshTimer.stop();
    }
}

QSize 
IgSettingsTree::sizeHint() const
{
    return QSize(800, 600);
}

void 
IgSettingsTree::setAutoRefresh(bool autoRefresh)
{
    this->autoRefresh = autoRefresh;
    if (settings) {
	if (autoRefresh) {
	    maybeRefresh();
	    refreshTimer.start();
	} else {
	    refreshTimer.stop();
	}
    }
}

void 
IgSettingsTree::setFallbacksEnabled(bool enabled)
{
    if (settings) {
	settings->setFallbacksEnabled(enabled);
	refresh();
    }
}

void 
IgSettingsTree::maybeRefresh()
{
    if (state() != EditingState)
	refresh();
}

void 
IgSettingsTree::refresh()
{
    if (!settings)
	return;

    disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem *, int)),
	       this, SLOT(updateSetting(QTreeWidgetItem *)));

    settings->sync();
    updateChildItems(0);

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem *, int)),
	    this, SLOT(updateSetting(QTreeWidgetItem *)));
}

bool 
IgSettingsTree::event(QEvent *event)
{
    if (event->type() == QEvent::WindowActivate) {
	if (isActiveWindow() && autoRefresh)
	    maybeRefresh();
    }
    return QTreeWidget::event(event);
}

void 
IgSettingsTree::updateSetting(QTreeWidgetItem *item)
{
    QString key = item->text(0);
    QTreeWidgetItem *ancestor = item->parent();
    while (ancestor) {
	key.prepend(ancestor->text(0) + "/");
	ancestor = ancestor->parent();
    }

    settings->setValue(key, item->data(2, Qt::UserRole));
    if (autoRefresh)
	refresh();
}

void 
IgSettingsTree::updateChildItems(QTreeWidgetItem *parent)
{
    int dividerIndex = 0;

    foreach (QString group, settings->childGroups()) {
	QTreeWidgetItem *child;
	int childIndex = findChild(parent, group, dividerIndex);
	if (childIndex != -1) {
	    child = childAt(parent, childIndex);
	    child->setText(1, "");
	    child->setText(2, "");
	    child->setData(2, Qt::UserRole, QVariant());
	    moveItemForward(parent, childIndex, dividerIndex);
	} else {
	    child = createItem(group, parent, dividerIndex);
	}
	child->setIcon(0, groupIcon);
	++dividerIndex;

	settings->beginGroup(group);
	updateChildItems(child);
	settings->endGroup();
    }

    foreach (QString key, settings->childKeys()) {
	QTreeWidgetItem *child;
	int childIndex = findChild(parent, key, 0);

	if (childIndex == -1 || childIndex >= dividerIndex) {
	    if (childIndex != -1) {
		child = childAt(parent, childIndex);
		for (int i = 0; i < child->childCount(); ++i)
		    delete childAt(child, i);
		moveItemForward(parent, childIndex, dividerIndex);
	    } else {
		child = createItem(key, parent, dividerIndex);
	    }
	    child->setIcon(0, keyIcon);
	    ++dividerIndex;
	} else {
	    child = childAt(parent, childIndex);
	}

	QVariant value = settings->value(key);
	if (value.type() == QVariant::Invalid) {
	    child->setText(1, "Invalid");
	} else {
	    child->setText(1, value.typeName());
	}
	child->setText(2, IgVariantDelegate::displayText(value));
	child->setData(2, Qt::UserRole, value);
    }

    while (dividerIndex < childCount(parent))
	delete childAt(parent, dividerIndex);
}

QTreeWidgetItem *
IgSettingsTree::createItem(const QString &text,
			   QTreeWidgetItem *parent, int index)
{
    QTreeWidgetItem *after = 0;
    if (index != 0)
	after = childAt(parent, index - 1);

    QTreeWidgetItem *item;
    if (parent)
	item = new QTreeWidgetItem(parent, after);
    else
	item = new QTreeWidgetItem(this, after);

    item->setText(0, text);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    return item;
}

QTreeWidgetItem *
IgSettingsTree::childAt(QTreeWidgetItem *parent, int index)
{
    if (parent)
	return parent->child(index);
    else
	return topLevelItem(index);
}

int 
IgSettingsTree::childCount(QTreeWidgetItem *parent)
{
    if (parent)
	return parent->childCount();
    else
	return topLevelItemCount();
}

int 
IgSettingsTree::findChild(QTreeWidgetItem *parent, const QString &text,
			  int startIndex)
{
    for (int i = startIndex; i < childCount(parent); ++i) 
    {
	if (childAt(parent, i)->text(0) == text)
	    return i;
    }
    return -1;
}

void 
IgSettingsTree::moveItemForward(QTreeWidgetItem *parent, int oldIndex,
				int newIndex)
{
    for (int i = 0; i < oldIndex - newIndex; ++i)
	delete childAt(parent, newIndex);
}
