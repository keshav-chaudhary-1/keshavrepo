#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <structs.h>

int main() {
    int id;
    char password[20];
    Voter v;

    printf("Content-type:text/html\n\n");
    printf("<html><body>");

    FILE *fp = fopen("../data/voters.dat", "rb+");
    if (!fp) {
        printf("<p>Error: Voter database not found.</p>");
        return 1;
    }

    // (Simulation: user enters credentials via console for now)
    printf("Enter Voter ID: ");
    scanf("%d", &id);
    printf("Enter Password: ");
    scanf("%s", password);

    int found = 0;
    while (fread(&v, sizeof(Voter), 1, fp)) {
        if (v.voterID == id && strcmp(v.password, password) == 0) {
            found = 1;
            if (v.hasVoted) {
                printf("<p>❌ Fraud Detected: You already voted!</p>");
            } else {
                printf("<p>✅ Login Successful! Proceed to voting.</p>");
            }
            break;
        }
    }

    if (!found) {
        printf("<p>❌ Invalid login details!</p>");
    }

    fclose(fp);
    printf("</body></html>");
    return 0;
}
