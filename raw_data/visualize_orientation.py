from vpython import canvas, vector, color, arrow, rate, label, radians, cos, sin
import serial
import time

# Function to convert degrees to radians


def deg_to_rad(degrees):
    return degrees * (3.141592653589793 / 180)


def orientation_vector(pitch, yaw, roll):
    # Convert angles from degrees to radians for calculations
    pitch_rad = radians(pitch)
    yaw_rad = radians(yaw)
    roll_rad = radians(roll)

    # Assuming initial orientation along the z-axis (0, 0, 1)
    # Calculate new orientation
    # These calculations can be adjusted based on your specific needs
    # Here is a simple example for illustration, focusing on yaw (rotation around the y-axis)
    x = cos(yaw_rad)
    y = sin(yaw_rad)
    z = 1  # Keeping the initial orientation for simplicity

    # Return the new vector
    return vector(x, y, z)


# Setup the 3D scene
scene = canvas(title='Orientation Visualization')
scene.forward = vector(0, -1, -1)

# Define axes
x_axis = arrow(pos=vector(0, 0, 0), axis=vector(2, 0, 0),
               shaftwidth=0.1, color=color.red)  # Length is 2
y_axis = arrow(pos=vector(0, 0, 0), axis=vector(0, 2, 0),
               shaftwidth=0.1, color=color.green)  # Length is 2
z_axis = arrow(pos=vector(0, 0, 0), axis=vector(0, 0, 2),
               shaftwidth=0.1, color=color.blue)  # Length is 2

# Labels for axes
# label(pos=x_axis.axis, text='X', xoffset=10)
# label(pos=y_axis.axis, text='Y', yoffset=10)
# label(pos=z_axis.axis, text='Z', zoffset=10)

# Setup serial connection
serial_port = '/dev/tty.usbmodem21101'
baud_rate = 9600  # Update this to match your device's baud rate
ser = serial.Serial(serial_port, baud_rate)


def update_orientation(ser):
    prev_pitch_rad = 0
    prev_roll_rad = 0
    prev_yaw_rad = 0
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            try:
                # Assuming data format is "Pitch: VALUE Roll: VALUE Yaw: VALUE"
                data = line.split()
                pitch = float(data[1])
                roll = float(data[3])
                yaw = float(data[5])

                # Convert to radians
                pitch_rad = deg_to_rad(pitch)
                roll_rad = deg_to_rad(roll)
                yaw_rad = deg_to_rad(yaw)

                delta_pitch_rad = pitch_rad - prev_pitch_rad
                delta_roll_rad = roll_rad - prev_roll_rad
                delta_yaw_rad = yaw_rad - prev_yaw_rad
                # print(delta_pitch_rad, delta_roll_rad, delta_yaw_rad)

                # how to keep all the axis perpendicular to each other

                # Update axes orientations
                # x_axis = arrow(pos=vector(0, 0, 0), axis=orientation_vector(
                #     pitch, yaw, roll), shaftwidth=0.1, color=color.red)
                x_axis.rotate(angle=delta_roll_rad, axis=vector(1, 0, 0))
                x_axis.rotate(angle=delta_pitch_rad, axis=vector(0, 0, -1))
                x_axis.rotate(angle=delta_yaw_rad, axis=vector(0, 1, 0))
                # set the orientation of the x axis

                y_axis.rotate(angle=delta_roll_rad, axis=vector(1, 0, 0))
                y_axis.rotate(angle=delta_pitch_rad, axis=vector(0, 0, -1))
                y_axis.rotate(angle=delta_yaw_rad, axis=vector(0, 1, 0))

                z_axis.rotate(angle=delta_roll_rad, axis=vector(1, 0, 0))
                z_axis.rotate(angle=delta_pitch_rad, axis=vector(0, 0, -1))
                z_axis.rotate(angle=delta_yaw_rad, axis=vector(0, 1, 0))
                t = 5

                if (abs(delta_pitch_rad) > t or abs(delta_roll_rad) > t or abs(delta_yaw_rad) > t):
                    print("large change detected")

                # if (delta_pitch_rad > 0):
                #     print("pitch up")
                # elif (delta_pitch_rad < 0):
                #     print("pitch down")
                # print(pitch, roll, yaw)
                prev_pitch_rad = pitch_rad
                prev_roll_rad = roll_rad
                prev_yaw_rad = yaw_rad
                # time.sleep(0.001)  # Adjust sleep time as needed
            except ValueError:
                # Handle the case where conversion fails
                print("Could not convert data to floats.")

            rate(500)  # Adjust as needed


update_orientation(ser)
