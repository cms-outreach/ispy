#include "Iguana/IgFileBrowser/interface/AppController.h"

int
main (int argc, char **argv)
{
    QApplication app (argc, argv);
    AppController controller (argc, argv);
    app.exec ();
}
