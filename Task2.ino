//read: Please uncomment lines 114 and 128 if you can't see left and right wheels.

//#define don't count as lines!
#define trigPinF A5
#define echoPinF A4
#define trigPinR A3
#define echoPinR A2
#define trigPinL A0
#define echoPinL A1

#define motor_pin_1 1
#define motor_pin_2 0
#define motor_pin_3 13
#define motor_pin_4 4

#define motor_pin_5 5
#define motor_pin_6 6
#define motor_pin_7 7
#define motor_pin_8 8

#define motor_pin_9 9
#define motor_pin_10 10
#define motor_pin_11 11
#define motor_pin_12 12

int count=0; //for left&right rotation
bool ToggleBool; //for turn off/on switch
float distanceF; //for distance
bool ledState = 0; // for led
unsigned long previousMillis=0; //for led
unsigned long previousMillis2=0; //for alarm

void setup () {

//Ultrasonic:
pinMode (trigPinF, OUTPUT); //TRIG
pinMode (echoPinF, INPUT); //ECHO
pinMode (trigPinR, OUTPUT); //TRIG
pinMode (echoPinL, INPUT); //ECHO
//pinMode (trigPinL, OUTPUT); //TRIG
pinMode (echoPinL, INPUT); //ECHO

//Blinking LED:
pinMode(A0,OUTPUT); 

//Power and alarm:
pinMode(3,INPUT);
pinMode(2,INPUT);

//Steppers:
  pinMode(motor_pin_9, OUTPUT);
  pinMode(motor_pin_10, OUTPUT);
  pinMode(motor_pin_11, OUTPUT);
  pinMode(motor_pin_12, OUTPUT);

  pinMode(motor_pin_5, OUTPUT);
  pinMode(motor_pin_6, OUTPUT);
  pinMode(motor_pin_7, OUTPUT);
  pinMode(motor_pin_8, OUTPUT);

  pinMode(motor_pin_1, OUTPUT);
  pinMode(motor_pin_2, OUTPUT);
  pinMode(motor_pin_3, OUTPUT);
  pinMode(motor_pin_4, OUTPUT);

//Interrupt:
attachInterrupt(digitalPinToInterrupt(3),Toggle,CHANGE);
ToggleBool=digitalRead(3);
}

void loop () {	
//Alarm:
if (digitalRead(2) == 1)
previousMillis2=millis();
while (digitalRead(2) == 1)
{
unsigned long currentMillis2 = millis();
if ((unsigned long)(currentMillis2-previousMillis2 >= 500)) {
while(digitalRead(2)==1)
	{
	MoveForward(motor_pin_1,motor_pin_2,motor_pin_3,motor_pin_4,6);
	}
}
}

//LED:
unsigned long currentMillis = millis();
if ((unsigned long)(currentMillis-previousMillis >= 1000)) {
ledState = !ledState;
digitalWrite(A0, ledState);
previousMillis=millis();
}


//Movement:
CheckFront(); //Front distance
if (distanceF < 25)
{ //Move both wheels
MoveForward(motor_pin_9,motor_pin_10,motor_pin_11,motor_pin_12,10);
MoveForward(motor_pin_5,motor_pin_6,motor_pin_7,motor_pin_8,10);
}
else
{
	CheckRight(); //Right distance
	if (distanceF < 25)
		{
		//Two full revolutions
		while (count < 64)
		{
		MoveForward(motor_pin_5,motor_pin_6,motor_pin_7,motor_pin_8,10);
		count=count+4;
		}
		count=0;
		//delay(300); ///////////DEBUGGING. PUT THIS DELAY TO SEE THE STEPPER CLEARLY.
		}
	else
		{
		CheckLeft(); //Left distance
		if(distanceF < 25)
			{
			//Two full revolutions
			while (count < 64)
			{
			MoveForward(motor_pin_9,motor_pin_10,motor_pin_11,motor_pin_12,10);
			count=count+4;
			}
			count=0;
			//delay(300); ///////////DEBUGGING. PUT THIS DELAY TO SEE THE STEPPER CLEARLY.
			}
		else
			{
			//Move backwards (inputs are backward)
			MoveForward(motor_pin_12,motor_pin_11,motor_pin_10,motor_pin_9,4);
			MoveForward(motor_pin_8,motor_pin_7,motor_pin_6,motor_pin_5,4);
			}
		}
}
}



void CheckFront() //Front distance
{
  digitalWrite(trigPinF, LOW ); 
  delayMicroseconds( 2 );
  digitalWrite(trigPinF, HIGH );
  delayMicroseconds( 10 );
  digitalWrite(trigPinF, LOW );
  distanceF = pulseIn(echoPinF, HIGH ); 
  distanceF = ( distanceF/2 ) * 0.0344;
}
void CheckRight() //Right distance
{
  digitalWrite(trigPinR, LOW ); 
  delayMicroseconds( 2 );
  digitalWrite(trigPinR, HIGH );
  delayMicroseconds( 10 );
  digitalWrite(trigPinR, LOW );
  distanceF = pulseIn(echoPinR, HIGH ); 
  distanceF = ( distanceF/2 ) * 0.0344;
}
void CheckLeft() //Left distance
{
	  digitalWrite(trigPinF, LOW ); 
 	  delayMicroseconds( 2 );
 	  digitalWrite(trigPinF, HIGH );
  	  delayMicroseconds( 10 );
  	  digitalWrite(trigPinF, LOW );
      distanceF = pulseIn(echoPinL, HIGH ); 
 	  distanceF = ( distanceF/2 ) * 0.0344;
}

void MoveForward(int motor1,int motor2,int motor3,int motor4, int speed) //Movement
{
        digitalWrite(motor1, HIGH);
        digitalWrite(motor2, LOW);
        digitalWrite(motor3, LOW);
        digitalWrite(motor4, HIGH);
delay(speed);
        digitalWrite(motor1, HIGH);
        digitalWrite(motor2, HIGH);
        digitalWrite(motor3, LOW);
        digitalWrite(motor4, LOW);
delay(speed);
        digitalWrite(motor1, LOW);
        digitalWrite(motor2, HIGH);
        digitalWrite(motor3, HIGH);
        digitalWrite(motor4, LOW);
delay(speed);
        digitalWrite(motor1, LOW);
        digitalWrite(motor2, LOW);
        digitalWrite(motor3, HIGH);
        digitalWrite(motor4, HIGH);
delay(speed);
}

void Toggle() //Toggle power
{
	int temp = ToggleBool;
	ToggleBool=!ToggleBool;
	while (ToggleBool == !temp)
	{
	if (digitalRead(3) == temp)
	ToggleBool=!ToggleBool;
	}
}