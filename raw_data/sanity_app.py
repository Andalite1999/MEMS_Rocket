from vpython import canvas, vector, color, arrow, rate

# Setup the 3D scene
scene = canvas(title='Rotating Coordinate Plane')

# Define axes with a specific length by adjusting the axis vector magnitude
x_axis = arrow(pos=vector(0, 0, 0), axis=vector(2, 0, 0),
               shaftwidth=0.1, color=color.red)  # Length is 2
y_axis = arrow(pos=vector(0, 0, 0), axis=vector(0, 2, 0),
               shaftwidth=0.1, color=color.green)  # Length is 2
z_axis = arrow(pos=vector(0, 0, 0), axis=vector(0, 0, 2),
               shaftwidth=0.1, color=color.blue)  # Length is 2

# Rotation speed
rotation_speed = 0.01  # Adjust this value to change the speed of rotation

rotation_axis = vector(0, 1, 1)  # Rotate around the Y-axis
while True:
    rate(100)  # Determines how fast the screen updates

    # Apply a rotation to each axis
    # Rotate around the Y-axis
    x_axis.rotate(angle=rotation_speed, axis=rotation_axis)
    # Rotate around the X-axis
    y_axis.rotate(angle=rotation_speed, axis=rotation_axis)
    # Rotate around the Y-axis again for z-axis for consistency
    z_axis.rotate(angle=rotation_speed, axis=rotation_axis)
