/****************************************************************************
** Form interface generated from reading ui file 'IgWizardBase.ui'
**
** Created: Fri Mar 1 02:14:58 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef IGWIZARDBASE_H
#define IGWIZARDBASE_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QFrame;
class QLabel;
class QPushButton;

class IgWizardBase : public QDialog
{ 
    Q_OBJECT

public:
    IgWizardBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~IgWizardBase();

    QLabel* m_titlePixmap;
    QLabel* m_title;
    QLabel* m_category;
    QFrame* m_titleSeparator;
    QFrame* m_content;
    QFrame* m_bottomSeparator;
    QPushButton* m_cancelButton;
    QPushButton* m_backButton;
    QPushButton* m_nextButton;
    QPushButton* m_finishButton;
    QPushButton* m_helpButton;


public slots:
    virtual void back();
    virtual void help();
    virtual void next();

protected:
    QVBoxLayout* IgWizardBaseLayout;
    QVBoxLayout* m_titleAreaLayout;
    QHBoxLayout* m_titleLayout;
    QVBoxLayout* m_titleTextLayout;
    QVBoxLayout* m_bottomLayout;
    QHBoxLayout* m_buttonLayout;
};

#endif // IGWIZARDBASE_H
