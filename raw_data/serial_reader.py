import serial
import time

serial_port = '/dev/tty.usbmodem1401'
baud_rate = 115200

ser = serial.Serial(serial_port, baud_rate, timeout=1)
time.sleep(2)  # wait for the connection to establish

ser.write(b'send\n')  # Send command to Arduino to start sending data

data = []
while True:
    line_bytes = ser.readline()
    print(line_bytes)
    try:
        line = line_bytes.decode('utf-8').strip()
        if line == "spirou":
            print(line_bytes)
            print(line)
            break
        if line:
            data.append(line)
    except UnicodeDecodeError:
        print("Decode error on bytes:", line_bytes)  # Debugging line


ser.close()  # Close serial connection

# Write data to a text file
with open('output.txt', 'w') as f:
    for line in data:
        f.write(line + '\n')
        print(line)

print("Data written to output.txt")
