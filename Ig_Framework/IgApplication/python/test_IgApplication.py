#!python
import sys
import dl
import thread
import time
sys.setdlopenflags(dl.RTLD_NOW|dl.RTLD_GLOBAL)
dl.open ("PyIgApplication.so", dl.RTLD_GLOBAL)
from PyIgApplication import IgApplication

def runIguana ():
    global a
    a=IgApplication ()
    a.pythonRun (["iguana","--iv", "../../Ig_Examples/IgOpenInventorView/test/simTracks.iv"])

if __name__ == "__main__":
    thread.start_new_thread (runIguana, ())
time.sleep (3)
