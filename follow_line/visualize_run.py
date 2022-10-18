
import matplotlib.pyplot as plt
import numpy as np
import serial

arduino_port = '/dev/ttyACM0'
baud_rate = 9600
serial_port = serial.Serial(arduino_port, baud_rate, timeout=1)

while True:
    try:
        data = serial_port.readline().decode()
    except UnicodeDecodeError:
        continue
        