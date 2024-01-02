/// Variaveis de tempo
int timerSnack;
float gaugeRetCirculacao1;
float gaugeRetCirculacao2;
float gaugeRetRecalque;
float gaugeDeslCirculacao2;
float gaugeFinaliza;
bool callFunctionFirst = false;

/// Declaração das funcionalidades
void offAll();
void fishSnack();
void setupSnack();
void coralSnack();

/// Desliga todas as maquinas
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

  digitalWrite(TPA_PUMP_1, HIGH);
  Blynk.virtualWrite(V14, 0);
  
  digitalWrite(TPA_PUMP_2, HIGH);
  Blynk.virtualWrite(V15, 0);

  timerGouleSkimmer = 0;
  initSkimmer();
}

void setupSnack() {
  Blynk.syncVirtual(V9, V22, V24, V26, V28, V32, V33, V34, V35, V36);
  
  Blynk.virtualWrite(V21, 0);
  Blynk.virtualWrite(V23, 0);
  Blynk.virtualWrite(V25, 0);
  Blynk.virtualWrite(V27, 0);
  Blynk.virtualWrite(V29, 0);

  gaugeRetCirculacao1 = 0;
  gaugeRetCirculacao2 = 0;
  gaugeRetRecalque = 0;
  gaugeDeslCirculacao2 = 0;
  gaugeFinaliza = 0;
  timerSnack = 0;
}

/// Inicio da alimentação dos peixes
void fishSnack() {
  if (!callFunctionFirst) {
    offAll();
    setupSnack();
    callFunctionFirst = true;
  }

  float circulation1Time = (RETURN_CIRCULATION_1 * 60);
  if (timerSnack <= circulation1Time) {
    Serial.println("Iniciando periodo de refeição");
    gaugeRetCirculacao1 += (circulation1Time <= 0) ? 1 : (100 / circulation1Time);
    Blynk.virtualWrite(V21, gaugeRetCirculacao1);

    timerSnack++;
    
    return;
  }

  float circulation2Time = (RETURN_CIRCULATION_2 * 60) + circulation1Time;
  if (timerSnack > circulation1Time and timerSnack <= circulation2Time) { ///// apartir dos 3 minutos liga a bomba de circulacao 1
    Serial.println("Ligando bomba de circulação 1");

    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);

    gaugeRetCirculacao2 += (float(100) / (float(RETURN_CIRCULATION_2) * float(60)));
    Blynk.virtualWrite(V23, gaugeRetCirculacao2);

    timerSnack++;

    return;
  }

  float recalqueTime = (RETURN_BOSTER_PUMP * 60) + circulation2Time;
  if (timerSnack > circulation2Time and timerSnack <= recalqueTime) {   ///// a partir de 3,50 minutos e ligada a bomba de circulacao 2
    Serial.println("Ligando bomba de circulação 1 e 2");
    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);

    digitalWrite(CIRCULATION_PUMP_2, LOW);
    Blynk.virtualWrite(V7, 1);

    gaugeRetRecalque += (float(100) / (float(RETURN_BOSTER_PUMP) * float(60)));
    Blynk.virtualWrite(V25, gaugeRetRecalque);

    timerSnack++;

    return;
  }

  float offCirculation2TIme = (FINISH_CIRCULATION_2 * 60) + recalqueTime;
  if (timerSnack > recalqueTime and timerSnack <= offCirculation2TIme) { /////// a partir dos 4,30 minutos e ligada a bomba de recalque
    Serial.println("Ligando bomba de circulação 1, 2 e recalque");
    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);

    digitalWrite(CIRCULATION_PUMP_2, LOW);
    Blynk.virtualWrite(V7, 1);

    digitalWrite(BOSTER_PUMP, LOW);
    Blynk.virtualWrite(V0, 1);

    gaugeDeslCirculacao2 += (float(100) / (float(FINISH_CIRCULATION_2) * float(60)));
    Blynk.virtualWrite(V27, gaugeDeslCirculacao2);

    timerSnack++;

    return;
  }

  float finalizaRefeicao = (FINISH_SNACK * 60) + offCirculation2TIme;
  if (timerSnack > offCirculation2TIme and timerSnack <= finalizaRefeicao) { ///// apos os 5,30 minutos e desligada a bomba de circulacao 2 agitacao
    Serial.println("Desligando bomba de circulação 2");
    digitalWrite(CIRCULATION_PUMP_2, HIGH);
    Blynk.virtualWrite(V7, 0);

    gaugeFinaliza += (float(100) / (float(FINISH_SNACK) * float(60)));
    Blynk.virtualWrite(V29, gaugeFinaliza);

    timerSnack++;

    return;
  }

  Serial.println("Finalizando periodo de alimentação...");
  Blynk.virtualWrite(V12, 0);
  SNACK = 0;
  timerSnack = 0;
  snackQtd = true;
  callFunctionFirst = false;
}

void coralSnack() {
  if (!callFunctionFirst) {
    offAll();
    setupSnack();
    callFunctionFirst = true;
  }

  float circulation1Time = (RETURN_CORAL_CIRCULATION_1 * 60);
  if (timerSnack <= circulation1Time) {
    Serial.println("Iniciando periodo de refeição");
    gaugeRetCirculacao1 += (RETURN_CORAL_CIRCULATION_1 <= 0) ? 1 : (100 / circulation1Time);
    Blynk.virtualWrite(V21, gaugeRetCirculacao1);

    timerSnack++;
    return;
  }

  float circulation2Time = (RETURN_CORAL_CIRCULATION_2 * 60) + circulation1Time;
  if (timerSnack > circulation1Time and timerSnack <= circulation2Time) { ///// apartir dos 3 minutos liga a bomba de circulacao 1
    Serial.println("Ligando bomba de circulação 1");

    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);

    gaugeRetCirculacao2 += (float(100) / (float(RETURN_CORAL_CIRCULATION_2) * float(60)));
    Blynk.virtualWrite(V23, gaugeRetCirculacao2);

    timerSnack++;

    return;
  }

  float recalqueTime = (RETURN_CORAL_RECALQUE * 60) + circulation2Time;
  if (timerSnack > circulation2Time and timerSnack <= recalqueTime) {   ///// a partir de 3,50 minutos e ligada a bomba de circulacao 2
    Serial.println("Ligando bomba de circulação 1 e 2");
    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);

    digitalWrite(CIRCULATION_PUMP_2, LOW);
    Blynk.virtualWrite(V7, 1);

    gaugeRetRecalque += (float(100) / (float(RETURN_CORAL_RECALQUE) * float(60)));
    Blynk.virtualWrite(V25, gaugeRetRecalque);

    timerSnack++;

    return;
  }

  float offCirculation2TIme = (FINISH_CIRCULATION2 * 60) + recalqueTime;
  if (timerSnack > recalqueTime and timerSnack <= offCirculation2TIme) { /////// a partir dos 4,30 minutos e ligada a bomba de recalque
    Serial.println("Ligando bomba de circulação 1, 2 e recalque");
    digitalWrite(CIRCULATION_PUMP_1, LOW);
    Blynk.virtualWrite(V6, 1);

    digitalWrite(CIRCULATION_PUMP_2, LOW);
    Blynk.virtualWrite(V7, 1);

    digitalWrite(BOSTER_PUMP, LOW);
    Blynk.virtualWrite(V0, 1);

    gaugeDeslCirculacao2 += (float(100) / (float(FINISH_CIRCULATION2) * float(60)));
    Blynk.virtualWrite(V27, gaugeDeslCirculacao2);

    timerSnack++;

    return;
  }

  float finalizaRefeicao = (FINISH_CORAL_SNACK * 60) + offCirculation2TIme;
  if (timerSnack > offCirculation2TIme and timerSnack <= finalizaRefeicao) { ///// apos os 5,30 minutos e desligada a bomba de circulacao 2 agitacao
    Serial.println("Desligando bomba de circulação 2");
    digitalWrite(CIRCULATION_PUMP_2, HIGH);
    Blynk.virtualWrite(V7, 0);

    gaugeFinaliza += (float(100) / (float(FINISH_CORAL_SNACK) * float(60)));
    Blynk.virtualWrite(V29, gaugeFinaliza);

    timerSnack++;

    return;
  }

  Serial.println("Finalizando periodo de alimentação...");
  Blynk.virtualWrite(V18, 0);
  SNACK_CORAL = 0;
  timerSnack = 0;
  callFunctionFirst = false;
}
