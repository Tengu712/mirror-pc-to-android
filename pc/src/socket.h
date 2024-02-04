#pragma once

typedef struct Communicator_t *Communicator;

void deleteCommunicator(Communicator com);

Communicator createCommunicator(unsigned short port);

int sendData(const Communicator com, const char *data, unsigned long size);
