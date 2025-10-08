#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

// Simple blockchain-style hash generator
void generateHash(char *input, char *output) {
    unsigned long hash = 5381;
    int c;
    while ((c = *input++)) {
        hash = ((hash << 5) + hash) + c;
    }
    sprintf(output, "%lx", hash);
}

int main() {
    int voterID, candidateID;
    Voter v;
    Candidate c;
    Vote newVote, lastVote;
    char inputStr[200];

    printf("Content-type:text/html\n\n");
    printf("<html><body>");

    printf("Enter your Voter ID: ");
    scanf("%d", &voterID);

    // Check voter details
    FILE *vf = fopen("../data/voters.dat", "rb+");
    if (!vf) {
        printf("<p>Error: voters.dat not found</p>");
        return 1;
    }

    int found = 0;
    while (fread(&v, sizeof(Voter), 1, vf)) {
        if (v.voterID == voterID) {
            found = 1;
            if (v.hasVoted) {
                printf("<p>❌ Fraud Detected: You already voted!</p>");
                fclose(vf);
                return 0;
            }
            break;
        }
    }

    if (!found) {
        printf("<p>❌ Invalid voter ID!</p>");
        fclose(vf);
        return 0;
    }

    // Display candidates
    printf("<h3>Select a Candidate:</h3>");
    FILE *cf = fopen("../data/candidates.dat", "rb");
    if (!cf) {
        printf("<p>Error: candidates.dat not found</p>");
        return 1;
    }

    while (fread(&c, sizeof(Candidate), 1, cf)) {
        printf("<p>%d - %s</p>", c.candidateID, c.name);
    }
    fclose(cf);

    printf("Enter Candidate ID: ");
    scanf("%d", &candidateID);

    // Prepare new vote
    newVote.voterID = voterID;
    newVote.candidateID = candidateID;

    // Get previous hash
    FILE *vf2 = fopen("../data/votes.dat", "rb");
    if (vf2 && fread(&lastVote, sizeof(Vote), 1, vf2)) {
        fseek(vf2, -sizeof(Vote), SEEK_END);
        fread(&lastVote, sizeof(Vote), 1, vf2);
        strcpy(newVote.prevHash, lastVote.currHash);
        fclose(vf2);
    } else {
        strcpy(newVote.prevHash, "GENESIS"); // First vote
    }

    // Generate current hash
    sprintf(inputStr, "%d%d%s", newVote.voterID, newVote.candidateID, newVote.prevHash);
    generateHash(inputStr, newVote.currHash);

    // Save vote
    FILE *vf3 = fopen("../data/votes.dat", "ab");
    fwrite(&newVote, sizeof(Vote), 1, vf3);
    fclose(vf3);

    // Mark voter as voted
    fseek(vf, -sizeof(Voter), SEEK_CUR);
    v.hasVoted = 1;
    fwrite(&v, sizeof(Voter), 1, vf);
    fclose(vf);

    printf("<p>✅ Vote successfully recorded with hash: %s</p>", newVote.currHash);
    printf("</body></html>");

    return 0;
}
