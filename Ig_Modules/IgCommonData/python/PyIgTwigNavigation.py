from PyIgObjectBrowser import *
from PyIgCommonData import *
from PyIgQtPython import *
from PyIgStudio import *

def setTwigVisibility (twigName, visibility):
    state = IgPythonState.get ()
    rootTwig = twigName.split ("/")[1]
    assert (rootTwig != None)
    dd = IgDocumentData.get (state)
    twig = dd.root (rootTwig, False)
    assert (twig != None)
    modifiedTwig = twig.lookup (twigName) 
    modifiedTwig.selfVisible (visibility)
    IgRepSet.update (modifiedTwig, -1)

def enableTwig (twigName):
    setTwigVisibility (twigName, True)

def disableTwig (twigName):
    setTwigVisibility (twigName, False)

def enableTwigs (twigNameList):
    for x in twigNameList:
        enableTwig (x)

def disableTwigs (twigNameList):
    for x in twigNameList:
        disableTwig (x)
