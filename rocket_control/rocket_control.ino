
#include <Servo.h>
#include <math.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Arduino_LSM6DSOX.h>


// current angle/orientation/acceleration IMU
float accel_x, accel_y, accel_z = 0;
double g_x, g_y, g_z = 0;
double twist_x, twist_y, twist_z = 0;
double yaw_corr_1s = .65; //subtract yaw by this value every 1000 micro
// airspeed
double position_x = 0, position_y = 0, position_z = 0; // Position in meters
double velocity_x = 0, velocity_y = 0, velocity_z = 0; // Velocity in m/s
double last_accel_x = 0, last_accel_y = 0, last_accel_z = 0; // Last acceleration values
unsigned long lastUpdateTime = 0; // Last update time in microseconds
unsigned long currentTime = 0;
float deltaTime = 0;
//quaternions
// Assuming these variables are declared globally
double pitch = 0, roll = 0, yaw = 0; // Euler angles
// double accel_x, accel_y, accel_z; // Accelerometer readings
// double twist_x, twist_y, twist_z; // Gyroscope readings
// double velocity_x, velocity_y, velocity_z; // Velocities
// double position_x, position_y, position_z; // Positions
// double last_accel_x, last_accel_y, last_accel_z; // Last acceleration readings
// unsigned long lastUpdateTime = 0; // Last update time
// const double alpha = 0.5; // Complementary filter parameter





// location
double pos_x, pos_y, pos_z = 0;
double orientation_x, orientation_y, orientation_z = 0; 
// Orientation angles
// double pitch = 0, roll = 0, yaw = 0;
// TODO: update path planning
// canard angles, note a parallel angle to rocket body is 0 degrees
float cr_angle = 0; //USING RADIANS
float cl_angle = 0;
// delay variables
unsigned long startTime;
unsigned long endTime;
unsigned long duration;
double dt = 0;
double g_trigger = 6; //acceleration threshold to begin control delay aft
double control_delay = 1000; // one second after G trigger hit
bool c = 0;

Servo cr_servo; // create servo object to control a servo
Servo cl_servo;
int cr_pin = 0; //TODO: which are right?
int cl_pin = 0;

void setup()
{
  // yaw_corr_1s = 0;
  setupIMU();
  setupServos();
  lastUpdateTime = millis(); 
}

void loop()
{
  currentTime = millis();
  deltaTime = (currentTime - lastUpdateTime) / 1000.0; // Time elapsed in seconds
  // update the current angle/orientation/acceleration of the rocket with the IMU
  update_accel();
  update_gyro();
  updateOrientation();
  // update the current velocity
  // update_airspeed();
  // update_location();
  // update the location of the target with the camera, calculate distance from target
  // update path planning
  // update the canard angles
  // get tick count
  // write_servo();
  save_data();
  endTime = millis();
  // Serial.print("Loop Duration: ");
  // Serial.print(duration);
  // Serial.print(" ms");
  updateVelocity();
  updatePositionWithOrientation();

  // Print acceleration
  

  

  // Optionally print the position to the Serial monitor
  
  // Delay for a bit to not flood the serial output
  // delay(100);
  // Serial.println();
  yaw -= yaw_corr_1s * deltaTime;
  lastUpdateTime = currentTime;
}

void write_servo()
{
  // for (a = -1.43; a <= 1.43; a += .01) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   cr_servo.write(final_p(a));              // tell servo to go to position in variable 'pos'
  //   // Serial.println("angle in degrees ");
  //   // Serial.println(a * 57.2958);
  //   // Serial.println("final position ");
  //   // Serial.println(final_p(a));
  //   Serial.print("degrees:");
  //   Serial.print(a * 57.2958);
  //   Serial.print(",");
  //   Serial.print("position:");
  //   Serial.println(final_p(a));
  //   delay(15);                       // waits 15 ms for the servo to reach the position
  // }
  double y_degrees = 90;
  double IMU_radian = y_degrees * M_PI / 180;
  cr_servo.write(final_p(IMU_radian)); // tell servo to go to position in variable 'pos'
  // Serial.println("angle in degrees ");
  // Serial.println(a * 57.2958);
  // Serial.println("final position ");
  // Serial.println(final_p(a));
  Serial.print("degrees:");
  // Serial.print(y_degrees());
  Serial.print(",");
  Serial.print("position:");
  Serial.println(final_p(IMU_radian));
  delay(15);
}

void save_data()
{

}
