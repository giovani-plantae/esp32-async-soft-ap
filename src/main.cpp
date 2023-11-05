#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncDNSServer.h>
#include <WiFi.h>
#include "config.h"

AsyncDNSServer DNS;
AsyncServer server(TCP_SERVER_PORT);

/**
 * @brief Handles received data from a client.
 * @param client The client that sent the data.
 * @param data The received data.
 * @param len The size of the received data.
 */
void handleData(void* arg, AsyncClient* client, void* data, size_t len);

/**
 * @brief Handles connection errors with the client.
 * @param client The client with the error.
 * @param error The error code.
 */
void handleError(void* arg, AsyncClient* client, int8_t error);

/**
 * @brief Handles the disconnection of a client.
 * @param client The client that got disconnected.
 */
void handleDisconnect(void* arg, AsyncClient* client);

/**
 * @brief Handles the ACK timeout of a client.
 * @param client The client with a timeout.
 * @param time The timeout time.
 */
void handleTimeout(void* arg, AsyncClient* client, uint32_t time);

/**
 * @brief Handles the connection of a new client.
 * @param client The new connected client.
 */
void handleClientConnection(void* arg, AsyncClient* client);

/**
 * @brief Sets up the server and starts listening for new clients.
 */
void setupServer();

/**
 * @brief Configures the DNS server for local resolution.
 */
void setupDNS();

void setup() {
    Serial.begin(115200);
    WiFi.softAP(NETWORK_SSID, NETWORK_PASS);
    setupDNS();
    setupServer();
}

void loop() {}

void setupDNS() {
    DNS.setTTL(300);
    DNS.setErrorReplyCode(AsyncDNSReplyCode::ServerFailure);
    DNS.start(DNS_PORT, DNS_HOSTNAME, WiFi.softAPIP());
}

void setupServer() {
    server.onClient(&handleClientConnection, NULL);
    server.begin();
}

void handleClientConnection(void* arg, AsyncClient* client) {
    Serial.printf("\n New client connected, IP: %s", client->remoteIP().toString().c_str());

    client->onData(&handleData, NULL);
    client->onError(&handleError, NULL);
    client->onDisconnect(&handleDisconnect, NULL);
    client->onTimeout(&handleTimeout, NULL);
}

void handleData(void* arg, AsyncClient* client, void* data, size_t len) {
    Serial.printf("\n Data received from client %s \n", client->remoteIP().toString().c_str());
    Serial.write((uint8_t*)data, len);

    String payload = "{\"connection\": true}";

    if (client->space() > payload.length() && client->canSend()) {
        client->add(payload.c_str(), payload.length());
        client->send();
    }
}

void handleError(void* arg, AsyncClient* client, int8_t error) {
    Serial.printf("\n Connection error %s from client %s \n", client->errorToString(error), client->remoteIP().toString().c_str());
}

void handleDisconnect(void* arg, AsyncClient* client) {
    Serial.printf("\n Client %s disconnected \n", client->remoteIP().toString().c_str());
}

void handleTimeout(void* arg, AsyncClient* client, uint32_t time) {
    Serial.printf("\n Client ACK timeout IP: %s \n", client->remoteIP().toString().c_str());
}
