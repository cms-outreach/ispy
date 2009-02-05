from PyIgQtProfileService import *
from  xmlrpclib import *

PORT= 23456
script=""
script+="from PyIgQtProfileService import *\n"
script+="rep = IgOProfRepresentableConcrete()\n"
script+=updateProcessScript;
script+="rep.requestUpdateProcessList()\n"
script+=updateProcessScript;
script+="browser = IgOProfPythonBrowser()\n"
script+="result = browser.getResults()\n"
serverProxy = ServerProxy("http://lxcmse1:23456")
exec (serverProxy.executeScript(script))
