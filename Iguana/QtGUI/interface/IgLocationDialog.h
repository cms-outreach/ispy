#ifndef IGGI_IG_LOCATION_DIALOG_H
# define IGGI_IG_LOCATION_DIALOG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <QDialog>
# include <QSettings>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QComboBox;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QTableWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgLocationDialog : public QDialog
{
    Q_OBJECT

public:
    IgLocationDialog (QWidget *parent = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    QSettings::Format format() const;
    QSettings::Scope scope() const;
    QString organization() const;
    QString application() const;

private slots:
    void updateLocationsTable();

private:
    QLabel *formatLabel;
    QLabel *scopeLabel;
    QLabel *organizationLabel;
    QLabel *applicationLabel;
    QComboBox *formatComboBox;
    QComboBox *scopeComboBox;
    QComboBox *organizationComboBox;
    QComboBox *applicationComboBox;
    QGroupBox *locationsGroupBox;
    QTableWidget *locationsTable;
    QDialogButtonBox *buttonBox;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IG_LOCATION_DIALOG_H
