from PyIgQtProfileService import *
repSingleton = IgOProfRepresentableSingleton()
rep = repSingleton.get()
rep.requestUpdateProcessList()
browser = IgOProfPythonBrowser(rep)
browser.getResults()
