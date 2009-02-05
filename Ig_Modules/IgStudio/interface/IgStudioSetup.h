/****************************************************************************
** Form interface generated from reading ui file 'IgStudioSetup.ui'
**
** Created: Tue Aug 19 09:43:25 2003
**      by: The User Interface Compiler ($Id: IgStudioSetup.h,v 1.2 2003/08/19 07:45:55 yana Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef IGSTUDIOSETUP_H
#define IGSTUDIOSETUP_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QPushButton;
class QListBox;
class QListBoxItem;

class IgStudioSetup : public QDialog
{
    Q_OBJECT

public:
    IgStudioSetup( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~IgStudioSetup();

    QLabel* m_label;
    QPushButton* m_cancel;
    QPushButton* m_help;
    QPushButton* m_ok;
    QListBox* m_configs;

public slots:
    virtual void helpSlot();

protected:
    QGridLayout* IgStudioSetupLayout;
    QGridLayout* m_layout2;
    QGridLayout* m_layout1;

protected slots:
    virtual void languageChange();

};

#endif // IGSTUDIOSETUP_H
