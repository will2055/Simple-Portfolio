    /******************************************************************************
  CSV_Logger_TinyGPSPlus.ino
  Log GPS data to a CSV file on a uSD card
  By Jim Lindblom @ SparkFun Electronics
  February 9, 2016
  https://github.com/sparkfun/GPS_Shield

  This example uses SoftwareSerial to communicate with the GPS module on
  pins 8 and 9, then communicates over SPI to log that data to a uSD card.

  It uses the TinyGPS++ library to parse the NMEA strings sent by the GPS module,
  and prints interesting GPS information - comma separated - to a newly created
  file on the SD card.

  Resources:
  TinyGPS++ Library  - https://github.com/mikalhart/TinyGPSPlus/releases
  SD Library (Built-in)
  SoftwareSerial Library (Built-in)

  Development/hardware environment specifics:
  Arduino IDE 1.6.7
  GPS Logger Shield v2.0 - Make sure the UART switch is set to SW-UART
  Arduino Uno, RedBoard, Pro, etc.
******************************************************************************/

#include <SPI.h>
#include <SD.h>
#include <TinyGPS++.h>
#include <SparkFun_Si7021_Breakout_Library.h>
#include <Wire.h>

#define ARDUINO_USD_CS 10 
#define LOG_FILE_PREFIX "gpslog"
#define MAX_LOG_FILES 100
#define LOG_FILE_SUFFIX "csv"
char logFileName[13];
#define LOG_COLUMN_COUNT 9
char * log_col_names[LOG_COLUMN_COUNT] = {
  "longitude", "latitude", "altitude", "speed", 
  "course", "date", "time", "satellites", "humidity"
};
#define LOG_RATE 5000
unsigned long lastLog = 0;
TinyGPSPlus tinyGPS;
#define GPS_BAUD 9600
#include <SoftwareSerial.h>
#define ARDUINO_GPS_RX 9
#define ARDUINO_GPS_TX 8
SoftwareSerial ssGPS(ARDUINO_GPS_TX, ARDUINO_GPS_RX);
#define gpsPort ssGPS
#define SerialMonitor Serial

float humidity = 0;

Weather sensor;

void setup()
{
  //sensor.begin();
  Wire.begin();
  SerialMonitor.begin(9600);
  gpsPort.begin(GPS_BAUD);

  SerialMonitor.println("Setting up SD card.");
  if (!SD.begin(ARDUINO_USD_CS))
  {
    SerialMonitor.println("Error initializing SD card.");
  }
  updateFileName();
  printHeader();
}

void loop()
{
  getWeather();
  if ((lastLog + LOG_RATE) <= millis())
  {
    if (tinyGPS.location.isUpdated())
    {
      delay(500);
      if (logGPSData())
      {
        SerialMonitor.println("GPS logged.");
        lastLog = millis();
      }
      else 
      {
        SerialMonitor.println("Failed to log new GPS data.");
      }
    }
    else
    {
      SerialMonitor.print("No GPS data. Sats: ");
      SerialMonitor.println(tinyGPS.satellites.value());
    }
  }

  while (gpsPort.available())
    tinyGPS.encode(gpsPort.read());
}

byte logGPSData()
{
  File logFile = SD.open(logFileName, FILE_WRITE); // Open the log file

  if (logFile)
  { 
    logFile.print(tinyGPS.location.lng(), 6);
    logFile.print(',');
    logFile.print(tinyGPS.location.lat(), 6);
    logFile.print(',');
    logFile.print(tinyGPS.altitude.feet(), 1);
    logFile.print(',');
    logFile.print(tinyGPS.speed.mph(), 1);
    logFile.print(',');
    logFile.print(tinyGPS.course.deg(), 1);
    logFile.print(',');
    logFile.print(tinyGPS.date.value());
    logFile.print(',');
    logFile.print(tinyGPS.time.value());
    logFile.print(',');
    logFile.print(tinyGPS.satellites.value());
    logFile.print(',');
    logFile.print(humidity);
    logFile.println();
    logFile.close();

    return 1;
  }

  return 0;
}

void printHeader()
{
  File logFile = SD.open(logFileName, FILE_WRITE);
  if (logFile) 
  {
    int i = 0;
    for (; i < LOG_COLUMN_COUNT; i++)
    {
      logFile.print(log_col_names[i]);
      if (i < LOG_COLUMN_COUNT - 1)
        logFile.print(',');
      else
        logFile.println();
    }
    logFile.close();
  }
}

void updateFileName()
{
  int i = 0;
  for (; i < MAX_LOG_FILES; i++)
  {
    memset(logFileName, 0, strlen(logFileName));
    sprintf(logFileName, "%s%d.%s", LOG_FILE_PREFIX, i, LOG_FILE_SUFFIX);
    if (!SD.exists(logFileName)) 
    {
      break; 
    }
    else 
    {
      SerialMonitor.print(logFileName);
      SerialMonitor.println(" exists");
    }
  }
  SerialMonitor.print("File name: ");
  SerialMonitor.println(logFileName);
}
void getWeather()
{
  humidity = sensor.getRH();
}
