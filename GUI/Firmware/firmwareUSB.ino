// the number of the LED pin
const int ledPin = 16;  // 16 corresponds to GPIO16

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;




int val = 0;
int ct = 36; //ADC 36 adc_01
int ct2 = 39; //ADC 36 adc_01
int c = 0;
int n = 0;


float Potstep = 0.0078; // fixed due to the DAC resolution
int vevals[] = {100}; //multiple scan rates values (mV/s)
int const count = 6;
long intervalos[count];






const int numReadings = 1; // number of readings for the filter

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average





void setup() {

  Serial.begin(9600);
  pinMode(16, OUTPUT);
  pinMode(ct, INPUT);
  pinMode(ct2, INPUT);

  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPin, ledChannel);





}
void loop() {



  if (Serial.available() > 0) {

    int inByte = Serial.read();


    switch (inByte) {



      case 'b':
        ledcWrite(ledChannel, 0);
        c = analogRead(ct);
        Serial.println(c);
        delay(5000);
        break;






      case 'c':
        ledcWrite(ledChannel, 255);
        c = analogRead(ct);
        Serial.println(ct);
        delay(5000);
        break;

      case 'a':
        //ledcWrite(ledChannel, 0);
        delay(2000);


        for (int pos = 0; pos < count; pos++) {
          intervalos[pos] = (1000000L / ((vevals[pos]) * 128L));
        }
        for (int pos = 0; pos <= count; pos++) {
          n = 0;
          //while(n <= 0){
          //Start the forward scan
          for (val = 0; val <= 255; val++) {
            ledcWrite(ledChannel, val);
            Serial.print(val);
            delay(intervalos[pos]);
            //c = ((0.00195*(analogRead(ct))-1)*1000); // Current reading outputs in uA!!!






           c = analogRead(ct2);
            Serial.print(";");
            Serial.println(c);
            // Serial.print("  ");
            //Serial.print(n);
            //Serial.print("  ");
            //Serial.print(vevals[pos]);
            //Serial.print("  ");
            //Serial.println(intervalos[pos]);
          }
          //Start the reverse scan
          for (val = 255; val >= 0; val--) {
            ledcWrite(ledChannel, val);
            Serial.print(val);
            delay(intervalos[pos]);
            //c = ((0.00195*(analogRead(ct))-1)*1000); // Current reading outputs in uA!!!

       
            
            
            
            c = analogRead(ct2);
            Serial.print(";");
            Serial.println(c);
            //Serial.print("  ");
            //Serial.print(n);
            //Serial.print("  ");
            //Serial.print(vevals[pos]);
            //Serial.print("  ");
            //Serial.println(intervalos[pos]);
          }
          break;
          n = n - 1;
        }
    }
  }
//}
}//Script ends
