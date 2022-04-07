#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h> // Las comillas indica que la libraria está en la misma carpeta del sketck

// Introduzca los datos de configuración del router WIFI
char ssid[] = "familiagarciajimenez";     // Aquí el SSID (nombre)
char password[] = "serviciosmcamj1213"; // La clave PASSWORD

// Los datos del Token del Telegram BOT proporcionado por Telegram.
#define BOTtoken "1982494445:AAE4gI9RG2IquWHOICzEYCywQm59DJO6zc0"
// Su Bot Token (Proporcionado desde Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 1000; //tiempo medio entre escaneo de mensajes – 1 segundo).
long Bot_lasttime;   //la última vez que se realizó la exploración de mensajes.
bool Start = false;

const int ledPin = 2; // Pin de configuración del Relé o LED usado
int ledStatus = 0;    // Iniciamos el estado a 0 – apagado.

// Boton1
int Boton1 = 0; // Boton1 PIN0
int flag1=0;    // flag Boton1 1 = 0 // Para saber cuando se pulsa el botón.
// Boton1

void handleNewMessages(int numNewMessages) {
  Serial.println("Nuevos mensajes"); // Para ver el estado en el puerto serie
  Serial.println(String(numNewMessages));  // Para ver el estado en el puerto serie
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";
    //————-
    //Aquí puedes añadir más comandos para tu control o modificarlos
    //————-
   
    if (text == "/ledon") {
      digitalWrite(ledPin, LOW);   // Activa, enciende el LED – on (HIGH es voltaje nivel alto)
      ledStatus = 1;
      bot.sendMessage(chat_id, "Led en ON", "");
    }
    if (text == "/ledoff") {
      ledStatus = 0;
      digitalWrite(ledPin, HIGH);    // Apaga el LED – off (LOW es voltaje nivel bajo)
      bot.sendMessage(chat_id, "Led en OFF", "");
    }
    if (text == "/estado") {        // Envia el estado del Relé/Led
      if (ledStatus) {
        bot.sendMessage(chat_id, "Led en ON", "");
      } else {
        bot.sendMessage(chat_id, "Led en OFF", "");
      }
    }
        if (text == "/start") {      //Es el mensaje que recibimos en el celular cuando conectamos
      String welcome = "Bienvenido al Bot Telegram, " + from_name + ".\n";
      welcome += "Puede usar los siguientes comandos:\n\n";
      welcome += "/ledon : Para encender el LED\n";
      welcome += "/ledoff : Para apagar el LED\n";
      welcome += "/estado : Estado del LED\n";
      bot.sendMessage(chat_id, welcome, "");
    }
  }
}

void setup() {
Serial.begin(115200);
// Boton1
pinMode(Boton1,INPUT);
// Establezca WiFi en modo estación y desconéctese de un AP si estaba conectado previamente
WiFi.mode(WIFI_STA);
WiFi.disconnect();
delay(100);
// Intenta conectarse a la red Wifi:
Serial.print("Conectando a red Wifi: ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {      // El programa se quedará en este bucle hasta que el ESP conecte con el router.
Serial.print(".");
delay(500);
}
Serial.println("");                            // Aquí continúa una vez conectado
Serial.println("WiFi conectado");
Serial.print("Dirección IP: ");
Serial.println(WiFi.localIP());
client.setInsecure(); // Si su núcleo no puede manejar conexiones seguras
pinMode(ledPin, OUTPUT); // Inicializa ledPin digital como salida.
delay(10);
digitalWrite(ledPin, HIGH); // Inicializa el Pin en OFF
}

void loop() {
// Boton1 a la conexión –> Activa el LED al pulsar a masa
// Al usar la funcion Telegram, el pulsador a veces es un poco lento de respuesta. Porque el procesador está ocupado busando mensajes nuevos. No es importante.
if(digitalRead(Boton1)==HIGH) flag1=0;     // Pongo flag a 0
delay(50);
// Comienza a leer el pulsador, si esta pulsado continúa una sola vez aunque se mantenga pulsado.
if(digitalRead(Boton1)==LOW && flag1==0){
delay(250);
if (ledStatus==1){
ledStatus=0;
}
else {
ledStatus=1;
}
flag1=0;                // flag a 1
digitalWrite(2, ledStatus); // Escribimos en el Pin2 el valor ledStatus.
}
// Boton1
// Serial.println(flag1); // Para probar esta variable y ver respuesta
if (millis() > Bot_lasttime + Bot_mtbs)  {
int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
while (numNewMessages) {
Serial.println("Obtuvo respuesta");
handleNewMessages(numNewMessages);
numNewMessages = bot.getUpdates(bot.last_message_received + 1);
}
Bot_lasttime = millis();
}
}
