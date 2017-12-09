#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
  int file;
  int adapter = 1;
  char filename[20];
  char data[2];
  double celsius;
  double percents;
#define TEMP_MEASURE_HOLD 0xE3
#define HUM_MEASURE_HOLD 0xE5
#define TEMP_MEASURE_NOHOLD 0xF3
#define HUM_MEASURE_NOHOLD 0xF5
#define WRITE_USER_REG 0xE6
#define READ_USER_REG 0xE7
#define SOFT_RESET 0xFE
#define ADDRESS 0x40

  snprintf(filename, 19, "/dev/i2c-%d", adapter);
  file = open(filename, O_RDWR);
  if (file < 0) {
    printf("Failed to open i2c file.\n");
    return -1;
  }

  if (ioctl(file, I2C_SLAVE, ADDRESS) < 0) {
    printf("Ioctl failed\n");
    return -2;
  }
  write(file, TEMP_MEASURE_NOHOLD, 1);
  sleep(1);
  if (read(file, data, 2) != 2) {
    printf("Temp data fetch failed. Exiting.");
    return -3;
  } else {
    celsius = (((data[0] * 256 + data[1]) * 175.72) / 65536.0) - 46.85;
    printf("Temperature in Celsius: %.3f", celsius);
  }
  write(file, HUM_MEASURE_NOHOLD, 1);
  sleep(1);
  if (read(file, data, 2) != 2) {
    printf("Humidity data fetch failed. Exiting.");
    return -4;
  } else {
    percents = (((data[0] * 256 + data[1]) * 125.0) / 65536.0) - 6;
    printf("Humidity in percent: %.2f", percents);
  }

  return 1;
}
