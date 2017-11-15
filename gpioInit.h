#include <fstream>
#include <iostream>
#include <string>

class gpioInit {
public:
  gpioInit(std::string test = "4");

  gpioInit(std::string &&pin);
  ~gpioInit();

private:
  std::string gpioID;
  static const std::string export_directory;
  static const std::string unexport_directory;
};
