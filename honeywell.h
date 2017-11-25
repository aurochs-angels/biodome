#include <fstream>
#include <iostream>
#include <string>
class honeywell {
public:
  honeywell(int adp, int addr);
  ~honeywell();

private:
  int address;
  int adapter;
  std::string filename = "/dev/i2c-";
};
