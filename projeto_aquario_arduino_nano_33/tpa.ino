/// Função responsável pelo reabastecimento de agua doce
void replenishmentTime(int minute) {
  
  if (minute <= 10) {
    Serial.println("maquinas desligadas para reabastecimento");

    replenishment = false;

    digitalWrite(BOSTER_PUMP, HIGH);
    Blynk.virtualWrite(V0, 0);
  
    digitalWrite(SKIMMER, HIGH);
    Blynk.virtualWrite(V1, 0);
  
    digitalWrite(CIRCULATION_PUMP_1, HIGH);
    Blynk.virtualWrite(V6, 0);
  
    digitalWrite(CIRCULATION_PUMP_2, HIGH);
    Blynk.virtualWrite(V7, 0);
  
    digitalWrite(TPA_PUMP_1, HIGH);
    Blynk.virtualWrite(V14, 0);
    
    digitalWrite(TPA_PUMP_2, HIGH);
    Blynk.virtualWrite(V15, 0);
    
    if (minute > 5 and minute <= 10) {
      Serial.println("reabastecimento de agua ativado");
      digitalWrite(REPLACEMENT_PUMP, LOW);
      Blynk.virtualWrite(V5, 1);
    } else {
      digitalWrite(REPLACEMENT_PUMP, HIGH);
      Blynk.virtualWrite(V5, 0);
    }

    return;
  }

   Serial.println("Finalizado reabastecimento de agua");
   digitalWrite(REPLACEMENT_PUMP, HIGH);
   Blynk.virtualWrite(V5, 0);
   replenishment = true;

   return;
}

bool tpaDaily() {
  int seconds = convertHoursAndMinutesToSeconds();
  bool execute = false;
  
  if (TIMER_TPA_DAILY == seconds && ACTION_TPA == 3) {
    INIT_TPA = 1;
    ACTION_TPA = -1;
    Blynk.virtualWrite(V38, 1);
    timeProcess1 = seconds + (TIME_OFF_MACHINE * 60);
    timeProcess2 = timeProcess1 + (TIME_OFF_MACHINE * 60);
  }

  // Serial.println(digitalRead(TPA_SENSOR_1));
  // Serial.println(digitalRead(TPA_SENSOR_2));
  bool sensor1 = digitalRead(TPA_SENSOR_1);
  bool sensor2 = digitalRead(TPA_SENSOR_2);

  switch (ACTION_TPA) {
    case -1: { // Item 1
      if(timeProcess1 > seconds) {
        Serial.println("Parando maquinario...");
        offAll();
        return true;
      }

      if(timeProcess2 > seconds) {
        Serial.println("Reabastecimento agua doce");
        digitalWrite(REPLACEMENT_PUMP, LOW);
        Blynk.virtualWrite(V5, 1);
        return true;
      }

      offAll();
      ACTION_TPA = 0;
      Blynk.virtualWrite(V39, 0);
      execute = true;
      break;
    }
    case 0: { // Item 1
      Serial.println("Remover a agua...");

      if (BOSTER_PUMP == LOW){
        offAll();
        timeProcess1 = (TIME_OFF_MACHINE * 60000);
        delay(timeProcess1);
      }

      if (sensor2 == LOW) {
        digitalWrite(TPA_PUMP_1, LOW);
        Blynk.virtualWrite(V14, 1);
        return true;
      }

      offAll();
      ACTION_TPA = 1;
      Blynk.virtualWrite(V39, 1);
      execute = true;
      break;
    }
    case 1: { // Item 2
      Serial.println("adicionando a agua...");
      if (BOSTER_PUMP == LOW){
        offAll();
        timeProcess1 = (TIME_OFF_MACHINE * 60000);
        delay(timeProcess1);
      }

      if (sensor1 == HIGH || sensor2 == HIGH) {
        digitalWrite(TPA_PUMP_2, LOW);
        Blynk.virtualWrite(V15, 1);
        return true;
      }
      
      offAll();
      ACTION_TPA = 2;
      Blynk.virtualWrite(V39, 2);
      execute = true;
      break;
    }
    case 2: { // Item 2
      Serial.println("Apaga tudo");

      INIT_TPA = 0;
      ACTION_TPA = 3;
      Blynk.virtualWrite(V38, 0);
      Blynk.virtualWrite(V39, 3);
      execute = false;
      break;
    }
    default: {
      INIT_TPA = 0;
      ACTION_TPA = 3;
      Blynk.virtualWrite(V38, 0);
      Blynk.virtualWrite(V39, 3);
      execute = false;
      break;
    }
  }

  return execute;
}
