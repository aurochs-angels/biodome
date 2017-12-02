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
      } else {
        unsigned int bits_0[8];
        unsigned int bits_1[8];
        unsigned int bits_2[8];
        unsigned int bits_3[8];
        unsigned int temp_bits[14];
        unsigned int humidity_bits[14];

        printf("Bytes returned: %03x\n, %03x\n, %03x\n, %03x\n", buffer[0],
               buffer[1], buffer[2], buffer[3]);
        for (int i = 0; i < 8; i++) {
          bits_0[i] = (buffer[0] & (1 << i)) != 0;
          bits_1[i] = (buffer[1] & (1 << i)) != 0;
          bits_2[i] = (buffer[2] & (1 << i)) != 0;
          bits_3[i] = (buffer[3] & (1 << i)) != 0;
        }
        for (int n = 0; n < 8; n++) {
          humidity_bits[n] = bits_0[n + 2];
        }
        for (int m = 0; m < 8; m++) {
          humidity_bits[6 + m] = bits_1[m];
        }
        for (int j = 0; j < 8; j++) {
          temp_bits[j] = bits_2[j];
        }
        for (int k = 0; k < 7; k++) {
          temp_bits[8 + k] = bits_3[k];
        }
        temperature = buffer_to_double(temp_bits);
        humidity = buffer_to_double(humidity_bits);
        celsius = 125 * (temperature / 16382);
        percents = 100 * (humidity / 16382);

        printf("Temperature %f\n Humidity %f percent.", celsius, percents);

        return 1;
      }
    }
  }
}
double buffer_to_int(unsigned int buff[14]) {
  double result = 0;
  for (int i = 0; i < 14; i++) {
    if (buff[i] == 1) {
      result += pow(2, i);
    }
  }

  return result;
}
