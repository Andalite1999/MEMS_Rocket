import serial
import time

serial_port = '/dev/tty.usbmodem1401'
baud_rate = 115200

ser = serial.Serial(serial_port, baud_rate, timeout=1)
time.sleep(2)  # wait for the connection to establish

ser.write(b'send\n')  # Send command to Arduino to start sending data

data = []
running = True
while running:
    line_bytes = ser.readline()
    print("Hex dump of received line:", line_bytes.hex())
    try:
        line = line_bytes.decode('utf-8').strip()
        if line:
            data.append(line)
        if line == "data":
            running = False
    except UnicodeDecodeError:
        print("Decode error on bytes:", line_bytes)  # Debugging line

ser.close()  # Close serial connection

# Write data to a text file
with open('output.txt', 'w') as f:
    for line in data:
        f.write(line + '\n')

print("Data written to output.txt")
