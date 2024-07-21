#ifndef MADGWICK_AHRS_H
#define MADGWICK_AHRS_H

// Include necessary libraries
#include <math.h>

#define betaDef 0.1f // 2 * proportional gain

extern float beta; // Algorithm gain

// Declare global variables
extern float q0, q1, q2, q3;   // Quaternion of sensor frame relative to auxiliary frame
extern float roll, pitch, yaw; // Euler angles
extern int anglesComputed;     // Flag to indicate if angles have been computed

// Function declarations
void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);
float invSqrt(float x);
void computeAngles();
float getRoll();
float getPitch();
float getYaw();

#endif // MADGWICK_AHRS_H
