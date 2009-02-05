#ifndef IG_QT_IG_QT_LINE_EDIT_H
# define IG_QT_IG_QT_LINE_EDIT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Extensions/IgQt/interface/config.h"
# include <qlineedit.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_API IgQtLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    IgQtLineEdit (QWidget *parent, const char *name = 0);
    IgQtLineEdit (const QString &contents,
		  QWidget	*parent,
		  const char	*name = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    virtual void	changed (void);
    virtual void	changed (const QString &text);
    virtual void	setChanged (bool option = true);

signals:
    void		valueChanged (const QString &text);

protected:
    void		focusOutEvent (QFocusEvent *event);

private:
    bool		m_textChanged;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_IG_QT_LINE_EDIT_H
