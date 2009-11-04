#include "Iguana/QtGUI/src/ISpyListView.h"
#include <QtGui>
#include <iostream>

ISpyListView::ISpyListView (QWidget *parent)
  : QTreeView(parent)
{
  setupUi(this);
}
