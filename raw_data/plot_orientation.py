import pandas as pd
import matplotlib.pyplot as plt

# Step 1: Read and parse the data
data = {
    'time': [],
    'pitch': [],
    'roll': [],
    'yaw': []
}

# Assuming your data is in 'data.txt'
with open('orientation_2.txt', 'r') as file:
    for line in file:
        parts = line.split()
        data['pitch'].append(float(parts[1]))
        data['roll'].append(float(parts[3]))
        data['yaw'].append(float(parts[5]))
        data['time'].append(float(parts[-1]))

# Convert to pandas DataFrame for easier plotting
df = pd.DataFrame(data)

# Step 2: Plot the data
plt.figure(figsize=(10, 7))

# Plotting each parameter
plt.plot(df['time'], df['pitch'], label='Pitch')
plt.plot(df['time'], df['roll'], label='Roll')
plt.plot(df['time'], df['yaw'], label='Yaw')

# Adding titles and labels
plt.title('Pitch, Roll, and Yaw over Time')
plt.xlabel('Time (ms)')
plt.ylabel('Degrees')
plt.legend()

# Show the plot
plt.show()
