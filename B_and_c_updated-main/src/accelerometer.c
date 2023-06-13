#include "SPI.h"
#include "SysTimer.h"
#include "accelerometer.h"

void accWrite(uint8_t addr, uint8_t val){
	// TODO access SPI_Transfer_Data
	uint8_t first = ~(3UL << 6) & addr;
	uint8_t second = val;
	uint16_t final = (first << 8) | second;
	SPI_Transfer_Data(final);
}

uint8_t accRead(uint8_t addr){
	// access SPI_Transfer_Data
	uint8_t first = (1UL << 7) | (addr & ~(1UL << 6));
	uint8_t second = 0;
	uint16_t final = (first << 8) | second;
	
	uint16_t output = SPI_Transfer_Data(final);
	uint8_t output2 = output;
	
	return output2; // TODO
}

void initAcc(void){
	// set full range mode
	// enable measurement
	uint8_t rangeaddr = 49;
	uint8_t measureaddr = 45;
	accWrite(rangeaddr, 8);
	accWrite(measureaddr, 8);
}

void readValues(double* x, double* y, double* z){
	// TODO
	// find scaler from data sheet
	// read values into x,y,z using accRead
	double scale = 3.9e-3;
	uint16_t lowx = accRead(50);
	uint16_t highx = accRead(51);
	uint16_t lowy = accRead(52);
	uint16_t highy = accRead(53);
	uint16_t lowz = accRead(54);
	uint16_t highz = accRead(55);
	
	
	

	
	*x = scale*(short)((highx << 8) | lowx);
	*y = scale*(short)((highy << 8) | lowy);
	*z = scale*(short)((highz << 8) | lowz);
	
	
}
