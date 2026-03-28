#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <RTClib.h>
#include <EEPROM.h> 

// --- CONFIGURAÇÕES ---
LiquidCrystal_I2C lcd(0x27, 20, 4);
DHT dht(2, DHT22); 
RTC_DS3231 rtc;

#define LED_LUZ_R 8
#define LED_LUZ_G 9
#define LED_TEMP_R 11
#define LED_TEMP_G 12
#define LED_HUM_R A1
#define LED_HUM_G A2
#define LDR A0
#define ArrowUp 3
#define ArrowDown 4
#define Enter 5
#define MenuButton 6
#define BUZZER 10 

// --- VARIÁVEIS GLOBAIS ---
int idioma = 0; 
float tempMin = 15.0, tempMax = 25.0;
float humMin = 30.0, humMax = 50.0;
int luzMin = 0, luzMax = 30;

bool alertaAtivo = false, editandoMin = true;
int totalRegistros = 0, enderecoAtual = 0, indiceHistorico = 0, menuIndex = 0, etapaAjuste = 0;

enum MenuState { MONITORAMENTO, MENU_PRINCIPAL, MENU_IDIOMA, SUBMENU_LIMITES, MENU_LIMITES, MENU_HISTORICO, MENU_LIMPAR };
MenuState estadoAtual = MONITORAMENTO;

struct Registro {
  float temp; float hum; int luz;
  byte dia; byte mes; byte hora; byte minute;
};

// Novos caracteres para a animação
byte termo[8] = {B00100, B01010, B01010, B01110, B01110, B11111, B11111, B01110};
byte gota[8]  = {B00100, B00100, B01010, B01010, B10001, B10001, B10001, B01110};
byte raio[8]  = {B00010, B00100, B01000, B11111, B00100, B01000, B10000, B00000};

// Caracteres para a logo animada
byte chip1[8] = {B01110, B10001, B10001, B11111, B10001, B10001, B10001, B01110};
byte chip2[8] = {B01110, B11111, B10101, B11111, B10101, B11111, B11111, B01110};
byte chip3[8] = {B01110, B11111, B11111, B11111, B11111, B11111, B11111, B01110};
byte onda1[8] = {B00000, B00000, B00001, B00011, B00110, B01100, B11000, B10000};
byte onda2[8] = {B00000, B10000, B11000, B01100, B00110, B00011, B00001, B00000};
byte seta[8]   = {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100};
byte quadrado[8] = {B11111, B10001, B10001, B10001, B10001, B10001, B10001, B11111};

// --- FUNÇÕES DE MEMÓRIA (CONFIGURAÇÕES) ---
void salvarConfiguracoes() {
  EEPROM.put(500, tempMin); EEPROM.put(504, tempMax);
  EEPROM.put(508, humMin);  EEPROM.put(512, humMax);
  EEPROM.put(516, luzMin);  EEPROM.put(520, luzMax);
  EEPROM.put(524, idioma);
}

void carregarConfiguracoes() {
  float t_test;
  EEPROM.get(500, t_test);
  if(!isnan(t_test) && t_test > -50 && t_test < 100) {
    EEPROM.get(500, tempMin); EEPROM.get(504, tempMax);
    EEPROM.get(508, humMin);  EEPROM.get(512, humMax);
    EEPROM.get(516, luzMin);  EEPROM.get(520, luzMax);
    EEPROM.get(524, idioma);
  }
}

// --- ANIMAÇÃO DE BOOT ---
void animacaoBoot() {
  // Cria os caracteres personalizados
  lcd.createChar(3, chip1);
  lcd.createChar(4, chip2);
  lcd.createChar(5, chip3);
  lcd.createChar(6, onda1);
  lcd.createChar(7, onda2);
  
  // Animação 1: Logo com efeito de onda
  for(int frame = 0; frame < 3; frame++) {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("DEBUGGERS");
    lcd.setCursor(7, 1);
    lcd.write(3);
    lcd.write(3);
    lcd.write(3);
    
    lcd.setCursor(7, 2);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    
    lcd.setCursor(7, 3);
    lcd.write(5);
    lcd.write(5);
    lcd.write(5);
    
    delay(300);
    
    if(frame < 2) {
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("DEBUGGERS");
      
      lcd.setCursor(7, 1);
      lcd.write(4);
      lcd.write(4);
      lcd.write(4);
      
      lcd.setCursor(7, 2);
      lcd.write(5);
      lcd.write(5);
      lcd.write(5);
      
      lcd.setCursor(7, 3);
      lcd.write(3);
      lcd.write(3);
      lcd.write(3);
      
      delay(300);
    }
  }
  
  // Animação 2: Ondas digitais
  for(int i = 0; i < 3; i++) {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(">> SYSTEM BOOT  <<");
    
    // Ondas se movendo
    for(int pos = 0; pos < 20; pos++) {
      lcd.setCursor(pos, 2);
      lcd.write(6);
      lcd.setCursor(19-pos, 3);
      lcd.write(7);
    }
    delay(200);
    
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(">> SYSTEM BOOT  <<");
    
    for(int pos = 0; pos < 20; pos++) {
      lcd.setCursor(19-pos, 2);
      lcd.write(7);
      lcd.setCursor(pos, 3);
      lcd.write(6);
    }
    delay(200);
  }
  
  // Animação 3: Barra de progresso
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("INICIALIZANDO");
  lcd.setCursor(0, 2);
  lcd.print("[");
  lcd.setCursor(19, 2);
  lcd.print("]");
  
  for(int i = 0; i <= 16; i++) {
    lcd.setCursor(1 + i, 2);
    lcd.write(255); // Bloco cheio
    delay(80);
    // Som de click suave durante a progressão
    if(i % 4 == 0) tone(BUZZER, 1000, 20);
  }
  
  // Animação 4: Piscando "READY"
  for(int i = 0; i < 3; i++) {
    lcd.setCursor(7, 3);
    lcd.print("READY!");
    delay(200);
    lcd.setCursor(7, 3);
    lcd.print("      ");
    delay(150);
  }
  
  lcd.setCursor(6, 3);
  lcd.print("READY!");
  delay(500);
  
  // Som de sucesso final
  tone(BUZZER, 1200, 100);
  delay(100);
  tone(BUZZER, 1800, 150);
  delay(200);
}

// --- FUNÇÕES DE APOIO ---
void somClick() { tone(BUZZER, 1500, 30); }
void somSucesso() { tone(BUZZER, 1200, 100); delay(100); tone(BUZZER, 1800, 100); }
void somAlerta() {
  for (int f = 600; f < 1000; f += 20) { tone(BUZZER, f); delay(3); }
  for (int f = 1000; f > 600; f -= 20) { tone(BUZZER, f); delay(3); }
  noTone(BUZZER);
}

String traduza(String pt, String en, String es) {
  if (idioma == 1) return en; if (idioma == 2) return es; return pt;
}

bool botaoPressionado(int pino) {
  if (digitalRead(pino) == LOW) {
    delay(50); 
    if (digitalRead(pino) == LOW) { 
      somClick();
      while (digitalRead(pino) == LOW); 
      return true; 
    }
  }
  return false;
}

void salvarRegistro() {
  Registro novo;
  DateTime now = rtc.now();
  novo.temp = dht.readTemperature(); 
  novo.hum = dht.readHumidity();
  novo.luz = map(analogRead(LDR), 0, 1023, 0, 100);
  novo.dia = now.day(); novo.mes = now.month();
  novo.hora = now.hour(); novo.minute = now.minute();
  EEPROM.put(enderecoAtual, novo);
  enderecoAtual += sizeof(Registro);
  if (enderecoAtual + sizeof(Registro) > 450) enderecoAtual = 0;
  if (totalRegistros < (450 / sizeof(Registro))) totalRegistros++;
}

void limparMemoria() {
  lcd.clear(); lcd.setCursor(0,1); lcd.print(traduza(" APAGANDO...", " CLEARING...", " BORRANDO..."));
  for (int i = 0 ; i < 450 ; i++) EEPROM.write(i, 0);
  totalRegistros = 0; enderecoAtual = 0; indiceHistorico = 0;
  somSucesso(); delay(1000);
}

// --- DESENHO DAS TELAS ---
void desenharTela() {
  lcd.clear();
  switch (estadoAtual) {
    case MONITORAMENTO: {
        DateTime now = rtc.now();
        lcd.setCursor(0,0);
        if(now.day() < 10) lcd.print('0'); lcd.print(now.day()); lcd.print('/');
        if(now.month() < 10) lcd.print('0'); lcd.print(now.month()); lcd.print('/');
        lcd.print(now.year());
        lcd.setCursor(15,0);
        if(now.hour() < 10) lcd.print('0'); lcd.print(now.hour());
        lcd.print(':'); if(now.minute() < 10) lcd.print('0'); lcd.print(now.minute());

        lcd.setCursor(0,1); lcd.write(0); lcd.print(traduza(" TEMP: ", " TEMP: ", " TEMP: ")); 
        lcd.print(dht.readTemperature(), 1); lcd.print("C");
        lcd.setCursor(0,2); lcd.write(1); lcd.print(traduza(" UMID: ", " HUMID:", " HUMED:")); 
        lcd.print((int)dht.readHumidity()); lcd.print("%");
        lcd.setCursor(0,3); lcd.write(2); lcd.print(traduza(" LUZ : ", " LIGHT:", " LUZ : ")); 
        lcd.print(map(analogRead(LDR), 0, 1023, 0, 100)); lcd.print("%");
      } break;

    case MENU_PRINCIPAL:
      lcd.setCursor(0,0); lcd.print(menuIndex == 0 ? "> 1. " : "  1. "); lcd.print(traduza("IDIOMA", "LANGUAGE", "IDIOMA"));
      lcd.setCursor(0,1); lcd.print(menuIndex == 1 ? "> 2. " : "  2. "); lcd.print(traduza("LIMITES", "LIMITS", "LIMITES"));
      lcd.setCursor(0,2); lcd.print(menuIndex == 2 ? "> 3. " : "  3. "); lcd.print(traduza("HISTORICO", "HISTORY", "HISTORIAL"));
      lcd.setCursor(0,3); lcd.print(menuIndex == 3 ? "> 4. " : "  4. "); lcd.print(traduza("LIMPAR", "CLEAR", "LIMPIAR"));
      if(menuIndex == 4) { lcd.setCursor(0,3); lcd.print("> 5. "); lcd.print(traduza("VOLTAR", "BACK", "VOLVER")); }
      break;

    case SUBMENU_LIMITES:
      lcd.setCursor(0,0); lcd.print(traduza("AJUSTAR:", "SELECT SENSOR:", "AJUSTAR:"));
      lcd.setCursor(0,1); lcd.print(menuIndex == 0 ? "> " : "  "); lcd.print(traduza("TEMPERATURA", "TEMPERATURE", "TEMPERATURA"));
      lcd.setCursor(0,2); lcd.print(menuIndex == 1 ? "> " : "  "); lcd.print(traduza("UMIDADE", "HUMIDITY", "HUMEDAD"));
      lcd.setCursor(0,3); lcd.print(menuIndex == 2 ? "> " : "  "); lcd.print(traduza("LUMINOSIDADE", "LIGHT LEVEL", "LUMINOSIDAD"));
      break;

    case MENU_LIMITES:
      lcd.setCursor(0,0); 
      if(etapaAjuste == 0) lcd.print(traduza("SET: TEMPERATURA", "SET: TEMPERATURE", "SET: TEMPERATURA"));
      else if(etapaAjuste == 1) lcd.print(traduza("SET: UMIDADE", "SET: HUMIDITY", "SET: HUMEDAD"));
      else lcd.print(traduza("SET: LUZ", "SET: LIGHT", "SET: LUZ"));
      lcd.setCursor(0,2); lcd.print(editandoMin ? "MIN -> " : "MAX -> ");
      if(etapaAjuste == 0) lcd.print(editandoMin ? tempMin : tempMax, 1);
      else if(etapaAjuste == 1) lcd.print(editandoMin ? humMin : humMax, 0);
      else lcd.print(editandoMin ? luzMin : luzMax);
      lcd.print(etapaAjuste == 0 ? " C" : " %");
      break;

    case MENU_HISTORICO:
      if(totalRegistros == 0) {
        lcd.setCursor(5,1); lcd.print(traduza("VAZIO", "EMPTY", "VACIO"));
      } else {
        Registro r; EEPROM.get(indiceHistorico * sizeof(Registro), r);
        lcd.setCursor(0,0); lcd.print("L"); lcd.print(indiceHistorico + 1); lcd.print("/"); lcd.print(totalRegistros);
        lcd.setCursor(9,0);
        if(r.dia < 10) lcd.print("0"); lcd.print(r.dia); lcd.print("/"); if(r.mes < 10) lcd.print("0"); lcd.print(r.mes);
        lcd.print(" "); if(r.hora < 10) lcd.print("0"); lcd.print(r.hora); lcd.print(":"); if(r.minute < 10) lcd.print("0"); lcd.print(r.minute);
        lcd.setCursor(0,1); lcd.write(0); lcd.print(traduza(" TEMP: ", " TEMP: ", " TEMP: ")); lcd.print(r.temp, 1); lcd.print("C");
        lcd.setCursor(0,2); lcd.write(1); lcd.print(traduza(" UMID: ", " HUMID:", " HUMED:")); lcd.print(r.hum, 0); lcd.print("%");
        lcd.setCursor(0,3); lcd.write(2); lcd.print(traduza(" LUZ : ", " LIGHT:", " LUZ : ")); lcd.print(r.luz); lcd.print("%");
      } break;

    case MENU_LIMPAR:
      lcd.setCursor(0,0); lcd.print(traduza("APAGAR TUDO?", "CLEAR ALL?", "BORRAR TODO?"));
      lcd.setCursor(0,2); lcd.print(traduza("ENTER: SIM", "ENTER: YES", "ENTER: SI"));
      lcd.setCursor(0,3); lcd.print(traduza("MENU: CANCELAR", "MENU: CANCEL", "MENU: CANCELAR"));
      break;
      
    case MENU_IDIOMA:
      lcd.setCursor(0,0); lcd.print(traduza("SELECIONE:", "SELECT:", "SELECCIONE:"));
      lcd.setCursor(2,1); lcd.print(idioma == 0 ? "> PORTUGUES" : "  PORTUGUES");
      lcd.setCursor(2,2); lcd.print(idioma == 1 ? "> ENGLISH" : "  ENGLISH");
      lcd.setCursor(2,3); lcd.print(idioma == 2 ? "> ESPANOL" : "  ESPANOL");
      break;
  }
}

void verificarBotoes() {
  if (botaoPressionado(MenuButton)) { 
    if(estadoAtual == MONITORAMENTO) estadoAtual = MENU_PRINCIPAL;
    else if(estadoAtual == MENU_LIMITES) estadoAtual = SUBMENU_LIMITES;
    else estadoAtual = MONITORAMENTO;
    menuIndex = 0; desenharTela(); return;
  }

  if (estadoAtual == MENU_PRINCIPAL) {
    if (botaoPressionado(ArrowUp))   { menuIndex = (menuIndex <= 0) ? 4 : menuIndex - 1; desenharTela(); }
    if (botaoPressionado(ArrowDown)) { menuIndex = (menuIndex >= 4) ? 0 : menuIndex + 1; desenharTela(); }
    if (botaoPressionado(Enter)) {
      if(menuIndex == 0) estadoAtual = MENU_IDIOMA;
      else if(menuIndex == 1) { estadoAtual = SUBMENU_LIMITES; menuIndex = 0; }
      else if(menuIndex == 2) { estadoAtual = MENU_HISTORICO; indiceHistorico = 0; }
      else if(menuIndex == 3) estadoAtual = MENU_LIMPAR;
      else estadoAtual = MONITORAMENTO;
      desenharTela();
    }
  }
  else if (estadoAtual == SUBMENU_LIMITES) {
    if (botaoPressionado(ArrowUp))   { menuIndex = (menuIndex <= 0) ? 2 : menuIndex - 1; desenharTela(); }
    if (botaoPressionado(ArrowDown)) { menuIndex = (menuIndex >= 2) ? 0 : menuIndex + 1; desenharTela(); }
    if (botaoPressionado(Enter)) { etapaAjuste = menuIndex; editandoMin = true; estadoAtual = MENU_LIMITES; desenharTela(); }
  }
  else if (estadoAtual == MENU_LIMITES) {
    if (botaoPressionado(ArrowUp)) { 
      if(etapaAjuste == 0) { if(editandoMin) tempMin += 0.5; else tempMax += 0.5; }
      else if(etapaAjuste == 1) { if(editandoMin) humMin += 1; else humMax += 1; }
      else { if(editandoMin) luzMin += 1; else luzMax += 1; }
      desenharTela(); 
    }
    if (botaoPressionado(ArrowDown)) { 
      if(etapaAjuste == 0) { if(editandoMin) tempMin -= 0.5; else tempMax -= 0.5; }
      else if(etapaAjuste == 1) { if(editandoMin) humMin -= 1; else humMax -= 1; }
      else { if(editandoMin) luzMin -= 1; else luzMax -= 1; }
      desenharTela(); 
    }
    if (botaoPressionado(Enter)) { 
      if(editandoMin) editandoMin = false; 
      else { somSucesso(); salvarConfiguracoes(); estadoAtual = SUBMENU_LIMITES; }
      desenharTela(); 
    }
  }
  else if (estadoAtual == MENU_HISTORICO) {
    if (botaoPressionado(ArrowUp))   { indiceHistorico = (indiceHistorico <= 0) ? totalRegistros - 1 : indiceHistorico - 1; desenharTela(); }
    if (botaoPressionado(ArrowDown)) { indiceHistorico = (indiceHistorico >= totalRegistros - 1) ? 0 : indiceHistorico + 1; desenharTela(); }
  }
  else if (estadoAtual == MENU_IDIOMA) {
    if (botaoPressionado(ArrowUp))   { idioma = (idioma <= 0) ? 2 : idioma - 1; desenharTela(); }
    if (botaoPressionado(ArrowDown)) { idioma = (idioma >= 2) ? 0 : idioma + 1; desenharTela(); }
    if (botaoPressionado(Enter))     { somSucesso(); salvarConfiguracoes(); estadoAtual = MENU_PRINCIPAL; desenharTela(); }
  }
  else if (estadoAtual == MENU_LIMPAR) {
    if (botaoPressionado(Enter))     { limparMemoria(); estadoAtual = MONITORAMENTO; desenharTela(); }
  }
}

void setup() {
  Wire.begin(); 
  lcd.init(); 
  lcd.backlight(); 
  dht.begin(); 
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  // Cria os caracteres principais
  lcd.createChar(0, termo); 
  lcd.createChar(1, gota); 
  lcd.createChar(2, raio);
  
  // Configura os pinos
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_TEMP_R, OUTPUT); pinMode(LED_TEMP_G, OUTPUT);
  pinMode(LED_HUM_R, OUTPUT); pinMode(LED_HUM_G, OUTPUT);
  pinMode(LED_LUZ_R, OUTPUT); pinMode(LED_LUZ_G, OUTPUT);
  pinMode(ArrowUp, INPUT_PULLUP); pinMode(ArrowDown, INPUT_PULLUP);
  pinMode(Enter, INPUT_PULLUP); pinMode(MenuButton, INPUT_PULLUP);
  
  carregarConfiguracoes();
  
  // Exibe a animação de boot
  animacaoBoot();
  
  desenharTela();
}

void loop() {
  verificarBotoes();
  float t = dht.readTemperature(); float h = dht.readHumidity(); int l = map(analogRead(LDR), 0, 1023, 0, 100);
  bool alerta = (t < tempMin || t > tempMax || h < humMin || h > humMax || l < luzMin || l > luzMax);
  digitalWrite(LED_TEMP_R, (t < tempMin || t > tempMax)); digitalWrite(LED_TEMP_G, !(t < tempMin || t > tempMax));
  digitalWrite(LED_HUM_R, (h < humMin || h > humMax)); digitalWrite(LED_HUM_G, !(h < humMin || h > humMax));
  digitalWrite(LED_LUZ_R, (l < luzMin || l > luzMax)); digitalWrite(LED_LUZ_G, !(l < luzMin || l > luzMax));
  if (alerta) { somAlerta(); if (!alertaAtivo) { salvarRegistro(); alertaAtivo = true; } } else alertaAtivo = false;
  static unsigned long lastUpdate = 0;
  if (estadoAtual == MONITORAMENTO && millis() - lastUpdate > 2000) { desenharTela(); lastUpdate = millis(); }
}