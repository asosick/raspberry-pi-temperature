#ifndef TEMP_H
#define TEMP_H

const char* GPIOMEN_PATH = "/dev/gpiomem";
const char* TEMPATURE_PATH = "/sys/class/thermal/thermal_zone0/temp";
const unsigned int OUTPUT_MASK = 0x1;
const unsigned int INPUT_MASK = 0x0;
const unsigned int FAN_GPIO_PIN = 14;

unsigned int* getGPIOAddresses();
void changeGPIOFunction(volatile unsigned int* gpio, unsigned int function, unsigned int pin);
long getTemp();

#endif