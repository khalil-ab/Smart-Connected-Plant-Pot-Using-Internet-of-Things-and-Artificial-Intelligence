#include <ESP8266WiFi.h>
#include <PubSubClient.h>  
#include <dht.h>

dht DHT;
#define DHT22_PIN D6

const char* ssid ="AndroidAP";
const char* password ="*****";
const char* mqtt_server ="192.168.43.77";// addresse ip de raspberry

WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];
float temp;
float hum;
int soilValue;

const int RED = 1;
const int GREEN = 2;
const int BLUE = 3;

void setup() 
{
  setup_wifi();
  client.setServer(mqtt_server,1883);
  //pinMode(D2, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  //client.setCallback(callback);
  pinMode(D0, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D4, OUTPUT);
  
  Serial.begin(9600);
}

void setup_wifi() 
{
  delay(10);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }
}

/*void callback(char* topic, byte* payload, unsigned int length) 
{
  if ((char)payload[0] == '0') 
  {
    digitalWrite(D2, LOW);   // Turn the LED on (Note that LOW is the voltage level
  } 

 if ((char)payload[0] == '1') 
 {
    digitalWrite(D2, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}*/

void reconnect() 
{
  while (!client.connected()) 
  {
    if (client.connect("ESP8266Client")) 
    {
      //client.publish("temperature", "hello world");
      //client.subscribe("temperature");
    } 
    else 
    {
      delay(3000);
    }
  }
}

void loop() 
{
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();

    delay(500);
    
    uint32_t start = micros();
    int chk = DHT.read22(DHT22_PIN);
    uint32_t stop = micros();
    
    temp = DHT.temperature;
    //snprintf (msg, 75, "Temperature %.2f",temp);
    snprintf (msg,5,"%.2f",temp);
    client.publish("temp",msg);
    //Serial.print("Temperature : ");
    //Serial.println(temp);

    if(temp > 28)
    {
    digitalWrite(D0, HIGH);//red
    delay (1000);
    digitalWrite(D0, LOW);//red
    }

    if(temp < 12)
    {
    digitalWrite(D4, HIGH);//blue  
    delay (1000);
    digitalWrite(D4, LOW);//blue  
    }
    
  
    hum = DHT.humidity;
    //snprintf (msg, 75, "Humidite %.2f",hum);
    snprintf (msg,5,"%.2f",hum);
    client.publish("hum",msg);
    //Serial.print("Humidity air : ");
    //Serial.println(hum);
  
    //snprintf (msg, 75, "Soil humidity %d",soilValue);
    soilValue = analogRead(0);
    soilValue=100-((soilValue*100)/1024);
    snprintf (msg,5,"%d",soilValue);
    client.publish("soil",msg);
    
    if(soilValue <20)
    {
    digitalWrite(D2, HIGH);//green
    delay (1000);
    digitalWrite(D2, LOW);//green
    }
  
  //Serial.print("Soil value : ");  
    //Serial.println(soilValue);  
    
    //Serial.println("");  
    //delay(2000);
}
