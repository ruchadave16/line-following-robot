import time
import matplotlib.pyplot as plt
import numpy as np
import serial

arduino_port = '/dev/ttyACM0'
baud_rate = 9600
# Data is expected to come in the format:
# "{Variable_Name} {Variable_Value} {Current_time}"
# TODO: fix this comment lol
serial_port = serial.Serial(arduino_port, baud_rate, timeout=1)

left_motor_values = []
right_motor_values = []
left_ir_values = []
right_ir_values = []

start_time = time.time()

try:
    while True:
        try:
            data = serial_port.readline().decode()
            data = data.split(' ')
            if data[0] == 'leftWheel':
                vel = int(data[2]) if data[1] == 'f' else -int(data[2])
                left_motor_values.append([vel, time.time() - start_time])
            if data[0] == 'rightWheel':
                vel = int(data[2]) if data[1] == 'f' else -int(data[2])
                right_motor_values.append([vel, time.time() - start_time])
            if data[0] == 'leftIR':
                left_ir_values.append([int(data[1]), time.time() - start_time])
            if data[0] == 'rightIR':
                right_ir_values.append([int(data[1]), time.time() - start_time])
        except UnicodeDecodeError and ValueError:
            continue
except KeyboardInterrupt:
    left_ir_values = np.array(left_ir_values)
    right_ir_values = np.array(right_ir_values)
    left_motor_values = np.array(left_motor_values)
    right_motor_values = np.array(right_motor_values)
    fig, ax1 = plt.subplots()
    ax2 = ax1.twinx()
    ax1.plot(left_motor_values[:,1], left_motor_values[:,0]/255, 'bx', label="Left Motor Speed")
    ax1.plot(right_motor_values[:,1], right_motor_values[:,0]/255, 'g.', label="RIght Motor Speed")
    ax2.plot(left_ir_values[:,1], left_ir_values[:,0]*5/1024, 'rx', label="Left IR Sensor")
    ax2.plot(right_ir_values[:,1], right_ir_values[:,0]*5/1024, 'c.', label="Right IR Sensor")
    ax2.plot([left_ir_values[0,1], left_ir_values[-1,1]], [510*5/1024, 510*5/1024], 'k-')
    ax1.set_xlabel('Time (s)')
    ax1.set_ylabel('Percent of Maximum Wheel Speed (%)')
    ax2.set_ylabel('Voltage (V)')
    ax1.legend(loc='upper right')
    ax2.legend(loc='lower right')
    ax1.set_title("Short Trial Run Data Collection")
    plt.savefig('data3.png')
    plt.show()
    
