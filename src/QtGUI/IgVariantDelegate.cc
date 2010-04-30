//<<<<<< INCLUDES                                                       >>>>>>

#include "QtGUI/IgVariantDelegate.h"
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

IgVariantDelegate::IgVariantDelegate(QObject *parent)
  : QItemDelegate(parent)
{
  boolExp.setPattern("true|false");
  boolExp.setCaseSensitivity(Qt::CaseInsensitive);

  byteArrayExp.setPattern("[\\x00-\\xff]*");
  charExp.setPattern(".");
  colorExp.setPattern("\\(([0-9]*),([0-9]*),([0-9]*),([0-9]*)\\)");
  doubleExp.setPattern("");
  pointExp.setPattern("\\((-?[0-9]*),(-?[0-9]*)\\)");
  rectExp.setPattern("\\((-?[0-9]*),(-?[0-9]*),(-?[0-9]*),(-?[0-9]*)\\)");
  signedIntegerExp.setPattern("-?[0-9]*");
  sizeExp = pointExp;
  unsignedIntegerExp.setPattern("[0-9]*");

  dateExp.setPattern("([0-9]{,4})-([0-9]{,2})-([0-9]{,2})");
  timeExp.setPattern("([0-9]{,2}):([0-9]{,2}):([0-9]{,2})");
  urlExp.setPattern(".");
  dateTimeExp.setPattern(dateExp.pattern() + "T" + timeExp.pattern());
}

void
IgVariantDelegate::paint(QPainter *painter,
                         const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
  if (index.column() == 2)
  {
    QVariant value = index.model()->data(index, Qt::UserRole);
    if (!isSupportedType(value.type()))
    {
      QStyleOptionViewItem myOption = option;
      myOption.state &= ~QStyle::State_Enabled;
      QItemDelegate::paint(painter, myOption, index);
      return;
    }
  }

  QItemDelegate::paint(painter, option, index);
}

QWidget *
IgVariantDelegate::createEditor(QWidget *parent,
                                const QStyleOptionViewItem & /* option */,
                                const QModelIndex &index) const
{
  if (index.column() != 2)
    return 0;

  QVariant originalValue = index.model()->data(index, Qt::UserRole);
  if (!isSupportedType(originalValue.type()))
    return 0;

  QLineEdit *lineEdit = new QLineEdit(parent);
  lineEdit->setFrame(false);

  QRegExp regExp;

  switch (originalValue.type())
  {
  case QVariant::Bool:
    regExp = boolExp;
    break;
  case QVariant::ByteArray:
    regExp = byteArrayExp;
    break;
  case QVariant::Char:
    regExp = charExp;
    break;
  case QVariant::Color:
    regExp = colorExp;
    break;
  case QVariant::Date:
    regExp = dateExp;
    break;
  case QVariant::DateTime:
    regExp = dateTimeExp;
    break;
  case QVariant::Double:
    regExp = doubleExp;
    break;
  case QVariant::Int:
  case QVariant::LongLong:
    regExp = signedIntegerExp;
    break;
  case QVariant::Point:
    regExp = pointExp;
    break;
  case QVariant::Rect:
    regExp = rectExp;
    break;
  case QVariant::Size:
    regExp = sizeExp;
    break;
  case QVariant::Time:
    regExp = timeExp;
    break;
  case QVariant::Url:
    regExp = urlExp;
    break;
  case QVariant::UInt:
  case QVariant::ULongLong:
    regExp = unsignedIntegerExp;
    break;
  default:
    ;
  }

  if (!regExp.isEmpty())
  {
    QValidator *validator = new QRegExpValidator(regExp, lineEdit);
    lineEdit->setValidator(validator);
  }

  return lineEdit;
}

void
IgVariantDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const
{
  QVariant value = index.model()->data(index, Qt::UserRole);
  if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor))
    lineEdit->setText(displayText(value));
}

void
IgVariantDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const
{
  QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
  if (!lineEdit->isModified())
    return;

  QString text = lineEdit->text();
  const QValidator *validator = lineEdit->validator();
  if (validator)
  {
    int pos;
    if (validator->validate(text, pos) != QValidator::Acceptable)
      return;
  }

  QVariant originalValue = index.model()->data(index, Qt::UserRole);
  QVariant value;

  switch (originalValue.type())
  {
  case QVariant::Char:
    value = text.at(0);
    break;
  case QVariant::Color:
    colorExp.exactMatch(text);
    value = QColor(qMin(colorExp.cap(1).toInt(), 255),
                   qMin(colorExp.cap(2).toInt(), 255),
                   qMin(colorExp.cap(3).toInt(), 255),
                   qMin(colorExp.cap(4).toInt(), 255));
    break;
  case QVariant::Date:
    {
      QDate date = QDate::fromString(text, Qt::ISODate);
      if (!date.isValid())
        return;
      value = date;
    }
    break;
  case QVariant::DateTime:
    {
      QDateTime dateTime = QDateTime::fromString(text, Qt::ISODate);
      if (!dateTime.isValid())
        return;
      value = dateTime;
    }
    break;
  case QVariant::Point:
    pointExp.exactMatch(text);
    value = QPoint(pointExp.cap(1).toInt(), pointExp.cap(2).toInt());
    break;
  case QVariant::Rect:
    rectExp.exactMatch(text);
    value = QRect(rectExp.cap(1).toInt(), rectExp.cap(2).toInt(),
                  rectExp.cap(3).toInt(), rectExp.cap(4).toInt());
    break;
  case QVariant::Size:
    sizeExp.exactMatch(text);
    value = QSize(sizeExp.cap(1).toInt(), sizeExp.cap(2).toInt());
    break;
  case QVariant::StringList:
    value = text.split(",");
    break;
  case QVariant::Time:
    {
      QTime time = QTime::fromString(text, Qt::ISODate);
      if (!time.isValid())
        return;
      value = time;
    }
    break;
  case QVariant::Url:
    {
      QUrl url = QUrl(text);
      if (!url.isValid())
        return;
      value = url;
    }
    break;
  default:
    value = text;
    value.convert(originalValue.type());
  }

  model->setData(index, displayText(value), Qt::DisplayRole);
  model->setData(index, value, Qt::UserRole);
}

bool
IgVariantDelegate::isSupportedType(QVariant::Type type)
{
  switch (type)
  {
  case QVariant::Bool:
  case QVariant::ByteArray:
  case QVariant::Char:
  case QVariant::Color:
  case QVariant::Date:
  case QVariant::DateTime:
  case QVariant::Double:
  case QVariant::Int:
  case QVariant::LongLong:
  case QVariant::Point:
  case QVariant::Rect:
  case QVariant::Size:
  case QVariant::String:
  case QVariant::StringList:
  case QVariant::Time:
  case QVariant::Url:
  case QVariant::UInt:
  case QVariant::ULongLong:
    return true;
  default:
    return false;
  }
}

QString
IgVariantDelegate::displayText(const QVariant &value)
{
  switch (value.type())
  {
  case QVariant::Bool:
  case QVariant::ByteArray:
  case QVariant::Char:
  case QVariant::Double:
  case QVariant::Int:
  case QVariant::LongLong:
  case QVariant::String:
  case QVariant::UInt:
  case QVariant::ULongLong:
    return value.toString();
  case QVariant::Color:
    {
      QColor color = qvariant_cast<QColor>(value);
      return QString("(%1,%2,%3,%4)")
        .arg(color.red()).arg(color.green())
        .arg(color.blue()).arg(color.alpha());
    }
  case QVariant::Date:
    return value.toDate().toString(Qt::ISODate);
  case QVariant::DateTime:
    return value.toDateTime().toString(Qt::ISODate);
  case QVariant::Invalid:
    return "<Invalid>";
  case QVariant::Point:
    {
      QPoint point = value.toPoint();
      return QString("(%1,%2)").arg(point.x()).arg(point.y());
    }
  case QVariant::Rect:
    {
      QRect rect = value.toRect();
      return QString("(%1,%2,%3,%4)")
        .arg(rect.x()).arg(rect.y())
        .arg(rect.width()).arg(rect.height());
    }
  case QVariant::Size:
    {
      QSize size = value.toSize();
      return QString("(%1,%2)").arg(size.width()).arg(size.height());
    }
  case QVariant::StringList:
    return value.toStringList().join(",");
  case QVariant::Time:
    return value.toTime().toString(Qt::ISODate);
  case QVariant::Url:
    return value.toUrl().toString();
  default:
    break;
  }
  return QString("<%1>").arg(value.typeName());
}

