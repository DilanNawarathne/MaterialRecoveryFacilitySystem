

#include "HX711.h"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define  trig  D2
#define  echo  D1

long duration;
int distance;
int distance2;

HX711 scale(D5, D6);
 

float weight;
float calibration_factor = -107325; 

char auth[] = "EYW1fEUIVyvm7r22NlXoL-pzOlpgd4Gm";
char ssid[] = " "; // Router name
char pass[] = " "; // Router password

BlynkTimer timer;
WidgetLCD lcd(V7);

BLYNK_WRITE(V3) {
  digitalWrite(D0, param.asInt());
}

void setup()
{

   pinMode(D0, OUTPUT);
  // Debug console
  pinMode(trig, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echo, INPUT);   // Sets the echoPin as an Inpu
  Serial.begin(115200);


  scale.set_scale();
  scale.tare(); //Reset the scale to 0
  long zero_factor = scale.read_average(); //Get a baseline reading
   Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
scale.set_scale(calibration_factor); //Adjust to this calibration factor
 
  weight = scale.get_units(5);
   Blynk.virtualWrite(V5, weight); 

  timer.run();

  // Serial.print("Weight: ");
  // Serial.print(weight);
  // Serial.println(" KG");
  
}
void sendSensor()
{
  digitalWrite(trig, LOW);   // Makes trigPin low
  delayMicroseconds(2);       // 2 micro second delay

  digitalWrite(trig, HIGH);  // tigPin high
  delayMicroseconds(10);      // trigPin high for 10 micro seconds
  digitalWrite(trig, LOW);   // trigPin low

  duration = pulseIn(echo, HIGH);   //Read echo pin, time in microseconds
  distance = duration * 0.034 / 2;   //Calculating actual/real distance
  distance2 = 100 - 100*duration * 0.034 / 600; //300cm high

  Serial.print("Distance = ");        //Output distance on arduino serial monitor
  Serial.println(distance);
  

  Blynk.virtualWrite(V0, distance2);
   Blynk.virtualWrite(V1, distance);
  
  lcd.print(0, 0, "voidloopRobotech"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
  lcd.print(0, 1, "Distance: " + String(distance) + "cm  ");
  delay(1000); 

}
