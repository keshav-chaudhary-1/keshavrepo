#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

int main() {
    Candidate c;
    Vote v;

    printf("Content-type:text/html\n\n");
    printf("<html><body>");
    printf("<h2>📊 Voting Results</h2>");

    // Reset candidate votes
    FILE *cf = fopen("../data/candidates.dat", "rb+");
    if (!cf) {
        printf("Error: candidates.dat not found");
        return 1;
    }

    while (fread(&c, sizeof(Candidate), 1, cf)) {
        c.votes = 0;
        fseek(cf, -sizeof(Candidate), SEEK_CUR);
        fwrite(&c, sizeof(Candidate), 1, cf);
    }
    fclose(cf);

    // Count votes from votes.dat
    FILE *vf = fopen("../data/votes.dat", "rb");
    if (!vf) {
        printf("<p>No votes cast yet.</p>");
    } else {
        while (fread(&v, sizeof(Vote), 1, vf)) {
            FILE *cf2 = fopen("../data/candidates.dat", "rb+");
            while (fread(&c, sizeof(Candidate), 1, cf2)) {
                if (c.candidateID == v.candidateID) {
                    c.votes++;
                    fseek(cf2, -sizeof(Candidate), SEEK_CUR);
                    fwrite(&c, sizeof(Candidate), 1, cf2);
                    break;
                }
            }
            fclose(cf2);
        }
        fclose(vf);
    }

    // Display final results
    cf = fopen("../data/candidates.dat", "rb");
    while (fread(&c, sizeof(Candidate), 1, cf)) {
        printf("<p>%s : %d votes</p>", c.name, c.votes);
    }
    fclose(cf);

    // Display fraud logs
    printf("<h2>⚠️ Fraud Logs</h2>");
    FILE *fl = fopen("../data/fraud.log", "r");
    if (fl) {
        char line[200];
        while (fgets(line, sizeof(line), fl)) {
            printf("<p>%s</p>", line);
        }
        fclose(fl);
    } else {
        printf("<p>No fraud attempts detected.</p>");
    }

    printf("</body></html>");
    return 0;
}
