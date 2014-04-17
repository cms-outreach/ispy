//<<<<<< INCLUDES                                                       >>>>>>

#include "QtGUI/IgLocationDialog.h"
#include <QtGui>
#include <QtWidgets>
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgLocationDialog::IgLocationDialog(QWidget *parent)
  : QDialog(parent)
{
  formatComboBox = new QComboBox;
  formatComboBox->addItem(tr("Native"));
  formatComboBox->addItem(tr("INI"));

  scopeComboBox = new QComboBox;
  scopeComboBox->addItem(tr("User"));
  scopeComboBox->addItem(tr("System"));

  organizationComboBox = new QComboBox;
  organizationComboBox->addItem(tr("iguana"));
  organizationComboBox->setEditable(true);

  applicationComboBox = new QComboBox;
  applicationComboBox->addItem(tr("Any"));
  applicationComboBox->addItem(tr("ispy"));
  applicationComboBox->addItem(tr("iguana"));
  applicationComboBox->setEditable(true);
  applicationComboBox->setCurrentIndex(1);

  formatLabel = new QLabel(tr("&Format:"));
  formatLabel->setBuddy(formatComboBox);

  scopeLabel = new QLabel(tr("&Scope:"));
  scopeLabel->setBuddy(scopeComboBox);

  organizationLabel = new QLabel(tr("&Organization:"));
  organizationLabel->setBuddy(organizationComboBox);

  applicationLabel = new QLabel(tr("&Application:"));
  applicationLabel->setBuddy(applicationComboBox);

  locationsGroupBox = new QGroupBox(tr("Setting Locations"));

  QStringList labels;
  labels << tr("Location") << tr("Access");

  locationsTable = new QTableWidget;
  locationsTable->setSelectionMode(QAbstractItemView::SingleSelection);
  locationsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  locationsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  locationsTable->setColumnCount(2);
  locationsTable->setHorizontalHeaderLabels(labels);
  locationsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  locationsTable->horizontalHeader()->resizeSection(1, 180);

  buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                   | QDialogButtonBox::Cancel);

  connect(formatComboBox, SIGNAL(activated(int)),
          this, SLOT(updateLocationsTable()));
  connect(scopeComboBox, SIGNAL(activated(int)),
          this, SLOT(updateLocationsTable()));
  connect(organizationComboBox->lineEdit(),
          SIGNAL(editingFinished()),
          this, SLOT(updateLocationsTable()));
  connect(applicationComboBox->lineEdit(),
          SIGNAL(editingFinished()),
          this, SLOT(updateLocationsTable()));
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  QVBoxLayout *locationsLayout = new QVBoxLayout;
  locationsLayout->addWidget(locationsTable);
  locationsGroupBox->setLayout(locationsLayout);

  QGridLayout *mainLayout = new QGridLayout;
  mainLayout->addWidget(formatLabel, 0, 0);
  mainLayout->addWidget(formatComboBox, 0, 1);
  mainLayout->addWidget(scopeLabel, 1, 0);
  mainLayout->addWidget(scopeComboBox, 1, 1);
  mainLayout->addWidget(organizationLabel, 2, 0);
  mainLayout->addWidget(organizationComboBox, 2, 1);
  mainLayout->addWidget(applicationLabel, 3, 0);
  mainLayout->addWidget(applicationComboBox, 3, 1);
  mainLayout->addWidget(locationsGroupBox, 4, 0, 1, 2);
  mainLayout->addWidget(buttonBox, 5, 0, 1, 2);
  setLayout(mainLayout);

  updateLocationsTable();

  setWindowTitle(tr("Open Application Settings"));
  resize(650, 400);
}

QSettings::Format
IgLocationDialog::format() const
{
  if (formatComboBox->currentIndex() == 0)
    return QSettings::NativeFormat;
  else
    return QSettings::IniFormat;
}

QSettings::Scope
IgLocationDialog::scope() const
{
  if (scopeComboBox->currentIndex() == 0)
    return QSettings::UserScope;
  else
    return QSettings::SystemScope;
}

QString
IgLocationDialog::organization() const
{
  return organizationComboBox->currentText();
}

QString
IgLocationDialog::application() const
{
  if (applicationComboBox->currentText() == tr("Any"))
    return "";
  else
    return applicationComboBox->currentText();
}

void
IgLocationDialog::updateLocationsTable()
{
  locationsTable->setUpdatesEnabled(false);
  locationsTable->setRowCount(0);

  for (int i = 0; i < 2; ++i) {
    if (i == 0 && scope() == QSettings::SystemScope)
      continue;

    QSettings::Scope actualScope = (i == 0) ? QSettings::UserScope
                                  : QSettings::SystemScope;
    for (int j = 0; j < 2; ++j) {
      if (j == 0 && application().isEmpty())
        continue;

      QString actualApplication;
      if (j == 0)
        actualApplication = application();
      QSettings settings(format(), actualScope, organization(),
                         actualApplication);

      int row = locationsTable->rowCount();
      locationsTable->setRowCount(row + 1);

      QTableWidgetItem *item0 = new QTableWidgetItem;
      item0->setText(settings.fileName());

      QTableWidgetItem *item1 = new QTableWidgetItem;
      bool disable = (settings.childKeys().isEmpty()
                     && settings.childGroups().isEmpty());

      if (row == 0) {
        if (settings.isWritable()) {
          item1->setText(tr("Read-write"));
          disable = false;
        } else {
          item1->setText(tr("Read-only"));
        }
        buttonBox->button(QDialogButtonBox::Ok)->setDisabled(disable);
      } else {
        item1->setText(tr("Read-only fallback"));
      }

      if (disable) {
        item0->setFlags(item0->flags() & ~Qt::ItemIsEnabled);
        item1->setFlags(item1->flags() & ~Qt::ItemIsEnabled);
      }

      locationsTable->setItem(row, 0, item0);
      locationsTable->setItem(row, 1, item1);
    }
  }
  locationsTable->setUpdatesEnabled(true);
}
