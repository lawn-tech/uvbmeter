/*! \file
 *  \brief Implementation of the UvbMeter class.
 *  \author lawn-tech
 *  \date March 2021
 */

#include "UvbMeter.h"

//! Constructor
UvbMeter::UvbMeter() :
  sprite(&M5.Lcd), motionSensor(), powerOffTimer(),
  ads(0x48), voltage(0.0f), offsetVoltage(0.0f),
  holdState(false), errorState(false), prevBtnState(false)
{
}

//! Destructor
UvbMeter::~UvbMeter()
{
}

//! Called from setup() function
void UvbMeter::OnSetup()
{
  // Setup M5
  M5.begin();
  M5.IMU.Init();
  M5.Lcd.setRotation(1);

  // Setup ASD1014
  ads.begin(0, 26);
  ads.setGain(2);  // +/-2.048 V
  ads.setMode(1);  // Single-shot mode
  ads.setDataRate(4);  // 1600 SPS

  // If button B is pressed at startup, calibration (zero adjustment) will be performed.
  M5.update();

  if (M5.BtnB.isPressed()) {
    M5.Lcd.drawString("Calibrating...", 5, 5, 2);
    delay(1000);
    UpdateVoltage();

    // Stores the offset (result of zero adjustment) in non-volatile memory.
    preferences.begin("calibration", false);
    preferences.putFloat("offset", GetVoltage());
    preferences.end();    
  }

  // Reads the offset (result of zero adjustment).
  preferences.begin("calibration", true);
  offsetVoltage = preferences.getFloat("offset", 0.0);
  preferences.end();    

  // Start the power off timer
  powerOffTimer.Begin(5 * 60 * 1000, millis());

  // Create a sprite
  sprite.createSprite(M5.Lcd.width(), M5.Lcd.height());
}

//! Called from loop() function
void UvbMeter::OnLoop()
{
  unsigned long t0 = millis();

  if (ads.isConnected()) {
    SetError(false);
    if (!IsHold()) {
      UpdateVoltage();
    }
  }
  else {
    SetError(true);
  }

  // The following process is executed only once every 100 ms.
  while (millis() - t0 < 100) {
  }

  // Update the state of M5
  M5.update();

  // If the power button is pressed for more than one second, turn off the power.
  if (M5.Axp.GetBtnPress() == 1) {
    PowerOff();
  }

  // When the auto power off timer expires, turn off the power.
  powerOffTimer.Update(millis());
  if (powerOffTimer.IsExpired()) {
    PowerOff();
  }

  // If the object is not stationary, reset the auto power off timer.
  float ax, ay, az;

  M5.IMU.getAccelData(&ax, &ay, &az);
  motionSensor.Update(ax, ay, az);

  if (!motionSensor.IsStationary()) {
    powerOffTimer.Reset();
  }

  // When button A or B is pressed, the hold state is reversed and the auto power off timer is reset.
  bool btnState = M5.BtnA.isPressed() || M5.BtnB.isPressed();

  if (prevBtnState == false && btnState == true) {
    SetHold(!IsHold());
    powerOffTimer.Reset();
  }
  prevBtnState = btnState;

  // When the power button is pressed briefly (within 1 second), the auto power off timer is reset.
  if (M5.Axp.GetBtnPress() == 2) {
    powerOffTimer.Reset();
  }

  // Draws the screen.
  Draw();
}

//! Draw the screen
void UvbMeter::Draw()
{
  // Preparing
  sprite.fillSprite(BLACK);
  sprite.setTextColor(WHITE, BLACK);

  // Draw the UVB power.
  char buf1[] = "----";
  if (!IsError()) {
    float power = GetUvPower();
    power = constrain(power, 0.0f, 1999.0f);
    int n = round(power);
    sprintf(buf1, "%d", n);
  }
  int x = sprite.width() - sprite.textWidth(buf1, 7) - 10;
  sprite.drawString(buf1, x, 5, 7);

  // Draw the UV index and bar.
  char buf2[] = "UVI=---";
  if (!IsError()) {
    float uvi = GetUvIndex();
    uvi = constrain(uvi, 0.0f, 13.0f);

    int n = round(uvi);
    if (n < 13) {
      sprintf(buf2, "UVI=%d", n);
    }
    else {
      strcpy(buf2, "UVI=13+");
    }

    int dy = round(48.0f / 13.0f * uvi);
    sprite.fillRect(5, 5 + 48 - dy, 10, dy, PURPLE);
  }
  sprite.drawString(buf2, 5, 60, 2);

  // Draw the unit.
  sprite.drawString("uW/cm", 110, 60, 2);
  sprite.drawString("2", 150, 60, 1);

  // Draw the hold satate.
  if (IsHold()) {
    sprite.setTextColor(RED, BLACK);
    sprite.drawString("HOLD", 65, 60, 2);
  }

  // Transfer the sprite to the screen.
  sprite.pushSprite(0, 0);
}

//! Power off the unit
void UvbMeter::PowerOff()
{
  M5.Axp.PowerOff();
  while (true) {
  }
}

//! Retrieve the hold state
/*! 
 *  \retval true Data hold is ON.
 *  \retval false Data hold is OFF.
 */
bool UvbMeter::IsHold() const
{
  return holdState;
}

//! Set the hold state.
/*! 
 *  \param[in] holdState The hold state to set. 
 *  \arg \c true Data hold is ON.
 *  \arg \c false Data hold is OFF.
 */
void UvbMeter::SetHold(bool holdState)
{
  this->holdState = holdState;
}

//! Retrieve the error state
/*!
 *  \retval true An error has occurred.
 *  \retval false No error has occurred.
 */
bool UvbMeter::IsError() const
{
  return errorState;
}

//! Set the error state
/*!
 *  \param[in] errorState The error state to set.
 *  \arg \c true An error has occurred.
 *  \arg \c false No error has occurred.
 */
void UvbMeter::SetError(bool errorState)
{
  this->errorState = errorState;
}

//! Retrieve the output voltage of the UVB sensor measured
float UvbMeter::GetVoltage() const
{
  return voltage - offsetVoltage;
}

//! Retrieve the power of UVB
float UvbMeter::GetUvPower() const
{
  return GetVoltage() / 0.93f * 1000.0f;
}

//! Retrieve the UV index
float UvbMeter::GetUvIndex() const
{
  return GetVoltage() / 0.008f;
}

//! Update the voltage measured
/*!
 *  Perform the AD conversion and calculate the output voltage of the UVB sensor from the result.
 *  The output voltage is calculated from the average of 100 AD conversion results.
 *  Each AD conversion is performed once every 1 ms.
 */
void UvbMeter::UpdateVoltage()
{
  unsigned long t1 = millis();
  const int n = 100;  // Number of AD conversions.
  long sum = 0;  // Sum of AD conversion results.

  for (int i = 0 ; i <= n ; i++) {
    unsigned long t;
    do {
      t = millis();
    } while (t == t1);
    t1 = t;

    if (i > 0) {
      sum += ads.getValue();
    }
    ads.requestADC_Differential_0_1();
  }

  voltage = (float)sum / (float)n * ads.toVoltage(1);
}
