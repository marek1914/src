/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */

//int prescale = 1;
//const byte mask = B11111000;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(2, INPUT);
  digitalWrite(11, LOW);
  digitalWrite(13, HIGH);
  delay(5000);
  digitalWrite(13, LOW);
  delay(5000);
  //TCCR2B = (TCCR2B & mask) | prescale;
  //Serial.begin(9600);
}

int i;
// the loop function runs over and over again forever
#if 0
void loop() {
  for(i=1; i<=250; i++)
  {
    analogWrite(11, i);   // turn the LED on (HIGH is the voltage level)
    if(i<20)
      delay(40);  // wait for a second
    else
      delay(15);
  }
  delay(200);
  for(i=250; i>=1; i--)
  {
    analogWrite(11, i);   // turn the LED on (HIGH is the voltage level)
    if(i<20)
      delay(40);
    else
      delay(15);  // wait for a second
  }
  delay(1000);
}
#endif

void loop()
{
  analogWrite(11, 63);
  delay(5000);
  analogWrite(11, 127);
  delay(5000);
  analogWrite(11, 191);
  delay(5000);
  analogWrite(11, 255);
  delay(5000);
}

/*
void loop() {
  if (digitalRead(2) == HIGH)
     //Serial.println("\n Hello!");
     digitalWrite(13, HIGH);
  else
     digitalWrite(13, LOW);
     
  }
*/

/*
void loop() {
    for(i=0; i<255; i++) {
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      
      analogWrite(11,i);
      delay(2000);
    }
  }
*/
