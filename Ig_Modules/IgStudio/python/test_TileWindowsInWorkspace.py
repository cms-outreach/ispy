from PyIgObjectBrowser import *
from PyIgCommonData import *
from PyIgQtPython import *
from PyIgQtBrowser import *
from PyIgStudio import *

state = IgPythonState.get ()
dd = IgDocumentData.get (state)
workspace = dd.document ().currentPage ().workspace ()
IgViewDB.get ().create ("3D Window", dd.document ().currentPage ())
IgViewDB.get ().create ("3D Window", dd.document ().currentPage ())
workspace.tile ()
