#ifndef QT_GUI_ISPY_EVENT_FILTER_H
# define QT_GUI_ISPY_EVENT_FILTER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <QObject>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class ISpyEventFilter : public QObject
{
  Q_OBJECT
public:
  bool eventFilter(QObject *obj, QEvent *event);

signals:
  void          open(const QString &fileName);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // QT_GUI_ISPY_EVENT_FILTER_H
