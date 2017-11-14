#include "gpioInit.h"
gpioInit::gpioInit(std::string test) : gpioID(test) {
  // Export pin
}
gpioInit::gpioInit(std::string &&pin) : gpioID(std::move(pin)) { // export pin
  std::ofstream gpio_add(export_directory, std::ofstream::app);
  if (gpio_add.is_open()) {
    // write the gpioPin number to the export file.
    gpio_add << gpioID;
    gpio_add.close();
  } else {
    // file opening failed
    std::cout << "ERROR! Export file operation failed!" << std::endl;
  }
}
gpioInit::~gpioInit() {
  // unexport pin
  std::ofstream gpio_remove(unexport_directory, std::ofstream::app);
  if (gpio_remove.is_open()) {
    gpio_remove << gpioID;
    gpio_remove.close();
  } else {
    // unexport file opening failed
    std::cout << "ERROR! Destructor failed." << std::endl;
  }
}
const std::string gpioInit::export_directory = "/sys/class/gpio/export";
const std::string gpioInit::unexport_directory = "/sys/class/gpio/unexport";
