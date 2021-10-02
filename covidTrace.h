#ifndef COVIDTRACE_H_
#define COVIDTRACE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <inttypes.h>
#include <signal.h>

int num; 
int maxindex;
int sleepTime;

uint64_t covidTestTime; 
uint64_t forgetTime;
uint64_t minTime;
uint64_t maxTime;

volatile sig_atomic_t exitRequest;


// Mac Address struct to store incoming macs
typedef struct macaddresses {
    char *mac;
    uint64_t time;
    bool far;
} macaddress;

// Structs used as arguments passed to thread functions
typedef struct workerArgs {
    macaddress *address;
    macaddress *addresses;
} workerArgs;

typedef struct writerArgs {
    FILE *ptr;
    uint64_t time;
} writerArgs;

typedef struct uploaderArgs {
    macaddress *addresses;
    uint64_t time;
} uploaderArgs;

// Function definitions
macaddress BTnearMe(char *addresses[]);

bool testCOVID();

void *workerThread(void *data);

void *sleeperThread();

void *uploaderThread(void *data);

void *writerThread(void *data);

void *cpuThread(void *ptr);

void handle_sigint(int sig);
#endif
