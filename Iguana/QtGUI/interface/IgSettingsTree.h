#ifndef IGGI_IG_SETTINGS_TREE_H
# define IGGI_IG_SETTINGS_TREE_H

//<<<<<< INCLUDES                                                       >>>>>>

#include <QIcon>
#include <QTimer>
#include <QTreeWidget>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QSettings;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgSettingsTree : public QTreeWidget
{
    Q_OBJECT
public:
    IgSettingsTree (QWidget *parent = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    void 		setSettingsObject(QSettings *settings);
    QSize 		sizeHint(void) const;

public slots:
    void 		setAutoRefresh(bool autoRefresh);
    void 		setFallbacksEnabled(bool enabled);
    void 		maybeRefresh(void);
    void 		refresh(void);

protected:
    bool 		event(QEvent *event);

private slots:
    void 		updateSetting(QTreeWidgetItem *item);

private:
    void 		updateChildItems(QTreeWidgetItem *parent);
    QTreeWidgetItem    *createItem(const QString &text, QTreeWidgetItem *parent,
				   int index);
    QTreeWidgetItem    *childAt(QTreeWidgetItem *parent, int index);
    int 			childCount(QTreeWidgetItem *parent);
    int 		findChild(QTreeWidgetItem *parent, const QString &text, int startIndex);
    void 		moveItemForward(QTreeWidgetItem *parent, int oldIndex, int newIndex);

    QSettings 	       *settings_;
    QTimer 		refreshTimer_;
    bool 		autoRefresh_;
    QIcon 		groupIcon_;
    QIcon 		keyIcon_;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IG_SETTINGS_TREE_H
