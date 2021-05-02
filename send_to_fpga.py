import serial
import sys

assert len(sys.argv) == 3

max_program_size = 4096

receiver = sys.argv[1] #eg: /dev/ttyUSB2
program = sys.argv[2]

ser = serial.Serial(receiver) 
ser.baudrate = 115200

file = open(program, 'r')
instructions = [l.strip() for l in file.readlines()]

assert len(instructions) <= max_program_size

success = True

# send the length (32 bit unsigned integer) of the program to the FPGA
length = len(instructions).to_bytes(4, 'little')
ser.write(length)
success = success and ser.read(4) == length

# send the instructions from the file to the FPGA
for i in instructions:
	inst_bytes = bytes.fromhex(i)
	ser.write(inst_bytes)
	success = success and ser.read(4) == inst_bytes

# verify that each instruction was delivered to the FPGA
print("SUCCESS" if success else "FAILURE")
