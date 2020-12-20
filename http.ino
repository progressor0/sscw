void webServerInit(void) {
  webServer.on("/", handleRoot);
  webServer.on("/set", HTTP_POST, handleSet);
  webServer.on("/pause", handlePause);
  webServer.onNotFound(handleNotFound);
  webServer.begin();
}

void handleRoot() {
  String message = "<html><head><meta charset='utf-8'></head><body>";
  message += "<form action='/set' method='post'>";
  message += "Время (миллисекунды): <input name='time' type='nnumber' value='60000'><br/>";
  message += "Деньги (рубли): <input name='money' type='number' value='500'><br/>";
  message += "<button type='submit'>Отправить</button>";
  message += "</form>";
  message += "<a href='/pause'>Пауза/Продолжить</a>";
  message += "</body></html>";
  webServer.send(200, "text/html", message);
}

void handleSet() {
  float t = webServer.arg("time").toInt();
  float m = webServer.arg("money").toInt();
  cw_startup(m, t);
  cw_toggle_pause(); // Для отладки
  webServer.send(200, "text/plain", "OK.");
}

void handlePause() {
  cw_toggle_pause();
  webServer.send(200, "text/plain", "OK.");
}

void handleNotFound() {
  String message = "Not Found\n\n";
  message += "URI: ";
  message += webServer.uri();
  message += "\nMethod: ";
  message += (webServer.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += webServer.args();
  message += "\n";
  for (uint8_t i = 0; i < webServer.args(); i++) {
    message += " " + webServer.argName(i) + ": " + webServer.arg(i) + "\n";
  }
  webServer.send(404, "text/plain", message);
}
