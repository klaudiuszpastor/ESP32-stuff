#ifndef DHT22_H_
#define DHT22_H_

// Includes

// Defines
#include <stdbool.h>
#define DHT22_PORT GPIO_NUM_4
#define DHT_OK 0
#define DHT_ERROR_TIMEOUT -1
#define DHT_ERROR_CRC -2

// Functions prototypes
int DHT22_SignalLevel(int us, bool state);
int DHT22_ReadData(void); 
float getHumidity();
float getTemperature();

#endif /* DHT22_H_ */
