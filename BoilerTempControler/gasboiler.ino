// Include the required Arduino libraries:
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const unsigned long Interval = 1000;
unsigned long previousTime = 0;

const unsigned long Interval2 = 1500;
unsigned long previousTime2 = 0;

// Define to which pin of the Arduino the 1-Wire bus is connected:
#define ONE_WIRE_BUS 7


float Setvariable = 20;
int Upbutton = 6;
int Downbutton = 10;
int relayPin = 13;

// Create a new instance of the oneWire class to communicate with any OneWire device:
OneWire oneWire(ONE_WIRE_BUS);

// Pass the oneWire reference to DallasTemperature library:
DallasTemperature sensors(&oneWire);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  // Begin serial communication at a baud rate of 9600:
  Serial.begin(9600);
  
  // Start up the library:
  sensors.begin();

  lcd.setCursor(0,0);  //Set LCD cursor to upper left corner: column 0, row 0
  lcd.print("IoT Gas Boiler");  //Print my message
  delay(1000);
  lcd.clear();
  pinMode(Upbutton,INPUT_PULLUP);
  pinMode(Downbutton,INPUT_PULLUP);
}

void loop() {
  
  unsigned long currentTime = millis();//Picks up the time to use for the button debouncing
  unsigned long currentTime2 = millis();//To use for tracking the time interval in between temp and humidity readings
  int tempC;
  // Send the command for all devices on the bus to perform a temperature conversion:
  sensors.requestTemperatures();

  if (currentTime2 - previousTime2 >= Interval2){
    tempC = sensors.getTempCByIndex(0); // the index 0 refers to the first device
    // Print the temperature in Celsius in the Serial Monitor:
    lcd.setCursor(0,0);
    lcd.print("Temp:");
    lcd.print(tempC);
    lcd.print((char)223);
    lcd.print("C");
    previousTime2 = currentTime2;
  }

  if (tempC > Setvariable){
    lcd.setCursor(9,0);
    lcd.print(" SW:");
    lcd.print("ON ");
    digitalWrite(relayPin,LOW); //switch value depending on the relay used
  }


  if (tempC < Setvariable){
    lcd.setCursor(9,0);
    lcd.print(" SW:");
    lcd.print("OFF");
    digitalWrite(relayPin,HIGH); //Switch value depending on the relay used
  }

    digitalRead(Upbutton);
    digitalRead(Downbutton);
    lcd.setCursor(10,1);
    lcd.print("Set:");
    lcd.print(Setvariable);

     if (currentTime - previousTime >= Interval) { //Reads everytime the set Interval is reached
    if (digitalRead(Upbutton) == 0) Setvariable = Setvariable+1; //Setvariable++ (increments the variable)
    if (digitalRead(Downbutton) == 0) Setvariable = Setvariable-1; // Setvariable-- (decrements the variable) 
      previousTime = currentTime;
      }  
  }
