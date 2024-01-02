/// Declaração de funções
void initSkimmer();

void initSkimmer() {    
  int skimmer = digitalRead(SKIMMER);
  int bosterPump = digitalRead(BOSTER_PUMP);
  int timerSkimmerEnd = (TIMER_RETURN_SKIMMER * 60);

  timerSkimmer++;

  /// Valida o estado do skimmer para retomada do fluxo.
  if ((skimmer && timerSkimmer < timerSkimmerEnd) || !skimmer) {
    if (!bosterPump) {
      timerGouleSkimmer += ((float(100) * float(1)) / float(timerSkimmerEnd));
      Blynk.virtualWrite(V31, timerGouleSkimmer);
    } else {
      timerSkimmer = 0;
    }
    
    return; 
  }

  /// Liga skimmer
  if (SKIMMER != LOW) {
    digitalWrite(SKIMMER, LOW);
    Blynk.virtualWrite(V1, 1);
  }

  Serial.println("Skimmer Iniciado.");
}
