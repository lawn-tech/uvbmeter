/*! \file
 *  \brief Implementation of the MotionSensor class.
 *  \author lawn-tech
 *  \date March 2021
 */

#include "MotionSensor.h"

//! Constructor
MotionSensor::MotionSensor() :
  firstTime(true),
  ax1(0.0f), ay1(0.0f), az1(0.0f),
  dxf(0.0f), dyf(0.0f), dzf(0.0f),
  value(0.0f),
  threshold(0.0001f),
  alpha(0.1f)
{
}

//! Destructor
MotionSensor::~MotionSensor()
{
}

//! Update the state with acceleration
/*!
 *  \param[in] ax The x component of acceleration.
 *  \param[in] ay The y component of acceleration.
 *  \param[in] az The z component of acceleration.
 */
void MotionSensor::Update(float ax, float ay, float az)
{
  if (firstTime) {
    dxf = dyf = dzf = 0.0f;
    firstTime = false;
  }
  else {
    dxf = alpha * (ax - ax1) + (1.0f - alpha) * dxf;
    dyf = alpha * (ay - ay1) + (1.0f - alpha) * dyf;
    dzf = alpha * (az - az1) + (1.0f - alpha) * dzf;
  }

  ax1 = ax;
  ay1 = ay;
  az1 = az;

  value = dxf * dxf + dyf * dyf + dzf * dzf;
}

//! Retrieve whether the object is stationary.
/*!
 *  \retval true The object is stationary.
 *  \retval false The object is not stationary.
 */
bool MotionSensor::IsStationary() const
{
  return value < threshold;
}

//! Retrieve the evaluation value of the motion.
float MotionSensor::GetValue() const
{
  return value;
}
