/// arquivo com as configurações do projeto
#include "config.h"

void setup()
{
  // Debug console
  Serial.begin(9600);
  timeClient.begin();
  setupInicial();

  Blynk.begin(auth, ssid, pass);

  timeClient.update();
  sendMessageBlynk(getHourMinuteSecund(timeClient), "Dispositivo Conectado");
  sendMessageBlynk(getHourMinuteSecund(timeClient), "Iniciando aplicação...");

  /// renicializa os parametros com valores do aplicativo
  setupControll();
}

void loop()
{
  Blynk.run();
  timer.run();
  if (!Blynk.connected())
  {
    Serial.println("Desconectou");
    Blynk.connect();
    delay(1000);
    return;
  }

  timeClient.update();

  int hour = getHour(timeClient);
  int minute = getMinute(timeClient);

  if (FORCED == HIGH)
  {
    Serial.println("botão forçar");
    delay(797);
    return;
  }

  /// Verifica os dias de TPA e executa a função
  /// K equivale aos dias da semana que será executado a TPA
  for (int i = 0; i < K; i++)
  {
    if (DAY_TPA_DAILY[i] == timeClient.getDay())
    {
      if (tpaDaily())
      {
        delay(900);

        return;
      }
    }
  }

  /// verifica as horas de abastecimento e habilita caso necessário
  bool hourReplenishment = false;
  for (int i = 0; i < N; i++)
  {
    if (hour == HOURS_REPLENISHMENT[i])
    {
      hourReplenishment = true;
      Serial.println("horario de reabastecimento");
    }
  }

  /// Verifica o horario e as condições para o reabbastecimento.
  if (!replenishment and hourReplenishment)
  {
    timerSkimmer = 0;
    replenishmentTime(minute);

    delay(800);
    return;
  }
  else if ((replenishment and !hourReplenishment) || !hourReplenishment)
  {
    replenishment = false;
  }
  else if (hourReplenishment)
  {
    replenishment = true;
  }


  Serial.println("Normal");
  delay(810);

  // if (FORCED == LOW) {

  //   for (int i=0; i < K; i++) {
  //     if (DAY_TPA_DAILY[i] == timeClient.getDay()) {
  //       if (tpaDaily()) {
  //         delay(900);

  //         return;
  //       }
  //     }
  //   }

  //   /// verifica as horas de abastecimento e habilita caso necessário
  //   bool hourReplenishment = false;
  //   for(int i = 0; i < N; i++) {
  //     if (hour == HOURS_REPLENISHMENT[i]) hourReplenishment = true;
  //   }

  //   /// Verifica o horario e as condições para o reabbastecimento.
  //   if(!replenishment and hourReplenishment) {
  //     timerSkimmer = 0;
  //     replenishmentTime(minute);

  //     delay(1000);
  //     return;
  //   } else if((replenishment and !hourReplenishment) || !hourReplenishment) {
  //     replenishment = false;
  //   } else if (hourReplenishment){
  //     replenishment = true;
  //   }

  //   if (SNACK) {
  //     timerSkimmer = 0;

  //     fishSnack();

  //     delay(1000);

  //     return;
  //   }

  //   if (SNACKBYTIMER) {
  //     /// verifica as horas de abastecimento e habilita caso necessário
  //     bool hourSnackAutomatic = false;
  //     for(int i = 0; i < J; i++) {
  //       if (hour == HOUR_STOP_SNACK_AUTOMATIC[i]) hourSnackAutomatic = true;
  //     }

  //     if (!snackQtd  and hourSnackAutomatic) {
  //       timerSkimmer = 0;

  //       fishSnack();

  //       delay(1000);

  //       return;
  //     } else if((snackQtd and !hourSnackAutomatic) || !hourSnackAutomatic) {
  //       snackQtd = false;
  //     } else if (hourSnackAutomatic){
  //       snackQtd = true;
  //     }
  //   }

  //   if (SNACK_CORAL) {
  //     timerSkimmer = 0;

  //     coralSnack();

  //     delay(1000);

  //     return;
  //   }

  //   /// chama a função para inicio da contagem para ligação do skimmer
  //   initSkimmer();

  //   goodMorning();
  //   goodAfeternoon();
  //   goodNight();
  //   finishDay();

  //   delay(400);
  // }

  // Serial.println("botão forçar");
  // delay(1000);
}
