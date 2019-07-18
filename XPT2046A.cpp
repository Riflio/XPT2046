#include "XPT2046A.h"

XPT2046A::XPT2046A()
{
	
}

XPT2046A::XPT2046A(uint16_t tftWidth, uint16_t tftHeight, XPT2046A::Rotations rotation, byte pinCS, XPT2046A::CalibrationParams calibrationParams, XPT2046A_SPI * spi)
{
	setParams(tftWidth, tftHeight, rotation, pinCS, calibrationParams, spi);
}

XPT2046A::~XPT2046A()
{
	
}

void XPT2046A::setParams(uint16_t tftWidth, uint16_t tftHeight, XPT2046A::Rotations rotation, byte pinCS, XPT2046A::CalibrationParams calibrationParams, XPT2046A_SPI * spi)
{
	_tftWidth = tftWidth;
	_tftHeight = tftHeight;
	_rotation = rotation;
	_pinCS = pinCS;
	calParams = calibrationParams;
	_spi = spi;
}

void XPT2046A::begin()
{
	if ( _spi==nullptr ) _spi = new XPT2046A_SPI();
	_spi->begin();
	beginTransaction();
	pinMode(_pinCS, OUTPUT);
	
    delay(10);
    digitalWrite(_pinCS, LOW);
    delay(3);
    _spi->transfer(0xD1);
    delay(3);
    _spi->transfer16(0);
    delay(3);
    digitalWrite(_pinCS, HIGH);
}

void XPT2046A::beginTransaction()
{
	_spi->setDataMode(SPI_MODE0);
	_spi->setBitOrder(MSBFIRST);
	_spi->setClockDivider(SPI_CLOCK_DIV8);
}

uint16_t XPT2046A::read(byte ctrl)
{
	uint16_t cur = 0xffff, prev = 0xffff;
	byte i = 0;
	
    do {
		prev = cur;
		cur = _spi->transfer16(ctrl) >> 3;
    } while ((prev != cur) && (++i<0xFF));
	return cur;
}

XPT2046A::TouchInfo XPT2046A::update()
{
	TouchInfo touchInfo;
	if ( _spi==nullptr ) { return touchInfo; }
		
	int16_t x, y, z;
    
	beginTransaction();	
	digitalWrite(_pinCS, LOW);
	
	_spi->transfer(0xB1);
	z = read(0xB1);
    _spi->transfer(0x91);
    x = read(0x91);
    _spi->transfer16(0xD1);
    y = read(0xD1);
    _spi->transfer16(0xD0);
    _spi->transfer16(0);
	
    digitalWrite(_pinCS, HIGH);
	
	if ( z<70 ) { return touchInfo; }
	
	touchInfo.isTouching = true;	
    touchInfo.point.y = 4096 - y;
    touchInfo.point.x = x;
	touchInfo.point.z = z;

    touchInfo.point.x = ((calParams.An * static_cast<long long>(touchInfo.point.x)) + (calParams.Bn * static_cast<long long>(touchInfo.point.y)) + calParams.Cn) / calParams.V;
    touchInfo.point.y = ((calParams.Dn * static_cast<long long>(touchInfo.point.x)) + (calParams.En * static_cast<long long>(touchInfo.point.y)) + calParams.Fn) / calParams.V;
     
	long tmp = 0;
    switch (_rotation) {
		case Rotations::R90:
			touchInfo.point.x = _tftWidth - touchInfo.point.x;
			tmp = touchInfo.point.x;
			touchInfo.point.x = touchInfo.point.y;
			touchInfo.point.y = tmp;
			break;
		case Rotations::R180:
			touchInfo.point.x = _tftWidth - touchInfo.point.x;
			touchInfo.point.y = _tftHeight - touchInfo.point.y;
			break;
		case Rotations::R270:
			touchInfo.point.y = _tftHeight - touchInfo.point.y;
			tmp = touchInfo.point.x;
			touchInfo.point.x = touchInfo.point.y;
			touchInfo.point.y = tmp;
			break;
		case Rotations::R0:
		default:
			break;
	}
	
	return touchInfo;
}