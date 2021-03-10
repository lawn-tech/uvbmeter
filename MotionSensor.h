/*! \file
 *  \brief Header file for the MotionSensor class.
 *  \author lawn-tech
 *  \date March 2021
 */

#ifndef _MOTION_SENSOR_H_
#define _MOTION_SENSOR_H_

//! Motion Sensor
class MotionSensor
{
  private:
    bool firstTime;  //!< Flag indicating the first call to the Update() function.
    float ax1, ay1, az1;  //!< Previous value of acceleration.
    float dxf, dyf, dzf;  //!< The filtered value of the difference between the current and previous values of acceleration.
    float value; //!< Evaluation value of the motion.
    float threshold;  //!< Threshold to determine whether the object is stationary or not.
    float alpha;  //!< Filter coefficient.

  public:
    MotionSensor();
    ~MotionSensor();
    void Update(float ax, float ay, float az);
    bool IsStationary() const;
    float GetValue() const;
};

#endif
