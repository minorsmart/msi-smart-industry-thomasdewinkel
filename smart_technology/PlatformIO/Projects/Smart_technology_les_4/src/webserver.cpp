#include "webserver.hpp"

#define PORT 80
#define GET_DHT_TEMP "GET /temp"
#define GET_DHT_HUMIDITY "GET /humidity"
#define GET_MOISTURE "GET /moisture"
#define GET_LDR "GET /ldr"
#define TEST_SCREEN "GET /test"
#define TEST_SCREEN_RF_ON "GET /test/rftransmitter"
#define TEST_SCREEN_WATERPUMP_ON "GET /test/waterpump"

#define DHT_TEMP_TITLE "DHT temperature"
#define DHT_HUMIDITY_TITLE "DHT humidity"
#define MOISTURE_TITLE "Moisture"
#define LDR_TITLE "Ldr"
#define MAIN_TITLE "ESP32 Web server"
#define TEST_SCREEN_TITLE "Test screen"

#define RFTRANSMITTERID 47115
#define DHTTYPE DHT11
#define DHTPIN 32

unsigned long currentTimeWebserver = millis();
unsigned long previousTimeWebserver = 0;
const long timeoutTimeWebserver = 2000;

unsigned long currentTimeTestRf = millis();
unsigned long previousTimeTestRf = 0;
bool previousTimeTestRfStart = false;
unsigned long currentTimeTestWaterpump = millis();
unsigned long previousTimeTestWaterpump = 0;
bool previousTimeTestWaterpumpStart = false;

const unsigned long testInterval = 5000;

//Set pin number
int moistureSensorPin = 36;
int pumpPin = 33;
int rfPin = 25;
int ldrSensorPin = 39;

String header;
String currentPageHeader = "ESP32 Web server";
String currentContent = "";

String testWaterpumpState = "off";
String testRfState = "off";

WiFiServer server(PORT);
WiFiClient client;

DHT dht(DHTPIN, DHTTYPE);

RfTransmitter rfTransmitter = RfTransmitter(rfPin, RFTRANSMITTERID);
DhtSensor dhtSensor = DhtSensor(DHTPIN, DHTTYPE, dht);
Waterpump waterpump = Waterpump(pumpPin);
MoistureSensor moistureSensor(moistureSensorPin);
LdrSensor ldrSensor = LdrSensor(ldrSensorPin);

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

    waterpump.setupWaterPump();
    dhtSensor.setupDht();
    ldrSensor.setupLdrSensor();
}

void Webserver::startWebserver()
{
    client = server.available(); // Listen for incoming clients

    if (server.available())
    { // If a new client connects,
        currentTimeWebserver = millis();
        previousTimeWebserver = currentTimeWebserver;
        Serial.println("New Client."); // print a message out in the serial port
        String currentLine = "";       // make a String to hold incoming data from the client
        while (client.connected() && currentTimeWebserver - previousTimeWebserver <= timeoutTimeWebserver)
        { // loop while the client's connected
            currentTimeWebserver = millis();
            if (client.available())
            {                           // if there's bytes to read from the client,
                char c = client.read(); // read a byte, then
                //Serial.write(c);               // print it out the serial monitor
                header += c;
                if (c == '\n')
                { // if the byte is a newline character
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0)
                    {
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();

                        if (header.indexOf(TEST_SCREEN_RF_ON) >= 0)
                        {
                            currentPageHeader = TEST_SCREEN_TITLE;

                            testRfState = "on";
                            if (testRfState == "on")
                            {
                                previousTimeTestRfStart = true;

                                if (previousTimeTestRfStart)
                                {
                                    rfTransmitter.sendRfSignalHigh(rfTransmitter.getRfPin(), rfTransmitter.getTransmitterId(), 1, 1, 3);
                                    previousTimeTestRf = millis();
                                    previousTimeTestRfStart = false;
                                }
                                while (true)
                                {
                                    testRfState = "off";
                                    currentTimeTestRf = millis();

                                    if (currentTimeTestRf - previousTimeTestRf >= testInterval)
                                    {
                                        rfTransmitter.sendRfSignalLow(rfTransmitter.getRfPin(), rfTransmitter.getTransmitterId(), 1, 1, 3);
                                        previousTimeTestRfStart = 0;
                                        break;
                                    }
                                }
                            }
                        }
                        else if (header.indexOf(TEST_SCREEN_WATERPUMP_ON) >= 0)
                        {
                            currentPageHeader = TEST_SCREEN_TITLE;

                            testWaterpumpState = "on";
                            if (testWaterpumpState == "on")
                            {
                                previousTimeTestWaterpumpStart = true;

                                if (previousTimeTestWaterpumpStart)
                                {
                                    waterpump.on();
                                    previousTimeTestWaterpump = millis();
                                    previousTimeTestWaterpumpStart = false;
                                }
                                while (true)
                                {
                                    testWaterpumpState = "off";
                                    currentTimeTestWaterpump = millis();

                                    if (currentTimeTestWaterpump - previousTimeTestWaterpump >= testInterval)
                                    {
                                        waterpump.off();
                                        previousTimeTestWaterpump = 0;
                                        break;
                                    }
                                }
                            }
                        }

                        else if (header.indexOf(TEST_SCREEN) >= 0)
                        {
                            currentPageHeader = TEST_SCREEN_TITLE;
                        }
                        else if (header.indexOf(GET_DHT_TEMP) >= 0)
                        {
                            currentPageHeader = DHT_TEMP_TITLE;
                        }
                        else if (header.indexOf(GET_DHT_HUMIDITY) >= 0)
                        {
                            currentPageHeader = DHT_HUMIDITY_TITLE;
                        }
                        else if (header.indexOf(GET_MOISTURE) >= 0)
                        {
                            currentPageHeader = MOISTURE_TITLE;
                        }
                        else if (header.indexOf(GET_LDR) >= 0)
                        {
                            currentPageHeader = LDR_TITLE;
                        }
                        else
                        {
                            currentPageHeader = MAIN_TITLE;
                        }

                        displayWebpage();
                        break;
                    }
                    else
                    { // if you got a newline, then clear currentLine
                        currentLine = "";
                    }
                }
                else if (c != '\r')
                {                     // if you got anything else but a carriage return character,
                    currentLine += c; // add it to the end of the currentLine
                }
            }
        }
        // Clear the header variable
        header = "";
        // Close the connection
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
    }
}

void Webserver::displayWebpage()
{
    // Display the HTML web page
    client.println("<!DOCTYPE html><html>");
    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/css/bootstrap.min.css\">");
    // CSS to style the on/off buttons
    // Feel free to change the background-color and font-size attributes to fit your preferences

    // Web Page Heading
    client.println("<body><h1>" + currentPageHeader + "</h1>");

    if (currentPageHeader == TEST_SCREEN_TITLE)
    {
        client.println("<div class=\"btn-group-vertical\">");

        if (testRfState == "off")
        {
            client.println("<a href=\"/test/rftransmitter\" class=\"btn btn-primary btn-lg active\" role=\"button\" aria-pressed=\"false\">RF transmitter on</a>");
        }
        else
        {
            client.println("<a href=\"#\" class=\"btn btn-primary btn-lg active\" role=\"button\" aria-pressed=\"true\" disabled>RF transmitter on</a>");
        }

        if (testWaterpumpState == "off")
        {
            client.println("<a href=\"/test/waterpump\" class=\"btn btn-primary btn-lg active\" role=\"button\" aria-pressed=\"false\">Waterpump on</a>");
        }
        else
        {
            client.println("<a href=\"#\" class=\"btn btn-primary btn-lg active\" role=\"button\" aria-pressed=\"true\" disabled>Waterpump on</a>");
        }

        client.println("</div>");
    }
    else if (currentPageHeader == DHT_TEMP_TITLE)
    {
        client.println("<h3>");
        client.println("Temperature: " + (String)dhtSensor.getTemperature() + "</h3>");
    }
    else if (currentPageHeader == DHT_HUMIDITY_TITLE)
    {
        client.println("<h3>");
        client.println("Humidity: " + (String)dhtSensor.getHumidity() + "</h3>");
    }
    else if (currentPageHeader == MOISTURE_TITLE)
    {
        client.println("<h3>");
        client.println("Moisture: " + (String)moistureSensor.getMoistSensorValue() + "</h3>");
    }
    else if (currentPageHeader == LDR_TITLE)
    {
        client.println("<h3>");
        client.println("Ldr: " + (String)ldrSensor.getLdrSensorValue() + "</h3>");
    }

    client.println("</body></html>");

    // The HTTP response ends with another blank line
    client.println();
    // Break out of the while loop
}
