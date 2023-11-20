#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "MQTTClient.h"

#define ADDRESS     "35.212.161.165"
#define CLIENTID    "Laptop"
#define TOPIC       "home/project-2"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

int main(int argc, char* argv[]) {
    printf("Type '1' to send payload 'ON'\nType '2' to send payload 'OFF'\nType '3' to quit\n");

    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc; // response code
    if ((rc = MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to create client, rc=%d\n", rc);
        exit(EXIT_FAILURE);
    }
    conn_opts.keepAliveInterval = 100; // keep alive to 5 seconds
    conn_opts.cleansession = 1; // make it clean
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, rc=%d\n", rc);
        exit(EXIT_FAILURE);
    }

    MQTTClient_message msg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    char ch = '0';
    while (ch != '3') {
        if (kbhit()) {
            ch = getch(); // char entered
            char* payload_msg;
            if (ch == '1') {
                payload_msg = "ON";
            } else if (ch == '2') {
                payload_msg = "OFF";
            } else {
                continue;
            }
            msg.payload = payload_msg;
            msg.payloadlen = (int) strlen(payload_msg);
            msg.qos = QOS;
            msg.retained = 0;
            if ((rc = MQTTClient_publishMessage(client, TOPIC, &msg, &token)) != MQTTCLIENT_SUCCESS) {
                printf("Failed to publish message, rc=%d\n", rc);
                exit(EXIT_FAILURE);
            }
            rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
            printf("Sending payload '%s'\n", payload_msg);
        }
    }
    if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS) printf("Failed to disconnect, rc=%d\n", rc);
    MQTTClient_destroy(&client);
    return rc;
}