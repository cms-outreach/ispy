#include <Inventor/Qt/SoQt.h>
  #include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
  #include <Inventor/nodes/SoNodes.h>
  
  int
  main(int argc, char ** argv)
  {
    QWidget * mainwin = SoQt::init(argv[0]);
  
    SoSeparator * root = new SoSeparator;
    root->ref();
  
    // Adds a camera and a red cone. The first camera found in the
    // scene graph by the SoQtExaminerViewer will be picked up and
    // initialized automatically.
  
    root->addChild(new SoPerspectiveCamera);
    SoMaterial * material = new SoMaterial;
    material->diffuseColor.setValue(1.0, 0.0, 0.0);
    root->addChild(material);
    root->addChild(new SoCone);
  
  
    // Set up a second camera for the remaining geometry. This camera
    // will not be picked up and influenced by the viewer, so the
    // geometry will be kept static.
  
    SoPerspectiveCamera * pcam = new SoPerspectiveCamera;
    pcam->position = SbVec3f(0, 0, 5);
    pcam->nearDistance = 0.1;
    pcam->farDistance = 10;
    root->addChild(pcam);
  
    // Adds a green cone to demonstrate static geometry.
  
    SoMaterial * greenmaterial = new SoMaterial;
    greenmaterial->diffuseColor.setValue(0, 1.0, 0.0);
    root->addChild(greenmaterial);
    root->addChild(new SoCone);
  
  
    SoQtExaminerViewer * viewer = new SoQtExaminerViewer(mainwin);
    viewer->setSceneGraph(root);
    viewer->show();
  
    SoQt::show(mainwin);
    SoQt::mainLoop();
  
    delete viewer;
    root->unref();
    return 0;
  }
