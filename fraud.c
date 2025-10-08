#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structs.h"

// Function to log fraud activities
void logFraud(int voterID, char *activity) {
    FILE *fp = fopen("../data/fraud.log", "a");
    if (!fp) {
        printf("Error: Could not open fraud.log\n");
        return;
    }

    time_t t;
    time(&t);
    fprintf(fp, "[%s] VoterID %d: %s\n", ctime(&t), voterID, activity);
    fclose(fp);
}

// Example test for fraud.c
int main() {
    printf("Content-type:text/html\n\n");
    printf("<html><body>");

    // Simulating a fraud attempt
    int voterID = 101;
    logFraud(voterID, "Attempted multiple votes!");

    printf("<p>Fraud logged for voter %d</p>", voterID);
    printf("</body></html>");

    return 0;
}
