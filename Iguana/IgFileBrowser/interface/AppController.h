# include <QtGui>
# include <iostream>
# include "Iguana/Framework/interface/IgCollection.h"
# include "Iguana/Framework/interface/IgParser.h"
# include "Iguana/Iggi/interface/IgCollectionTableModel.h"
# include "Iguana/Iggi/interface/IgMultiStorageTreeModel.h"

class AppController : public QObject
{
    Q_OBJECT
public:
    AppController (int argc, char **argv)
	{
	    if (argc < 2)
	    {
		std::cerr << "igbrowser <file>" << std::endl;
		exit (1);
	    }

	    storage = new IgDataStorage;
	    IgParser parser(storage);

	    FILE *f = fopen(argv[1], "r");
	    fseek(f, 0, SEEK_END);
	    int size = ftell(f);
	    char *buffer = new char[size];
	    fseek(f, 0, SEEK_SET);
	    fread (buffer, size, 1, f);
	    fclose(f);
	    parser.parse(buffer);
  
	    if (storage->collectionNames().size() == 0)
	    {
		std::cerr << "Error: no collections found." << std::endl;
		exit (1);
	    }

	    collectionModel = new IgCollectionTableModel (&(storage->getCollectionByIndex (2)));
	    storageModel = new IgMultiStorageTreeModel;

	    widget = new QWidget;
	    QHBoxLayout *layout = new QHBoxLayout (widget);

	    storageView = new QTreeView;
	    storageView->setModel (storageModel);
	    layout->addWidget (storageView);
	    storageModel->addStorage(storage, argv[1]);

	    collectionView = new QTableView;
	    collectionView->setModel (collectionModel);
	    layout->addWidget (collectionView);

	    connect (storageView,SIGNAL(activated(const QModelIndex)),this,SLOT(collectionChanged(const QModelIndex)));
	    widget->show ();
	}
private slots:
void 
collectionChanged(const QModelIndex &index)
	{
	    QString collectionName = storageModel->data(index, Qt::DisplayRole).toString();
	    qDebug() << collectionName << " selected";
	    if (!index.parent().isValid())
	    {
		return;
	    }
	    collectionModel->setCollection (storage->getCollectionPtr(collectionName.toAscii()));
	}
  
private:
    IgDataStorage 		*storage;
    IgCollectionTableModel 	*collectionModel;
    IgMultiStorageTreeModel 	*storageModel; 
    QTreeView 			*storageView;
    QTableView 			*collectionView;
    QWidget 			*widget;
};
