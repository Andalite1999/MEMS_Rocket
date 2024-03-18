#include <Arduino_LSM6DSOX.h>

// double accel_x, accel_y, accel_z = 0;
// double twist_x, twist_y, twist_z = 0;

// Complementary filter parameter
// const double alpha = 0.5;

// Accelerometer sample rate = 104.00 Hz



// Time tracking
// unsigned long previousTime = 0;

void updateOrientation() {
  // Calculate the time elapsed
  // unsigned long currentTime = micros();
  // float deltaTime = (currentTime - lastUpdateTime) / 1000.0; // Convert to seconds
  // previousTime = currentTime;

  // Accelerometer angles (no change from previous)
  // double accel_roll = atan2(-accel_x, accel_z) * 180.0 / PI;
  double accel_roll = atan2(accel_y, accel_z) * 180.0 / PI;
  // double accel_pitch = atan2(accel_y, sqrt(accel_x * accel_x + accel_z * accel_z)) * 180.0 / PI;
  // double accel_pitch = atan(-accel_x / (accel_y * sin(accel_roll * PI / 180.0) + accel_z * cos(accel_roll * PI / 180.0))) * 180.0 / PI;
  double accel_pitch = atan2(accel_y * sin(accel_roll * PI / 180.0) + accel_z * cos(accel_roll * PI / 180.0), -accel_x) * 180.0 / PI;
  //phase ambiguity, when sine and cosine. Observe when printing out all three axis
  //note that the pitch goes crazy in one of the orientations
  //mi

  // Gyroscope rates in degrees/sec
  double gyro_pitch_rate = twist_x;
  double gyro_roll_rate = twist_y;
  double gyro_yaw_rate = twist_z; // Z-axis rotation rate for yaw

  // Integrate the gyroscope data to update pitch, roll, and now yaw
  pitch = alpha * (pitch + gyro_pitch_rate * deltaTime) + (1 - alpha) * accel_pitch;
  roll = alpha * (roll + gyro_roll_rate * deltaTime) + (1 - alpha) * accel_roll;
  yaw += gyro_yaw_rate * deltaTime; // Integrate the gyro yaw rate over time to estimate yaw

  // Correct yaw angle to keep it within -180 to 180 degrees
  if (yaw > 180) {
    yaw -= 360;
  } else if (yaw < -180) {
    yaw += 360;
  }

  // Print the orientation
  // Serial.print("Pitch: ");
  // Serial.print(pitch);
  // Serial.print(" Roll: ");
  // Serial.print(roll);
  // Serial.print(" Yaw: ");
  // Serial.print(yaw);
}

void updateVelocity() {
  // Assuming you have a function to read acceleration data from your accelerometer
  // float accel_x, accel_y, accel_z;
  bool newData = false; // Set to true if new data is read
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(accel_x, accel_y, accel_z);
    newData = true;
  }

  // Serial.print("accel_x: ");
  // Serial.print(accel_x);
  // Serial.print(" accel_y: ");
  // Serial.print(accel_y);
  // Serial.print(" accel_z: ");
  // Serial.println(accel_z);

  // Convert pitch and roll to radians for calculation
  if (newData){
    double pitchRad = pitch * DEG_TO_RAD;
    double rollRad = roll * DEG_TO_RAD;

    // double gx = sin(rollRad) * 9.81;
    // double gy = -sin(pitchRad) * cos(rollRad) * 9.81;
    // double gz = -cos(pitchRad) * cos(rollRad) * 9.81;

    // Print the orientation
    Serial.print("Pitch: ");
    Serial.print(pitch);
    Serial.print(" Roll: ");
    Serial.print(roll);
    Serial.print(" Yaw: ");
    Serial.print(yaw);

    // Serial.print(" pitchRad: ");
    // Serial.print(pitchRad);
    // Serial.print(" rollRad: ");
    // Serial.print(rollRad);
    // Calculate gravity vector
    // g_x = -sin(rollRad) * 9.81 * cos(pitchRad);

    g_x = -sin(rollRad) * 9.81;
    g_y = sin(pitchRad) * 9.81;
    g_z = cos(pitchRad) * cos(rollRad) * 9.81;

    // Serial.print(" Gravity X: ");
    // Serial.print(g_x);
    // Serial.print(" m/s^2, Y: ");
    // Serial.print(g_y);
    // Serial.print(" m/s^2, Z: ");
    // Serial.print(g_z);
    // Serial.print(" m/s^2");

    // Correct acceleration readings by subtracting gravity component
    // accel_x = (accel_x * 9.81) - g_x;
    // accel_y = (accel_y * 9.81) - g_y;
    // accel_z = (accel_z * 9.81) - g_z;

     accel_x = (accel_x * 9.81);
    accel_y = (accel_y * 9.81);
    accel_z = (accel_z * 9.81);

    // Print velocities
    // Serial.print(" Velocity X: ");
    // Serial.print(velocity_x);
    // Serial.print(" m/s, Y: ");
    // Serial.print(velocity_y);
    // Serial.print(" m/s, Z: ");
    // Serial.print(velocity_z);
    // Serial.print(" m/s");

    // Serial.print(" Acceleration X: ");
    // Serial.print(accel_x);
    // Serial.print(" m/s^2, Y: ");
    // Serial.print(accel_y);
    // Serial.print(" m/s^2, Z: ");
    // Serial.print(accel_z);
    // Serial.print(" m/s^2");
    Serial.print(" accel_x: ");
    Serial.print(accel_x);
    Serial.print(" accel_y: ");
    Serial.print(accel_y);
    Serial.print(" accel_z: ");
    Serial.print(accel_z);
    Serial.print(" time: ");
    Serial.println(currentTime);
    // Serial.print(" Position X: ");
    // Serial.print(position_x);
    // Serial.print(" m, Y: ");
    // Serial.print(position_y);
    // Serial.print(" m, Z: ");
    // Serial.print(position_z);
    // Serial.println(" m");


    // Integrate acceleration to get velocity
    velocity_x += ((accel_x + last_accel_x) / 2) * deltaTime;
    velocity_y += ((accel_y + last_accel_y) / 2) * deltaTime;
    velocity_z += ((accel_z + last_accel_z) / 2) * deltaTime;

    last_accel_x = accel_x;
    last_accel_y = accel_y;
    last_accel_z = accel_z;
  }
  // lastUpdateTime = currentTime;
}

void updatePositionWithOrientation() {
    

    // Convert pitch, roll, and yaw to radians
    double pitchRad = pitch * DEG_TO_RAD;
    double rollRad = roll * DEG_TO_RAD;
    double yawRad = yaw * DEG_TO_RAD;

    // Calculate the rotation matrix components
    // Simplified rotation matrix, assuming small angles for pitch, roll, yaw
    double cosPitch = cos(pitchRad);
    double sinPitch = sin(pitchRad);
    double cosRoll = cos(rollRad);
    double sinRoll = sin(rollRad);
    double cosYaw = cos(yawRad);
    double sinYaw = sin(yawRad);

    // Transform velocity from body frame to world frame
    double velX_world = velocity_x * cosYaw * cosPitch + velocity_y * (cosYaw * sinPitch * sinRoll - sinYaw * cosRoll) + velocity_z * (cosYaw * sinPitch * cosRoll + sinYaw * sinRoll);
    double velY_world = velocity_x * sinYaw * cosPitch + velocity_y * (sinYaw * sinPitch * sinRoll + cosYaw * cosRoll) + velocity_z * (sinYaw * sinPitch * cosRoll - cosYaw * sinRoll);
    double velZ_world = velocity_x * -sinPitch + velocity_y * cosPitch * sinRoll + velocity_z * cosPitch * cosRoll;

    // Update position using transformed velocities
    position_x += velX_world * deltaTime;
    position_y += velY_world * deltaTime;
    position_z += velZ_world * deltaTime;

    // lastUpdateTime = currentTime; // Update the last update time
}