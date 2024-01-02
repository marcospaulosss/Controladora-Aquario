#define SECRET_SSID "MARCOS_PAULO"
#define SECRET_PASS "marcos3891"
#define SECRET_TOKEN "uq8KNQOTEmaOOciq_NX19YWtm79jhuLk"
#define DEVICE_NAME "controladora 2"
#define TEMPLATE_ID "TMPL2u82mqxMK"
#define BLYNK_TEMPLATE_ID TEMPLATE_ID
#define BLYNK_TEMPLATE_NAME DEVICE_NAME
#define BLYNK_AUTH_TOKEN SECRET_TOKEN
#define BLYNK_PRINT Serial

/// Bibbliotecas do projeto
#include <SPI.h>
#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>
#include <NTPClient.h>
#include <string>
#include <vector>

/// parametros para conectar a internet
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// Configuração para buscar a data atual
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, -10800);
BlynkTimer timer;

/// Variaveis de controle de tempo
int timerSkimmer = 0;           /// Variavel respoonsável pela contagem corrida do skimmer desligado em segundos
float timerGouleSkimmer = 0;
bool snackQtd = false;               /// Variavel que representa a quantidade de refeição ja foi feita
bool replenishment = false;          /// Variavel que representa se ocorreu o reabastecimento
int timeProcess1 = 0;              /// Variavel responsável por administrar o tempo de desligamento do maquinario para TPA
int timeProcess2 = 0;              /// Variavel responsável por administrar o tempo de reabastecimento de agua doce antes da TPA

/// Variaveis de controle da controladora
int FORCED = 0;                             /// Botão de controle total
int SNACK = 0;                              /// Botão de alimentação
int SNACK_CORAL = 0;                        /// Botão de alimentação
int32_t HOUR_GOOD_MORNING_START = 28800;        /// V11 hora programada para inicio dos trabalhos
int32_t HOUR_GOOD_MORNING_END = 43200;          /// V11 hora programada para inicio dos trabalhos
int32_t HOUR_GOOD_AFTERNOON_START = 46800;      /// V11 hora programada para inicio dos trabalhos
int32_t HOUR_GOOD_AFTERNOON_END = 64800;        /// V11 hora programada para inicio dos trabalhos
int32_t HOUR_GOOD_NIGHT_START = 64860;      /// V11 hora programada para inicio dos trabalhos
int32_t HOUR_GOOD_NIGHT_END = 82800;        /// V11 hora programada para inicio dos trabalhos
int RETURN_CIRCULATION_1;                       /// V9  tempo para o retorno da circulação 1 apos refeição dos peixes
int RETURN_CIRCULATION_2;                       /// V22 tempo para o retorno da circulação 2 apos refeição dos peixes
int RETURN_BOSTER_PUMP;                         /// V24 tempo para o retorno da bomba de recalque apos refeição dos peixes
int FINISH_CIRCULATION_2;                       /// V26 tempo para desligamento da bomba de circulação 2 apos refeição dos peixes
int FINISH_SNACK;                               /// V28 tempo para finalizar o periodo de refeição dos peixes
int TIMER_RETURN_SKIMMER = 1;                   /// V30 tempo configurado para a volta do skimmer
int RETURN_CORAL_CIRCULATION_1 = 1;             /// V32 tempo de maquinas paradas para alimentação dos corais
int RETURN_CORAL_CIRCULATION_2 = 1;             /// V33 tempo de circulação 1 ativada para alimentação dos corais
int RETURN_CORAL_RECALQUE = 1;                  /// V34 tempo que levará para o recalque seja iniciado apos a circulação 2
int FINISH_CIRCULATION2 = 1;                    /// V35 tempo que levará para as maquinas circulação 1,2 e requalque ficarão acionadas
int FINISH_CORAL_SNACK = 1;                     /// V36 tempo que levará para as maquinas ficarão acionadas ate o fim da alimentação de corais
int SNACKBYTIMER = 0;                           /// V17 Botão de alimentação automatica
int *HOUR_STOP_SNACK_AUTOMATIC, J;              /// V08 Array dinamico do menu de periodicidade de alimentação automatica
int *HOURS_REPLENISHMENT, N;                    /// V40 Arrey dinamico do menu de quantidade de reabastecimento de agua
bool TPA_DAILY = false;                         /// V37 Botão responsável pela habilitação da TPA diária
int32_t TIMER_TPA_DAILY = 28800;                /// V10 Horario da TPA diária
std::vector< int > DAY_TPA_DAILY;               /// V10 Horario da TPA diária
int K=0;
bool INIT_TPA = 1;                              /// V38 Representa se a tpa diaria foi realizada ou iniciou
int ACTION_TPA;                                 /// V39 Representa a ação que foi feita pelo fluxo de TPA
int TIME_OFF_MACHINE = 5;                           /// V41 Representa o tempo gasto para desligamento completo das maquinas


//PinModes
/// Variaveis responsáveis pelo pinos do arduino
int SKIMMER = 2;               /// Porta digital D2
int BOSTER_PUMP = 3;           /// Porta digital D3
int REPLACEMENT_PUMP = 4;      /// Porta digital D4
int CIRCULATION_PUMP_1 = 5;    /// Porta digital D5
int CIRCULATION_PUMP_2 = 6;    /// Porta digital D6
int LAMPSUMP = 7;              /// Porta digital D7
int TPA_PUMP_1 = 8;            /// Porta digital D8
int TPA_PUMP_2 = 9;            /// Porta digital D9
int TPA_SENSOR_1 = 10;         /// Porta digital D10
int TPA_SENSOR_2 = 11;         /// Porta digital D11

/// Declaração da Funcões
extern void setupInicial();
extern void setupControll();                             /// Arquivo setup
extern void initSkimmer();                               /// Inicializa o skimmer
extern void sendMessageBlynk(String hour, String msg);   /// Arquivo variaveis-blynk
extern String getDayOfTheWeek(NTPClient timeClient);
extern String getHourMinuteSecund(NTPClient timeClient);
extern int getHour(NTPClient timeClient);
extern int getMinute(NTPClient timeClient);
extern void goodMorning();
extern void goodAfeternoon();
extern void goodNight();
extern void finishDay();
extern void offAll();
extern void fishSnack();
extern void coralSnack();
extern void offAll();
extern void replenishmentTime(int minute);
extern int convertHoursAndMinutesToSeconds();
extern bool tpaDaily();
