#include "sensor.h"
Sensor::Sensor(std::string &&analogPinNo, std::string &&digiPinNo)
    : analogPin(gpioInit(analogPinNo)), digiPin(gpioInit(digiPinNo)) {}
