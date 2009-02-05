import os
from SimpleXMLRPCServer import SimpleXMLRPCServer, SimpleXMLRPCRequestHandler
from PyIgQtProfileService import *

myServer = SimpleXMLRPCServer( (os.getenv("HOSTNAME"),23456) )


def helloRPC(params):
    print params
    print "Hello World"
    return "foo"

def executeScript(params):
    print "Received Script:\n"+params+"\n--------------------------\n"
    exec(params)
    return "processMap={}\n"+result

myServer.register_function(helloRPC)
myServer.register_function(executeScript)

myServer.serve_forever()

