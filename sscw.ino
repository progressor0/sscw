#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

// TODO: Работа с дисплеем
// #include <SPI.h>
// const byte CS = ?;
// const byte CLK = ?;

const byte RELAY_PIN = LED_BUILTIN;
const byte BUTTON_PIN = 4;

// Статусы
const byte STANDBY  = 0; // Ожидание
const byte PAUSED   = 1; // Пауза
const byte ACTIVE   = 2; // Работа

// Начальные значения
byte state = STANDBY;
long timeLeft;
float moneyLeft, price;

ESP8266WebServer webServer (80);

unsigned long timer;
const int period = 500;

void cw_reset() {
  state = STANDBY;
  moneyLeft = 0;
  timeLeft = 0;
  price = 0;
  digitalWrite(RELAY_PIN, HIGH);
}

void cw_startup(float m, long t) {
  timeLeft = t;
  moneyLeft = m;
  price = m / (float) t;
  state = PAUSED;
  digitalWrite(RELAY_PIN, HIGH);
}

void cw_toggle_pause() {
  if (state == PAUSED) {
    state = ACTIVE;
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("--- ACTIVE ---");
  } else if (state == ACTIVE) {
    state = PAUSED;
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("--- PAUSED ---");
  }
  // TODO: Добавить таймер для паузы. 
  // Переходить в режим STANDBY через несколько минут простоя.
}

void setup() {
  Serial.begin(115200);
  
  WiFiManager wifiManager;
  wifiManager.autoConnect("CarWash");

  webServerInit();
  
  pinMode(RELAY_PIN, OUTPUT);
  // pinMode(BUTTON_PIN, INPUT_PULLUP);
 
  cw_reset();
}

void loop() {
  // TODO: Обработка нажатий кнопки пауза/старт
  
  if (millis() - timer >= period) {
    if (state == ACTIVE) {
      timeLeft -= period;
      moneyLeft = price * (float) timeLeft;
      if (timeLeft <= 0) {
        cw_reset();
      }
      Serial.print("Time left: ");
      Serial.print(timeLeft);
      Serial.println();
      Serial.print("Money left: ");
      Serial.print(floor(moneyLeft));
      Serial.println();
      // TODO: Вывод данных на дисплей
    }
    timer = millis();
  }

  webServer.handleClient();
}
