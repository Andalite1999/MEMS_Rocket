import matplotlib.pyplot as plt
import numpy as np

# Function to read data from the file and plot


def plot_accelerometer_data(file_path, type='accelerometer'):
    # Load data from the text file
    data = np.loadtxt(file_path, delimiter='\t')

    # Assuming the data columns are X, Y, Z
    plt.figure(figsize=(10, 6))  # Move this line outside of the loop

    x = data[:, 0]
    y = data[:, 1]
    z = data[:, 2]

    # Creating time axis assuming each row is a new measurement
    # If you have a specific sampling rate, you can adjust the time array accordingly
    t = np.arange(len(x))

    # Plotting the data
    plt.plot(t, x, label='X')
    plt.plot(t, y, label='Y')
    plt.plot(t, z, label='Z')

    plt.xlabel('Iteration')
    plt.yscale('symlog')
    plt.legend()
    plt.grid(True)

    if type == 'accelerometer':
        plt.title('Accelerometer Data')
        plt.ylabel('Acceleration (g)')
    else:
        plt.title('Gyroscope Data')
        plt.ylabel('degrees/second')

    plt.show()


file_path_0 = 'accelerometer.txt'
file_path_1 = 'gyro.txt'
plot_accelerometer_data(file_path_0, type='accelerometer')
plot_accelerometer_data(file_path_1, type='gyro')
