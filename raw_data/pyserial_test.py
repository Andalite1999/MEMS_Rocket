import serial
import time

# Replace '/dev/tty.usbmodem1401' with your serial port name
# Replace '9600' with the baud rate of your serial device
serial_port = '/dev/tty.usbmodem1401'
baud_rate = 115200

ser = serial.Serial(serial_port, baud_rate)
try:
    # Set up the serial connection

    print(f"Connected to {serial_port} at {baud_rate} baud.")

    # Continuously read and print the data
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            print(line)
        # time.sleep(0.001)  # Adjust sleep time as needed for your application

except serial.SerialException as e:
    print(f"Error opening serial port: {e}")
except KeyboardInterrupt:
    print("Program terminated by user.")
finally:
    ser.close()
    print("Serial connection closed.")
