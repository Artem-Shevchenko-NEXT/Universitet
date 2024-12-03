// Artem Shevchenko // ashevc24@student.aau.dk // grupper 9 // software udvikling

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEAM_AMOUNT 12
#define MATCHES_AMOUNT 90

static const char *TEAM_NAMES[TEAM_AMOUNT] = {
    "AGF", "FCM", "FCN", "AaB", "SIF", 
    "SJF", "VB", "RFC", "VFF", "BIF",
    "LBK", "FCK"
};

// Structs til hold og kampe
typedef struct {
    char weekday[4];
    char date[6];
    char time[6];
    char home_team[4];
    char away_team[4];
    int home_goals;
    int away_goals;
    int spectators;
} Match;

typedef struct {
    char name[4];
    int points;
    int goals_scored;
    int goals_conceded;
} Team;

// Prototyper for alle funktioner brugt i programmet
void Turnement_Analyzer(FILE *file, int match_amount, int team_amount );
void Read_Matches_From_File(FILE *file, Match Matches_info[], int length);
FILE* File_Reader(const char* filename);
void Initialize_Teams(Team teams[]);
void Update_Team_Stats(Match matches[], Team teams[], int match_amount);
void Print_Matches(Match matches[], int length);
void Print_Teams(Team teams[], int length);
void Sort_Teams(Team teams[], int length);
int Compare_Teams(const void* a, const void* b);

int main (void){
    // implement the file reader function here so that i can pass a file to the analyzer function 
    FILE *file = File_Reader("kampe-2024-2025.txt");
    Turnement_Analyzer(file, MATCHES_AMOUNT, TEAM_AMOUNT);
    fclose(file);
}

void Turnement_Analyzer(FILE *file, int match_amount, int team_amount){
    //arrayet til kamp opretteres on indlæses gennem Read_Matches_From_File funktionen
    Match matches_Info[MATCHES_AMOUNT];
    Team teams[TEAM_AMOUNT];

    Initialize_Teams(teams);
    Read_Matches_From_File(file, matches_Info, match_amount);

    Update_Team_Stats(matches_Info, teams, match_amount);
    Sort_Teams(teams, team_amount);  

    Print_Teams(teams, team_amount);
}

void Read_Matches_From_File(FILE *file, Match Matches_info[], int length) {
    // Midlertidig Match variabel sikrer at data kun gemmes i array hvis de er ingen fejl
    Match match;
    for(int i = 0; i < length; i++) {
        //Læser tekst/tal og springer mellemrum over (%*[ ]), hvor - matcher præcis bindestreg i filet
        int result = fscanf(file, "%s%*[ ]%s%*[ ]%s%*[ ]%s%*[ ]-%*[ ]%s%*[ ]%d%*[ ]-%*[ ]%d%*[ ]%d",
                          match.weekday,
                          match.date,
                          match.time,
                          match.home_team,
                          match.away_team,
                          &match.home_goals,
                          &match.away_goals,
                          &match.spectators);
        
        if (result != 8) {
            printf("error in reading of match %d. Result: %d\n", i, result);
            exit(EXIT_FAILURE);
        }
        
        Matches_info[i] = match;
    }
}

FILE* File_Reader(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Filen kan ikke aabnes!\n");
        exit(EXIT_FAILURE);
    }
    return file;
}

void Initialize_Teams(Team teams[]) {
    // Løkke der initialiserer hold-array ved at kopiering af vores hold navne gennem strncpy
    for (int i = 0; i < TEAM_AMOUNT; i++) {
        strncpy(teams[i].name, TEAM_NAMES[i], sizeof(teams[i].name) - 1);
        teams[i].name[sizeof(teams[i].name) - 1] = '\0'; // Placerer null-terminator på sidste position (index 3)
        teams[i].points = 0;
        teams[i].goals_scored = 0;
        teams[i].goals_conceded = 0;
    }
}

int hash_team(const char* team_name) {
    for(int i = 0; i < TEAM_AMOUNT; i++) {
        if(strcmp(team_name, TEAM_NAMES[i]) == 0) {
            return i;
        }
    }
    return -1;
}

void Update_Team_Stats(Match matches[], Team teams[], int match_amount) {
    for (int i = 0; i < match_amount; i++) {
        // Finder array-indekser for hjemme- og udehold ved hjælp af hash-funktion
        int home_index = hash_team(matches[i].home_team);
        int away_index = hash_team(matches[i].away_team);
        //tjeker om det er korrekt
        if (home_index == -1 || away_index == -1) {
            printf("Error: Invalid team name found\n");
            continue;
        }

        //Opdaterer mål for både hjemme- og udehold
        //Hjemmeholdets scorede mål er udeholdets mål imod og omvendt
        teams[home_index].goals_scored += matches[i].home_goals;
        teams[home_index].goals_conceded += matches[i].away_goals;
        teams[away_index].goals_scored += matches[i].away_goals;
        teams[away_index].goals_conceded += matches[i].home_goals;

        // Tildeler point baseret på kampens resultat:
        // vinder = 3 point
        // Uafgjort = 1 point til hver
        // Tab = 0 point
        if (matches[i].home_goals > matches[i].away_goals) {
            teams[home_index].points += 3;
        } else if (matches[i].home_goals < matches[i].away_goals) {
            teams[away_index].points += 3;
        } else {
            teams[home_index].points += 1;
            teams[away_index].points += 1;
        }
    }
}

// Sorterer hold array ved brug af C's indbyggede quicksort funktion
// Sorteringen sker efter point og målforskel
void Sort_Teams(Team teams[], int length) {
    qsort(teams, length, sizeof(Team), Compare_Teams);
}
// Sammenligner to hold for sortering:
// 1. Primært efter point (højest først)
// 2. Sekundært efter målforskel hvis point er ens
int Compare_Teams(const void* a, const void* b) {
    Team* team_a = (Team*)a;
    Team* team_b = (Team*)b;
    
    // sortere efter score desc
    if (team_b->points != team_a->points) {
        return team_b->points - team_a->points;
    }
    
    // sekundere sortering efter mål forskelle også desc
    int goal_diff_a = team_a->goals_scored - team_a->goals_conceded;
    int goal_diff_b = team_b->goals_scored - team_b->goals_conceded;
    return goal_diff_b - goal_diff_a;
}



void Print_Teams(Team teams[], int length) {
    printf("\nTeam Statistics:\n");
    printf("%-4s %-7s %-13s %-13s\n", 
           "Team", "Points", "Goals scored", "Goals conceded");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < length; i++) {
        printf("%-4s %-7d %-13d %-13d\n",
               teams[i].name,
               teams[i].points,
               teams[i].goals_scored,
               teams[i].goals_conceded);
    }
    printf("\n");
}
