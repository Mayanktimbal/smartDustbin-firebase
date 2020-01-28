#include <Servo.h>
#include <ESP8266WiFi.h>

#include <FirebaseArduino.h>



#define WIFI_SSID "Redmi"     //wifi ssid
#define WIFI_PASSWORD "12345678" // wifi password
#define FIREBASE_HOST "######.firebaseio.com"   //you would get it from firebase
#define FIREBASE_AUTH "NoDmCMsaRHmAv2O7liI5cucv7khkKnjbjbczjPs7"    // you would find it in firebase secret key



int trigpin=4;  // d2   height detect ultrasonic triggerpin
int echopin =12; //D6   height detect ultrasonic echopin
int agaltrig= 5 ;//D1  front tringger pin
int agalecho=16;// D0  front echo pin


long duration;
int distance; // will store height of garbage in dustbin
long durationagal; 
int distanceagal; // will store front-distance
String distancefire;
Servo servo1;
Servo servo2;

void setup() {
 pinMode(trigpin,OUTPUT);
 pinMode(echopin,INPUT);
  pinMode(agaltrig,OUTPUT);
 pinMode(agalecho,INPUT);

 servo1.attach(15);
servo2.attach(13);
servo1.write(0);
servo2.write(0);
 Serial.begin(9600);



 
wifiConnect();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  delay(10);
  

  delay(1000);
 

}

void loop() {



  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  
digitalWrite(trigpin,LOW);
delayMicroseconds(2);
digitalWrite(trigpin,HIGH);
delayMicroseconds(10);
digitalWrite(trigpin,LOW);
duration=pulseIn(echopin,HIGH);
distance=duration*0.034/2;
Serial.print("height Distance: ");
Serial.println(distance);


digitalWrite(agaltrig,LOW);
delayMicroseconds(2);
digitalWrite(agaltrig,HIGH);
delayMicroseconds(10);
digitalWrite(agaltrig,LOW);
durationagal=pulseIn(agalecho,HIGH);
distanceagal=durationagal*0.034/2;
Serial.print("agal Distance: ");
Serial.println(distanceagal);
 distancefire =String(distance);

if(distanceagal <=12)
{
    servo1.write(180);
    servo2.write(180);
    delay(5000);
    
digitalWrite(trigpin,LOW);
delayMicroseconds(2);
digitalWrite(trigpin,HIGH);
delayMicroseconds(10);
digitalWrite(trigpin,LOW);
duration=pulseIn(echopin,HIGH);
distance=duration*0.034/2;
Serial.print("height Distance: ");
Serial.println(distance);

 distancefire =String(distance);
    servo1.write(0);
    servo2.write(0);

  delay(1000);
  
   Firebase.setString("Database/height",distancefire);     // set garbage level in firebase database
    delay(1000);
    
  }




 

delay(10);

if(WiFi.status() != WL_CONNECTED)
{
  wifiConnect();
}
delay(10);



}









void wifiConnect()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID); Serial.println(" ...");

  int teller = 0;
  while (WiFi.status() != WL_CONNECTED)
  {                                       // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++teller); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}
