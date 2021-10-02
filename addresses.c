#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {

    char *a = argv[1];
    int num = atoi(a);

    // int MACs[num];
    int MAC1, MAC2, MAC3, MAC4, MAC5, MAC6, close_contact;

    FILE *fptr;
    fptr = fopen("addresses.txt", "w");

    for (int i = 1; i <= num; i++) {
        MAC1 = rand() % 256;
        MAC2 = rand() % 256;
        MAC3 = rand() % 256;
        MAC4 = rand() % 256;
        MAC5 = rand() % 256;
        MAC6 = rand() % 256;
        fprintf(fptr, "%x:", MAC1);
        fprintf(fptr, "%x:", MAC2);
        fprintf(fptr, "%x:", MAC3);
        fprintf(fptr, "%x:", MAC4);
        fprintf(fptr, "%x:", MAC5);
        fprintf(fptr, "%x\n", MAC6);
    };

    fclose(fptr);
    return 0;
}; 


