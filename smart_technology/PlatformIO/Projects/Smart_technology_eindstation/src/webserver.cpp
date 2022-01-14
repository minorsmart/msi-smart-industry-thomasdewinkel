#include "webserver.hpp"

#define PORT 5000

#define RFTRANSMITTERID 47115
#define DHTTYPE DHT11
#define DHTPIN 32

unsigned long currentTimeTestRf = millis();
unsigned long previousTimeTestRf = 0;
bool previousTimeTestRfStart = false;

const unsigned long testInterval = 1000;

String ip = "";
float currentTemperature = 0;
int roomtemperature = 16;

int rfPin = 25;

String testRfState = "off";

WiFiServer server(PORT);
WebServer localserver(80);

WiFiClient client;

const char *serverWeather = "http://192.168.43.74:5000/postweatherdata";
const char *serverNameTemperature = "http://192.168.43.74:5000/gettemperature";

String weatherData;

DHT dht(DHTPIN, DHTTYPE);

RfTransmitter rfTransmitter = RfTransmitter(rfPin, RFTRANSMITTERID);
DhtSensor dhtSensor = DhtSensor(DHTPIN, DHTTYPE, dht);

Webserver::Webserver(char *ssid, char *password) : ssid(ssid), password(password)
{
}

Webserver::~Webserver()
{
}

void Webserver::setupWebserver()
{
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();

    localserver.on("/update", gettemperature);
    localserver.begin();

    dhtSensor.setupDht();
}

void Webserver::startWebserver()
{
    client = server.available(); // Listen for incoming clients
    localserver.handleClient();

    previousTimeTestRfStart = true;

    if (previousTimeTestRfStart)
    {

        //rfTransmitter.sendRfSignalLow(rfTransmitter.getRfPin(), rfTransmitter.getTransmitterId(), 1, 1, 3);
        previousTimeTestRf = millis();
        previousTimeTestRfStart = false;
    }

    while (true)
    {
        testRfState = "off";
        currentTimeTestRf = millis();

        if (currentTimeTestRf - previousTimeTestRf >= testInterval)
        {

            Serial.println("temperature: " + (String)dhtSensor.getTemperature());

            if (WiFi.status() == WL_CONNECTED)
            {
                WiFiClient client;
                HTTPClient http;

                // Your Domain name with URL path or IP address with path
                http.begin(client, serverWeather);

                http.addHeader("Content-Type", "application/json");
                ip = WiFi.localIP().toString();
                currentTemperature = dhtSensor.getTemperature();

                http.POST("{\"ip\":\"" + ip + "\",\"temperature\":\"" + currentTemperature + "\"}");

                //Serial.print("HTTP Response code: ");
                //Serial.println(httpResponseCode);

                // Free resources
                http.end();
            }
            else
            {
                Serial.println("WiFi Disconnected");
            }

            previousTimeTestRfStart = 0;
            break;
        }
    }

    if (roomtemperature != 16)
    {
        if (currentTemperature <= roomtemperature + 0.2)
        {
            rfTransmitter.sendRfSignalHigh(rfTransmitter.getRfPin(), rfTransmitter.getTransmitterId(), 1, 1, 3);
        }
        else
        {
            rfTransmitter.sendRfSignalLow(rfTransmitter.getRfPin(), rfTransmitter.getTransmitterId(), 1, 1, 3);
        }
    }
}

void Webserver::gettemperature()
{
    String s = "<HTML><HEAD><TITLE>My first web page</TITLE></HEAD><BODY><CENTER><B>Get temperature</B></CENTER></BODY></HTML>"; //Read HTML contents
    localserver.send(200, "text/html", s);

    HTTPClient http;
    http.begin(serverNameTemperature);
    int httpResponseCode = http.GET();
    String payload = "{}";

    if (httpResponseCode > 0)
    {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        payload = http.getString();
    }
    else
    {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();

    weatherData = payload;

    JSONVar myObject = JSON.parse(weatherData);

    Serial.print("JSON object = ");
    Serial.println(myObject);

    // myObject.keys() can be used to get an array of all the keys in the object
    JSONVar keys = myObject.keys();

    JSONVar old = myObject["temperature"];
    String newVar = old.stringify(old);

    newVar.replace("\"", "");

    roomtemperature = newVar.toInt();

    Serial.print("VALUE: ");
    Serial.println(newVar.toInt());
}
