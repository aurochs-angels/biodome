/*based on tutorial by derek molley*/
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
static int ledPin = 75; // numbers from M-series docs.
static bool ledState = 0;
static unsigned int presses = 0;
static unsigned int irqNumber;

static irq_handler_t blinky_handler(unsigned int irq, void dev_id *,
                                    struct pt_regs *regs);
static int __init blinky_init(void) {
  int result = 0;
  printk(KERN_INFO "GPIO module loaded.\n");
  if(!(gpio_is_valid(userButton) && gpio_is_valid(ledPin)){
    printk(KERN_INFO "GPIO numbers invalid!");
    return -1;
    }
    ledState = 1;
    gpio_request(ledPin, "sysfs"); // ledPin 75 requested
    gpio_direction_output(ledPin, ledState); //GPIO output and on.
    gpio_export(ledPin, false); // Export to /sys/class/gpio
    gpio_request(userButton, "sysfs");  // Add board userButton
    gpio_direction_input(userButton); // Set the button GPIO to be an input
    gpio_set_debounce(userButton, 200); // Check button press after 200ms
    gpio_export(userButton, false);  // export tp /sys/class/gpio/67

    printk(KERN_INFO "GPIO_TEST: The button state is currently: %d\n", gpio_get_value(userButton));

// Map button to an irq number.
     irqNumber = gpio_to_irq(userButton);
     printk(KERN_INFO "GPIO_TEST: The button is mapped to IRQ: %d\n", irqNumber);

     // use number to set up irq handling.
     result = request_irq(irqNumber,             // The interrupt number requested
                          (irq_handler_t) blinky_handler, // The pointer to the handler function below
                          IRQF_TRIGGER_RISING,   // Interrupt on rising edge (button press, not release)
                          "blinky_handler",    // Used in /proc/interrupts to identify the owner
                          NULL);    // The *dev_id for shared interrupt lines, NULL is okay

     printk(KERN_INFO "GPIO_TEST: The interrupt request result is: %d\n", result);
  return result; // Non-zero return means that the module couldn't be loaded.
}

static void __exit blinky_cleanup(void) {
  printk(KERN_INFO "GPIO_TEST: The button state is currently: %d\n",
         gpio_get_value(userButton));
  printk(KERN_INFO "GPIO_TEST: The button was pressed %d times\n", presses);
  gpio_set_value(ledPin,
                 0); // Turn the LED off, makes it clear the device was unloaded
  gpio_unexport(ledPin); // Unexport the LED GPIO
  free_irq(irqNumber,
           NULL); // Free the IRQ number, no *dev_id required in this case
  gpio_unexport(userButton); // Unexport the Button GPIO
  gpio_free(ledPin);         // Free the LED GPIO
  gpio_free(userButton);
  printk(KERN_INFO "Cleaning up GPIO module.\n");
}
static irq_handler_t blinky_handler(unsigned int irq, void *dev_id,
                                    struct pt_regs *regs) {
  ledState = !ledState;             // Invert the LED state on each button press
  gpio_set_value(ledPin, ledState); // Set the physical LED accordingly
  printk(KERN_INFO "GPIO_TEST: Interrupt! (button state is %d)\n",
         gpio_get_value(userButton));
  presses++; // Global counter, will be outputted when the module is
             // unloaded
  return (irq_handler_t)
      IRQ_HANDLED; // Announce that the IRQ has been handled correctly
}
module_init(blinky_init);
module_exit(blinky_cleanup);
