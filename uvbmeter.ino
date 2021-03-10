/*! \file
 *  \brief Sketch main
 *  \author lawn-tech
 *  \date March 2021
 */

#include "UvbMeter.h"

UvbMeter uvbMeter; //!< The object of UvbMeter class.

void setup()
{
  uvbMeter.OnSetup();
}

void loop()
{
  uvbMeter.OnLoop();
}
