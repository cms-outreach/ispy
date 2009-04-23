//<<<<<< INCLUDES                                                       >>>>>>

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>

QGraphicsItem *makeTest (void);

int 
main (int argv, char *args[])
{
    QApplication app (argv, args);
    QGraphicsScene scene (-200, -200, 400, 400);
    scene.addItem (makeTest ());
    QGraphicsView view (&scene);
    view.show ();

    return app.exec();
}
