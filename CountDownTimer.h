/*! \file
 *  \brief Header file for the CountDownTimer class.
 *  \author lawn-tech
 *  \date March 2021
 */

#ifndef _COUNT_DOWN_TIMER_H_
#define _COUNT_DOWN_TIMER_H_

//! Count Down Timer
class CountDownTimer
{
  private:
    unsigned long timeLimit;  //!< The time limit in ms.
    unsigned long count;  //!< The present count in ms.
    unsigned long prevMillis;  //!< The value of the millis argument at previous call of the Update() function.

  public:
    CountDownTimer();
    ~CountDownTimer();

    void Begin(unsigned long timeLimit, unsigned long millis);
    void Update(unsigned long millis);
    void Reset();
    bool IsExpired() const;
    unsigned long GetCount() const;
};

#endif
