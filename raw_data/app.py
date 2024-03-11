from flask import Flask, render_template
from flask_socketio import SocketIO
import serial
import threading
import time

app = Flask(__name__)
socketio = SocketIO(app)
serial_port = '/dev/tty.usbmodem1401'
baud_rate = 9600


def read_from_serial():
    try:
        ser = serial.Serial(serial_port, baud_rate)
        while True:
            if ser.in_waiting > 0:
                data = ser.readline().decode('utf-8').rstrip()
                print(data)
                socketio.emit('newdata', {'data': data})
                print("Data sent to client.")
            # Adjust sleep time as needed for your application
            time.sleep(0.01)
    except serial.SerialException as e:
        print(f"Error opening serial port: {e}")
    except KeyboardInterrupt:
        print("Program terminated by user.")
    finally:
        ser.close()
        print("Serial connection closed.")


@app.route('/')
def index():
    # Serve your HTML page with the 3D model here
    return render_template('index.html')


if __name__ == '__main__':
    # Start reading from serial in a background thread
    t = threading.Thread(target=read_from_serial)  # is this the issue?
    t.start()
    socketio.run(app, debug=True)
