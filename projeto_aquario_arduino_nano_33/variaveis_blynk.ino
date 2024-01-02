/// variáveis de interação blink projeto
String msgOld;

void sendMessageBlynk(String hour, String msg) {
  if (msg == msgOld) return;

  msgOld = msg;
  Serial.println(msg);
  
  String msgComplete = hour + " - " + msg;
  Blynk.virtualWrite(V16, msgComplete);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");

  Blynk.syncAll();
}

// Responsável pela bomba de recalque
BLYNK_WRITE(V0)
{ 

  timerGouleSkimmer = 0;
  timerSkimmer = 0;
  Blynk.virtualWrite(V31, 0);
  initSkimmer();
  
  // Set incoming value from pin V0 to a variable
  int boster_pump = param.asInt();
  if (boster_pump == HIGH) {
    digitalWrite(BOSTER_PUMP, LOW);
    sendMessageBlynk(getHourMinuteSecund(timeClient), "Info: Acionamento da bomba de recalque.");
  } else {
    digitalWrite(SKIMMER, HIGH);
    digitalWrite(BOSTER_PUMP, HIGH);
    Blynk.virtualWrite(V1, 0);
    sendMessageBlynk(getHourMinuteSecund(timeClient), "Info: Desligamento da bomba de recalque e skimmer.");
  }
}

/// responsável pelo skimmer
BLYNK_WRITE(V1)
{
  /// Tempo para retomada do skimmer em segundos.
  int timerSkimmerEnd = (TIMER_RETURN_SKIMMER * 60);
  
  // Botão responsável pela ação do skimmer
  int skimmer = param.asInt();
  if (skimmer == HIGH) {
    sendMessageBlynk(getHourMinuteSecund(timeClient), "Info: Acionamento do skimmer.");
    
    if (FORCED) {
      digitalWrite(SKIMMER, LOW);
      return;
    }
    
    if (timerSkimmer >= timerSkimmerEnd){
      digitalWrite(SKIMMER, LOW);
    } else {
      Blynk.virtualWrite(V1, 0);
    }
  } else {
    digitalWrite(SKIMMER, HIGH);
    sendMessageBlynk(getHourMinuteSecund(timeClient), "Info: Desligamento do skimmer.");
  }
}

/// Responsável pelas horas enviadas para o servidor
BLYNK_WRITE(V2) {   
  TimeInputParam t(param);
  
  int starTime1 = param[0].asInt();
  int endTime1 = param[1].asInt();

  timeClient.update();
  int HH = timeClient.getHours();
  int MM = timeClient.getMinutes();
  int SS= timeClient.getSeconds();
  int server_time = 3600*HH + 60*MM +SS;
  
  Serial.println(server_time);

  if (starTime1 == server_time) {
    Serial.println("esta dentro dos valores");
  }
}

/// Responsável pelo botão forçar
BLYNK_WRITE(V4)
{
  // Botão que força a configuração da controladora
  FORCED = param.asInt();
  if (FORCED == HIGH) sendMessageBlynk(getHourMinuteSecund(timeClient), "Warning: Botão FORCAR foi acionado.");
}

/// R=esponsável pela bomba de reposição
BLYNK_WRITE(V5)
{
  // Set incoming value from pin V5 to a variable
  int replacement_pump = param.asInt();
  if (replacement_pump == HIGH) {
    sendMessageBlynk(getHourMinuteSecund(timeClient), "Info: Acionamento da bomba de reabastecimento.");
    digitalWrite(REPLACEMENT_PUMP, LOW);
  } else {
    digitalWrite(REPLACEMENT_PUMP, HIGH);
    sendMessageBlynk(getHourMinuteSecund(timeClient), "Info: Desligamento da bomba de reabastecimento.");
  }
}

/// Responsável para primeira bomba de circulação
BLYNK_WRITE(V6)
{
  // Set incoming value from pin V6 to a variable
  int circulation_pump_1 = param.asInt();
  if (circulation_pump_1 == HIGH) {
    digitalWrite(CIRCULATION_PUMP_1, LOW);
    sendMessageBlynk(getHourMinuteSecund(timeClient), "Info: Acionamento da bomba de circulação 1.");
  } else {
    digitalWrite(CIRCULATION_PUMP_1, HIGH);
    sendMessageBlynk(getHourMinuteSecund(timeClient), "Info: Desligamento da bomba de circulação 1.");
  }
}

/// Responsável para segunda bomba de circulação
BLYNK_WRITE(V7)
{
  // Set incoming value from pin V7 to a variable
  int circulation_pump_2 = param.asInt();
  if (circulation_pump_2 == HIGH) {
    digitalWrite(CIRCULATION_PUMP_2, LOW);
    sendMessageBlynk(getHourMinuteSecund(timeClient), "Info: Acionamento da bomba de circulação 2.");
  } else {
    digitalWrite(CIRCULATION_PUMP_2, HIGH);
    sendMessageBlynk(getHourMinuteSecund(timeClient), "Info: Desligamento da bomba de circulação 2.");
  }
}

/// Alimentação periodica
BLYNK_WRITE(V8)
{  
  sendMessageBlynk(getHourMinuteSecund(timeClient), "Warning: Alteração dos horarios periodicos das refeições automaticas.");
  switch (param.asInt()) {
    case 0: { // Item 1
      J = 1;
      HOUR_STOP_SNACK_AUTOMATIC = new int[J];
      HOUR_STOP_SNACK_AUTOMATIC[0] = 12;
      break;
    }
    case 1: { // Item 2
      J = 2;
      HOUR_STOP_SNACK_AUTOMATIC = new int[J];
      HOUR_STOP_SNACK_AUTOMATIC[0] = 10;
      HOUR_STOP_SNACK_AUTOMATIC[1] = 18;
      break;
    }   
    case 2: { // Item 2
      J = 3;
      HOUR_STOP_SNACK_AUTOMATIC = new int[J];
      HOUR_STOP_SNACK_AUTOMATIC[0] = 9;
      HOUR_STOP_SNACK_AUTOMATIC[1] = 13;
      HOUR_STOP_SNACK_AUTOMATIC[2] = 20;
      break;
    }
  }
}

/// Respoonsável pelo tempo para retorno da circulação 1 apos refeição dos peixes.
BLYNK_WRITE(V9)
{
  // Set incoming value from pin V17 to a variable
  RETURN_CIRCULATION_1 = param.asInt();
}

/// Responsável por administrar a hora que inicia a TPA diaria
BLYNK_WRITE(V10)
{
  sendMessageBlynk(getHourMinuteSecund(timeClient), "Warning: Alteração no horario das tpas.");

  TimeInputParam t(param);
  
  // param[0] is the user time value selected in seconds.
  TIMER_TPA_DAILY = param[0].asInt();

  K = 0;
  DAY_TPA_DAILY.clear();
  for (int i = 1; i <= 7; i++) {
    if (t.isWeekdaySelected(i)) {
      K++;
      DAY_TPA_DAILY.push_back(i);
    }
  }
}

/// Responsável por administrar a hora que inicia a rotina do aquario
BLYNK_WRITE(V11)
{
  // sendMessageBlynk(getHourMinuteSecund(timeClient), "Warning: Alteração no horario das rotinas.");
  
  String tz = "America/Sao_Paulo";
  uint8_t dow = 1;
  int32_t utc_offset = -144000;
  
  // param[0] is the user time value selected in seconds.
  HOUR_GOOD_MORNING_START = param[0].asInt();
  
  // param[1] is the stop time in seconds (when option enabled)
  HOUR_GOOD_MORNING_END = param[1].asInt();

  if ((HOUR_GOOD_MORNING_START >= HOUR_GOOD_MORNING_END)) {
    HOUR_GOOD_MORNING_END = HOUR_GOOD_MORNING_START + 60;
    Blynk.virtualWrite(V11, HOUR_GOOD_MORNING_START, HOUR_GOOD_MORNING_END, tz, dow, utc_offset);
  }

  if ((HOUR_GOOD_MORNING_END >= HOUR_GOOD_AFTERNOON_START)) {
    HOUR_GOOD_AFTERNOON_START = HOUR_GOOD_MORNING_END + 60;
    Blynk.virtualWrite(V19, HOUR_GOOD_AFTERNOON_START, HOUR_GOOD_AFTERNOON_END, tz, dow, utc_offset);
  }
  
  Blynk.syncVirtual(V19);
}

/// Responsável pelo botão de alimentação
BLYNK_WRITE(V12)
{
  // Set incoming value from pin V4 to a variable
  SNACK = param.asInt();
  sendMessageBlynk(getHourMinuteSecund(timeClient), "Info: Acionamento da refeição.");
}

/// Responsável pel lampada do samp
BLYNK_WRITE(V13)
{
  // Set incoming value from pin V0 to a variable
  int lamp_1 = param.asInt();
  if (lamp_1 == HIGH) {
    digitalWrite(LAMPSUMP, LOW);
    sendMessageBlynk(getHourMinuteSecund(timeClient), "Info: Acionamento luz do sump.");
  } else {
    digitalWrite(LAMPSUMP, HIGH);
    sendMessageBlynk(getHourMinuteSecund(timeClient), "Info: Desligamento da luz do sump.");
  }
}

/// Responsável pela bomba da TPA 1 retirada de agua
BLYNK_WRITE(V14)
{ 
  // Set incoming value from pin V0 to a variable
  int bomb_1 = param.asInt();
  if (bomb_1 == HIGH) {
    digitalWrite(TPA_PUMP_1, LOW);
    sendMessageBlynk(getHourMinuteSecund(timeClient), "Warning: Acionamento da bomba de TPA.");
  } else {
    digitalWrite(TPA_PUMP_1, HIGH);
    sendMessageBlynk(getHourMinuteSecund(timeClient), "Warning: Desligamento da bomba de TPA.");
  }
}

/// Responsável pela bomba da TPA 1 adição de agua
BLYNK_WRITE(V15)
{
  // Set incoming value from pin V0 to a variable
  int bomb_2 = param.asInt();
  if (bomb_2 == HIGH) {
    digitalWrite(TPA_PUMP_2, LOW);
    sendMessageBlynk(getHourMinuteSecund(timeClient), "Warning: Acionamento da bomba de TPA.");
  } else {
    digitalWrite(TPA_PUMP_2, HIGH);
    sendMessageBlynk(getHourMinuteSecund(timeClient), "Warning: Desligamento da bomba de TPA.");
  }
}

/// Responsável pelo envio de mensagem para o terminal
BLYNK_WRITE(V16)
{
  // Set incoming value from pin V4 to a variable
  String msg = param.asStr();
  if (msg == "clear") {
    Blynk.virtualWrite(V16, "clr");
  }
}

/// Responsável pelo botão de acionamento para alimentação automatica dos peixes
BLYNK_WRITE(V17)
{
  // Set incoming value from pin V17 to a variable
  SNACKBYTIMER = param.asInt();
  sendMessageBlynk(getHourMinuteSecund(timeClient), "Info: Acionamento da alimentação periodica automatica.");
}

/// Responsável pelo botão de alimentação de corais
BLYNK_WRITE(V18)
{
  // Set incoming value from pin V17 to a variable
  SNACK_CORAL = param.asInt();
  sendMessageBlynk(getHourMinuteSecund(timeClient), "Info: Acionamento da alimentação para os corais.");
}

/// Responsável por administrar a hora que inicia a rotina do aquario
BLYNK_WRITE(V19)
{
  String tz = "America/Sao_Paulo";
  uint8_t dow = 1;
  int32_t utc_offset = -144000;

  // param[0] is the user time value selected in seconds.
  HOUR_GOOD_AFTERNOON_START = param[0].asInt();
  // param[1] is the stop time in seconds (when option enabled)
  HOUR_GOOD_AFTERNOON_END = param[1].asInt();


  if ((HOUR_GOOD_AFTERNOON_START >= HOUR_GOOD_AFTERNOON_END)) {
    HOUR_GOOD_AFTERNOON_END = HOUR_GOOD_AFTERNOON_START + 60;
    Blynk.virtualWrite(V19, HOUR_GOOD_AFTERNOON_START, HOUR_GOOD_AFTERNOON_END, tz, dow, utc_offset);
  }

  if ((HOUR_GOOD_AFTERNOON_END >= HOUR_GOOD_NIGHT_START)) {
    HOUR_GOOD_NIGHT_START = HOUR_GOOD_AFTERNOON_END + 60;
    Blynk.virtualWrite(V20, HOUR_GOOD_NIGHT_START, HOUR_GOOD_NIGHT_END, tz, dow, utc_offset);
  }
  
  Blynk.syncVirtual(V20);
}

/// Responsável por administrar a hora que inicia a rotina do aquario
BLYNK_WRITE(V20)
{
  String tz = "America/Sao_Paulo";
  uint8_t dow = 1;
  int32_t utc_offset = -144000;

  // param[0] is the user time value selected in seconds.
  HOUR_GOOD_NIGHT_START = param[0].asInt();
  // param[1] is the stop time in seconds (when option enabled)
  HOUR_GOOD_NIGHT_END = param[1].asInt();

  if ((HOUR_GOOD_NIGHT_START >= HOUR_GOOD_NIGHT_END)) {
    HOUR_GOOD_NIGHT_END = HOUR_GOOD_NIGHT_START + 60;
    Blynk.virtualWrite(V20, HOUR_GOOD_NIGHT_START, HOUR_GOOD_NIGHT_END, tz, dow, utc_offset);
  }
  
  Blynk.syncVirtual(V11);
}

/// Respoonsável pelo tempo para retorno da circulação 2 apos refeição dos peixes.
BLYNK_WRITE(V22)
{
  // Set incoming value from pin V17 to a variable
  RETURN_CIRCULATION_2 = param.asInt();
}

/// Respoonsável pelo tempo para retorno da bombab de recalque apos refeição dos peixes.
BLYNK_WRITE(V24)
{
  // Set incoming value from pin V17 to a variable
  RETURN_BOSTER_PUMP = param.asInt();
}

/// Respoonsável pelo tempo de ligação da bomba de requalque.
BLYNK_WRITE(V26)
{
  // Set incoming value from pin V17 to a variable
  FINISH_CIRCULATION_2 = param.asInt();
}

/// Respoonsável pelarada da bombab de circulação 2 apos refeição dos peixes.
BLYNK_WRITE(V28)
{
  // Set incoming value from pin V17 to a variable
  FINISH_SNACK = param.asInt();
}

/// Responsável pelo tempo de desligamento do skimer
BLYNK_WRITE(V30)
{
  // Set incoming value from pin V17 to a variable
  TIMER_RETURN_SKIMMER = param.asInt();
}

/// Responsável pelo tempo de maquinas desligadas para a limentação dos corais
BLYNK_WRITE(V32)
{
  RETURN_CORAL_CIRCULATION_1 = param.asInt();
}

/// Responsável pelo tempo que a circulação 2 levará para ligar
BLYNK_WRITE(V33)
{
  RETURN_CORAL_CIRCULATION_2 = param.asInt();
}

/// Responsável pelo tempo que o recalque levara para ligar
BLYNK_WRITE(V34)
{
  RETURN_CORAL_RECALQUE = param.asInt();
}

/// Responsável pelo tempo que o as maquinas circulaçao 1,2 e recalque ficaram ativadas
BLYNK_WRITE(V35)
{
  FINISH_CIRCULATION2 = param.asInt();
}

/// Responsável pelo tempo levará para finalizar a refeição dos corais
BLYNK_WRITE(V36)
{
  // Set incoming value from pin V17 to a variable
  FINISH_CORAL_SNACK = param.asInt();
}

/// Responsável pelo acionamento da tpa diaria
BLYNK_WRITE(V37)
{
  TPA_DAILY = param.asInt();
}

/// Representa o sinalizador de TPA iniciada
BLYNK_WRITE(V38)
{
  INIT_TPA = param.asInt();
}

/// Representa a ação que a TPA esta fazendo
BLYNK_WRITE(V39)
{
  ACTION_TPA = param.asInt();
}

/// Responsável pelo menu de quantidade de reabastecimentos diarios
BLYNK_WRITE(V40)
{
  sendMessageBlynk(getHourMinuteSecund(timeClient), "Warning: Alteração dos horarios de reabastecimento.");
  switch (param.asInt()) {
    case 0: { // Item 1
      N = 1;
      HOURS_REPLENISHMENT = new int[N];
      HOURS_REPLENISHMENT[0] = 0;
      break;
    }
    case 1: { // Item 2
      N = 2;
      HOURS_REPLENISHMENT = new int[N];
      HOURS_REPLENISHMENT[0] = 0;
      HOURS_REPLENISHMENT[1] = 12;
      break;
    }   
    case 2: { // Item 2
      N = 3;
      HOURS_REPLENISHMENT = new int[N];
      HOURS_REPLENISHMENT[0] = 0;
      HOURS_REPLENISHMENT[1] = 9;
      HOURS_REPLENISHMENT[2] = 18;
      break;
    }
    case 3: { // Item 2
      N = 4;
      HOURS_REPLENISHMENT = new int[N];
      HOURS_REPLENISHMENT[0] = 0;
      HOURS_REPLENISHMENT[1] = 6;
      HOURS_REPLENISHMENT[2] = 12;
      HOURS_REPLENISHMENT[3] = 18;
      break;
    }
    case 4: { // Item 2
      N = 5;
      HOURS_REPLENISHMENT = new int[N];
      HOURS_REPLENISHMENT[0] = 0;
      HOURS_REPLENISHMENT[1] = 5;
      HOURS_REPLENISHMENT[2] = 10;
      HOURS_REPLENISHMENT[3] = 15;
      HOURS_REPLENISHMENT[4] = 18;
      break;
    } 
  }
}

/// Representa o tempo gasto para desligamento do maquinario
BLYNK_WRITE(V41)
{
  TIME_OFF_MACHINE = param.asInt();
}
