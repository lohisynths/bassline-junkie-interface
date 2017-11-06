#include "mbed.h"
#include "USBSerial.h"

// Virtual USB Serial port
USBSerial usb_serial;

DigitalOut led(LED1);

int main(void)
{
    int i = 0;

    // Print on STDIO
    printf("USBDevice Serial started\r\n");

    while(1) {

        // Print on Virtual USB Serial port
        usb_serial.printf("I am a virtual serial port: %d\r\n", i++);

        led = !led;
        wait(0.1);
    }
}
