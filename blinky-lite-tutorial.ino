char serialBuf[32];
String serialCommand = "";
unsigned long debounceDelay = 100;    

const int redLed = 10;
const int redButton = 8;
int redLedValue   = 255;
int redLedOn = 1;
int lastRedButtonReading = LOW;   
unsigned long lastRedDebounceTime = 0;  

const int greenLed = 9;
const int greenButton = 7;
int greenLedValue = 255;
int greenLedOn = 1;

int lastGreenButtonReading = LOW;   
unsigned long lastGreenDebounceTime = 0;  

void setup() 
{
  pinMode(redLed, OUTPUT); 
  pinMode(greenLed, OUTPUT); 
  pinMode(redButton, INPUT);
  pinMode(greenButton, INPUT);

  analogWrite(redLed, redLedValue * redLedOn);
  analogWrite(greenLed, greenLedValue * greenLedOn);
  Serial.begin(9600);
}

void loop() 
{
  readSerial();
  if (serialCommand.length() > 0)
  {
    Serial.println(serialCommand);
    if (serialCommand.indexOf("redLedOn") >= 0)
    {
      redLedOn = newIntSetting("redLedOn", serialCommand);
      analogWrite(redLed, redLedValue * redLedOn);
    }
    if (serialCommand.indexOf("redLedValue") >= 0)
    {
      redLedValue = newIntSetting("redLedValue", serialCommand);
      analogWrite(redLed, redLedValue * redLedOn);
    }
    if (serialCommand.indexOf("greenLedOn") >= 0)
    {
      greenLedOn = newIntSetting("greenLedOn", serialCommand);
      analogWrite(greenLed, greenLedValue * greenLedOn);
    }
    if (serialCommand.indexOf("greenLedValue") >= 0)
    {
      greenLedValue = newIntSetting("greenLedValue", serialCommand);
      analogWrite(greenLed, greenLedValue * greenLedOn);
    }
  }
  buttonDebouncer(&lastGreenDebounceTime, &lastGreenButtonReading, &greenLedOn, greenButton, greenLed, greenLedValue, "green");
  buttonDebouncer(&lastRedDebounceTime, &lastRedButtonReading, &redLedOn, redButton, redLed, redLedValue, "red");
}
void buttonDebouncer(unsigned long* lastDebounceTime, int* lastButtonReading, int* ledOn, int buttonPin, int ledPin, int ledValue, String color)
{
  if ((millis() - *lastDebounceTime) > debounceDelay)
  {
    int reading = digitalRead(buttonPin);
    if ((reading == HIGH) && (*lastButtonReading == LOW))
    {
      *lastDebounceTime = millis();
      if (*ledOn > 0) 
      {
        *ledOn = 0;
        Serial.println(color + "LedOn 0!");
      }
      else
      {
        *ledOn = 1;
        Serial.println(color + "LedOn 1!");
      }
      analogWrite(ledPin, ledValue * *ledOn);
    }
    *lastButtonReading = reading;
  }
}
void readSerial()
{
  serialCommand = "";
  while(Serial.available() > 0)
  {
    
    for (int ii = 0; ii < sizeof(serialBuf); ++ii) serialBuf[ii] = 0;
    Serial.readBytesUntil('!', serialBuf, 31);
    serialCommand =  String(serialBuf);
  }
}
int newIntSetting(String setParse, String input)
{
  char intbuf[32];
  String newSettingString;
  newSettingString = input.substring(input.lastIndexOf(setParse) + setParse.length() + 1,input.length());
  newSettingString.toCharArray(intbuf, sizeof(intbuf));
  return atoi(intbuf);
}

