from flask import Flask, render_template
from flask_socketio import SocketIO
import serial
import threading

app = Flask(__name__)
socketio = SocketIO(app)
# Adjust your serial port and baud rate
ser = serial.Serial('/dev/tty.usbmodem11401', 9600, timeout=1)


@app.route('/')
def index():
    # You will create this HTML file in the next step
    return render_template('index.html')


def read_serial():
    while True:
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').strip()
            socketio.emit('orientation_data', data)


if __name__ == '__main__':
    t = threading.Thread(target=read_serial)
    t.start()
    socketio.run(app, debug=True)
