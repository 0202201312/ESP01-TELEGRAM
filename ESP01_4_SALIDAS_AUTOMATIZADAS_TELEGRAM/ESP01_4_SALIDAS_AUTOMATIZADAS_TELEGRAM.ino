// Telegram bot library by Brian: https://github.com/sponsors/witnessmenow/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>


char ssid[] = "familiagarciajimenez";     // your network SSID (name)
char password[] = "serviciosmcamj1213"; // your network key

// Initialize Telegram BOT
#define BOTtoken "1982494445:AAE4gI9RG2IquWHOICzEYCywQm59DJO6zc0" // your Bot Token (Get from Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 500;
unsigned long lastTimeBotRan;

const int r1 = 0;
const int r2 = 1;
const int r3 = 2;
const int r4 = 3;
bool r1Status = LOW;
bool r2Status = LOW;
bool r3Status = LOW;
bool r4Status = LOW;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/encender_todos") {
      bot.sendMessage(chat_id, "Bombillos Encendidos", "");
      digitalWrite(r1,HIGH);
      delay(2000);
      r1Status = LOW;
      digitalWrite(r1, r1Status);
      digitalWrite(r2,HIGH);
      delay(2000);                    
      r2Status = LOW;
      digitalWrite(r2, r2Status);
      digitalWrite(r3,HIGH);
      delay(2000);
      r3Status = LOW;
      digitalWrite(r3, r3Status);
      digitalWrite(r4,HIGH);
      delay(2000);
      r4Status = LOW;
      digitalWrite(r4, r4Status);
     
    }

    if (text == "/apagar_todos") {
      bot.sendMessage(chat_id, "Bombillos apagados", "");
      digitalWrite(r1,LOW);
      delay(2000);
      r1Status = HIGH;
      digitalWrite(r1, r1Status);
      digitalWrite(r2,LOW);
      delay(2000);                    
      r2Status = HIGH;
      digitalWrite(r2, r2Status);
      digitalWrite(r3,LOW);
      delay(2000);
      r3Status = HIGH;
      digitalWrite(r3, r3Status);
      digitalWrite(r4,LOW);
      delay(2000);
      r4Status = HIGH;
      digitalWrite(r4, r4Status);
      
    }

    if (text == "/r1on") {
      bot.sendMessage(chat_id, "Luz1 Encendida", "");
      r1Status = LOW;
      digitalWrite(r1, r1Status);
      
      
    }

    if (text == "/r1off") {
      bot.sendMessage(chat_id, "Luz1 Apagada", "");
      r1Status = HIGH;
      digitalWrite(r1, r1Status);
      
    }

    if (text == "/r2on") {
      digitalWrite(r2,LOW);
      r2Status = 1;
      bot.sendMessage(chat_id, "Luz2 Encendida", "");
    }

    if (text == "/r2off") {
      r2Status = 0;
      digitalWrite(r2, HIGH);
      bot.sendMessage(chat_id, "Luz2 Apagada", "");
    }

    if (text == "/r3on") {
      digitalWrite(r3,LOW);
      r3Status = 1;
      bot.sendMessage(chat_id, "Luz3 Encendida", "");
    }

    if (text == "/r3off") {
      r3Status = 0;
      digitalWrite(r3, HIGH);
      bot.sendMessage(chat_id, "Luz3 Apagada", "");
    }
    
    if (text == "/r4on") {
      digitalWrite(r4,LOW);
      r4Status = 1;
      bot.sendMessage(chat_id, "Luz4 Encendida", "");
    }

    if (text == "/r4off") {
      r4Status = 0;
      digitalWrite(r4, HIGH);
      bot.sendMessage(chat_id, "Luz4 Apagada", "");
    }

    if (text == "/estado") {
      if (digitalRead(r1)){
        bot.sendMessage(chat_id, "Luz1 Apagada", "");
      }
      else{
        bot.sendMessage(chat_id, "Luz1 Encendida", "");
      }
      if (digitalRead(r2)){
        bot.sendMessage(chat_id, "Luz2 Apagada", "");
      }
      else{
        bot.sendMessage(chat_id, "Luz2 Encendida", "");
      }
      if (digitalRead(r3)){
        bot.sendMessage(chat_id, "Luz3 Apagada", "");
      }
      else{
        bot.sendMessage(chat_id, "Luz3 Encendida", "");
      }
      if (digitalRead(r4)){
        bot.sendMessage(chat_id, "Luz4 Apagada", "");
      }
      else{
        bot.sendMessage(chat_id, "Luz4 Encendida", "");
      }
    }

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/r1on encender luz \n";
      welcome += "/r1off apagar luz \n";
      welcome += "/r2on encender luz \n";
      welcome += "/r2off apagar luz \n";
      welcome += "/r3on encender luz \n";
      welcome += "/r3off apagar luz \n";
      welcome += "/r4on encender luz \n";
      welcome += "/r4off apagar luz \n";
      welcome += "/estado de los elementos \n";
      welcome += "/encender_todos los elementos \n";
      welcome += "/apagar_todos los elementos \n";
      bot.sendMessage(chat_id, welcome, "");
    }
  }
}

void setup() {
  Serial.begin(115200);

  client.setInsecure();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);
  pinMode(r4, OUTPUT);
  delay(10);
  digitalWrite(r1, HIGH);
  digitalWrite(r2, HIGH);
  digitalWrite(r3, HIGH);
  digitalWrite(r4, HIGH);
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTimeBotRan = millis();
  }
}
