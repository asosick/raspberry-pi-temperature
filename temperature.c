#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "temperature.h"

int main(void){
    volatile unsigned int* gpio = getGPIOAddresses();
    long temp = getTemp();
    changeGPIOFunction(gpio, OUTPUT_MASK, FAN_GPIO_PIN);
    exit(EXIT_SUCCESS);
}

long getTemp(){
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(TEMPATURE_PATH, "r");
    if (fp == NULL){
        printf("Error reading temperature file");
        exit(EXIT_FAILURE);
    }

    read = getline(&line, &len, fp);
    fclose(fp);

    return strtol(line, NULL, 10);
}

unsigned int* getGPIOAddresses(){
    int fdgpio=open(GPIOMEN_PATH, O_RDWR);
    if (fdgpio < 0){
        printf("Error reading pins from %s\n", GPIOMEN_PATH);
        exit(EXIT_FAILURE);
    }

    unsigned int *gpio=(unsigned int*)mmap(0, 4096, PROT_READ+PROT_WRITE, MAP_SHARED, fdgpio, 0);
    return gpio;
}

//see https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf 
void changeGPIOFunction(volatile unsigned int* gpio, unsigned int function, unsigned int pin){
    int index;
    if(pin < 10)
        index = 0;
    else
        index = 1;

    unsigned int shift = ((pin-(index*10))*3);
    unsigned int bit_mask = (7<<shift);
    unsigned int updated_gpios = gpio[index];
    updated_gpios = (updated_gpios & (~bit_mask)) | (function<< shift);
    gpio[index] = updated_gpios;
}