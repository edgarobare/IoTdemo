const int pwm = 0  ;  //Initializing Pin for pwm 
const int adc = A0 ;  //Initializing Pin for adc


void setup()
{
pinMode(pwm,OUTPUT) ;  // To change LED brightness  
}
void loop()
{
  int sensor_val = analogRead(adc) ; 
  sensor_val = map(sensor_val, 0, 1023, 0, 255) ; 
  
 /*
   -----------map funtion------------
   The above funtion scales the output of adc,which is
   10 bit and gives values btw 0 to 1023, in values btw
   0 to 255 form analogWrite funtion which only recieves 
   values btw this range. 
 */
 
  analogWrite(pwm,sensor_val) ; // setting sensor value as pwm
 
}
