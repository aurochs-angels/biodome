#include "honeywell.h"
honeywell::honeywell(int adp, int addr) : address(addr), adapter(adp) {
  std::ofstream i2c_dev((filename += adapter),
                        std::ofstream::in | std::ofstream::out);
  if (i2c_dev.is_open()) {
    if (ioctl(i2c_dev, I2C_MODE, address)) {

    } else {
      std::cout << "Fatal error, ioctl failed!" << std::endl;
      exit(-1);
    }
  } else {
    std::cout << "Fatal error! i2c directory closed." << std::endl;
    exit(-1);
  }
}
