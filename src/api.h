#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
String baseUrl = "http://192.168.1.11:2000";
String user[2];
String candidate[2];

void getUser(String uid)
{
    Serial.println("\n\n");
    HTTPClient http;
    WiFiClient client;

    String url = baseUrl + "/api/user?uid=" + uid;

    bool beginResult = http.begin(client, url);

    // Debug prints
    Serial.print("HTTP begin result: ");
    Serial.println(beginResult);
    Serial.println("Connecting to: " + url);

    // Add header after http.begin()
    http.addHeader("Content-Type", "application/json");

    // Send HTTP POST request
    int httpResponseCode = http.GET();
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode > 0)
    {
        String response = http.getString();
        Serial.print("response");
        Serial.println(response);
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, response);
        String status = doc["status"];
        String nama = doc["nama"];
        user[0] = status;
        user[1] = nama;
    }
    else
    {
        Serial.print("Error on sending POST: ");
        Serial.println(http.errorToString(httpResponseCode));
    }
    http.end();
}

void registerUid(String uid)
{
    Serial.println("\n\n");
    HTTPClient http;
    WiFiClient client;

    DynamicJsonDocument doc(1024);
    doc["uid"] = uid;
    String jsonString;
    serializeJson(doc, jsonString);

    // Debug print to check payload
    Serial.print("Serialized JSON: ");
    Serial.println(jsonString);

    String url = baseUrl + "/api/register";

    bool beginResult = http.begin(client, url);

    // Debug prints
    Serial.print("HTTP begin result: ");
    Serial.println(beginResult);
    Serial.println("Connecting to: " + url);
    Serial.println("Payload: " + jsonString);

    // Add header after http.begin()
    http.addHeader("Content-Type", "application/json");

    // Send HTTP POST request
    int httpResponseCode = http.POST(jsonString);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode > 0)
    {
        Serial.println(http.getString());
    }
    else
    {
        Serial.print("Error on sending POST: ");
        Serial.println(http.errorToString(httpResponseCode));
    }
    http.end();
}

void createResult(String selection, String uid)
{
    Serial.println("\n\n");
    HTTPClient http;
    WiFiClient client;

    DynamicJsonDocument doc(1024);
    doc["selection"] = selection;
    doc["uid"] = uid;
    String jsonString;
    serializeJson(doc, jsonString);

    // Debug print to check payload
    Serial.print("Serialized JSON: ");
    Serial.println(jsonString);

    String url = baseUrl + "/api/post/result";

    bool beginResult = http.begin(client, url);

    // Debug prints
    Serial.print("HTTP begin result: ");
    Serial.println(beginResult);
    Serial.println("Connecting to: " + url);
    Serial.println("Payload: " + jsonString);

    // Add header after http.begin()
    http.addHeader("Content-Type", "application/json");

    // Send HTTP POST request
    int httpResponseCode = http.POST(jsonString);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode > 0)
    {
        Serial.println(http.getString());
    }
    else
    {
        Serial.print("Error on sending POST: ");
        Serial.println(http.errorToString(httpResponseCode));
    }
    http.end();
}