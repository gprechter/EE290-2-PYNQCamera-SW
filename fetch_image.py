import serial
import sys
from graphics import *

ser = serial.Serial(sys.argv[1])
ser.baudrate = 115200
win = GraphWin("Image", 160, 120)
while ser.read() != 0x00:
	pass
print("Beginning...")
bytes = ser.read(160 * 2 * 120)
print(len(bytes))
for i in range(120):
	j = 0
	for e in list(range(160 * 2))[::2]: 
		first = bytes[i * (160 * 2) + e]
		second = bytes[i * (160 * 2) + e + 1]
		r = (first) & 0xF8
		g = (first & 0x7) << 5 | (second >> 3)
		b = (second << 3) & 0xF8
		pt = Point(j, i)
		pt.setFill(color_rgb(int(r), int(g), int(b)))
		pt.draw(win)
		j += 1

while True:
	pass
