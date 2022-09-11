// Code generated by Arduino IoT Cloud, DO NOT EDIT.

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>


const char THING_ID[]           = "5d928318-b8ea-4419-84ed-b2816314db54";
const char DEVICE_LOGIN_NAME[]  = "5cfc93b8-7323-4015-a644-873692981182";

const char SSID[]               = SECRET_SSID;    // Network SSID (name)
const char PASS[]               = SECRET_PASS;    // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[]  = SECRET_DEVICE_KEY;    // Secret device password

void onSecondNowChange();
void onDateNowChange();
void onHourNowChange();
void onMinuteNowChange();
void onStatusChange();

CloudTime second_now;
String date_now;
CloudTime hour_now;
CloudTime minute_now;
bool status;

void initProperties(){

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.setThingId(THING_ID);
  ArduinoCloud.addProperty(second_now, READWRITE, ON_CHANGE, onSecondNowChange);
  ArduinoCloud.addProperty(date_now, READWRITE, ON_CHANGE, onDateNowChange);
  ArduinoCloud.addProperty(hour_now, READWRITE, ON_CHANGE, onHourNowChange);
  ArduinoCloud.addProperty(minute_now, READWRITE, ON_CHANGE, onMinuteNowChange);
  ArduinoCloud.addProperty(status, READWRITE, ON_CHANGE, onStatusChange);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);