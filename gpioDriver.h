#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/init.h>   // included for __init and __exit macros
#include <linux/kernel.h> // included for KERN_INFO
#include <linux/module.h> // included for all kernel modules
#include <linux/types.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nummikallio");
MODULE_DESCRIPTION("Simple GPIO module");
MODULE_VERSION("0.1");
static int userButton = 67;
static int ledPin = 75;
static bool ledState = 0;
static unsigned int presses = 0;
static unsigned int irqNumber;

static irq_handler_t blinky_handler(unsigned int irq, void dev_id *,
                                    struct pt_regs *regs);
static int __init hello_init(void) {
  printk(KERN_INFO "GPIO module loaded.\n");
  if(!(gpio_is_valid(userButton) && gpio_is_valid(ledPin)){
    printk(KERN_INFO "GPIO numbers invalid!");
    return -1;

  }

  return 0; // Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void) {
  printk(KERN_INFO "Cleaning up GPIO module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
