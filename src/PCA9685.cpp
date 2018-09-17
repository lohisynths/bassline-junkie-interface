#include "PCA9685.h"
#include "mbed.h"
#include "utils.h"

static I2C i2c(I2C_SDA, I2C_SCL); // sda, scl
static int addr_found[128]={};

PCA9685::PCA9685() : _i2caddr(0) {}

int *PCA9685::i2c_probe()
{
	DEBUG_PCA9685_LOG("Searching for I2C devices...\n");

    int *tab = addr_found;
    int count = 0;
    for (int address=4; address<256; address+=2) {
        if (!i2c.write(address, NULL, 0)) { // 0 returned is ok
        	DEBUG_PCA9685_LOG(" - I2C device found at address 0x%02X\r\n", address);
            *tab = address;
            tab++;
            count++;
        }
    }

    DEBUG_PCA9685_LOG("%d devices found\r\n", count);
    return addr_found;
}


void PCA9685::begin(void)
{
    reset();

    setPrescale(64);
    frequencyI2C(800000);

}

void PCA9685::frequencyI2C(int freq)
{
    i2c.frequency(freq);
}
void PCA9685::write8(uint8_t address, uint8_t data)
{
    char cmd[2];
    cmd[0] = address;
    cmd[1] = data;
    i2c.write(_i2caddr, cmd, 2);
}

char PCA9685::read8(char address)
{
    i2c.write(_i2caddr, &address, 1);
    char rtn;
    i2c.read(_i2caddr, &rtn, 1);
    return rtn;
}
int count = 0;
int adr[10] = {};

void PCA9685::reset(void)
{
    write8(PCA9685_MODE1, 0x0);
}

void PCA9685::setOpenDrain(bool enable) {
    	uint8_t oldmode = read8(PCA9685_MODE2);
    	if(!enable) {
        	//uint8_t newmode = CLEAR_BIT(oldmode, 4); // sleep
        	uint8_t newmode = CLEAR_BIT(oldmode, 1);
        	uint8_t tmp = newmode;
        	newmode = CLEAR_BIT(tmp, 2);
        	tmp = newmode;
        	newmode = SET_BIT(tmp, 4);
        	write8(PCA9685_MODE2, 0x04);
    	} else {
        	uint8_t newmode = SET_BIT(oldmode, 1);
        	uint8_t tmp = newmode;
        	newmode = SET_BIT(tmp, 2);
        	tmp = newmode;
        	newmode = SET_BIT(tmp, 4);
        	write8(PCA9685_MODE2, 0x13);
    	}
}

void PCA9685::setPrescale(uint8_t prescale) {
    uint8_t oldmode = read8(PCA9685_MODE1);
    uint8_t newmode = (oldmode&0x7F) | 0x10; // sleep
    write8(PCA9685_MODE1, newmode); // go to sleep
    wait_ms(5);
    write8(PCA9685_PRESCALE, prescale); // set the prescaler
    write8(PCA9685_MODE1, oldmode);
    wait_ms(5);
    write8(PCA9685_MODE1, oldmode | 0xa1);
}
void PCA9685::setPWMFreq(float freq)
{
    float prescaleval = 25000000;
    prescaleval /= 4096;
    prescaleval /= freq;
    uint8_t prescale = floor(prescaleval  + 0.5) - 1;
    setPrescale(prescale);
}

void PCA9685::setPWM(uint8_t num, uint16_t on, uint16_t off)
{
    char cmd[5];
    cmd[0] = LED0_ON_L + 4 * num;
    cmd[1] = on;
    cmd[2] = on >> 8;
    cmd[3] = off;
    cmd[4] = off >> 8;
    int ret = i2c.write(_i2caddr, cmd, 5);
    ret++;

}
