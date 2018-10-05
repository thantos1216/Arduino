//Duck Waterer 2.3
//Chuck Anderson
//Created 8/17/2018

// *************************************************************************
//Only change the following group of variables. The rest are set properly.
const int motorPin = 12;
const int waterPin = 3;
const int switchPin = 9;
const int speakerPin = 13;

//Delay times are in seconds
const int motorOnTime = 10;
const int waterOffTime = 2;
const int maxMotorOnTime = 3;

double slowDownDelay = 1.5;

int motorSpeed = 90; //Enter the speed as a percentage
// *************************************************************************

//Do not adjust the following lines.
unsigned long int currentTime = 0;
unsigned long int previousMotorTime = 0;
unsigned long int previousWaterTime = 0;
unsigned long int previousMaxMotorTime = 0;

int switchMode = 0;

int motorCounter = 0;
int waterCounter = 0;
int maxMotorCounter = 0;

int speed = (255 / 100) * motorSpeed;
unsigned long int motorSlowDownDelay = slowDownDelay * 1000;
int motorSlowDown = speed;

bool motorStart = false;
bool switchPosition = false;
bool waterStart = false;

//Setup the pinmodes for the pins in use.
void setup()
{
    pinMode(motorPin, OUTPUT);
    pinMode(waterPin, OUTPUT);
    pinMode(switchPin, INPUT_PULLUP);
    pinMode(speakerPin, OUTPUT);
}

//Starts the main loop that will continue indefinately.
void loop()
{
    currentTime = millis();

    if (currentTime < previousMotorTime || currentTime < previousWaterTime)
    {
        previousMotorTime = currentTime;
        previousWaterTime = currentTime;
        previousMaxMotorTime = currentTime;
    }

    switchMode = digitalRead(switchPin);

    if (currentTime - previousMotorTime >= 1000 && waterStart == false)
    {
        motorCounter++;
        previousMotorTime = currentTime;
    }

    if (motorStart == true && currentTime - previousMaxMotorTime >= 1000)
    {
        maxMotorCounter++;
        previousMaxMotorTime = currentTime;
    }

    if (maxMotorCounter >= maxMotorOnTime)
    {
        //Used for a hard stop to the motor
        //analogWrite(motorPin, 0);

        //Used to slow down the motor before a complete stop.
        motorSlowDown = motorSpeed;
        while (motorSlowDown >= 0)
        {
            analogWrite(motorPin, motorSlowDown);
            motorSlowDown--;
            delayMicroseconds(motorSlowDownDelay);
        }

        motorStart = false;
        maxMotorCounter = 0;
        //motorCounter = 0;
    }

    if (motorCounter >= motorOnTime)
    {
        if (switchMode == HIGH)
        {
            switchPosition = true;
        }
        else
        {
            switchPosition = false;
        }

        for (int i = 700; i < 800; i++)
        {
            tone(speakerPin, i);
            delay(15);
        }
        for (int i = 800; i > 700; i--)
        {
            tone(speakerPin, i);
            delay(15);
        }

        noTone(speakerPin);

        analogWrite(motorPin, speed);
        motorStart = true;

        previousMotorTime = millis();
        previousMaxMotorTime = millis();

        motorCounter = 0;
    }

    if (switchPosition == true && switchMode == LOW)
    {
        switchPosition = false;
    }

    if (switchMode == HIGH && switchPosition == false && motorStart == true)
    {
        //Use the following line to have a hard stop to the code.
        //analogWrite(motorPin, 0);

        //The following code is set up for the motor slowdown for a soft stop.
        motorSlowDown = motorSpeed;
        while (motorSlowDown >= 0)
        {
            analogWrite(motorPin, motorSlowDown);
            motorSlowDown--;
            delayMicroseconds(motorSlowDownDelay);
        }

        motorStart = false;
        previousWaterTime = millis();
        waterStart = true;

        digitalWrite(waterPin, HIGH);

        motorCounter = 0;
    }

    if (currentTime - previousWaterTime >= 1000 && waterStart == true)
    {
        waterCounter++;
        previousWaterTime = millis();
    }

    if (waterCounter >= waterOffTime)
    {
        waterCounter = 0;
        waterStart = false;
        digitalWrite(waterPin, LOW);
    }
}
