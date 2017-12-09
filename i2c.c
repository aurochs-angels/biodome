#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
double buffer_to_double(unsigned int buff[14]);
int main() {
  int file;
  int adapter = 1;
  char filename[20];
  char buffer[10];
  double temperature;
  double humidity;
  double celsius;
  double percents;
#define TEMP_MEASURE_HOLD 0xE3
#define HUM_MEASURE_HOLD 0xE5
#define TEMP_MEASURE_NOHOLD 0xF3
#define HUM_MEASURE_NOHOLD 0xF5
#define WRITE_USER_REG 0xE6
#define READ_USER_REG 0xE7
#define SOFT_RESET 0xFE

  snprintf(filename, 19, "/dev/i2c-%d", adapter);
  file = open(filename, O_RDWR);
  if (file < 0) {
    printf("Failed to open i2c file.\n");
    return -1;
  } else {
    int address = 0x27;
    if (ioctl(file, I2C_SLAVE, address) < 0) {
      printf("Ioctl failed\n");
      return -2;
    } else {

      if (read(file, buffer, 4) != 4) {
        printf("Four bytes not read.\n");
        return -3;
      }
      return 1;
    }
  }
}
