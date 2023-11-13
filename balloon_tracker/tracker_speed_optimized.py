"""import sensor, image, time

# Set the thresholds for the red color (may need adjustment)<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
red_threshold = (30, 100, 15, 127, 15, 127)  # LAB color space thresholds for red
distance_threshold = 150  # Distance threshold for resolution switch (in cm)
pixels_threshold = 200  # Minimum number of pixels in a blob
area_threshold = 200  # Minimum area (in pixels) of a blob
BALLOON_WIDTH = 30  # Known width of the balloon in cm
FOCAL_LENGTH_PIXELS = 1257  # Estimated effective focal length in pixels
# Focal Length (in pixels)= Known Object Size/ (Object Size in Image (in pixels)×Distance to Object (in same unit as known size)

# Initialize the sensor
sensor.reset()
sensor.set_pixformat(sensor.RGB565)

# Start in high-resolution mode
sensor.set_framesize(sensor.SVGA)  # Replace with the actual constant for 2MP resolution
sensor.skip_frames(time = 2000)
clock = time.clock()

def estimate_distance(blob, known_width, focal_length):
    # Calculate the distance based on the perceived width of the blob
    # and the known width of the object (red balloon).
    # This method assumes the camera and object are aligned on the same plane.
    percieved_width = blob.w() # Width of the blob in pixels
    distance = (known_width * focal_length) / percieved_width
    return distance

while(True):
    clock.tick()
    img = sensor.snapshot()  # Create a mutable copy of the image

    blobs = img.find_blobs([red_threshold], pixels_threshold=200, area_threshold=200, merge=True)
    print(size(blobs))

    if blobs:
        largest_blob = max(blobs, key=lambda b: b.pixels())
        img.draw_cross(largest_blob.cx(), largest_blob.cy())  # Now you can draw on the image
        distance = estimate_distance(largest_blob, BALLOON_WIDTH, FOCAL_LENGTH_PIXELS)

        if high_res_mode and distance < distance_threshold:
            # Switch to low-resolution, high frame rate mode
            sensor.set_framesize(sensor.VGA)  # Or another appropriate lower resolution
            high_res_mode = False

        print("Blob center:", largest_blob.cx(), largest_blob.cy(), "Distance:", distance)

    print("FPS:", clock.fps())"""

import pyb # Import module for board related functions
import sensor # Import the module for sensor related functions
import image # Import module containing machine vision algorithms
import time # Import module for tracking elapsed time

sensor.reset() # Resets the sensor
sensor.set_pixformat(sensor.RGB565) # Sets the sensor to RGB
sensor.set_framesize(sensor.QVGA) # Sets the resolution to 320x240 px

sensor.skip_frames(time = 2000) # Skip some frames to let the image stabilize

# Define the min/max LAB values we're looking for
thresholdsApple = (24, 60, 32, 54, 0, 42)
thresholdsBanana = (45, 75, 5, -10, 40, 12)

ledRed = pyb.LED(1) # Initiates the red led
ledGreen = pyb.LED(2) # Initiates the green led

clock = time.clock() # Instantiates a clock object

while(True):
    clock.tick() # Advances the clock
    img = sensor.snapshot()# Takes a snapshot and saves it in memory
    # Find blobs with a minimal area of 50x50 = 2500 px
    # Overlapping blobs will be merged
    blobs = img.find_blobs([thresholdsApple, thresholdsBanana], area_threshold=2500, merge=True)

    # Draw blobs
    for blob in blobs:
        # Draw a rectangle where the blob was found
        img.draw_rectangle(blob.rect(), color=(0,255,0))
        # Draw a cross in the middle of the blob
        img.draw_cross(blob.cx(), blob.cy(), color=(0,255,0))

    # Turn on green LED if a blob was found
    if len(blobs) > 0:
        ledGreen.on()
        ledRed.off()
    else:
    # Turn the red LED on if no blob was found
        ledGreen.off()
        ledRed.on()

    pyb.delay(50) # Pauses the execution for 50ms
    print(clock.fps()) # Prints the framerate to the serial console
