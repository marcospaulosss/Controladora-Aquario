/// Configuração das entradas dos pinos do arduino
void pinoutConfig() {
  pinMode(SKIMMER, OUTPUT);
  pinMode(BOSTER_PUMP, OUTPUT);
  pinMode(REPLACEMENT_PUMP, OUTPUT);
  pinMode(CIRCULATION_PUMP_1, OUTPUT);
  pinMode(CIRCULATION_PUMP_2, OUTPUT);
  pinMode(LAMPSUMP, OUTPUT);
  pinMode(TPA_PUMP_1, OUTPUT);
  pinMode(TPA_PUMP_2, OUTPUT);
  pinMode(TPA_SENSOR_1, INPUT_PULLUP);
}

/// Função responsável por iniciar as maquinas mesmo sem acesso a internet
void setupInicial(){
  digitalWrite(SKIMMER, HIGH);
  digitalWrite(BOSTER_PUMP, LOW);
  digitalWrite(REPLACEMENT_PUMP, LOW);
  digitalWrite(CIRCULATION_PUMP_1, LOW);
  digitalWrite(CIRCULATION_PUMP_2, HIGH);
  digitalWrite(LAMPSUMP, HIGH);
  digitalWrite(TPA_PUMP_1, HIGH);
  digitalWrite(TPA_PUMP_2, HIGH);
}

/// Função responsável pelo sync com a blink
void setupControll(){
  pinoutConfig();

  snackQtd = false;
  replenishment = false;

  if(!Blynk.connected()) {
    Serial.println("não conectado");
    setupInicial();
    return;
  }

  /// Desliga o skimmer ao iniciar arduino independentemente se esta ligado no servidor
  digitalWrite(SKIMMER, HIGH);
  Blynk.virtualWrite(V1, 0);

  /// Atualiza a controladora com o que esta no servidor
  Blynk.syncVirtual(V0, V1, V2, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13, V14, V15, V16, V17, V18, V19, V20, V21, V22, V23, V24, V25, V26, V27, V28, V29, V30, V31, V32, V33, V34, V35, V36);
  Blynk.syncVirtual(V37, V38, V39, V40, V41);
}
