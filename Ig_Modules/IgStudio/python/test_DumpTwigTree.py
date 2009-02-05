from PyIgStudio import *
from PyIgObjectBrowser import *
from PyIgCommonData import *
from PyIgQtPython import *

def printChildren (twig):    
    print "Twig:" + twig.name ()
    print "Num of children:" + str (twig.children())
    print "Children:"
    for i in range (0, twig.children()):
        print twig.child (i).name ()
    print
    for i in range (0, twig.children()):
        printChildren(twig.child (i))


state = IgPythonState.get ()
dd=IgDocumentData.get(state)
root = dd.root ("Objects", True)
printChildren (root)
