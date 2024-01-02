/**
 * @file main.cpp
 * @author Marcos Paulo de Sousa Santos (marcospaulo.s.s@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-12-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <clsPCA9555.h>
#include <Wire.h>

/**
 * Responsável pelas configurações de acesso ao despositivo do blynk
 */

//#define BLYNK_TEMPLATE_ID           "TMPLCJQxjDZK"
//#define BLYNK_DEVICE_NAME           "Quickstart Device"
//#define BLYNK_AUTH_TOKEN            "hOR-xlN_RNt8QI4EtHkVIyovdMnVXR9f"

/////////DEVICE PARA BACKUP
#define BLYNK_TEMPLATE_ID "TMPLCJQxjDZK"
#define BLYNK_DEVICE_NAME "Aquario Marinho Backup"
#define BLYNK_AUTH_TOKEN "42VhEtszQJOXa5giIkgwF5ahl6jXYjmD"
#define BLYNK_PRINT Serial

///Responsavel pelo modulo de espançor de portas
PCA9555 ioport(0x20);
const byte interruptPin = 0;
volatile bool houveInterrupcao = false;

/**
 * @brief setup inicial da aplicação
 * 
 */
void setup() {
  Wire.begin();

  Serial.begin(115200);

  ioport.begin();
  ioport.setClock(400000);
  
  // for (uint8_t i = 0; i < 1; i++){
  //     ioport.pinMode(i, OUTPUT);
  // }

  ioport.pinMode(ED0, OUTPUT);
  
}


/**
 * @brief codigo principal da aplicação
 * 
 */
void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Iniciando aplicação");

  ioport.digitalWrite(ED0, LOW);
	delay(1000);
}