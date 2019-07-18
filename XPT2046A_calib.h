/*  
@brief Three point calibration
http://www.ti.com/lit/an/slyt277/slyt277.pdf  "Calibration in touch-screen systems" TI Analog Applications Journal 3Q 2007
http://www.embedded.com/design/system-integration/4023968/How-To-Calibrate-Touch-Screens is copyright (c) 2001 Carlos E. Vidales. Modified January 2016.
*/
 
#ifndef _XPT2046A_CALIB_h
#define _XPT2046A_CALIB_h
 
#include "XPT2046A.h"
#include "arduino.h"
 
/**
* @brief
* @param dp - Display points
* @param tp - Touch points (raw)
* @param cal - Result calibration
*/
bool XPT2046A_MakeCalibrationParams(XPT2046A::Point * dp, XPT2046A::Point * tp, XPT2046A::CalibrationParams & cal)
{
	cal.V = 	((tp[0].x - tp[2].x) * (tp[1].y - tp[2].y)) - 
				((tp[1].x - tp[2].x) * (tp[0].y - tp[2].y));
			
    if (cal.V == 0) return false;
	
    cal.An = 	((dp[0].x - dp[2].x) * (tp[1].y - tp[2].y)) - 
				((dp[1].x - dp[2].x) * (tp[0].y - tp[2].y));
				
    cal.Bn = 	((tp[0].x - tp[2].x) * (dp[1].x - dp[2].x)) - 
				((dp[0].x - dp[2].x) * (tp[1].x - tp[2].x));
				
	cal.Cn =	(tp[2].x * dp[1].x - tp[1].x * dp[2].x) * tp[0].y + 
				(tp[0].x * dp[2].x - tp[2].x * dp[0].x) * tp[1].y + 
				(tp[1].x * dp[0].x - tp[0].x * dp[1].x) * tp[2].y;
				
	cal.Dn =	((dp[0].y - dp[2].y) * (tp[1].y - tp[2].y)) -
				((dp[1].y - dp[2].y) * (tp[0].y - tp[2].y));
				
    cal.En = 	((tp[0].x - tp[2].x) * (dp[1].y - dp[2].y)) -
				((dp[0].y - dp[2].y) * (tp[1].x - tp[2].x));
	
    cal.Fn = 	(tp[2].x * dp[1].y - tp[1].x * dp[2].y) * tp[0].y +
				(tp[0].x * dp[2].y - tp[2].x * dp[0].y) * tp[1].y +
				(tp[1].x * dp[0].y - tp[0].x * dp[1].y) * tp[2].y;
	
    return true;
}

#endif