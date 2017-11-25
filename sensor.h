#include "gpioInit.h"
#include "string.h"
class Sensor {
public:
  Sensor(std::string &&analogPinNo, std::string &&digiPinNo);
  ~Sensor();

private:
  gpioInit analogPin;
  gpioInit digiPin;
};
