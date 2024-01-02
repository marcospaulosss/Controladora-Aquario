
String getDayOfTheWeek(NTPClient timeClient);
String getHourMinuteSecund(NTPClient timeClient);
int getHour(NTPClient timeClient);
int getMinute(NTPClient timeClient);

String getDayOfTheWeek(NTPClient timeClient)  {
  char daysOfTheWeek[7][14] = {"Domingo", "Segunda Feira", "Terca Feira", "Quarta Feira", "Quinta Feira", "Sexta Feira", "Sabado"};
  String semana = daysOfTheWeek[timeClient.getDay()];
  
  return semana;
}

//String getDayOfTheWeekInValue(NTPClient timeClient)  {
//  int daysOfTheWeek[7][14] = {7, 1, 2, 3, 4, 5, 6};
//  int dayWeek = daysOfTheWeek[timeClient.getDay().asInt()];
//  
//  return semana;
//}

String getHourMinuteSecund(NTPClient timeClient)  {
   return timeClient.getFormattedTime();
}

int getHour(NTPClient timeClient)  {
   return timeClient.getHours();
}

int getMinute(NTPClient timeClient)  {
   return timeClient.getMinutes();
}

int convertHoursAndMinutesToSeconds() {
  int hours = getHour(timeClient);
  int minutes = getMinute(timeClient);
  
  int segundos = (hours * 3600) + (minutes * 60);
    
  return segundos;
}
