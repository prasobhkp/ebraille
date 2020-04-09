import time
import serial
import sys
import msvcrt as m


def wait():
    m.getch()


def sendToSerial(data):
    ser.write(data.encode('utf-8'))
    print("Data Sent: "+str(data))
    time.sleep(0.2)


def readSerial():
    out = ''
    while ser.inWaiting():
        out += ser.read(1)
    return out


# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(port='COM7', baudrate=512000, timeout=None)

if(ser.isOpen()):
	sendToSerial('Welcome to eBraille! Everything is ready for you here.\r\n')
