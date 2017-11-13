#include <fstream>
#include <iostream>
#include <string>

class gpioInit {
public:
  gpioInit();
  gpioInit(std::string &&pin);
  ~gpioInit();

private:
  std::string gpioID;
  static const std::string export_directory;
  static const std::string unexport_directory;
};
