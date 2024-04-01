#include <math.h>

#include <iostream>
using namespace std;
// double accel_x, accel_y, accel_z = 0;
// double twist_x, twist_y, twist_z = 0;

//-------------------------------------------------------------------------------------------
// Definitions

#define sampleFreqDef 104.0f  // sample frequency in Hz
#define betaDef 0.1f          // 2 * proportional gain

float q0 = 0.72f;
float q1 = -0.11f;
float q2 = 0.68f;
float q3 = 0.11f;
float invSampleFreq = 1.0f / sampleFreqDef;
float anglesComputed = false;
float gain = .1;
float beta;
float grav[2];
double pitch;
double roll;
double yaw;
const float PI = 3.14159265358979323846f;
//============================================================================================
// Functions

//-------------------------------------------------------------------------------------------
// AHRS algorithm update

// Complementary filter parameter
// Accelerometer sample rate = 104.00 Hz

// Time tracking
unsigned long previousTime = 0;

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

void updateIMU(float gx, float gy, float gz, float ax, float ay, float az,
               float dt) {
    float recipNorm = 0;
    float s0, s1, s2, s3 = 0;
    float qDot1, qDot2, qDot3, qDot4 = 0;
    float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2, _8q1, _8q2, q0q0, q1q1,
        q2q2, q3q3 = 0;

    // Convert gyroscope degrees/sec to radians/sec
    gx *= 0.0174533f;
    gy *= 0.0174533f;
    gz *= 0.0174533f;

    // print all the values
    std::cout << "gx: " << gx << " gy: " << gy << " gz: " << gz << " ax: " << ax
              << " ay: " << ay << " az: " << az << " dt: " << dt << std::endl;

    // Rate of change of quaternion from gyroscope
    qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
    qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
    qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);
    // print qDot1, qDot2, qDot3, qDot4
    std::cout << "qDot1: " << qDot1 << " qDot2: " << qDot2
              << " qDot3: " << qDot3 << " qDot4: " << qDot4 << std::endl;

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
                            s3 * s3);  // normalise step magnitude
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

    // print qDot1, qDot2, qDot3, qDot4
    std::cout << "qDot1: " << qDot1 << " qDot2: " << qDot2
              << " qDot3: " << qDot3 << " qDot4: " << qDot4 << std::endl;

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
    std::cout << "q0: " << q0 << " q1: " << q1 << " q2: " << q2 << " q3: " << q3
              << std::endl;
}

void computeAngles() {  // in degrees
    std::cout << "q0: " << q0 << " q1: " << q1 << " q2: " << q2 << " q3: " << q3
              << std::endl;
    roll = atan2f(q0 * q1 + q2 * q3, 0.5f - q1 * q1 - q2 * q2) * 180.0 / PI;
    pitch = asinf(-2.0f * (q1 * q3 - q0 * q2)) * 180.0 / PI;
    yaw = atan2f(q1 * q2 + q0 * q3, 0.5f - q2 * q2 - q3 * q3) * 180.0 / PI;
    cout << "Calculated Pitch: " << pitch << " Roll: " << roll
         << " Yaw: " << yaw << endl;
    // grav[0] = 2.0f * (q1 * q3 - q0 * q2);
    // grav[1] = 2.0f * (q0 * q1 + q2 * q3);
    // grav[2] = 2.0f * (q1 * q0 - 0.5f + q3 * q3);
    anglesComputed = 1;
}

void unit_test() {
    // real data
    // Pitch: 85.17 Roll: 158.02 Yaw: -155.47 a_x: -1.00 a_y: 0.03 a_z: -0.08
    // g_x: 1.16 g_y: 0.06 g_z: 0.73 Time: 27463
    // Pitch: 85.25 Roll: 159.59 Yaw: -153.90 a_x: -1.00 a_y: 0.03 a_z: -0.08
    // g_x: -0.85 g_y: 0.37 g_z: 0.18 Time: 27477
    float g_x = -0.85;
    float g_y = 0.37;
    float g_z = 0.18;
    float deltaT = (27477 - 27463) / 1000.0f;
    deltaT = 0.01;
    float accel_x = -1;
    float accel_y = 0.03;
    float accel_z = -0.08;
    updateIMU(g_x, g_y, g_z, accel_x, accel_y, accel_z, deltaT);
    computeAngles();
    // print expected pitch, roll, yaw
    std::cout << "Expected Pitch: 85.25 Roll: ~0 Yaw: -153.90" << std::endl;
    // print pitch, roll, and yaw from the data
    std::cout << "Arduino output: Pitch: 85.25 Roll: 159.59 Yaw: -153.90"
              << std::endl;

    std::cout << "Next one is correct" << std::endl;
    // Pitch: 4.23 Roll: -0.02 Yaw: 21.20 a_x: -0.07 a_y: -0.01 a_z: 1.00 g_x:
    // -0.92 g_y: -1.22 g_z: 0.73 Time: 222417 Pitch: 4.21 Roll: -0.14
    // Yaw: 21.21 a_x: -0.07 a_y: -0.01 a_z: 0.99 g_x: -0.31 g_y: 0.18 g_z: 1.65
    // Time: 222427
    g_x = -0.31;
    g_y = 0.18;
    g_z = 1.65;
    deltaT = (222427 - 222417) / 1000.0f;
    deltaT = 0.01;
    accel_x = -0.07;
    accel_y = -0.01;
    accel_z = 0.99;
    updateIMU(g_x, g_y, g_z, accel_x, accel_y, accel_z, deltaT);
    computeAngles();
    // print expected pitch, roll, yaw
    std::cout << "Expected Pitch: 4.21 Roll: -0.14 Yaw: 21.21" << std::endl;
    // print pitch, roll, and yaw from the data
    std::cout << "Arduino output: Pitch: 4.21 Roll: -0.14 Yaw: 21.21"
              << std::endl;
}

int main() {
    unit_test();
    return 0;
}