/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPLCJQxjDZK"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "hOR-xlN_RNt8QI4EtHkVIyovdMnVXR9f"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "***";
char pass[] = "***";

// Configuração para buscar a data atual
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, -10800);

BlynkTimer timer;

int BOSTER_PUMP = 16;
int SKIMMER = 5;
int REPLACEMENT_PUMP = 4;
int CIRCULATION_PUMP_1 = 3;
int CIRCULATION_PUMP_2 = 2;

int FORCED = 0;
int snack = 0;
int hourStopSnack = 12;
int hourGoodNight = 20;
int hourGoodMorning = 8;
bool skimmer = false;
int tempSkimmer = 0;
int tempSnack = 0;

void goodAfeternoon(int, int);
bool stopSnack();
void goodNight(int, int);
void goodMorning(int, int);
bool finishDay(int, int);
void offAll();

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");

  Blynk.syncAll();
}

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int boster_pump = param.asInt();
  if(boster_pump == HIGH) {
    digitalWrite(BOSTER_PUMP, LOW);
  }else {
    digitalWrite(BOSTER_PUMP, HIGH);
  }
}

BLYNK_WRITE(V1)
{
  // Set incoming value from pin V1 to a variable
  int skimmer = param.asInt();
  if(skimmer == HIGH) {
    digitalWrite(SKIMMER, LOW);
  }else {
    digitalWrite(SKIMMER, HIGH);
  }
}

BLYNK_WRITE(V4)
{
  // Set incoming value from pin V4 to a variable
  FORCED = param.asInt();
}

BLYNK_WRITE(V5)
{
  // Set incoming value from pin V5 to a variable
  int replacement_pump = param.asInt();
  if(replacement_pump == HIGH) {
    digitalWrite(REPLACEMENT_PUMP, LOW);
  }else {
    digitalWrite(REPLACEMENT_PUMP, HIGH);
  }
}

BLYNK_WRITE(V6)
{
  // Set incoming value from pin V6 to a variable
  int circulation_pump_1 = param.asInt();
  if(circulation_pump_1 == HIGH) {
    digitalWrite(CIRCULATION_PUMP_1, LOW);
  }else {
    digitalWrite(CIRCULATION_PUMP_1, HIGH);
  }
}

BLYNK_WRITE(V7)
{
  // Set incoming value from pin V7 to a variable
  int circulation_pump_2 = param.asInt();
  if(circulation_pump_2 == HIGH) {
    digitalWrite(CIRCULATION_PUMP_2, LOW);
  }else {
    digitalWrite(CIRCULATION_PUMP_2, HIGH);
  }
}

BLYNK_WRITE(V8)
{
  // Set incoming value from pin V8 to a variable
  hourStopSnack = param.asInt();
}

BLYNK_WRITE(V10)
{
  // Set incoming value from pin V10 to a variable
  hourGoodNight = param.asInt();
}

BLYNK_WRITE(V11)
{
  // Set incoming value from pin V11 to a variable
  hourGoodMorning = param.asInt();
}

BLYNK_WRITE(V12)
{
  // Set incoming value from pin V4 to a variable
  snack = param.asInt();
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  pinMode(BOSTER_PUMP, OUTPUT);
  digitalWrite(BOSTER_PUMP, HIGH);
  Blynk.virtualWrite(V0, 0);

  pinMode(CIRCULATION_PUMP_1, OUTPUT);
  digitalWrite(CIRCULATION_PUMP_1, HIGH);
  Blynk.virtualWrite(V6, 0);

  pinMode(SKIMMER, OUTPUT);
  digitalWrite(SKIMMER, HIGH);
  Blynk.virtualWrite(V1, 0);

  pinMode(REPLACEMENT_PUMP, OUTPUT);
  digitalWrite(REPLACEMENT_PUMP, HIGH);
  Blynk.virtualWrite(V5, 0);

  pinMode(CIRCULATION_PUMP_2, OUTPUT);
  digitalWrite(CIRCULATION_PUMP_2, HIGH);
  Blynk.virtualWrite(V7, 0);

  tempSnack = 0;

  timeClient.begin();
}

void loop()
{
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!

  timeClient.update();

  int hour = timeClient.getHours();
  int minute = timeClient.getMinutes();
  Serial.println(hour);
  Serial.println(minute);

  if (FORCED == 0 and snack == 0){
    if (hour != hourStopSnack) {
      if (!finishDay(hour, minute)) {
        goodMorning(hour, minute);
      
        goodAfeternoon(hour, minute);
      
        goodNight(hour, minute);
      } 
    } else {
      Blynk.virtualWrite(V12, 1);
      stopSnack();
    }
  } else {
    if (snack != 0) {
      Serial.println("Botão refeição acionado");
      Blynk.virtualWrite(V12, 1);
      stopSnack();
    } else {
      Serial.println("Botão forçar acionado");
    }
    
    skimmer = false;
  }

  getDateNow();
  
  delay(1000);
}

void goodMorning(int hour = 0, int minute = 10) {
  if(hour >= hourGoodMorning and hour < 12) {
    Serial.println("Periodo diurno...");

    digitalWrite(SKIMMER, HIGH);
    Blynk.virtualWrite(V1, 0); 

    digitalWrite(REPLACEMENT_PUMP, HIGH);
    Blynk.virtualWrite(V5, 0);

    digitalWrite(CIRCULATION_PUMP_2, HIGH);
    Blynk.virtualWrite(V7, 0);

    digitalWrite(BOSTER_PUMP, LOW);
    Blynk.virtualWrite(V0, 1);

    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);
  }
}

void goodAfeternoon(int hour = 0, int minute = 10) {
  if(hour >= 12 and hour < 18) {
    Serial.println("Periodo vespertino...");

    digitalWrite(SKIMMER, HIGH);
    Blynk.virtualWrite(V1, 0); 

    digitalWrite(REPLACEMENT_PUMP, HIGH);
    Blynk.virtualWrite(V5, 0);

    digitalWrite(CIRCULATION_PUMP_2, HIGH);
    Blynk.virtualWrite(V7, 0);

    digitalWrite(BOSTER_PUMP, LOW);
    Blynk.virtualWrite(V0, 1);

    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);
  }
}

void goodNight(int hour = 0, int minute = 10) {
  if((hour >= 18 and hour < hourGoodNight)) {
    Serial.println("Periodo noturno...");

    digitalWrite(SKIMMER, HIGH);
    Blynk.virtualWrite(V1, 0);

    digitalWrite(CIRCULATION_PUMP_2, HIGH);
    Blynk.virtualWrite(V7, 0);

    digitalWrite(BOSTER_PUMP, LOW);
    Blynk.virtualWrite(V0, 1);

    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);

    digitalWrite(REPLACEMENT_PUMP, LOW);
    Blynk.virtualWrite(V5, 1);
  }
}

bool finishDay(int hour = 0, int minute = 10) {
  if((hour >= hourGoodNight and hour < 24) or (hour < hourGoodMorning and hour >= 0)) {
    Serial.println("Periodo de desligamento...");

    digitalWrite(CIRCULATION_PUMP_1, HIGH);
    Blynk.virtualWrite(V6, 0);
    
    digitalWrite(CIRCULATION_PUMP_2, HIGH);
    Blynk.virtualWrite(V7, 0);

    digitalWrite(BOSTER_PUMP, LOW);
    Blynk.virtualWrite(V0, 1);

    digitalWrite(REPLACEMENT_PUMP, LOW);
    Blynk.virtualWrite(V5, 1);
    
    onSkimmer();

    return true;
  } else {
    return false;
  }
}

bool stopSnack() {
  digitalWrite(SKIMMER, HIGH);
  Blynk.virtualWrite(V1, 0); 
    
  if(tempSnack <= 1000) {
    Serial.println("Periodo de refeição...");
    offAll();
    
    tempSnack++;
    Serial.println(tempSnack);

    return true;
  } 

  if(tempSnack > 1000 and tempSnack <= 1200) {
    Serial.println("Ligando bomba de circulação 1");
    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);
    
    tempSnack++;
    Serial.println(tempSnack);

    return true;
  }

  if(tempSnack > 1200 and tempSnack <= 1400) {
    Serial.println("Ligando bomba de circulação 1 e 2");
    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);
    
    digitalWrite(CIRCULATION_PUMP_2, LOW);
    Blynk.virtualWrite(V7, 1);

    tempSnack++;
    Serial.println(tempSnack);
    
    return true;
  }
  
  if(tempSnack > 1400 and tempSnack <= 1600) {
    Serial.println("Ligando bomba de circulação 1, 2 e recalque");
    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);
    
    digitalWrite(CIRCULATION_PUMP_2, LOW);
    Blynk.virtualWrite(V7, 1);
    
    digitalWrite(BOSTER_PUMP, LOW); 
    Blynk.virtualWrite(V0, 1);

    tempSnack++;
    Serial.println(tempSnack);
    
    return true;
  }

  if(tempSnack > 1600 and tempSnack <= 1800) {
    Serial.println("Desligando bomba de circulação 2");
    digitalWrite(CIRCULATION_PUMP_2, HIGH);
    Blynk.virtualWrite(V7, 0);

    tempSnack++;
    Serial.println(tempSnack);
    
    return true;
  }

  Serial.println("Finalizando periodo de alimentação...");

  snack = 0;
  Blynk.virtualWrite(V12, 0);
  tempSnack = 0;
  Serial.println(tempSnack);

  return false;
}

void getDateNow()  {
  char daysOfTheWeek[7][14] = {"Domingo", "Segunda Feira", "Terca Feira", "Quarta Feira", "Quinta Feira", "Sexta Feira", "Sabado"};
  String months[12] = {"Janeiro", "Fevereiro", "Marco", "Abril", "Maio", "Junho", "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"};

  unsigned long epochTime = timeClient.getEpochTime();

  struct tm *ptm = gmtime ((time_t *)&epochTime);
  
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon;
  String currentMonthName = months[currentMonth - 1];
  
  double currentYear = ptm->tm_year+1900;

  String semana = daysOfTheWeek[timeClient.getDay()];

  int day = timeClient.getDay();
  
  String currentDate = semana + " - " + String(day) + "/" + String(currentMonth);
  Blynk.virtualWrite(V2, currentDate);

  int hour_now = timeClient.getHours();
  int minute_now = timeClient.getMinutes();
  int second_now = timeClient.getSeconds();
  String currentHour = String(hour_now) + ":" + String(minute_now) + ":" + String(second_now);
  Blynk.virtualWrite(V3, currentHour);
}

void onSkimmer() {
  Serial.println(tempSkimmer);
  if (skimmer == false and tempSkimmer < 200) {
    digitalWrite(SKIMMER, HIGH);
    Blynk.virtualWrite(V1, 0); 
    tempSkimmer++;
  } else {
    Serial.println(tempSkimmer);
    skimmer = true;
    digitalWrite(SKIMMER, LOW);
    Blynk.virtualWrite(V1, 1); 
    tempSkimmer = 0;
  }
}

void offAll() {
  digitalWrite(BOSTER_PUMP, HIGH);
   Blynk.virtualWrite(V0, 0);

   digitalWrite(SKIMMER, HIGH);
   Blynk.virtualWrite(V1, 0);
    
   digitalWrite(REPLACEMENT_PUMP, HIGH);
   Blynk.virtualWrite(V5, 0);
    
   digitalWrite(CIRCULATION_PUMP_1, HIGH);
   Blynk.virtualWrite(V6, 0);
    
   digitalWrite(CIRCULATION_PUMP_2, HIGH);
   Blynk.virtualWrite(V7, 0);
}
