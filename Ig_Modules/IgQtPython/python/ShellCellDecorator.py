from PyIgQtPython import IgQtDecorator

class ColorfulDecorator(IgQtDecorator):
    counter = 1
    subStyleColors = ["#A0A0A0","#88FFBE","#FFA7A7"]
    def decorate(self, contents, subStyle):
        retString = "<TABLE BORDER=0 WIDTH=100% CELLSPACING=0 CELLPADDING=0 "
        retString += "BGCOLOR=" + self.subStyleColors[subStyle]+">"
        retString += "<TR><TD WIDTH=5%>"+str(self.__class__.counter)+"</TD><TD WIDTH=95%><PRE>"+contents+"</PRE></TD></TR></TABLE>"
        self.__class__.counter+=1
        return retString
