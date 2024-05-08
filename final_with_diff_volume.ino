#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif
#define PIN        6 
#define NUMPIXELS 35 
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 

int valvepin1 = 28;       //solenoidal valve digital pin declaration
int valvepin2 = 30;       //solenoidal valve digital pin declaration
int Reflect_sensor = 29;  //object detective & reflective sensor digital pin declaration

int MotorEn1 = 2;   //pump or stepper motor1 digital pin declaration for enabling
int MotorEn2 = 5;   //pump or stepper motor2 digital pin declaration for enabling
int MotorEn3 = 8;   //pump or stepper motor3 digital pin declaration for enabling
int MotorEn4 = 11;  //pump or stepper motor4 digital pin declaration for enabling
int MotorEn5 = 22;  //pump or stepper motor5 digital pin declaration for enabling
int MotorEn6 = 25;  //pump or stepper motor6 digital pin declaration for enabling

int Dir1 = 3;   //pump or stepper motor1 digital pin declaration for direction
int Dir2 = 6;   //pump or stepper motor2 digital pin declaration for direction
int Dir3 = 9;   //pump or stepper motor3 digital pin declaration for direction
int Dir4 = 12;  //pump or stepper motor4 digital pin declaration for direction
int Dir5 = 23;  //pump or stepper motor5 digital pin declaration for direction
int Dir6 = 26;  //pump or stepper motor6 digital pin declaration for direction

int Step1 = 4;   //pump or stepper motor digital pin declaration for pulses or steps
int Step2 = 7;   //pump or stepper motor digital pin declaration for pulses or steps
int Step3 = 10;  //pump or stepper motor digital pin declaration for pulses or steps
int Step4 = 13;  //pump or stepper motor digital pin declaration for pulses or steps
int Step5 = 24;  //pump or stepper motor digital pin declaration for pulses or steps
int Step6 = 27;  //pump or stepper motor digital pin declaration for pulses or steps

String serialResponse = "";           //declaration to read Serial data's
char sz[] = "F 20000 20000 500 200";  //steps washtime
bool stringComplete = false;
char *opt;                                                                //Serial data's alphabet serial response declaration
unsigned long long steps_1, steps_2, steps_3, steps_4, steps_5, steps_6;  // variable declaration for steps to take different voulme in all pump
float washtime;                                                           //variable declaration for steps to take washing time for pump
int j = 0;                                                                // Global Declaration to control the washing time & Prime in loop mode


void setup() {

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
 
  pixels.begin(); 
  pixels.clear(); 

  for(int i=0; i<NUMPIXELS; i++) 
  { 
    if(i<27)
    pixels.setPixelColor(i, pixels.Color(255,255,255));
    pixels.show();   
    delay(DELAYVAL); 
  }

  pinMode(MotorEn1, OUTPUT);  // Sets MotorEn1 pins as Output
  pinMode(MotorEn2, OUTPUT);  // Sets MotorEn2 pins as Output
  pinMode(MotorEn3, OUTPUT);  // Sets MotorEn3 pins as Output
  pinMode(MotorEn4, OUTPUT);  // Sets MotorEn4 pins as Output
  pinMode(MotorEn5, OUTPUT);  // Sets MotorEn5 pins as Output
  pinMode(MotorEn6, OUTPUT);  // Sets MotorEn6 pins as Output

  pinMode(Dir1, OUTPUT);  // Sets Dir1 pins as Output
  pinMode(Dir2, OUTPUT);  // Sets Dir2 pins as Output
  pinMode(Dir3, OUTPUT);  // Sets Dir3 pins as Output
  pinMode(Dir4, OUTPUT);  // Sets Dir4 pins as Output
  pinMode(Dir5, OUTPUT);  // Sets Dir5 pins as Output
  pinMode(Dir6, OUTPUT);  // Sets Dir6 pins as Output

  pinMode(Step1, OUTPUT);  // Sets Step1 pins as Output
  pinMode(Step2, OUTPUT);  // Sets Step2 pins as Output
  pinMode(Step3, OUTPUT);  // Sets Step3 pins as Output
  pinMode(Step4, OUTPUT);  // Sets Step4 pins as Output
  pinMode(Step5, OUTPUT);  // Sets Step5 pins as Output
  pinMode(Step6, OUTPUT);  // Sets Step6 pins as Output

  pinMode(A5, INPUT);  // sets photoelectric sensor pin as input for pump1
  pinMode(A3, INPUT);  // sets photoelectric sensor pin as input for pump2
  pinMode(A2, INPUT);  // sets photoelectric sensor pin as input for pump3
  pinMode(A1, INPUT);  // sets photoelectric sensor pin as input for pump4
  pinMode(A0, INPUT);  // sets photoelectric sensor pin as input for pump5

  pinMode(A4, INPUT);  // sets photoelectric sensor pin as input for conveyor

  pinMode(29, INPUT);          // sets Reflective or object sensor pin as input for cuvette detection
  pinMode(valvepin1, OUTPUT);  // sets solenoidal valve pin as output for all valve
  pinMode(valvepin2, OUTPUT);  // sets solenoidal valve pin as output for all valve

  Serial.begin(9600);  // Serial begin to monitor serail data's from aurduino IDE
}

void loop() 
{
  digitalWrite(MotorEn1, HIGH);  //disable motor to keep always ideal current OFF form driver until to start the pump or motor1
  digitalWrite(MotorEn2, HIGH);  //disable motor to keep always ideal current OFF form driver until to start the pump or motor2
  digitalWrite(MotorEn3, HIGH);  //disable motor to keep always ideal current OFF form driver until to start the pump or motor3
  digitalWrite(MotorEn4, HIGH);  //disable motor to keep always ideal current OFF form driver until to start the pump or motor4
  digitalWrite(MotorEn5, HIGH);  //disable motor to keep always ideal current OFF form driver until to start the pump or motor5
  digitalWrite(MotorEn6, HIGH);  //disable motor to keep always ideal current OFF form driver until to start the pump or motor6

  digitalWrite(valvepin1, LOW);  // solenoidal valve_1 intially keep always OFF
  digitalWrite(valvepin2, LOW);  // solenoidal valve_1 intially keep always OFF

  if (stringComplete)  //checking the stringComplete boolean for any serial data's in serial event
  {
    stringComplete = false;  //forcefully make false the stringComplete boolean after complete serial data's reading in serial event

    switch (opt[0])  //checking the serial read alphabet wheather fill,prime,wash
    {
      case 'F':
        fill();  //call function to call filling
        break;   //break statement to beak all loop once filling completed
      case 'P':
        prime();  //call function to prime for tube filling
        break;    //break statement to beak all loop once prime completed
      case 'W':
        wash();  //call function to call washing pump tube
        break;   //break statement to beak all loop once washing completed
    }
  }
}

void conveyor() 
{
  digitalWrite(MotorEn1, LOW);  //enabling motor
  digitalWrite(MotorEn2, LOW);  //enabling motor
  digitalWrite(MotorEn3, LOW);  //enabling motor
  digitalWrite(MotorEn4, LOW);  //enabling motor
  digitalWrite(MotorEn5, LOW);  //enabling motor
  delay(500);

  int j = 0;  //

  while (1)  //loop to run motors until to reach home sensor
  {
    digitalWrite(valvepin1, HIGH);  //solenoidal valve_1 ON(valve keep open for while)
    digitalWrite(valvepin2, LOW);  // solenoidal valve_1 intially keep always OFF

    if (Serial.available())
      break;

    if (j >= 11) 
    {
      fill();
    }

    digitalWrite(Dir1, LOW);  // direction
    digitalWrite(Dir2, LOW);  // direction
    digitalWrite(Dir3, LOW);  // direction
    digitalWrite(Dir4, LOW);  // direction
    digitalWrite(Dir5, LOW);  // direction

    int sensorValue = analogRead(A5);  //reading stopper sensor value
    float voltage = sensorValue * (5.0 / 1023.0);
    int sensorValue_1 = analogRead(A3);  //reading stopper sensor value
    float voltage_1 = sensorValue_1 * (5.0 / 1023.0);
    int sensorValue_2 = analogRead(A2);  //reading stopper sensor value
    float voltage_2 = sensorValue_2 * (5.0 / 1023.0);
    int sensorValue_3 = analogRead(A1);  //reading stopper sensor value
    float voltage_3 = sensorValue_3 * (5.0 / 1023.0);
    int sensorValue_4 = analogRead(A0);  //reading stopper sensor value
    float voltage_4 = sensorValue_4 * (5.0 / 1023.0);

    if (voltage > 4.8)
      digitalWrite(MotorEn1, HIGH);  //enabling motor
    if (voltage_1 > 4.8)
      digitalWrite(MotorEn2, HIGH);  //enabling motor
    if (voltage_2 > 4.8)
      digitalWrite(MotorEn3, HIGH);  //enabling motor
    if (voltage_3 > 4.8)
      digitalWrite(MotorEn4, HIGH);  //enabling motor
    if (voltage_4 > 4.8)
      digitalWrite(MotorEn5, HIGH);  //enabling motor


   if (voltage > 4.8 && voltage_1 > 4.8 && voltage_2 > 4.8 && voltage_3 > 4.8 && voltage_4 > 4.8) 
  {
      j++;
      digitalWrite(valvepin1, LOW);  // solenoidal valve_1 ON(valve keep open for a secound)
      digitalWrite(valvepin2, HIGH);  // solenoidal valve_1 intially keep always OFF
      delay(500);

      digitalWrite(MotorEn1, LOW);  //enabling motor
      digitalWrite(MotorEn2, LOW);  //enabling motor
      digitalWrite(MotorEn3, LOW);  //enabling motor
      digitalWrite(MotorEn4, LOW);  //enabling motor
      digitalWrite(MotorEn5, LOW);  //enabling motor

      digitalWrite(Dir1, HIGH);  // direction
      digitalWrite(Dir2, HIGH);  // direction
      digitalWrite(Dir3, HIGH);  // direction
      digitalWrite(Dir4, HIGH);  // direction
      digitalWrite(Dir5, HIGH);  // direction

      unsigned int steps = max(max(max(steps_1, steps_2), max(steps_3, steps_4)), max(steps_5, steps_6));

      if (j <= 10) 
      {
        for (int i = 0; i < steps * 4.2; i++)
         {
          if (Serial.available())
            break;

          if (i < steps_1 * 4.2) digitalWrite(Step1, HIGH);
          if (i < steps_2 * 4.2) digitalWrite(Step2, HIGH);
          if (i < steps_3 * 4.2) digitalWrite(Step3, HIGH);
          if (i < steps_4 * 4.2) digitalWrite(Step4, HIGH);
          if (i < steps_5 * 4.2) digitalWrite(Step5, HIGH);
          delayMicroseconds(400);
          if (i < steps_1 * 4.2) digitalWrite(Step1, LOW);
          if (i < steps_2 * 4.2) digitalWrite(Step2, LOW);
          if (i < steps_3 * 4.2) digitalWrite(Step3, LOW);
          if (i < steps_4 * 4.2) digitalWrite(Step4, LOW);
          if (i < steps_5 * 4.2) digitalWrite(Step5, LOW);
          delayMicroseconds(400);
        }

        digitalWrite(MotorEn1, HIGH);  //dISABLE motor
        digitalWrite(MotorEn2, HIGH);  //dISABLE motor
        digitalWrite(MotorEn3, HIGH);  //dISABLE motor
        digitalWrite(MotorEn4, HIGH);  //dISABLE motor
        digitalWrite(MotorEn5, HIGH);  //dISABLE motor

        digitalWrite(MotorEn6, LOW);  //enabling motor
        digitalWrite(Dir6, LOW);      // direction

        delay(500);

        if (j >= 2)
         {
          for (int i = 0; i < 65; i++)
           {
            if (Serial.available())
              break;

            digitalWrite(Step6, HIGH);
            delayMicroseconds(1400);
            digitalWrite(Step6, LOW);
            delayMicroseconds(1400);
          }
          digitalWrite(MotorEn6, HIGH);  //disbling motor
        }
        digitalWrite(MotorEn1, LOW);  //enabling motor
        digitalWrite(MotorEn2, LOW);  //enabling motor
        digitalWrite(MotorEn3, LOW);  //enabling motor
        digitalWrite(MotorEn4, LOW);  //enabling motor
        digitalWrite(MotorEn5, LOW);  //enabling motor
      }
      delay(500);
    }
    digitalWrite(Step1, HIGH);
    digitalWrite(Step2, HIGH);
    digitalWrite(Step3, HIGH);
    digitalWrite(Step4, HIGH);
    digitalWrite(Step5, HIGH);
    delayMicroseconds(400);
    digitalWrite(Step1, LOW);
    digitalWrite(Step2, LOW);
    digitalWrite(Step3, LOW);
    digitalWrite(Step4, LOW);
    digitalWrite(Step5, LOW);
    delayMicroseconds(400);
  }
}


void prime()
 {
  digitalWrite(MotorEn1, LOW);  //enabling motor
  digitalWrite(MotorEn2, LOW);  //enabling motor
  digitalWrite(MotorEn3, LOW);  //enabling motor
  digitalWrite(MotorEn4, LOW);  //enabling motor
  digitalWrite(MotorEn5, LOW);  //enabling motor

  int j = 0;

  while (1) 
  {
    if (Serial.available())
      break;

    digitalWrite(valvepin1, HIGH);  //solenoidal valve_1 ON(valve keep open for a secound)
    digitalWrite(valvepin2, LOW);  // solenoidal valve_1 intially keep always OFF

    if (j > 4) 
    {
      if (Serial.available())
      break;
      break;
    }

    digitalWrite(Dir1, LOW);  // direction
    digitalWrite(Dir2, LOW);  // direction
    digitalWrite(Dir3, LOW);  // direction
    digitalWrite(Dir4, LOW);  // direction
    digitalWrite(Dir5, LOW);  // direction

    int sensorValue = analogRead(A5);  //reading stopper sensor value
    float voltage = sensorValue * (5.0 / 1023.0);
    int sensorValue_1 = analogRead(A3);  //reading stopper sensor value
    float voltage_1 = sensorValue_1 * (5.0 / 1023.0);
    int sensorValue_2 = analogRead(A2);  //reading stopper sensor value
    float voltage_2 = sensorValue_2 * (5.0 / 1023.0);
    int sensorValue_3 = analogRead(A1);  //reading stopper sensor value
    float voltage_3 = sensorValue_3 * (5.0 / 1023.0);
    int sensorValue_4 = analogRead(A0);  //reading stopper sensor value
    float voltage_4 = sensorValue_4 * (5.0 / 1023.0);

    if (voltage > 4.8)
      digitalWrite(MotorEn1, HIGH);  //enabling motor
    if (voltage_1 > 4.8)
      digitalWrite(MotorEn2, HIGH);  //enabling motor
    if (voltage_2 > 4.8)
      digitalWrite(MotorEn3, HIGH);  //enabling motor
    if (voltage_3 > 4.8)
      digitalWrite(MotorEn4, HIGH);  //enabling motor
    if (voltage_4 > 4.8)
      digitalWrite(MotorEn5, HIGH);  //enabling motor

    if (voltage > 4.8 && voltage_1 > 4.8 && voltage_2 > 4.8 && voltage_3 > 4.8 && voltage_4 > 4.8)
     {
      j++;

      digitalWrite(valvepin1, LOW);  // solenoidal valve_1 ON(valve keep open for a secound)
      digitalWrite(valvepin2, HIGH);  // solenoidal valve_1 intially keep always OFF
      delay(500);

      digitalWrite(MotorEn1, LOW);  //enabling motor
      digitalWrite(MotorEn2, LOW);  //enabling motor
      digitalWrite(MotorEn3, LOW);  //enabling motor
      digitalWrite(MotorEn4, LOW);  //enabling motor
      digitalWrite(MotorEn5, LOW);  //enabling motor

      digitalWrite(Dir1, HIGH);  // direction
      digitalWrite(Dir2, HIGH);  // direction
      digitalWrite(Dir3, HIGH);  // direction
      digitalWrite(Dir4, HIGH);  // direction
      digitalWrite(Dir5, HIGH);  // direction

      if (Serial.available())
        break;

      if (j <= 3)
       {
        for (int i = 0; i < 2100; i++) 
        {
          digitalWrite(Step1, HIGH);
          digitalWrite(Step2, HIGH);
          digitalWrite(Step3, HIGH);
          digitalWrite(Step4, HIGH);
          digitalWrite(Step5, HIGH);
          delayMicroseconds(450);
          digitalWrite(Step1, LOW);
          digitalWrite(Step2, LOW);
          digitalWrite(Step3, LOW);
          digitalWrite(Step4, LOW);
          digitalWrite(Step5, LOW);
          delayMicroseconds(450);

          if (Serial.available())
            break;
        }

        digitalWrite(MotorEn1, HIGH);  //dISABLE motor
        digitalWrite(MotorEn2, HIGH);  //dISABLE motor
        digitalWrite(MotorEn3, HIGH);  //dISABLE motor
        digitalWrite(MotorEn4, HIGH);  //dISABLE motor
        digitalWrite(MotorEn5, HIGH);  //dISABLE motor

        delay(500);

        digitalWrite(MotorEn1, LOW);  //enabling motor
        digitalWrite(MotorEn2, LOW);  //enabling motor
        digitalWrite(MotorEn3, LOW);  //enabling motor
        digitalWrite(MotorEn4, LOW);  //enabling motor
        digitalWrite(MotorEn5, LOW);  //enabling motor
      }
    }
    digitalWrite(Step1, HIGH);
    digitalWrite(Step2, HIGH);
    digitalWrite(Step3, HIGH);
    digitalWrite(Step4, HIGH);
    digitalWrite(Step5, HIGH);
    delayMicroseconds(400);
    digitalWrite(Step1, LOW);
    digitalWrite(Step2, LOW);
    digitalWrite(Step3, LOW);
    digitalWrite(Step4, LOW);
    digitalWrite(Step5, LOW);
    delayMicroseconds(400);
  }
}


void wash() 
{
  digitalWrite(MotorEn1, LOW);  //enabling motor
  digitalWrite(MotorEn2, LOW);  //enabling motor
  digitalWrite(MotorEn3, LOW);  //enabling motor
  digitalWrite(MotorEn4, LOW);  //enabling motor
  digitalWrite(MotorEn5, LOW);  //enabling motor

  int j = 0;

  while (1) 
  {
    digitalWrite(valvepin1, HIGH);  //solenoidal valve_1 ON(valve keep open for a secound)
    digitalWrite(valvepin2, LOW);  // solenoidal valve_1 intially keep always OFF

    if (Serial.available())
      break;

    if (j >= 8.0 * washtime)
    {
      if (Serial.available())
        break;

      break;
    }

    digitalWrite(Dir1, LOW);  // direction
    digitalWrite(Dir2, LOW);  // direction
    digitalWrite(Dir3, LOW);  // direction
    digitalWrite(Dir4, LOW);  // direction
    digitalWrite(Dir5, LOW);  // direction

    int sensorValue = analogRead(A5);  //reading stopper sensor value
    float voltage = sensorValue * (5.0 / 1023.0);

    int sensorValue_1 = analogRead(A3);  //reading stopper sensor value
    float voltage_1 = sensorValue_1 * (5.0 / 1023.0);

    int sensorValue_2 = analogRead(A2);  //reading stopper sensor value
    float voltage_2 = sensorValue_2 * (5.0 / 1023.0);

    int sensorValue_3 = analogRead(A1);  //reading stopper sensor value
    float voltage_3 = sensorValue_3 * (5.0 / 1023.0);

    int sensorValue_4 = analogRead(A0);  //reading stopper sensor value
    float voltage_4 = sensorValue_4 * (5.0 / 1023.0);


    if (voltage > 4.8)
      digitalWrite(MotorEn1, HIGH);  //enabling motor
    if (voltage_1 > 4.8)
      digitalWrite(MotorEn2, HIGH);  //enabling motor
    if (voltage_2 > 4.8)
      digitalWrite(MotorEn3, HIGH);  //enabling motor
    if (voltage_3 > 4.8)
      digitalWrite(MotorEn4, HIGH);  //enabling motor
    if (voltage_4 > 4.8)
      digitalWrite(MotorEn5, HIGH);  //enabling motor

    if (voltage > 4.8 && voltage_1 > 4.8 && voltage_2 > 4.8 && voltage_3 > 4.8 && voltage_4 > 4.8)
     {
      j++;

      digitalWrite(valvepin1, LOW);  // solenoidal valve_1 ON(valve keep open for a secound)
      digitalWrite(valvepin2, HIGH);  // solenoidal valve_1 intially keep always OFF
      delay(500);

      if (Serial.available())
        break;

      digitalWrite(MotorEn1, LOW);  //enabling motor
      digitalWrite(MotorEn2, LOW);  //enabling motor
      digitalWrite(MotorEn3, LOW);  //enabling motor
      digitalWrite(MotorEn4, LOW);  //enabling motor
      digitalWrite(MotorEn5, LOW);  //enabling motor

      digitalWrite(Dir1, HIGH);  // direction
      digitalWrite(Dir2, HIGH);  // direction
      digitalWrite(Dir3, HIGH);  // direction
      digitalWrite(Dir4, HIGH);  // direction
      digitalWrite(Dir5, HIGH);  // direction

      if (j <= 7.0 * washtime)
       {
        if (Serial.available())
          break;

        for (int i = 0; i < 2100; i++) 
        {
          digitalWrite(Step1, HIGH);
          digitalWrite(Step2, HIGH);
          digitalWrite(Step3, HIGH);
          digitalWrite(Step4, HIGH);
          digitalWrite(Step5, HIGH);
          delayMicroseconds(450);
          digitalWrite(Step1, LOW);
          digitalWrite(Step2, LOW);
          digitalWrite(Step3, LOW);
          digitalWrite(Step4, LOW);
          digitalWrite(Step5, LOW);
          delayMicroseconds(450);

          if (Serial.available())
            break;
        }

        digitalWrite(MotorEn1, HIGH);  //dISABLE motor
        digitalWrite(MotorEn2, HIGH);  //dISABLE motor
        digitalWrite(MotorEn3, HIGH);  //dISABLE motor
        digitalWrite(MotorEn4, HIGH);  //dISABLE motor
        digitalWrite(MotorEn5, HIGH);  //dISABLE motor

        delay(500);

        digitalWrite(MotorEn1, LOW);  //enabling motor
        digitalWrite(MotorEn2, LOW);  //enabling motor
        digitalWrite(MotorEn3, LOW);  //enabling motor
        digitalWrite(MotorEn4, LOW);  //enabling motor
        digitalWrite(MotorEn5, LOW);  //enabling motor
      }
    }
    digitalWrite(Step1, HIGH);
    digitalWrite(Step2, HIGH);
    digitalWrite(Step3, HIGH);
    digitalWrite(Step4, HIGH);
    digitalWrite(Step5, HIGH);
    delayMicroseconds(400);
    digitalWrite(Step1, LOW);
    digitalWrite(Step2, LOW);
    digitalWrite(Step3, LOW);
    digitalWrite(Step4, LOW);
    digitalWrite(Step5, LOW);
    delayMicroseconds(400);
  }
}

void fill() 
{
  while (1) 
  {
    digitalWrite(valvepin1, LOW);
    
    if (digitalRead(29))  // reflective object sensor
    {
      if (Serial.available())
        break;

      digitalWrite(valvepin1, LOW);

      int sensorValue_6 = analogRead(A4);  //reading stopper sensor value
      float voltage_6 = sensorValue_6 * (5.0 / 1023.0);

      digitalWrite(MotorEn6, LOW);  //enabling motor

      digitalWrite(Dir6, LOW);  // direction

      digitalWrite(Step6, HIGH);
      delayMicroseconds(1400);
      digitalWrite(Step6, LOW);
      delayMicroseconds(1400);

      if (voltage_6 > 4.5)  // optical sensor
      {
        if (Serial.available())
          break;
        conveyor();
      }
    } else {
      if (Serial.available())
        break;

      int sensorValue_1 = analogRead(A4);  //reading stopper sensor value
      float voltage_1 = sensorValue_1 * (5.0 / 1023.0);

      digitalWrite(valvepin1, LOW);

      digitalWrite(MotorEn6, LOW);  //enabling motor

      digitalWrite(Dir6, LOW);  // direction

      digitalWrite(Step6, HIGH);
      delayMicroseconds(1400);
      digitalWrite(Step6, LOW);
      delayMicroseconds(1400);
    }
  }
  delay(1000);
}



void serialEvent() 
{
  if (Serial.available()) 
  {
    serialResponse = Serial.readStringUntil('\r\n');
    char buf[sizeof(sz)];
    serialResponse.toCharArray(buf, sizeof(buf));
    char *p = buf;
    char *str;
    int i = 0;
    while ((str = strtok_r(p, " ", &p)) != NULL) 
    {
      if (i == 0)
        opt = str;
      else if (i == 1)
        steps_1 = atoi(str);
      else if (i == 2)
        steps_2 = atoi(str);
      else if (i == 3)
        steps_3 = atoi(str);
      else if (i == 4)
        steps_4 = atoi(str);
      else if (i == 5)
        steps_5 = atoi(str);
      else if (i == 6)
        washtime = atoi(str);
      i++;
    }
    stringComplete = true;
  }
}
