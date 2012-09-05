import serial

class TapSerial:
    

    def __init__(self, port="/dev/tty.usbserial-A6008l19"):
        self.serial = serial.Serial(port, 9600)

    def read(self):
        resp = self.serial.read(5)
        self.serial.flushInput();
        print repr(resp)

def main():
    while(1):
        s = TapSerial()
        s.read()

if __name__ == "__main__":
    main()
