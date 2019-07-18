/*
 @brief XPT2046 Touch controller
 https://github.com/PaulStoffregen/XPT2046_Touchscreen
*/

#ifndef _XPT2046A_h
#define _XPT2046A_h
 
#include <Arduino.h>
#include <SPI.h>
 
class XPT2046A_SPI
{
public:
	XPT2046A_SPI() {}
	virtual ~XPT2046A_SPI() {}
	virtual void begin() { return SPI.begin(); }
	virtual void setBitOrder(uint8_t order) { return SPI.setBitOrder(order); }
	virtual void setDataMode(uint8_t mode) { return SPI.setDataMode(mode); }
	virtual void setClockDivider(uint8_t divider) { return SPI.setClockDivider(divider); }
	virtual uint8_t transfer(uint8_t data) { return SPI.transfer(data); }
	virtual uint16_t transfer16(uint16_t data) { return SPI.transfer16(data); }
};

class XPT2046A
{
public:	
	struct Point
	{
		Point(long pX=0, long pY=0, long pZ=0): x(pX), y(pY), z(pZ) {  }
		long x = 0, y = 0, z = 0;
	};

	struct CalibrationParams {
		long long An, Bn, Cn, Dn, En, Fn, V;
	};

	enum Rotations: byte {
		R0,
		R90,
		R180,
		R270
	};
	
	struct TouchInfo {
		bool isTouching = false;
		Point point;
	};
	
	CalibrationParams calParams;
	
	XPT2046A();
	XPT2046A(uint16_t tftWidth, uint16_t tftHeight, Rotations rotation = Rotations::R0, byte pinCS = 10, CalibrationParams calibrationParams = {1, 0, 0, 0, 1, 0, 1}, XPT2046A_SPI * spi = nullptr);	
	~XPT2046A();
	
	void setParams(uint16_t tftWidth, uint16_t tftHeight, Rotations rotation = Rotations::R0, byte pinCS = 10, CalibrationParams calibrationParams = {1, 0, 0, 0, 1, 0, 1}, XPT2046A_SPI * spi = nullptr);
	void begin();
	TouchInfo update();
	
private:
	void beginTransaction();
	uint16_t read(byte ctrl);
	
	uint16_t _tftWidth;
	uint16_t _tftHeight;
	Rotations _rotation;
	byte _pinCS;	
	XPT2046A_SPI * _spi;
	
};


#endif
