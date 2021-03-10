/*! \file
 *  \brief Implementation of the CountDownTimer class.
 *  \author lawn-tech
 *  \date March 2021
 */

#include "CountDownTimer.h"

//! Constructor
CountDownTimer::CountDownTimer():
  timeLimit(0), count(0), prevMillis(0)
{
}

//! Destructor
CountDownTimer::~CountDownTimer()
{
}

//! Start the timer
/*!
 *  \param[in] timeLimit The time limit in ms.
 *  \param[in] millis The present time in ms. Normally, pass the return value of millis() function.
 */
void CountDownTimer::Begin(unsigned long timeLimit, unsigned long millis)
{
  this->timeLimit = timeLimit;
  count = timeLimit;
  prevMillis = millis;
}

//! Update the timer with present time
/*!
 *  \param[in] millis The present time in ms. Normally, pass the return value of millis() function.
 */
void CountDownTimer::Update(unsigned long millis)
{
  unsigned long d = millis - prevMillis;

  if (d > count) {
    count = 0;
  }
  else {
    count -= d;
  }

  prevMillis = millis;
}

//! Reset the timer
void CountDownTimer::Reset()
{
  count = timeLimit;
}

//! Retrieve whether the timer is expired 
/*!
 * \retval true the timer is expired
 * \retval false the timer is not expired
 */
bool CountDownTimer::IsExpired() const
{
  return count == 0;
}

//! Retrieve the present count in ms
unsigned long CountDownTimer::GetCount() const
{
  return count;
}
