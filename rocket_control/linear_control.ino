#include <cmath>

extern int cr_pos = 0; //TODO: is this the right start?  How to guarantee the canards get fully centered
extern int cl_pos = 0; 

extern int B_c = 11;
extern int A_b = 8;
extern int C_y = 9.5;
extern double stroke = 26.454;

struct servo_translation{ // s is the travel a is the desired angle
  int pos;
  double a;
  double s;
};

void setupServos() {
  cr_servo.attach(cr_pin);
  cl_servo.attach(cl_pin);
}

double *canard_angles(double x, double y)
{
    double *angles = new double[2];
    angles[0] = atan2(x, y);
    angles[1] = atan2(x, y);
    return angles;
}

void pathPlanning()
{
    // this function take in x and y coordinates of target.  The next step is to orient the rocket perpendicular to the target then a path to move the rocket forwards or backwards in tandem with the canards to move the rocket to the target.  The angles of attack will also depend on the current acceleration and speed of the rocket
}

// calculate current airspeed based off of acceleration and time, adding the integration of acceleration over time to the current airspeed
double current_airspeed(double current_airspeed, double acceleration, double time)
{
    return current_airspeed + acceleration * time;
}

int final_p(double angle)
{
  float s = (B_c * sin(angle)) + A_b * cos(asin((B_c * cos(angle) - C_y) / A_b));
  if (s <= 16.454 && s >= -10) // 26.454
    return (s + 10) * (180 / stroke);
  else
    return 0;
}
