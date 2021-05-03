#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Mqtt_functions.h>
#include "utilities.h"

unsigned long lastPublish;
int msgCount;

WiFiClientSecure wiFiClient;
PubSubClient pubSubClient(AWS_ENDPOINT, MQTT_PORT, msgReceived, wiFiClient);

// xxxxxxxxxx-certificate.pem.crt
const char* certificate_pem_crt = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDWjCCAkKgAwIBAgIVAIREVAPJoTPHMg5swTlYH+yDh5R+MA0GCSqGSIb3DQEB\n" \
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n" \
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMTAzMTEwOTQw\n" \
"MzFaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n" \
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC4uCDrYZMGl+LtUmvF\n" \
"l7qIeiOGxP3+M9US5Wtale0xSjLU6Y4yUPxB3O+EUkowNaQh3lBtbZig5e7JKnQ9\n" \
"svnZHwHmbZew6D0dbgbZE5ikp+nrwWxGKo5vwBZSRtiEFI+nbK6L6Oqif9Wew0m4\n" \
"0rsPPholDhYCGzJrdurc2gzzGFC/xzKMUNPDfmdXj2c2cm5smIz769WajxGpUzGF\n" \
"KEfSvp6rqWsbmvVK+v4aR77XXz9yH1hVSd5oUJFlPclqujLfdNN9S8K3pBMnps3A\n" \
"CTuNfcy7PhTGM//8b7Q7PQ+nLl/vlJed1lOwWjHxIWafGfxcAjf8I9n6M4plBaQY\n" \
"kpHdAgMBAAGjYDBeMB8GA1UdIwQYMBaAFNV7L6Qkq7lnAZNW4CA3YC6sBRzJMB0G\n" \
"A1UdDgQWBBRGjEI0JF1p3AgutcGBDWuqvbsGxDAMBgNVHRMBAf8EAjAAMA4GA1Ud\n" \
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAbLHy23GeST8VymHN1TWe6QFh\n" \
"joB6bPo4YFDEeJnNkW2azynaxIG6vkl+EwfrNWBeECv4MJdZ6u6K5TvQDzCRvA1k\n" \
"2/uQRNpOLA2cqu3mkTwSWYcu6qr3llkSeInxFJPUd2WCqh3fbDOq2Jb/7uuvrRde\n" \
"E5h4ZoEVuos8FBrcC2XvS8z+dYMqtXGBnVshxT9yrjcGjsS70T8u0xjwDtVzdTgY\n" \
"C46QaJqDbs+jN1VlwEpgwaJshSySNF0KcbePvetuvV0vyIxK9a9FD3QCRrZLcG2H\n" \
"iAoJ3TTL9ZUy8YQ5pjLwURbg19jFXaeA0bCjPi6lAosev0OUUjhphir9bBEV1w==\n" \
"-----END CERTIFICATE-----\n";

// xxxxxxxxxx-private.pem.key
const char* private_pem_key = \
"-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIEowIBAAKCAQEAuLgg62GTBpfi7VJrxZe6iHojhsT9/jPVEuVrWpXtMUoy1OmO\n" \
"MlD8QdzvhFJKMDWkId5QbW2YoOXuySp0PbL52R8B5m2XsOg9HW4G2ROYpKfp68Fs\n" \
"RiqOb8AWUkbYhBSPp2yui+jqon/VnsNJuNK7Dz4aJQ4WAhsya3bq3NoM8xhQv8cy\n" \
"jFDTw35nV49nNnJubJiM++vVmo8RqVMxhShH0r6eq6lrG5r1Svr+Gke+118/ch9Y\n" \
"VUneaFCRZT3Jaroy33TTfUvCt6QTJ6bNwAk7jX3Muz4UxjP//G+0Oz0Ppy5f75SX\n" \
"ndZTsFox8SFmnxn8XAI3/CPZ+jOKZQWkGJKR3QIDAQABAoIBABipjMugdW7V7Jhe\n" \
"/sCc52y9Uez2dCryZKpnMxQfHFIOZAcFP9T79xIcG2aBTiJtpwJWmkph7ZTd3inB\n" \
"n091i+IvP6OyUiRufWpmfEeTVDf3q0Wsx3X+zJGjBdfqWZIY5K0n144ahOk5RB40\n" \
"WK1yhUMduYPlA484blzmvajK/Bugk2UWo0ihAsysIBIWmEyPCWfB0punKveOK60T\n" \
"fY7jy3et9Yf4BeRnsVWyUA3IJuVYiqEW2qXojJaOfPZwG47aCGBlGiEVmt/fhlAZ\n" \
"zVH2HrAH72p0G4oWKNPCwqcLIWZDfdvsvWAu15v6z44hb5kR4GflYAY4kMWr0Jdg\n" \
"v33UyMkCgYEA2/7HP7y2AVnPZ7V72n3PQLNRpsSIge5ByTv4+Y+gepfQ0mX9EuyZ\n" \
"gETiD9ztZNmLehIbKlzaIkIpbNDJ5mhWh6bkAX2u1Y5RsPiLADEmajxNxZzzi0ma\n" \
"dab9CrnELED4pvVhlvROxNSmTwjrD7ngTilEtEc1ZFkq5m+kPI9wIQMCgYEA1vNh\n" \
"MDbBwCpnEBHCuuntBlIFli8k37wvOqPLis3gcLwHN1r8R2+eG4PL3+ag9VyUDLb7\n" \
"0di+xfJ0C3M+Z6IWN+9wJ2yEDx1uhgF2z0MRYENcKexGf/F2K8brTnaYTChVjP3e\n" \
"PGDZMQkn7wzuJAgdfe45+gjS3sW0fT7kzVVmW58CgYEAj8MX29vrKEjTU5dyX161\n" \
"sDc+lXd7KUVUcjehxRwJp/NLwCJr1hENYpghDnK2I6BSGCx62h0hYx/PSE/MkKgA\n" \
"W9dAyoSiWuYNFlPlKfP0gZn0zL17Hbu0l0vPslGhE7ZFh311D5uJWfEI2T8ftQAn\n" \
"x4wKXLDoADKQ6VNthTHK+ZcCgYBPsl9ni/XcdRxPZfZobRFX1ZOCsqoeVlzAhjiS\n" \
"l3DO+7hFOvPs3KKnmv89Xfpx3sh5ymKl1JIvPIzwLeOGcx9/z8Qqqd5w6D2xa9uM\n" \
"T1ai9Xa3iIY/K9zfsFpQdkJTgmTt6La8dkAxIS7wQEAPlf6xUWGnphvhQzZ/dZFJ\n" \
"3I8eCwKBgE8zqGq+jJCKBnKyaB27EdhUqm/qoJ9AuGqlDS7+COz6fkNmq0EAzr72\n" \
"37C3qgPIn+OaUdnBIcrFF4HtElMTQ6yORiS//ZWYVd5DIpa49oymlwKqOqzgOIK1\n" \
"o8dgY1sjQt5QkpaJ1al9jeXA+WUx7zrwZzsqB8vJxLxGKxt7rPsf\n" \
"-----END RSA PRIVATE KEY-----\n";

const char* rootCA = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n" \
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n" \
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n" \
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n" \
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n" \
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n" \
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n" \
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n" \
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n" \
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n" \
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n" \
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n" \
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n" \
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n" \
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n" \
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n" \
"rqXRfboQnoZsG4q5WTP468SQvvG5\n" \
"-----END CERTIFICATE-----\n";

void setup() {
    Serial.begin(BAUD_RATE);
    delay(50);
    Serial.println();
    Serial.println("ESP-Publisher");
    Serial.println("ESP32 AWS IoT Example");
    Serial.printf("SDK version: %s\n", ESP.getSdkVersion());

    Serial.print("Connecting to ");
    Serial.print(SSID);
    WiFi.begin(SSID, WiFi_PASS);
    WiFi.waitForConnectResult();
    Serial.print(", WiFi connected, IP address: ");
    Serial.println(WiFi.localIP());

    wiFiClient.setCACert(rootCA);
    wiFiClient.setCertificate(certificate_pem_crt);
    wiFiClient.setPrivateKey(private_pem_key);
}

void loop() {

    publisherCheckConnect(pubSubClient, AWS_ENDPOINT);

    if (millis() - lastPublish > 10000) {
        String msg = String("Hello from ESP-Publisher: ") + ++msgCount;

        /** Testing **/

        // Define arrays
        uint8_t iv[16];
        uint8_t tag[16];
        uint8_t plaintext[msg.length()];
        uint8_t ciphertext[msg.length()];

        // Copy message to plaintext as uint8_t
        charToUint8(msg.c_str(), plaintext, (int) msg.length());

        // Perform encryption
        performEncryption(1, plaintext, (int) msg.length(), ciphertext, tag, iv);

        // Concatenate to one big message containing iv + tag + ciphertext
        uint8_t concatenatedMessage[sizeof(iv) + sizeof (tag) + sizeof(ciphertext)];
        int concatenatedMessageSize = sizeof(iv) + sizeof (tag) + sizeof(ciphertext);
        AssembleAuthenticatedEncryptionPacket(iv, tag, 16, ciphertext, concatenatedMessage, concatenatedMessageSize);

        /** Testing **/
        // Public message to encryption topic
        boolean rc = pubSubClient.publish("Encryption", concatenatedMessage, concatenatedMessageSize);
        Serial.print("Published, rc="); Serial.print( (rc ? "OK: " : "FAILED: ") );
        Serial.println(msg);
        lastPublish = millis();
    }
}
