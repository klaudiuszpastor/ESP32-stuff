#include <stdio.h>
#include <rom/ets_sys.h>
#include "esp_timer.h"
#include "driver/gpio.h"
#include "DHT22.h"


float humidity = 0.;
float temperature = 0.;

float getHumidity() {
	return humidity;
}

float getTemperature() {
	return temperature;
}


int DHT22_SignalLevel(int us, bool state) {
	int utime = 0;
	while (gpio_get_level(DHT22_PORT) == state) {
		if (utime >= us) {
			return -1;
		}
		++utime;
		ets_delay_us(1);
	}

	return utime;
}

int DHT22_ReadData() {
	int utime = 0;
	uint8_t data[5];
	uint8_t byteInx = 0;
	uint8_t bitInx = 7;

	for (int i = 0; i < 5; i++) {
		data[i] = 0;
	}

	// Start signal
	gpio_set_direction(DHT22_PORT, GPIO_MODE_OUTPUT);
	gpio_set_level(DHT22_PORT, 0);
	ets_delay_us(3000); // ensure AM2302 could detect MCU's signal

	gpio_set_level(DHT22_PORT, 1);
	ets_delay_us(25); // Host pulls up 20-40 us for AM2302 response

	gpio_set_direction(DHT22_PORT, GPIO_MODE_INPUT);
	// Sensor pulls low 80us

	utime = DHT22_SignalLevel(85, 0);
	if (utime < 0) {
		return DHT_ERROR_TIMEOUT;
	}

	// Sensor pulls up 80us
	utime = DHT22_SignalLevel(85, 1);
	if (utime < 0) {
		return DHT_ERROR_TIMEOUT;
	}

	// Read data
	for (int i = 0; i < 40; i++) {
		utime = DHT22_SignalLevel(56, 0);
		if (utime < 0) {
			return DHT_ERROR_TIMEOUT;
		}

		utime = DHT22_SignalLevel(75, 1);
		if (utime < 0) {
			return DHT_ERROR_TIMEOUT;
		}

		if (utime > 40) {
			data[byteInx] |= (1 << bitInx);
		}

		if (bitInx == 0) {
			bitInx = 7;
			++byteInx;
		} else {
			--bitInx;
		}
	}

	humidity = (float) ((data[0] << 8) + data[1]) / 10; 
	temperature = (float) (((data[2] & 0x7F) << 8) + data[3]) / 10;

	if (data[2] & 0x80) {
		temperature *= -1;
	}

	// Check checksum

	if(data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
		return DHT_OK;
	} else {
		return DHT_ERROR_CRC;
	}

	return DHT_OK;
}

