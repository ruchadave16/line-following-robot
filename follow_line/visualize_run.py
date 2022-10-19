import time
import matplotlib.pyplot as plt
import numpy as np
import serial

# sets the port ID and baud rate of serial input
ARDUINO_PORT = "/dev/tty/ACM0"
BAUD_RATE = "9600"

# Sets the file path to be used to store the generated visualization
FIGURE_TITLE = "short_trial_visualization.png"


def prepare_serial_analysis():
    """
    This function prepares the python client to analyze the serial input from
    an arduino. This includes establishing the serial connection and defining
    the start time of that establishing connection.
    """
    serial_port = serial.Serial(ARDUINO_PORT, BAUD_RATE, timeout=1)
    start_time = time.time()

    return serial_port, start_time


def listen_to_serial(serial_port, start_time):
    """
    Using the inputted serial connection, this function listens and parses
    through the serial connection for relevant data and returns that
    accumulated data when a keyboard interrupt is raised.
    """
    # Establishes lists to accumulate data of interest
    left_motor_values = []
    right_motor_values = []
    left_ir_values = []
    right_ir_values = []

    # Entire workflow wrapped in a try-except statement to allow for
    # KeyboardInterrupts to signify that the user no longer desires for data
    # acquisition from the serial connection
    try:
        while True:
            # Serial decoding wrapped in try-except statement to avoid
            # termination due to decode errors and value errors
            try:
                # From this serial port, data is expected to come in the format:
                # For IR sensor data:
                # "{IR_Sensor_name} {Value}"
                # For wheel speed data:
                # "{Wheel_Motor_Name} {Direction} {Value}"
                data = serial_port.readline().decode()

                # Data split into array based on relevant components
                data = data.split("\r\n")[0].split(" ")

                # The respective array corresponding to the inputted data is
                # updated Each array consists of an array of sensor value-time
                # detected pairs
                if data[0] == "leftWheel":
                    vel = int(data[2]) if data[1] == "f" else -int(data[2])
                    left_motor_values.append([vel, time.time() - start_time])
                if data[0] == "rightWheel":
                    vel = int(data[2]) if data[1] == "f" else -int(data[2])
                    right_motor_values.append([vel, time.time() - start_time])
                if data[0] == "leftIR":
                    left_ir_values.append([int(data[1]), time.time() - start_time])
                if data[0] == "rightIR":
                    right_ir_values.append([int(data[1]), time.time() - start_time])
            except UnicodeDecodeError and ValueError:
                continue

    # Once a KeyboardInterrupt has been detected, the acquired data arrays are
    # converted to numpy arrays and returned
    except KeyboardInterrupt:
        left_ir_values = np.array(left_ir_values)
        right_ir_values = np.array(right_ir_values)
        left_motor_values = np.array(left_motor_values)
        right_motor_values = np.array(right_motor_values)

        return left_ir_values, right_ir_values, left_motor_values, right_motor_values


def plot_serial_data(
    left_ir_values, right_ir_values, left_motor_values, right_motor_values
):
    """
    This function plots the inputted data and saves the resulting figure.
    """
    # Creates twin axes plots in order to overlay wheel speeds and IR sensor
    # readings on the same plot.
    _, ax1 = plt.subplots()
    ax2 = ax1.twinx()

    # Plots the motor speeds (plotted as a percent of maximum speed as m/s
    # readings were not available)
    ax1.plot(
        left_motor_values[:, 1],
        left_motor_values[:, 0] / 255,
        "bx",
        label="Left Motor Speed",
    )
    ax1.plot(
        right_motor_values[:, 1],
        right_motor_values[:, 0] / 255,
        "g.",
        label="RIght Motor Speed",
    )

    # Plots the IR sensors values, along with a horizontal boundary value line
    # (threshold that determined whether the sensor was currently over the
    # black line). The actual readings are converted from analog messages to
    # voltages.
    ax2.plot(
        left_ir_values[:, 1],
        left_ir_values[:, 0] * 5 / 1024,
        "rx",
        label="Left IR Sensor",
    )
    ax2.plot(
        right_ir_values[:, 1],
        right_ir_values[:, 0] * 5 / 1024,
        "c.",
        label="Right IR Sensor",
    )
    ax2.plot(
        [left_ir_values[0, 1], left_ir_values[-1, 1]],
        [510 * 5 / 1024, 510 * 5 / 1024],
        "k-",
    )

    # Establishes axes labels, legends, and a title for the figure
    ax1.set_xlabel("Time (s)")
    ax1.set_ylabel("Percent of Maximum Wheel Speed (%)")
    ax2.set_ylabel("Voltage (V)")
    ax1.legend(loc="upper right")
    ax2.legend(loc="lower right")
    ax1.set_title("Short Trial Run Data Collection")

    # Saves and displays the generated figure
    plt.savefig(FIGURE_TITLE)
    plt.show()


# Main runner for visualization workflow
if __name__ == "__main__":
    serial_port, start_time = prepare_serial_analysis()
    (
        left_ir_values,
        right_ir_values,
        left_motor_values,
        right_motor_values,
    ) = listen_to_serial(serial_port, start_time)
    plot_serial_data(
        left_ir_values, right_ir_values, left_motor_values, right_motor_values
    )
