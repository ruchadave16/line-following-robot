import matplotlib.pyplot as plt
import numpy as np
import serial

arduino_port = '/dev/ttyACM0'
baud_rate = 9600
# Data is expected to come in the format:
# "{Variable_Name} {Variable_Value} {Current_time}"
serial_port = serial.Serial(arduino_port, baud_rate, timeout=1)

left_motor_values = []
right_motor_values = []
left_ir_values = []
right_ir_values = []

try:
    while True:
        data = serial_port.readline().decode()
        print(data)
except KeyboardInterrupt:
    pass

# import sys, signal
# def signal_handler(signal, frame):
#     print("\nprogram exiting gracefully")
#     sys.exit(0)

# signal.signal(signal.SIGINT, signal_handler)

# try:
#     while True:
#         # data = serial_port.readline().decode()
#         print(1)
# except KeyboardInterrupt:
#     print("here")
#     pass

# try:
#     while True:
#         pass # Do something
# except KeyboardInterrupt:
#     pass
# # Do something. Program doesn't terminate after CTRL-C
# print('hello world')
    