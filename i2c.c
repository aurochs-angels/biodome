#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
int main() {
  int file;
  int adapter = 0;
  char filename[20];
  char buffer[10];

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
        printf("Bytes returned: %02x\n, %02x\n, %02x\n, %02x\n", buffer[0],
               buffer[1], buffer[2], buffer[3]);
        return buffer[0];
      }
    }
  }
}
