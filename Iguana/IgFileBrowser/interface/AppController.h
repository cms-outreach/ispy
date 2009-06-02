# include <QtGui>
# include <iostream>
# include "Iguana/Framework/interface/IgCollection.h"
# include "Iguana/Framework/interface/IgParser.h"
# include "Iguana/QtGUI/interface/IgCollectionTableModel.h"
# include "Iguana/QtGUI/interface/IgMultiStorageTreeModel.h"

class AppController : public QObject
{
    Q_OBJECT
public:
    AppController (int argc, char **argv)
	: storageModel (new IgMultiStorageTreeModel),
          collectionModel (new IgCollectionTableModel)
        {
	    if (argc < 2)
	    {
		std::cerr << "igbrowser <file>" << std::endl;
		exit (1);
	    }

	    storage = new IgDataStorage;
	    IgParser parser(storage);
            
            setCurrentFile (argv[1]);

	    widget = new QWidget;
            mainWindow = new QMainWindow;
            mainWindow->setCentralWidget (widget);
            QMenu *fileMenu = mainWindow->menuBar ()->addMenu (tr("&File"));
            fileMenu->addAction ("&Open", this, SLOT(openFileDialog()));

	    QHBoxLayout *layout = new QHBoxLayout (widget);

	    storageView = new QTreeView;
	    storageView->setModel (storageModel);
	    layout->addWidget (storageView);

	    collectionView = new QTableView;
	    collectionView->setModel (collectionModel);
	    layout->addWidget (collectionView);

	    connect (storageView,SIGNAL(activated(const QModelIndex)),this,SLOT(collectionChanged(const QModelIndex)));
	    mainWindow->show ();
	}
private slots:
void 
collectionChanged(const QModelIndex &index)
	{
	    QString collectionName = storageModel->data(index, Qt::DisplayRole).toString();
	    if (!index.parent().isValid())
	    {
		return;
	    }
	    collectionModel->setCollection (storage->getCollectionPtr(collectionName.toAscii().constData()));
	}

    void
    openFileDialog (void)
    {
        storageModel->clear();
        QString newFile = QFileDialog::getOpenFileName (widget,  tr("Pick up an Event / Geometry file"));
        QFileInfo info (newFile);
        if (!info.exists())
        {
           QMessageBox::warning (widget, "File not found", "File '" + newFile + "' could not be opened.");
           return;
        }
        setCurrentFile (newFile.toAscii());
    }
 
private:
    void setCurrentFile (const char *filename)
    {   
        if (storage)
            delete storage;

        storage = new IgDataStorage;
        IgParser parser(storage);

        FILE *f = fopen(filename, "r");
        if (!f)
        {
            std::cerr << "File '" << filename << "' does not exists"<< std::endl;
            exit (1);
        }
        
        fseek (f, 0, SEEK_END);
        int size = ftell (f);
        char *buffer = new char[size];
        fseek (f, 0, SEEK_SET);
        fread (buffer, size, 1, f);
        fclose (f);
        parser.parse (buffer);
        if (storage->collectionNames ().size () == 0)
        {
            std::cerr << "Error: no collections found." << std::endl;
            exit (1);
        }

        storageModel->addStorage (storage, filename);
        collectionModel->setCollection (&(storage->getCollectionByIndex(0)));
    }

    IgDataStorage 		*storage;
    IgMultiStorageTreeModel     *storageModel;
    IgCollectionTableModel 	*collectionModel;
    QTreeView 			*storageView;
    QTableView 			*collectionView;
    QWidget 			*widget;
    QMainWindow                 *mainWindow;
};
