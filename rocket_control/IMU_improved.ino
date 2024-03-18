#include <Arduino_LSM6DSOX.h>
#include <math.h>
// double accel_x, accel_y, accel_z = 0;
// double twist_x, twist_y, twist_z = 0;

//-------------------------------------------------------------------------------------------
// Definitions

#define sampleFreqDef 104.0f // sample frequency in Hz
#define betaDef 0.1f         // 2 * proportional gain

float q0 = 1.0f;
float q1 = 0.0f;
float q2 = 0.0f;
float q3 = 0.0f ;
float invSampleFreq = 1.0f / sampleFreqDef;
float anglesComputed = false;
float gain = .1;
float beta;
float grav[2];
//============================================================================================
// Functions

//-------------------------------------------------------------------------------------------
// AHRS algorithm update

// Complementary filter parameter
// Accelerometer sample rate = 104.00 Hz

// Time tracking
unsigned long previousTime = 0;

void setupIMU()
{
  Serial.begin(9600);
  while (!Serial)
    ;
  if (!IMU.begin())
  {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }
  beta = gain;
}

void update_accel()
{
  float x, y, z;

  if (IMU.accelerationAvailable())
  {
    IMU.readAcceleration(x, y, z);

    // Serial.print(x);
    // Serial.print('\t');
    // Serial.print(y);
    // Serial.print('\t');
    // Serial.println(z);
    accel_x = x;
    accel_y = y;
    accel_z = z;
  }
}

void update_gyro()
{
  float x, y, z;

  if (IMU.gyroscopeAvailable())
  {
    IMU.readGyroscope(x, y, z);

    // Serial.print(x);
    // Serial.print('\t');
    // Serial.print(y);
    // Serial.print('\t');
    // Serial.println(z);
    twist_x = x;
    twist_y = y;
    twist_z = z;
  }
}


void updateIMU(float gx, float gy, float gz, float ax,
                                  float ay, float az, float dt) {
  float recipNorm;
  float s0, s1, s2, s3;
  float qDot1, qDot2, qDot3, qDot4;
  float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2, _8q1, _8q2, q0q0, q1q1, q2q2,
      q3q3;

  // Convert gyroscope degrees/sec to radians/sec
  gx *= 0.0174533f;
  gy *= 0.0174533f;
  gz *= 0.0174533f;

  // Rate of change of quaternion from gyroscope
  qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
  qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
  qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
  qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

  // Compute feedback only if accelerometer measurement valid (avoids NaN in
  // accelerometer normalisation)
  if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

    // Normalise accelerometer measurement
    recipNorm = invSqrt(ax * ax + ay * ay + az * az);
    ax *= recipNorm;
    ay *= recipNorm;
    az *= recipNorm;

    // Auxiliary variables to avoid repeated arithmetic
    _2q0 = 2.0f * q0;
    _2q1 = 2.0f * q1;
    _2q2 = 2.0f * q2;
    _2q3 = 2.0f * q3;
    _4q0 = 4.0f * q0;
    _4q1 = 4.0f * q1;
    _4q2 = 4.0f * q2;
    _8q1 = 8.0f * q1;
    _8q2 = 8.0f * q2;
    q0q0 = q0 * q0;
    q1q1 = q1 * q1;
    q2q2 = q2 * q2;
    q3q3 = q3 * q3;

    // Gradient decent algorithm corrective step
    s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
    s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 +
         _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
    s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 +
         _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
    s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;
    recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 +
                        s3 * s3); // normalise step magnitude
    s0 *= recipNorm;
    s1 *= recipNorm;
    s2 *= recipNorm;
    s3 *= recipNorm;

    // Apply feedback step
    qDot1 -= beta * s0;
    qDot2 -= beta * s1;
    qDot3 -= beta * s2;
    qDot4 -= beta * s3;
  }

  // Integrate rate of change of quaternion to yield quaternion
  q0 += qDot1 * dt;
  q1 += qDot2 * dt;
  q2 += qDot3 * dt;
  q3 += qDot4 * dt;

  // Normalise quaternion
  recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
  q0 *= recipNorm;
  q1 *= recipNorm;
  q2 *= recipNorm;
  q3 *= recipNorm;
  anglesComputed = 0;
}

float invSqrt(float x) {
  float halfx = 0.5f * x;
  union {
    float f;
    long i;
  } conv = {x};
  conv.i = 0x5f3759df - (conv.i >> 1);
  conv.f *= 1.5f - (halfx * conv.f * conv.f);
  conv.f *= 1.5f - (halfx * conv.f * conv.f);
  return conv.f;
}

void computeAngles() {
  roll = atan2f(q0 * q1 + q2 * q3, 0.5f - q1 * q1 - q2 * q2);
  pitch = asinf(-2.0f * (q1 * q3 - q0 * q2));
  yaw = atan2f(q1 * q2 + q0 * q3, 0.5f - q2 * q2 - q3 * q3);
  grav[0] = 2.0f * (q1 * q3 - q0 * q2);
  grav[1] = 2.0f * (q0 * q1 + q2 * q3);
  grav[2] = 2.0f * (q1 * q0 - 0.5f + q3 * q3);
  anglesComputed = 1;
}

void updateIMUh() {
  // Assuming you have a function to read acceleration data from your accelerometer
  // float accel_x, accel_y, accel_z;
  bool newDataA = false; // Set to true if new data is read
  bool newDataG = false;
  while (!IMU.accelerationAvailable()) {
    IMU.readAcceleration(accel_x, accel_y, accel_z);
    newDataA = true;
  }
  while (!IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(g_x, g_y, g_z);
    newDataA = true;
  }
  updateIMU(g_x, g_y, g_z, accel_x, accel_y, accel_z, deltaTime); 
}