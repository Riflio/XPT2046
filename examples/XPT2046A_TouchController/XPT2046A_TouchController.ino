#include <XPT2046A.h>
#include <XPT2046A_calib.h>


TXPT2046A_Touch touch;  // MOSI=11, MISO=12, SCK=13


void setup() {
  Serial.begin(38400);
  while (!Serial && (millis() <= 1000));

  //-- Set params
  touch.rotation = TXPT2046A_Rotations::R0;
  touch.tftWidth = 1024;
  touch.tftHeight = 768;
  touch.pinCS = 8;

  //-- Begin
  XPT2046A_Begin(touch);
  
  //-- Make calibration
  Point calDisplayPoints[3] = {Point(5,8), Point(950, 20), Point(512, 700)};
  Point calTouchPoints[3] = {Point(280, 370), Point(3750, 570), Point(2121, 3791)};  
  XPT2046A_MakeCalibrationParams(calDisplayPoints, calTouchPoints, touch.calParams);

  Serial.println("Cal params: ");
  Serial.print((long)touch.calParams.An); Serial.print(", ");
  Serial.print((long)touch.calParams.Bn); Serial.print(", ");
  Serial.print((long)touch.calParams.Cn); Serial.print(", ");
  Serial.print((long)touch.calParams.Dn); Serial.print(", ");
  Serial.print((long)touch.calParams.En); Serial.print(", ");
  Serial.print((long)touch.calParams.Fn); Serial.print(", ");
  Serial.print((long)touch.calParams.V);
  Serial.println();

}

void loop() {

    XPT2046A_Update(touch);
    
    if ( touch.isTouching ) {
      Serial.print("Pressure: ");
      Serial.print(touch.point.z);
      Serial.print(", x: ");
      Serial.print(touch.point.x);
      Serial.print(", y: ");
      Serial.print(touch.point.y);
      Serial.println();
    }

    delay(30);
}
