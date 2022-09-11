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
#include <string>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "MARCOS_PAULO";
char pass[] = "marcos3891";

// Configuração para buscar a data atual
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, -10800);

BlynkTimer timer;

int BOSTER_PUMP = 16;             ///////// D0 - GPIO16 esp8266
int SKIMMER = 5;                  ///////// D1 - GPIO5 esp8266
int REPLACEMENT_PUMP = 4;         ///////// D2 - GPIO4 esp8266
int CIRCULATION_PUMP_1 = 0;       ///////// D3 - GPIO0 esp8266
int CIRCULATION_PUMP_2 = 2;       ///////// D4 - GPIO2 esp8266
int TERMOSTATO_1 = 14;            ///////// D5 - GPIO14 esp8266
int TERMOSTATO_2 = 12;            ///////// D6 - GPIO12 esp8266
int OUTHERS = 13;                   ///////// D7 - GPIO13 esp8266


int FORCED = 0;
int SNACKBYTIMER = 0;
int snack = 0;
int snackCorais = 0;
int hourStopSnack;
int hourStopSnack2;
int hourStopSnack3;
int hourGoodNight;
int hourGoodMorning;
bool skimmer = false;
int tempSkimmer = 0;
int tempSnack = 0;

///Declaração de funções
void goodAfeternoon(int, int);
bool stopSnack();
bool stopSnackCoral();
void goodNight(int, int);
void goodMorning(int, int);
bool finishDay(int, int);
void offAll();
void sendMessageBlynk(String);


//////Variaveis para controlar as mensagens enviadas para o servidor
int forcedButton = 0;
int snackButton = 0;
int infoCirculationPump1 = 0;
int infoCirculationPump2 = 0;
int infoBosterPump = 0;
int snackQtd = 0;
int infoMorning = 0;
int infoAfternoon = 0;
int infoNight = 0;
int infoFinish = 0;
int infoSnackCoral = 0;

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
  if (boster_pump == HIGH) {
    digitalWrite(BOSTER_PUMP, LOW);
  } else {
    digitalWrite(BOSTER_PUMP, HIGH);
  }
}

BLYNK_WRITE(V1)
{
  // Set incoming value from pin V1 to a variable
  int skimmer = param.asInt();
  if (skimmer == HIGH) {
    digitalWrite(SKIMMER, LOW);
  } else {
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
  if (replacement_pump == HIGH) {
    digitalWrite(REPLACEMENT_PUMP, LOW);
  } else {
    digitalWrite(REPLACEMENT_PUMP, HIGH);
  }
}

BLYNK_WRITE(V6)
{
  // Set incoming value from pin V6 to a variable
  int circulation_pump_1 = param.asInt();
  if (circulation_pump_1 == HIGH) {
    digitalWrite(CIRCULATION_PUMP_1, LOW);
  } else {
    digitalWrite(CIRCULATION_PUMP_1, HIGH);
  }
}

BLYNK_WRITE(V7)
{
  // Set incoming value from pin V7 to a variable
  int circulation_pump_2 = param.asInt();
  if (circulation_pump_2 == HIGH) {
    digitalWrite(CIRCULATION_PUMP_2, LOW);
  } else {
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

BLYNK_WRITE(V13)
{
  // Set incoming value from pin V0 to a variable
  int termo_1 = param.asInt();
  if (termo_1 == HIGH) {
    digitalWrite(TERMOSTATO_1, LOW);
  } else {
    digitalWrite(TERMOSTATO_1, HIGH);
  }
}

BLYNK_WRITE(V14)
{
  // Set incoming value from pin V0 to a variable
  int termo_2 = param.asInt();
  if (termo_2 == HIGH) {
    digitalWrite(TERMOSTATO_2, LOW);
  } else {
    digitalWrite(TERMOSTATO_2, HIGH);
  }
}

BLYNK_WRITE(V15)
{
  // Set incoming value from pin V0 to a variable
  int outhers = param.asInt();
  if (outhers == HIGH) {
    digitalWrite(OUTHERS, LOW);
  } else {
    digitalWrite(OUTHERS, HIGH);
  }
}

BLYNK_WRITE(V16)
{
  // Set incoming value from pin V4 to a variable
  String msg = param.asStr();
  if (msg == "clear") {
    Blynk.virtualWrite(V16, "clr");
  }
}

BLYNK_WRITE(V17)
{
  // Set incoming value from pin V17 to a variable
  SNACKBYTIMER = param.asInt();
}

BLYNK_WRITE(V18)
{
  // Set incoming value from pin V17 to a variable
  snackCorais = param.asInt();
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  sendMessageBlynk("Dispositivo Conectado");
  sendMessageBlynk("Iniciando aplicação...");

  Blynk.virtualWrite(V4, 0);
  Blynk.virtualWrite(V12, 0);

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

  pinMode(TERMOSTATO_1, OUTPUT);
  digitalWrite(TERMOSTATO_1, LOW);
  Blynk.virtualWrite(V13, 1);

  pinMode(TERMOSTATO_2, OUTPUT);
  digitalWrite(TERMOSTATO_2, LOW);
  Blynk.virtualWrite(V14, 1);

  pinMode(OUTHERS, OUTPUT);
  digitalWrite(OUTHERS, LOW);
  Blynk.virtualWrite(V15, 1);

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
  Serial.print("Hora ");
  Serial.print(hour);
  Serial.print(":");
  Serial.println(minute);

  Blynk.syncVirtual(V8, V17, V11, V18);
  hourStopSnack2 = hourStopSnack + 5;
  hourStopSnack3 = hourStopSnack2 + 5;
  if (FORCED == 0 and snack == 0 and snackCorais == 0) {
    if ((hour != hourStopSnack and hour != hourStopSnack2 and hour != hourStopSnack3) or SNACKBYTIMER == 0 or snackQtd > 0) {
      if (!finishDay(hour, minute)) {
        goodMorning(hour, minute);

        goodAfeternoon(hour, minute);

        goodNight(hour, minute);
      }

      if (hour != hourStopSnack and hour != hourStopSnack2 and hour != hourStopSnack3) {
        snackQtd = 0;
      }

    } else if (SNACKBYTIMER == 1 and snackQtd == 0) {
      Blynk.virtualWrite(V12, 1);

      stopSnack();
    }

    if (forcedButton > 0) {
      sendMessageBlynk("Info: Botão FORCAR foi desacionado.");
      forcedButton = 0;
    }

    forcedButton = false;
  } else {
    if (snack != 0) {
      Serial.println("Botão refeição acionado...");

      Blynk.virtualWrite(V12, 1);
      stopSnack();
    } else if (snackCorais != 0) {
      Serial.println("Botão refeição para corais acionado...");

      Blynk.virtualWrite(V18, 1);
      stopSnackCoral();
    } else {
      Serial.println("Botão forçar acionado");
      if (forcedButton == 0) {
        sendMessageBlynk("Warning: Botão FORCAR foi acionado.");
        forcedButton++;
      }
    }

    skimmer = false;
  }

  getDateNow();

  delay(1000);
}

void goodMorning(int hour = 0, int minute = 10) {

  if (hour >= hourGoodMorning and hour < 12) {
    if (infoMorning == 0) {
      sendMessageBlynk("Info: Monitorando periodo da manha...");
      infoMorning++;
    }

    Serial.println("Periodo diurno...");

    digitalWrite(REPLACEMENT_PUMP, LOW);
    Blynk.virtualWrite(V5, 1);

    digitalWrite(CIRCULATION_PUMP_2, HIGH);
    Blynk.virtualWrite(V7, 0);

    digitalWrite(BOSTER_PUMP, LOW);
    Blynk.virtualWrite(V0, 1);

    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);

    digitalWrite(OUTHERS, LOW);
    Blynk.virtualWrite(V15, 1);

    onSkimmer();
  }
}

void goodAfeternoon(int hour = 0, int minute = 10) {

  if (hour >= 12 and hour < 18) {
    if (infoAfternoon == 0) {
      sendMessageBlynk("Info: Monitorando periodo da tarde...");
      infoAfternoon++;
    }

    Serial.println("Periodo vespertino...");

    digitalWrite(REPLACEMENT_PUMP, LOW);
    Blynk.virtualWrite(V5, 1);

    digitalWrite(CIRCULATION_PUMP_2, HIGH);
    Blynk.virtualWrite(V7, 0);

    digitalWrite(BOSTER_PUMP, LOW);
    Blynk.virtualWrite(V0, 1);

    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);

    digitalWrite(OUTHERS, LOW);
    Blynk.virtualWrite(V15, 1);

    onSkimmer();
  }
}

void goodNight(int hour = 0, int minute = 10) {
  if ((hour >= 18 and hour < hourGoodNight)) {
    if (infoNight == 0) {
      sendMessageBlynk("Info: Monitorando periodo da noite...");
      infoNight++;
    }

    Serial.println("Periodo noturno...");

    digitalWrite(CIRCULATION_PUMP_2, HIGH);
    Blynk.virtualWrite(V7, 0);

    digitalWrite(BOSTER_PUMP, LOW);
    Blynk.virtualWrite(V0, 1);

    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);

    digitalWrite(REPLACEMENT_PUMP, LOW);
    Blynk.virtualWrite(V5, 1);

    digitalWrite(OUTHERS, HIGH);
    Blynk.virtualWrite(V15, 0);

    onSkimmer();
  }
}

bool finishDay(int hour = 0, int minute = 10) {
  if ((hour >= hourGoodNight and hour < 24) or (hour < hourGoodMorning and hour >= 0)) {
    if (infoFinish == 0) {
      sendMessageBlynk("Info: Periodo de desligamento...");
      infoFinish++;
    }

    Serial.println("Periodo de desligamento...");

    digitalWrite(CIRCULATION_PUMP_1, HIGH);
    Blynk.virtualWrite(V6, 0);

    digitalWrite(CIRCULATION_PUMP_2, HIGH);
    Blynk.virtualWrite(V7, 0);

    digitalWrite(BOSTER_PUMP, LOW);
    Blynk.virtualWrite(V0, 1);

    digitalWrite(REPLACEMENT_PUMP, LOW);
    Blynk.virtualWrite(V5, 1);

    digitalWrite(OUTHERS, HIGH);
    Blynk.virtualWrite(V15, 0);

    onSkimmer();

    return true;
  } else {
    return false;
  }
}

bool stopSnack() {
  if (snackButton == 0) {
    sendMessageBlynk("Info: Iniciando parada para REFEICAO.");
    snackButton++;
  }

  digitalWrite(SKIMMER, HIGH);
  Blynk.virtualWrite(V1, 0);

  if (tempSnack <= 180) { ///// 3 minutos de paralização
    Serial.println("Periodo de refeição...");
    offAll();

    tempSnack++;
    Serial.println(tempSnack);

    return true;
  }

  if (tempSnack > 180 and tempSnack <= 210) { ///// apartir dos 3 minutos liga a bomba de circulacao 1
    Serial.println("Ligando bomba de circulação 1");

    if (infoCirculationPump1 == 0) {
      sendMessageBlynk("Info: Ligando Bomba de Circulacao de numero 1 (circulacao)...");
      infoCirculationPump1++;
    }

    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);

    tempSnack++;
    Serial.println(tempSnack);

    return true;
  }

  if (tempSnack > 210 and tempSnack <= 260) {   ///// a partir de 3,50 minutos e ligada a bomba de circulacao 2
    Serial.println("Ligando bomba de circulação 1 e 2");
    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);

    if (infoCirculationPump2 == 0) {
      sendMessageBlynk("Info: Ligando Bomba de Circulacao de numero 2 (agitacao)...");
      infoCirculationPump2++;
    }

    digitalWrite(CIRCULATION_PUMP_2, LOW);
    Blynk.virtualWrite(V7, 1);

    tempSnack++;
    Serial.println(tempSnack);

    return true;
  }

  if (tempSnack > 260 and tempSnack <= 320) { /////// a partir dos 4,30 minutos e ligada a bomba de recalque
    Serial.println("Ligando bomba de circulação 1, 2 e recalque");
    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);

    digitalWrite(CIRCULATION_PUMP_2, LOW);
    Blynk.virtualWrite(V7, 1);

    if (infoBosterPump == 0) {
      sendMessageBlynk("Info: Ligando Bomba de recalque...");
      infoBosterPump++;
    }

    digitalWrite(BOSTER_PUMP, LOW);
    Blynk.virtualWrite(V0, 1);

    tempSnack++;
    Serial.println(tempSnack);

    return true;
  }

  if (tempSnack > 320 and tempSnack <= 330) { ///// apos os 5,30 minutos e desligada a bomba de circulacao 2 agitacao
    Serial.println("Desligando bomba de circulação 2");
    digitalWrite(CIRCULATION_PUMP_2, HIGH);
    Blynk.virtualWrite(V7, 0);

    tempSnack++;
    Serial.println(tempSnack);

    return true;
  }

  Serial.println("Finalizando periodo de alimentação...");

  if (snackButton > 0) {
    sendMessageBlynk("Info: Final da parada para REFEICAO.");
    snackButton = 0;
  }

  snack = 0;
  Blynk.virtualWrite(V12, 0);
  tempSnack = 0;
  Serial.println(tempSnack);

  infoCirculationPump1 = 0;
  infoCirculationPump2 = 0;
  infoBosterPump = 0;
  snackQtd++;

  return false;
}

bool stopSnackCoral() {
  if (infoSnackCoral == 0) {
    sendMessageBlynk("Info: Iniciando parada para REFEICAO DOS CORAIS.");
    infoSnackCoral++;
  }

  digitalWrite(SKIMMER, HIGH);
  Blynk.virtualWrite(V1, 0);

  if (tempSnack <= 180) { ///// tempo de repouso
    Serial.println("Periodo de refeição...");
    offAll();

    tempSnack++;
    Serial.println(tempSnack);

    return true;
  }

  if (tempSnack > 180 and tempSnack <= 300) { //// a partir dos 3 minutos é ligada a bomba de circulacao
    Serial.println("Ligando bomba de circulação 1");

    if (infoCirculationPump1 == 0) {
      sendMessageBlynk("Info: Ligando Bomba de Circulacao de numero 1 (circulacao)...");
      infoCirculationPump1++;
    }

    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);

    tempSnack++;
    Serial.println(tempSnack);

    return true;
  }

  if (tempSnack > 300 and tempSnack <= 360) {   ///// a partir dos 5 minutos e ligada a bomba de circulacao 2
    Serial.println("Ligando bomba de circulação 1 e 2");
    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);

    if (infoCirculationPump2 == 0) {
      sendMessageBlynk("Info: Ligando Bomba de Circulacao de numero 2 (agitacao)...");
      infoCirculationPump2++;
    }

    digitalWrite(CIRCULATION_PUMP_2, LOW);
    Blynk.virtualWrite(V7, 1);

    tempSnack++;
    Serial.println(tempSnack);

    return true;
  }

  if (tempSnack > 360 and tempSnack <= 380) { /////// a partir dos 6 minutos e ligada a bomba de recalque
    Serial.println("Ligando bomba de circulação 1, 2 e recalque");
    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);

    digitalWrite(CIRCULATION_PUMP_2, LOW);
    Blynk.virtualWrite(V7, 1);

    if (infoBosterPump == 0) {
      sendMessageBlynk("Info: Ligando Bomba de recalque...");
      infoBosterPump++;
    }

    digitalWrite(BOSTER_PUMP, LOW);
    Blynk.virtualWrite(V0, 1);

    tempSnack++;
    Serial.println(tempSnack);

    return true;
  }

  if (tempSnack > 380 and tempSnack <= 1800) { ///// a partir dos 6,30 minutos e desligada a bomba de circulacao 2
    Serial.println("Desligando bomba de circulação 2");
    digitalWrite(CIRCULATION_PUMP_2, HIGH);
    Blynk.virtualWrite(V7, 0);

    tempSnack++;
    Serial.println(tempSnack);

    return true;
  }

  Serial.println("Finalizando periodo de alimentação...");

  if (infoSnackCoral > 0) {
    sendMessageBlynk("Info: Final da parada para REFEICAO DOS CORAIS.");
    infoSnackCoral = 0;
  }

  snackCorais = 0;
  Blynk.virtualWrite(V18, 0);
  tempSnack = 0;
  Serial.println(tempSnack);

  infoCirculationPump1 = 0;
  infoCirculationPump2 = 0;
  infoBosterPump = 0;
  snackQtd++;

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

  double currentYear = ptm->tm_year + 1900;

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

void sendMessageBlynk(String msg) {
  int hour = timeClient.getHours();
  int minute = timeClient.getMinutes();

  String msgComplete = "Horario: ";
  msgComplete.concat(String(hour));
  msgComplete.concat(":");
  msgComplete.concat(String(minute));
  msgComplete.concat(" - ");
  msgComplete.concat(msg);

  Blynk.virtualWrite(V16, msgComplete);
}

Device ID
5cfc93b8-7323-4015-a644-873692981182
Secret Key
LI2X1ZGTO2U7TOWRUVCN