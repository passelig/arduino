/*working variables*/
unsigned long lastTime, now;
double Input, Output, Setpoint, error;
double integralSum, lastErr, derivativeErr, timePeriod;
double kp, ki, kd;

void Compute()
{
   /*How long since we last calculated*/
   now = millis();
   timePeriod = (double)(now - lastTime);
  
   /*Compute all the working error variables*/
   // Proportional
   error = Setpoint - Input;
   // Integral
   integralSum += (error * timePeriod);
   // Derivative
   derivativeErr = (error - lastErr) / timePeriod;
  
   /*Compute PID Output*/
   Output = kp * error + 
            ki * integralSum + 
            kd * derivativeErr ;
  
   /*Remember some variables for next time*/
   lastErr = error;
   lastTime = now;
}
void setup(){}
void loop(){}