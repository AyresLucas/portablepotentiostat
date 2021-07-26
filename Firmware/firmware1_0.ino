//-------- LIBRARIES -----------
#include <WiFi.h>
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3 /ok
#include <Esp32MQTTClient.h> // 
#include <Arduino.h>
#include <Wire.h>


//---------- VARIABLES ----------

#define pinLED1 2  //GP2 ESP-01 // Blue LED used as an indicator
#define ID_MQTT  "potentiostat001"             // Device's ID 
#define TOPIC_SUBSCRIBE "uLABHUB2021"   // Topic. It must be the same as the main software


const int ledPin = 16;  // 16 corresponds to GPIO16

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;


int val = 0;
int channel_1 = 36; //ADC 36 adc_01
int channel_2 = 39; //ADC 36 adc_01
int data_cv = 0;
int n = 0;


float Potstep = 0.0078; // fixed due to the DAC resolution
int vevals[] = {100}; //multiple scan rates values (mV/s)
int const count = 6;
long intervalos[count];



const int numReadings = 30; // number of readings for the filter

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average


char xString[10];
char yString[10];
char globalString[10];
String gString;
String xs;
String ys;



// -- FINAL VARIABLES --
int applied_potentialbits = 0; // amperometry
int applied_timebits = 0;
int applied_timeinterval = 0;

int applied_initialpotentialbits = 0; // cv
int applied_ehighbits = 0;
int applied_elowbits = 0;
int applied_scanrate = 0;
int applied_sweepsegments;


// -- VARIABLES USED FOR CONVERSION --
float potentialfloat = 0;  // amperometry
float timefloat = 0;
float sampleintervalfloat = 0;

float initialpotentialfloat = 0; // cv
float e_highfloat = 0;
float e_lowfloat = 0;
float scanratefloat = 0;
float sweepsegmentsfloat = 0;

//-- INCOME VALUES FROM MAIN APPLICATION --
String potential; // amperometry
String timee;
String sampleinterval;


String initialpotential; // cv
String e_high;
String e_low;
String scanrate;
String sweepsegments;



//-------- WI-FI -----------;
const char* ssid = "Carbon-Net";                // wifi's name
const char* password = "+18643244669";   // wifi's password

WiFiClient wifiClient;
PubSubClient MQTT(wifiClient);


//-------- BROKER MQTT CONNECTION  -----------
const char* BROKER_MQTT = "mqtt.eclipseprojects.io"; // broker's URL
int BROKER_PORT = 1883;                      // broker's port



//-------Functions---------------------------
void mantemConexoes();  //Garante que as conexoes com WiFi e MQTT Broker se mantenham ativas
void conectaWiFi();     //Faz conexão com WiFi
void conectaMQTT();     //Faz conexão com Broker MQTT
void recebePacote(char* topic, byte* payload, unsigned int length);



unsigned long myTime;


void setup()
{
  Serial.begin(9600);
  Wire.begin();
  pinMode(pinLED1, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(channel_1, INPUT);
  pinMode(channel_2, INPUT);

  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPin, ledChannel);
  conectaWiFi();
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(recebePacote);

  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }


}
void loop() {
  mantemConexoes();
  MQTT.loop();


  delay (1000);
}

void mantemConexoes() {
  if (!MQTT.connected()) {
    conectaMQTT();
  }

  conectaWiFi(); //se não há conexão com o WiFI, a conexão é refeita

}

void conectaWiFi() {

  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  Serial.print("Connecting to: ");
  Serial.print(ssid);
  Serial.println("Wait");

  WiFi.begin(ssid, password); // Conecta na rede WI-FI
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso, na rede: ");
  Serial.print(ssid);
  Serial.print("  IP obtido: ");
  Serial.println(WiFi.localIP());
}

void conectaMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Conectando ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado ao Broker com sucesso!");

      MQTT.subscribe(TOPIC_SUBSCRIBE);

    }
    else {
      Serial.println("Noo foi possivel se conectar ao broker.");
      Serial.println("Nova tentativa de conexao em 10s");

      delay(3000);
    }
  }
}



void recebePacote(char* topic, byte* payload, unsigned int length)
{
  String msg;

  //obtem a string do payload recebido
  for (int i = 0; i < length; i++)
  {
    char c = (char)payload[i];
    msg += c;
    //Serial.println(msg);
  }


  // Amperometry
  if (msg[0] == '0') { // start amperometry if the tag is "0"

    // Picking up the potential's value
    potential.concat(msg[2]);
    potential.concat(msg[3]);
    potential.concat(msg[4]);
    potential.concat(msg[5]);
    potential.concat(msg[6]);



    potentialfloat = potential.toFloat();

    applied_potentialbits = (potentialfloat + 0.2340) / 0.003898; // Final variable to be used


    //Serial.println(applied_potentialbits);
    potential = ""; // reseting the string
    //applied_potentialbits = 0;


    // Picking up the time's value
    timee.concat(msg[8]);
    timee.concat(msg[9]);
    timee.concat(msg[10]);
    timee.concat(msg[11]);

    timefloat = timee.toFloat(); // Final variable to be used

    //Serial.print("time: ");
    Serial.println(timefloat);
    timee = ""; // reseting the string


    // Picking up sample's interval
    sampleinterval.concat(msg[13]);
    sampleinterval.concat(msg[14]);
    sampleinterval.concat(msg[15]);

    sampleintervalfloat = sampleinterval.toFloat(); // Final variable to be used


    //Serial.print("sample interval: ");
    //Serial.println(sampleintervalfloat);
    sampleinterval = ""; // reseting the string





    // Starting Amperometry....
    myTime = millis();
    unsigned long tmilli = timefloat * 1000;
    unsigned long t = myTime + tmilli; // last variable to be used as a t controller
    long l = 1;
    while (myTime < t) {


      ledcWrite(ledChannel, applied_potentialbits);

      xs = String(l); // passing val into a string

      total = total - readings[readIndex];
      readings[readIndex] = analogRead(channel_1);
      total = total + readings[readIndex];
      readIndex = readIndex + 1;
      if (readIndex >= numReadings) {
        readIndex = 0;
      }
      average = total / numReadings;

      ys = String(average); // passing y into a string

      gString = ys + ";" + xs; // putting x and y into a string
      gString.toCharArray(globalString, 10); // converting string into a char array
      MQTT.publish("pot_channel1_data", globalString);// sendind char array to the main app
      Serial.println(globalString);




      myTime = millis();
      l++;

    }

    Serial.println("sai");




  }

  // CV
  if (msg[0] == '1') {


    initialpotential.concat(msg[2]);
    initialpotential.concat(msg[3]);
    initialpotential.concat(msg[4]);
    initialpotential.concat(msg[5]);
    initialpotential.concat(msg[6]);



    e_high.concat(msg[8]);
    e_high.concat(msg[9]);
    e_high.concat(msg[10]);
    e_high.concat(msg[11]);
    e_high.concat(msg[12]);


    e_low.concat(msg[14]);
    e_low.concat(msg[15]);
    e_low.concat(msg[16]);
    e_low.concat(msg[17]);
    e_low.concat(msg[18]);

    scanrate.concat(msg[20]);
    scanrate.concat(msg[21]);
    scanrate.concat(msg[22]);

    sweepsegments.concat(msg[24]);
    sweepsegments.concat(msg[25]);




    initialpotentialfloat = initialpotential.toFloat();
    e_highfloat = e_high.toFloat();
    e_lowfloat = e_low.toFloat();
    scanratefloat = scanrate.toFloat();
    sweepsegmentsfloat = sweepsegments.toFloat();

    applied_initialpotentialbits = (initialpotentialfloat + 0.2340) / 0.003898; // Final variable to be used
    applied_ehighbits = (e_highfloat + 0.2340) / 0.003898; // Final variable to be used
    applied_elowbits = (e_highfloat + 0.2340) / 0.003898; // Final variable to be used

    Serial.println(applied_initialpotentialbits);
    Serial.println(applied_ehighbits);
    Serial.println(applied_elowbits);
    Serial.println(scanratefloat);
    Serial.println(sweepsegmentsfloat);

    initialpotential = ""; // reseting income initial potential
    e_high = "";
    e_low = "";
    scanrate = "";
    sweepsegments = "";






    /*
        for (int pos = 0; pos < count; pos++) {
          intervalos[pos] = (1000000L / ((vevals[pos]) * 128L));
        }
        for (int pos = 0; pos <= count; pos++) {
          n = 0;
          //while(n <= 1){
          //Start the forward scan
          for (val = 0; val <= 255; val++) {
            ledcWrite(ledChannel, val);


            // SENDING X VALUE TO THE MAIN APP
            //dtostrf(val, 1, 2, xString); // convertando para char - O mqtt publish soh funciana com char[]
            //MQTT.publish("pot_channel1_data", xString);
            //Serial.print(val);
            xs = String(val); // passing val into a string
            delay(intervalos[pos]);






            total = total - readings[readIndex];

            readings[readIndex] = analogRead(channel_1);

            total = total + readings[readIndex];

            readIndex = readIndex + 1;


            if (readIndex >= numReadings) {

              readIndex = 0;
            }


            average = total / numReadings;
            // Serial.print(",");









            // Sending Y value to the main app
            //MQTT.publish("pot_channel1_data", yString);
            //dtostrf(average, 1, 2, yString); // convertando para char - O mqtt publish soh funciana com char[]
            //Serial.println(average);

            ys = String(average); // passing y into a string
            gString = ys + ";" + xs; // putting x and y into a string
            gString.toCharArray(globalString, 10); // converting string into a char array
            MQTT.publish("pot_channel1_data", globalString);// sendind char array to the main app

          }
          //Start the reverse scan
          for (val = 255; val >= 0; val--) {
            ledcWrite(ledChannel, val);
            //Serial.print(val);

            //dtostrf(val, 1, 2, xString); // convertando para char - O mqtt publish soh funciana com char[]
            //MQTT.publish("pot_channel1_data", xString);



            xs = String(val); // passing val into a string
            delay(intervalos[pos]);
            //c = ((0.00195*(analogRead(ct))-1)*1000); // Current reading outputs in uA!!!


            total = total - readings[readIndex];

            readings[readIndex] = analogRead(channel_1);



            total = total + readings[readIndex];

            readIndex = readIndex + 1;


            if (readIndex >= numReadings) {

              readIndex = 0;
            }


            average = total / numReadings;
            //Serial.print(",");
            //Serial.println(average);

            // Sending Y value to the main app
            //MQTT.publish("pot_channel1_data", yString);
            //dtostrf(average, 1, 2, yString); // convertando para char - O mqtt publish soh funciana com char[]

            ys = String(average); // passing y into a string
            gString = ys + ";" + xs; // putting x and y into a string
            gString.toCharArray(globalString, 10); // converting string into a char array
            MQTT.publish("pot_channel1_data", globalString);// sendind char array to the main app




          }
          break;
          n = n + 1;
        }
    */

  }





}
