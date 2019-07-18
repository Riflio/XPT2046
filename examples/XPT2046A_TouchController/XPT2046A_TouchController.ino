#include <XPT2046A.h>
#include <XPT2046A_calib.h>


XPT2046A touch;  //-- SPI on Arduino UNO: MOSI=11, MISO=12, SCK=13

void setup() {
  Serial.begin(38400);
  while (!Serial && (millis() <= 1000));

  //-- Set params
  touch.setParams(1024, 768, XPT2046A::Rotations::R0, 8);
  
  //-- Begin
  touch.begin();
  
  //-- Make calibration
  XPT2046A::Point calDisplayPoints[3] = {XPT2046A::Point(5,8), XPT2046A::Point(950, 20), XPT2046A::Point(512, 700)};
  XPT2046A::Point calTouchPoints[3] = {XPT2046A::Point(280, 370), XPT2046A::Point(3750, 570), XPT2046A::Point(2121, 3791)};  
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

    XPT2046A::TouchInfo info = touch.update();
    
    if ( info.isTouching ) {
      Serial.print("Pressure: ");
      Serial.print(info.point.z);
      Serial.print(", x: ");
      Serial.print(info.point.x);
      Serial.print(", y: ");
      Serial.print(info.point.y);
      Serial.println();
    }

    delay(30);
}
