#include "socket.h"

#include <ws2tcpip.h>

#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <winsock2.h>

typedef struct Communicator_t {
    SOCKET sock;
} *Communicator;

void deleteCommunicator(Communicator com) {
    if (com == NULL) {
        return;
    }
    if (com->sock != INVALID_SOCKET) {
        closesocket(com->sock);
    }
    WSACleanup();
    free((void *)com);
}

Communicator createCommunicator(unsigned short port) {
    Communicator com = (Communicator)malloc(sizeof(struct Communicator_t));
    com->sock = INVALID_SOCKET;

    // start Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("[ error ] createCommunicator(): failed to start Winsock.\n");
        return NULL;
    }

    // create a socket
    com->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (com->sock == INVALID_SOCKET) {
        printf("[ error ] main(): failed to create a socket: %d\n", WSAGetLastError());
        deleteCommunicator(com);
        return NULL;
    }

    // connect
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(addr.sin_family, "127.0.0.1", &addr.sin_addr.S_un.S_addr);
    if (connect(com->sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        printf("[ error ] main(): failed to connect.\n");
        deleteCommunicator(com);
        return NULL;
    }

    // finish
    return com;
}

int sendData(const Communicator com, const char *data, unsigned long size) {
    if (send(com->sock, data, size, 0) == SOCKET_ERROR) {
        printf("[ warning ] sendData(): failed to send data: %d\n", WSAGetLastError());
        return 0;
    } else {
        return 1;
    }
}
