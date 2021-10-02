#include "covidTrace.h"

int maxindex = 0;
int sleepTime = 99720;       

uint64_t covidTestTime = 144;          // ( 4 * 60 * 60 / 100 )s
uint64_t forgetTime = 12096000000;     // ( 14 * 24 * 60 * 60 * 1000 * 1000 )us
uint64_t minTime = 2400000;            // ( 4 * 60 * 1000 * 1000 / 100 )us
uint64_t maxTime = 12000000;           // (12 * 60 * 1000 * 1000 / 100 )us
 
volatile sig_atomic_t exitRequest = 0;

macaddress BTnearMe(char *addresses[])
{
    struct timespec tv;
    if(clock_gettime(CLOCK_REALTIME, &tv))
    perror("error clock_gettime\n");

    // calculate microseconds since EPOCH
    uint64_t seconds = ((uint64_t)tv.tv_sec * 1000000) + ((uint64_t)tv.tv_nsec / 1000);  
 
    // Picks a MAC at random
    int random = rand() % (num);
    macaddress newAddress;
    newAddress.mac = addresses[random];
    newAddress.time = seconds;

    // Initially every incoming entry is considered to be far
    newAddress.far = 1;

    return newAddress;
}

bool testCOVID()
{
    if (((double) rand() / (RAND_MAX)) >= 0.5)
        return true;
    else
        return false;
};

void *workerThread(void *data) 
{
    bool add = true;
        
    workerArgs *args = data;

    // If array is empty add MAC
    if (maxindex == 0) {
        args->addresses[maxindex++] = *args->address; 
        add = false;
    
    // Else check if MAC already stored in array
    } else {
        for (int i = 0; i < maxindex; i++) {
            if (args->addresses[i].mac == args->address->mac) {
                add = false;

                // Change far attribute based on how long it's been since MAC appeared before
                int timeBetween = args->address->time - args->addresses[i].time;
                if ( timeBetween < minTime ) {
                    args->addresses[i].time = args->address->time;
                    args->addresses[i].far = 1;
                } else if ( (timeBetween > minTime) && (timeBetween < maxTime)) {
                    args->addresses[i].time = args->address->time;
                    args->addresses[i].far = 0;
                } else {
                    args->addresses[i].time = args->address->time;
                    args->addresses[i].far = 1;
                }
            }
        }
    }

    if ( add ) {
        args->addresses[maxindex++] = *args->address;
    }

    return 0;
}

// Function that acts as a timer
void *sleeperThread() 
{
    usleep(sleepTime);
    return 0;
}

// On every call writes time in timestamp.txt file
void *writerThread(void *data) 
{
    writerArgs *args = data;

    //fprintf(args->ptr, "%" PRIu64 "\n", args->time);
    uint64_t buffer = args->time;
    fwrite(&buffer, sizeof(buffer), 1, args->ptr);
    return 0;
}

// On call writes cpu usage in cpu.bin
void *cpuThread(void *ptr) {
    struct timespec tv;
    if(clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tv))
    perror("error clock_gettime\n");

    uint64_t active = ((uint64_t)tv.tv_sec * 1000000) + ((uint64_t)tv.tv_nsec / 1000);
    fwrite(&active, sizeof(active), 1, ptr);
}

// Saves MACs stored in array if they are considered to be near
void *uploaderThread(void *data) 
{

    // Open a file to write the contents of the array
    FILE *sptr;

    if ((sptr = fopen("server.txt","w+")) == NULL) {
        printf("Error! opening file");
        exit(1);
    }

    uploaderArgs *args = data;

    for (int i = 0; i < maxindex; i++) {
        if ( (args->time <= (args->addresses[i].time + forgetTime)) && (args->addresses[i].far == false)) {
            fprintf(sptr, "%s", args->addresses[i].mac);
	    }
    }

    return 0;
}

void handle_sigint(int sig)
{
    exitRequest = 1;
}



