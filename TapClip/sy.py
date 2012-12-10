import sys
import serial

class TapSerial:
    

    def __init__(self, port):
        self.serial = serial.Serial(port, 57600)

    def read(self):
        resp = self.serial.read(7)
        self.serial.flushInput();
        print repr(resp)

def main(port):
    s = TapSerial(port)
    while(1):
        s.read()

if __name__ == "__main__":
    if(len(sys.argv) > 1):
        main(sys.argv[1])
    else:
        print "You must enter a port. <script-name> <port-name>"
