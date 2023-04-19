 #include <Servo.h>
 int trigPin = 6;
 int echoPin = 7;
 int pingTravelTime;
 float distance;
 bool barrierIsOpen = false;

 int positionOpen = 90;                                    // Posisjon i grader på bommen når den er open
 int positionClosed = 0;                                   // Posisjon i grader på bommen når den er lukket

 Servo barrier;

 void setup()
 {
   pinMode(trigPin,OUTPUT);                                // Setter opp kommunikasjon med ultralydssensoren
   pinMode(echoPin,INPUT);                                 // Setter opp kommunikasjon med ultralydssensoren
   Serial.begin(9600);                                     // Setter opp seriell kommunikasjon for feilsøking                     
  
   barrier.attach(9, 500, 2500);                           // Tilkobler servomotoren til de angitte pinnene
   barrier.write(positionOpen);                            
   delay(500);
 }

 void loop()
 {
  float distanceFromSensor = readDistanceFromSensor ();
  if (barrierIsOpen && distanceFromSensor > 10) {          // Dersom distansen fra sensor er større enn 10, porten skal vere lukket
    barrier.write(positionClosed);                         // Skriv til servoen som styrer bommen
    Serial.println("Closing barrier");                     // Skriv ut når til serial monitor, når me lukkar porten 
    barrierIsOpen = false;  
 }  
  if (!barrierIsOpen && distanceFromSensor < 10) {          // Dersom distansen fra sensor er mindre enn 10, porten skal opnes
    barrier.write(positionOpen);                           // Skriv til servoen som styrer bommen
    Serial.println("Opening barrier");                     // Skriv ut når til serial monitor, når me opnar porten      
    barrierIsOpen = true; 
 }
  delay (1000);                                            // Tid til neste måling
 }

float  readDistanceFromSensor (){                        // Leser ut avstanden som ultralydsensoren oppfatter
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pingTravelTime = pulseIn(echoPin,HIGH);
  distance = ((pingTravelTime-200)/60.)+1;
  Serial.println(" Distance = " + String(distance));
  return distance;
 }