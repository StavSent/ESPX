#include  "covidTrace.h"

int main(int argc, char *argv[]) {

    char *a = argv[1];
    int num = atoi(a);
    
    FILE *fptr, *tptr, *cpuptr;
    int bufferLength = 255;
    char buffer[bufferLength];
    int counter = 0;
    char *mac[num];

    pthread_t  tworker, tsleeper, twriter, tupload, tcpu;
     srand(time(0));

    // Read MAC addresses from "addresses.txt"
    if (((fptr = fopen("addresses.txt","r")) == NULL) || ((tptr = fopen("timestamp.bin", "wb+")) == NULL)
        || ((cpuptr = fopen("cpu.bin", "wb+")) == NULL)){
        printf("Error! opening file");
        exit(1);
    }

    // Save addresses to mac
    while(fgets(buffer, bufferLength, fptr)) {
        mac[counter] = strdup(buffer);
        counter++;
    }
    // Close file
    fclose(fptr);

    // Array to store all mac addresses entries
    macaddress* addresses = (macaddress *)malloc(num * sizeof(macaddress));

    int uploadIndex = 1;
    bool covid = 0;

    signal(SIGINT, handle_sigint);

    struct timespec tv;
    if(clock_gettime(CLOCK_REALTIME, &tv))
    perror("error clock_gettime\n");

    uint64_t start = (tv.tv_sec);  
    printf("%" PRIu64 "\n", start);

    // Infinite loop
    while ( !exitRequest ) {

        // On entry call sleep and cpu thread
        pthread_create(&tsleeper, NULL, sleeperThread, NULL);
        pthread_create(&tcpu, NULL, cpuThread, cpuptr);
        
        // Get macAddress new entry
        macaddress currentAddress = BTnearMe(mac, num);

        // Create arguments for thread functions
        workerArgs *args = malloc(sizeof(workerArgs));
        args->address = &currentAddress;
        args->addresses = addresses;

        writerArgs *wargs = malloc(sizeof(writerArgs));
        wargs->ptr = tptr;
        wargs->time = currentAddress.time;

        // Create 3 threads => 1.Manages mac entries stored in array
        //                     2.Sleeps for 10seconds
        //                     3.Writes timestamp to timastamps.txt file
        pthread_create(&tworker, NULL, workerThread, args);
        pthread_create(&twriter, NULL, writerThread, wargs);

        // If a certain amount of time has passed, create an extra thread to upload arrays contents to server.txt
        if ((currentAddress.time / 1000000) > (uploadIndex * covidTestTime) + start) {
            covid = testCOVID();

            if (covid) {
                uploaderArgs *uargs = malloc(sizeof(uploaderArgs));

                uargs->addresses = addresses;
                uargs->time = currentAddress.time;

                pthread_create(&tupload, NULL, uploaderThread, uargs);
            }
        }

        // Join all open threads
        pthread_join(tworker, NULL);
        pthread_join(twriter, NULL);

        if ((currentAddress.time / 1000000) > (uploadIndex * covidTestTime) + start) {
            uploadIndex++;
            if (covid) {
                pthread_join(tupload, NULL);
            }
        }
        pthread_join(tcpu, NULL);
	    pthread_join(tsleeper, NULL);

    }

    // Print contents of array
    for (int i = 0; i < maxindex; i++) {
        printf("Index: %d, -> %d, -> %s", i, addresses[i].far, addresses[i].mac);
        printf("%" PRIu64 "\n", addresses[i].time);

    }

    free(addresses);

    return 0;
}
