#ifndef QT_GUI_IG_COLLECTION_LIST_MODEL_H
# define QT_GUI_IG_COLLECTION_LIST_MODEL_H

# include <QAbstractListModel>
# include <QModelIndex>
# include <QVariant>

class IgCollection;

class IgCollectionListModel : public QAbstractListModel
{
  Q_OBJECT
public:
  IgCollectionListModel (IgCollection *collection = 0,
			 QObject *parent = 0);

  const char *  label(void);
  QVariant      data(const QModelIndex &index, int role) const;
  QVariant 	headerData(int section, Qt::Orientation orientation, int role) const;
  int           rowCount(const QModelIndex &parent = QModelIndex()) const;
  void          setCollection(IgCollection *collection);
  void          clear(void);

private:
  IgCollection	*m_collection;
};

#endif // QT_GUI_IG_COLLECTION_LIST_MODEL_H
