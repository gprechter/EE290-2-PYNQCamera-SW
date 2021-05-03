import serial
import sys
from graphics import *

ser = serial.Serial(sys.argv[1])
ser.baudrate = 115200
win = GraphWin("Image", 160, 120)
while ser.read() != b'\x00':
	pass 
ser.write(0)
print("Beginning...")
bytes = ser.read(160 * 2 * 120)
print(len(bytes))
for r in range(120):
	for c in range(160):
		first = bytes[(r * 160 + c) * 2]
		second = bytes[(r * 160 + c) * 2 + 1]
		red = (first) & 0xF8
		green = (first & 0x7) << 5 | (second >> 3)
		blue = (second << 3) & 0xF8
		pt = Point(c, r)
		pt.setFill(color_rgb(int(red), int(green), int(blue)))
		pt.draw(win)
