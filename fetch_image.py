import serial
import sys
from graphics import *

ser = serial.Serial(sys.argv[1])
ser.baudrate = 115200
win = GraphWin("Image", 160 * 2, 120 * 2)
while ser.read() != b'\x00':
	pass 
print("Retrieving Image From UART...")
bytes = ser.read(120 * 160)
print("Successfully Recieved", len(bytes) / 2, "pixels.")
for r in range(120):
	for c in range(160):
		color = bytes[(r * 160 + c)]
		pt = Point(c * 2, r * 2)
		pt.setFill(color_rgb(int(color), int(color), int(color)))
		pt2 = Point(c * 2 + 1, r * 2 + 1)
		pt3 = Point(c * 2 + 1, r * 2)
		pt4 = Point(c * 2, r * 2 + 1)
		pt2.setFill(color_rgb(int(color), int(color), int(color)))
		pt3.setFill(color_rgb(int(color), int(color), int(color)))
		pt4.setFill(color_rgb(int(color), int(color), int(color)))
		pt.draw(win)
while True:
	pass
