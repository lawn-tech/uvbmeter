/*! \file
 *  \brief Header file for the UvbMeter class.
 *  \author lawn-tech
 *  \date March 2021
 */

#ifndef _UVB_METER_H_
#define _UVB_METER_H_

#include <M5StickC.h>
#include <ADS1X15.h>
#include <Preferences.h>
#include "MotionSensor.h"
#include "CountDownTimer.h"

//! UVB Meter
class UvbMeter
{
  private:
    TFT_eSprite sprite;  //!< The sprite to draw values. 
    MotionSensor motionSensor;  //!< The motion sensor for detecting motion.
    CountDownTimer powerOffTimer;  //!< The count down timer for auto power off.
    ADS1014 ads;  //!< The A/D converter.
    Preferences preferences;
    float voltage;  //!< The output voltage of the UVB sensor measured.
    float offsetVoltage;
    bool holdState;  //!< The hold state. \li \c true Data hold is ON. \li \c false Data hold is OFF.
    bool errorState;  //!< The error state. \li \c true An error has occurred (The sensor HAT is not connected). \li \c false No error has occurred.
    bool prevBtnState;  //!< The previous button state. \li \c true Button A or B was pressed. \li \c false Neither button A nor B was pressed.

    void UpdateVoltage();
  public:
    UvbMeter();
    ~UvbMeter();
    void OnSetup();
    void OnLoop();
    void Draw();
    void PowerOff();
    bool IsHold() const;
    void SetHold(bool holdState);
    bool IsError() const;
    void SetError(bool errorState);
    float GetVoltage() const;
    float GetUvPower() const;
    float GetUvIndex() const;
};

#endif
