void goodMorning();
void goodAfeternoon();
void goodNight();
void finishDay();

/// Função responsável pelo monitoramento e acionamento da necessidade do aquario
/// basicamente do inicio da rotina ate as 12
void goodMorning() {
  int seconds = convertHoursAndMinutesToSeconds();
  
  if (seconds >= HOUR_GOOD_MORNING_START and seconds <= HOUR_GOOD_MORNING_END) {
    if (REPLACEMENT_PUMP != HIGH) { 
      digitalWrite(REPLACEMENT_PUMP, HIGH);
      Blynk.virtualWrite(V5, 0);
    }

    if (CIRCULATION_PUMP_2 != HIGH) {
      digitalWrite(CIRCULATION_PUMP_2, HIGH);
      Blynk.virtualWrite(V7, 0);
    }

    if (BOSTER_PUMP != LOW) {
      digitalWrite(BOSTER_PUMP, LOW);
      Blynk.virtualWrite(V0, 1);
    }

    if (CIRCULATION_PUMP_1 != LOW) {
      digitalWrite(CIRCULATION_PUMP_1, LOW);
      Blynk.virtualWrite(V6, 1);
    }

    if (TPA_PUMP_1 != HIGH) {
      digitalWrite(TPA_PUMP_1, HIGH);
      Blynk.virtualWrite(V14, 0);
    }

    if (TPA_PUMP_2 != HIGH) {
      digitalWrite(TPA_PUMP_2, HIGH);
      Blynk.virtualWrite(V15, 0);
    }
  }
}

/// Função responsável pelo monitoramento e acionamento da necessidade do aquario
/// basicamente as 12 do inicio da rotina ate as 18
void goodAfeternoon() {
  int seconds = convertHoursAndMinutesToSeconds();

  Serial.println("Periodo da tarde");
  
  if (seconds >= HOUR_GOOD_AFTERNOON_START and seconds <= HOUR_GOOD_AFTERNOON_END) {
    if (REPLACEMENT_PUMP != HIGH) { 
      digitalWrite(REPLACEMENT_PUMP, HIGH);
      Blynk.virtualWrite(V5, 0);
    }

    if (CIRCULATION_PUMP_2 != HIGH) {
      digitalWrite(CIRCULATION_PUMP_2, HIGH);
      Blynk.virtualWrite(V7, 0);
    }

    if (BOSTER_PUMP != LOW) {
      digitalWrite(BOSTER_PUMP, LOW);
      Blynk.virtualWrite(V0, 1);
    }

    if (CIRCULATION_PUMP_1 != LOW) {
      digitalWrite(CIRCULATION_PUMP_1, LOW);
      Blynk.virtualWrite(V6, 1);
    }

    if (TPA_PUMP_1 != HIGH) {
      digitalWrite(TPA_PUMP_1, HIGH);
      Blynk.virtualWrite(V14, 0);
    }

    if (TPA_PUMP_2 != HIGH) {
      digitalWrite(TPA_PUMP_2, HIGH);
      Blynk.virtualWrite(V15, 0);
    }
  }
}

/// Função responsável pelo monitoramento e acionamento da necessidade do aquario
/// basicamente as 18 ate o fim da rotina
void goodNight() {
  int seconds = convertHoursAndMinutesToSeconds();
  
  if (seconds >= HOUR_GOOD_NIGHT_START and seconds <= HOUR_GOOD_NIGHT_END) {
    if (REPLACEMENT_PUMP != HIGH) { 
      digitalWrite(REPLACEMENT_PUMP, HIGH);
      Blynk.virtualWrite(V5, 0);
    }
    
    if (CIRCULATION_PUMP_2 != HIGH) {
      digitalWrite(CIRCULATION_PUMP_2, HIGH);
      Blynk.virtualWrite(V7, 0);
    }

    if (BOSTER_PUMP != LOW) {
      digitalWrite(BOSTER_PUMP, LOW);
      Blynk.virtualWrite(V0, 1);
    }

    if (CIRCULATION_PUMP_1 != LOW) {
      digitalWrite(CIRCULATION_PUMP_1, LOW);
      Blynk.virtualWrite(V6, 1);
    }

    if (TPA_PUMP_1 != HIGH) {
      digitalWrite(TPA_PUMP_1, HIGH);
      Blynk.virtualWrite(V14, 0);
    }

    if (TPA_PUMP_2 != HIGH) {
      digitalWrite(TPA_PUMP_2, HIGH);
      Blynk.virtualWrite(V15, 0);
    }
  }
}

void finishDay() {
  int seconds = convertHoursAndMinutesToSeconds();
  
  if (seconds > HOUR_GOOD_NIGHT_END or seconds < HOUR_GOOD_MORNING_START) {
    if (REPLACEMENT_PUMP != HIGH) { 
      digitalWrite(REPLACEMENT_PUMP, HIGH);
      Blynk.virtualWrite(V5, 0);
    }

    if (CIRCULATION_PUMP_2 != HIGH) {
      digitalWrite(CIRCULATION_PUMP_2, HIGH);
      Blynk.virtualWrite(V7, 0);
    }

    if (BOSTER_PUMP != LOW) {
      digitalWrite(BOSTER_PUMP, LOW);
      Blynk.virtualWrite(V0, 1);
    }

    if (CIRCULATION_PUMP_1 != HIGH) {
      digitalWrite(CIRCULATION_PUMP_1, HIGH);
      Blynk.virtualWrite(V6, 0);
    }

    if (LAMPSUMP != HIGH) {
      digitalWrite(LAMPSUMP, HIGH);
      Blynk.virtualWrite(V13, 0);
    }

    if (TPA_PUMP_1 != HIGH) {
      digitalWrite(TPA_PUMP_1, HIGH);
      Blynk.virtualWrite(V14, 0);
    }

    if (TPA_PUMP_2 != HIGH) {
      digitalWrite(TPA_PUMP_2, HIGH);
      Blynk.virtualWrite(V15, 0);
    }
  }
}
