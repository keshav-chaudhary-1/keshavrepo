#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
    int voterID;
    char name[50];
    char password[20];
    int hasVoted; // 0 = No, 1 = Yes
} Voter;

typedef struct {
    int candidateID;
    char name[50];
    int votes;
} Candidate;

typedef struct {
    int voterID;
    int candidateID;
    char prevHash[65];
    char currHash[65];
} Vote;

#endif
