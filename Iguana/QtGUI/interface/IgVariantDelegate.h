#ifndef IGGI_IG_VARIANT_DELEGATE_H
# define IGGI_IG_VARIANT_DELEGATE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <QItemDelegate>
# include <QRegExp>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgVariantDelegate : public QItemDelegate
{
  Q_OBJECT

public:
  IgVariantDelegate(QObject *parent = 0);
  // implicit copy constructor
  // implicit assignment operator
  // implicit destructor
  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const;
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const;
  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const;

  static bool isSupportedType(QVariant::Type type);
  static QString displayText(const QVariant &value);

private:
  mutable QRegExp boolExp;
  mutable QRegExp byteArrayExp;
  mutable QRegExp charExp;
  mutable QRegExp colorExp;
  mutable QRegExp dateExp;
  mutable QRegExp dateTimeExp;
  mutable QRegExp doubleExp;
  mutable QRegExp pointExp;
  mutable QRegExp rectExp;
  mutable QRegExp signedIntegerExp;
  mutable QRegExp sizeExp;
  mutable QRegExp timeExp;
  mutable QRegExp urlExp;
  mutable QRegExp unsignedIntegerExp;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IG_VARIANT_DELEGATE_H
