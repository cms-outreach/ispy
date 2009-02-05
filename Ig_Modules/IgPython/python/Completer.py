from PyIgPython import *
from string import split
from string import join
import re

class IgDefaultCompleter(IgPythonCompleter):
    def getValidDict(self,name):
        currentDict = __globals__
        subNames = split(name,'.')
        subNames.pop()
        for x in subNames:
            if x in currentDict.keys():
                currentDict=currentDict[x].__dict__
            else:
                return None
        return currentDict

    def isValid(self,name):
        if self.getValidDict(name) == None:
            return 0
        else:
            return 1
    
    def completeName(self,name):
        finalDict = self.getValidDict(name)
        subNames = split(name,'.')
        finalName = subNames.pop()
        if finalDict!=None:
            for x in finalDict.keys():
                if re.compile("^"+finalName).match(x) != None:
                    subNames.append(x)
                    return join(subNames,'.')
        return name
