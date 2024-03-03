import matplotlib.pyplot as plt
import re

# Initialize lists to store the data
pitch = []
roll = []
yaw = []
velocity_x = []
velocity_y = []
velocity_z = []
position_x = []
position_y = []
position_z = []

# Function to extract numbers from a string


def extract_numbers(s):
    return [float(num) for num in re.findall(r"-?\d+\.?\d*", s)]


# Read and parse the data from the file
with open("everything_0.txt", "r") as file:
    for line in file:
        # Extract numerical values using regular expressions
        numbers = extract_numbers(line)

        # Check if the line has the expected 9 numerical values
        if len(numbers) == 9:
            pitch.append(numbers[0])
            roll.append(numbers[1])
            yaw.append(numbers[2])
            velocity_x.append(numbers[3])
            velocity_y.append(numbers[4])
            velocity_z.append(numbers[5])
            position_x.append(numbers[6])
            position_y.append(numbers[7])
            position_z.append(numbers[8])
        else:
            # Optionally, print a warning or handle the incomplete data
            print(f"Warning: Skipping line due to insufficient data: {
                  line.strip()}")

# Plot each parameter over iterations
# plt.yscale('symlog')
fig, axs = plt.subplots(3, 3, figsize=(15, 10))
fig.suptitle('Flight Parameters Over Iterations')

axs[0, 0].plot(pitch)
axs[0, 0].set_title('Pitch')

axs[0, 1].plot(roll)
axs[0, 1].set_title('Roll')

axs[0, 2].plot(yaw)
axs[0, 2].set_title('Yaw')

axs[1, 0].plot(velocity_x)
axs[1, 0].set_title('Velocity X')

axs[1, 1].plot(velocity_y)
axs[1, 1].set_title('Velocity Y')

axs[1, 2].plot(velocity_z)
axs[1, 2].set_title('Velocity Z')

axs[2, 0].plot(position_x)
axs[2, 0].set_title('Position X')

axs[2, 1].plot(position_y)
axs[2, 1].set_title('Position Y')

axs[2, 2].plot(position_z)
axs[2, 2].set_title('Position Z')

for ax in axs.flat:
    ax.set(xlabel='Iteration', ylabel='Value')

# Hide x labels and tick labels for top plots and y ticks for right plots.
for ax in axs.flat:
    ax.label_outer()

plt.tight_layout(rect=[0, 0.03, 1, 0.95])
plt.show()
