/*
 @brief XPT2046 Touch controller
 https://github.com/PaulStoffregen/XPT2046_Touchscreen
*/

#ifndef _XPT2046A_h
#define _XPT2046A_h
 
#include <Arduino.h>
#include <SPI.h>

#define SPI_SETTING SPISettings(2000000, MSBFIRST, SPI_MODE0)

struct TXPT2046A_CalParams {
    long long An, Bn, Cn, Dn, En, Fn, V;
};
 
enum TXPT2046A_Rotations: byte {
	R0,
	R90,
	R180,
	R270
};

struct Point
{
	Point(long pX=0, long pY=0, long pZ=0): x(pX), y(pY), z(pZ) {  }
	long x = 0, y = 0, z = 0;
};

struct TXPT2046A_Touch
{
	bool isTouching = false; //-- Is touching
	Point point; //-- Touch point
	TXPT2046A_Rotations rotation = TXPT2046A_Rotations::R0; //-- Display rotation
	uint16_t tftWidth = 1024, tftHeight = 768; //-- Display width and height
	byte pinCS = 8; //-- Chip select pin for SPI
	TXPT2046A_CalParams calParams = {1, 0, 0, 0, 1, 0, 1}; //-- Calibration parameters. See example.
};

void XPT2046A_Begin(TXPT2046A_Touch & t)
{
	pinMode(t.pinCS, OUTPUT);
    
    SPI.begin();
	SPI.beginTransaction(SPI_SETTING);

    delay(10);
    digitalWrite(t.pinCS, LOW);
    delay(3);
    SPI.transfer(0xD1);
    delay(3);
    SPI.transfer16(0);
    delay(3);
    digitalWrite(t.pinCS, HIGH);
} 

uint16_t XPT2046A_Read(byte ctrl)
{
    uint16_t cur = 0xffff, prev = 0xffff;
    byte i = 0;
	
    do {
		prev = cur;
		cur = SPI.transfer16(ctrl) >> 3;
    } while ((prev != cur) && (++i<0xFF));
	return cur;
}

void XPT2046A_Update(TXPT2046A_Touch & t)
{
	int16_t x, y, z;
    
	SPI.beginTransaction(SPI_SETTING);
	digitalWrite(t.pinCS, LOW);
	
	SPI.transfer(0xB1);
	z = XPT2046A_Read(0xB1);
    SPI.transfer(0x91);
    x = XPT2046A_Read(0x91);
    SPI.transfer16(0xD1);
    y = XPT2046A_Read(0xD1);
    SPI.transfer16(0xD0);
    SPI.transfer16(0);
	
    digitalWrite(t.pinCS, HIGH);
	
	if ( z<70 ) { t.isTouching = false; return; }
	
	t.isTouching = true;	
    t.point.y = 4096 - y;
    t.point.x = x;
	t.point.z = z;

    t.point.x = ((t.calParams.An * static_cast<long long>(t.point.x)) + (t.calParams.Bn * static_cast<long long>(t.point.y)) + t.calParams.Cn) / t.calParams.V;
    t.point.y = ((t.calParams.Dn * static_cast<long long>(t.point.x)) + (t.calParams.En * static_cast<long long>(t.point.y)) + t.calParams.Fn) / t.calParams.V;
     
	long tmp = 0;
    switch (t.rotation) {
		case TXPT2046A_Rotations::R90:
			t.point.x = t.tftWidth - t.point.x;
			tmp = t.point.x;
			t.point.x = t.point.y;
			t.point.y = tmp;
			break;
		case TXPT2046A_Rotations::R180:
			t.point.x = t.tftWidth - t.point.x;
			t.point.y = t.tftHeight - t.point.y;
			break;
		case TXPT2046A_Rotations::R270:
			t.point.y = t.tftHeight - t.point.y;
			tmp = t.point.x;
			t.point.x = t.point.y;
			t.point.y = tmp;
			break;
		case TXPT2046A_Rotations::R0:
		default:
			break;
	}
}

#endif
