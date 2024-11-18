// Artem Shevchenko // ashevc24@student.aau.dk // grupper 9 // software udvikling

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MIN_DICE 5
#define CONFIRMATION_THRESHOLD 10000
#define YATZY_ROUNDS 15
#define DIE_MAX_EYES 6
#define SCORE_BOARD_SIZE 16
#define MAX_OF_SAME_FACE 5
#define BONUS_THRESHOLD 63
#define BONUS_SCORE 50 
#define SMALL_STRAIGHT_SCORE 15
#define LARGE_STRAIGHT_SCORE 20
#define YATZE_SCORE 50

// Enumeration af de forskellige runder i Yatzy
typedef enum {
    ONES = 1, TWOS, THREES, FOURS, FIVES, SIXES, 
    PAIR, TWO_PAIRS, THREE_OF_A_KIND, 
    FOUR_OF_A_KIND, SMALL_STRAIGHT, LARGE_STRAIGHT, 
    FULL_HOUSE, CHANCE, YATZY
} Rounds;

// Prototyper for alle funktioner brugt i programmet
void YatzyGame(void);
int Validate_Dice_Input(void);
void clear_input();
int* Roll_Multiple_Dice(int number_Of_Dice);
void Print_Dice_Results(int* dice_Results, int number_Of_Dice);
void Play_Rounds(int number_Of_Dice, int* score_Board);
int Calculate_Best_Score(int* dice_Rolls, int round, int number_Of_Dice);
int Sum_Of_Same_Value(int counts[],int round );
int Best_Pair_Score(int counts[]);
int Best_Two_Pairs_Score(int counts[]);
int Best_Three_Of_A_Kind_Score(int counts[]);
int Best_Four_Of_A_Kind_Score(int counts[]);
int Small_Straight_Score(int counts[]);
int Large_Straight_Score(int counts[]);
int Full_House_Score(int counts[]);
int Sum_Five_Dice(int counts[]);
int Yatzy_Score(int counts[]);
int Display_Calculated_Score(int* score_board);
int Is_There_Bonus(int upper_section_sum);


int main (void){
    srand(time(NULL)); // Initialiserer tilfældighedsgeneratoren med aktuelt klokkeslæt
    YatzyGame();  // Starter Yatzy-spillet
    return(0);
}

// Hoved kontrolløkke der styrer spilets tilstand 
// Bruger statisk pointtavle-array og kører indtil spilleren indtaster < 5 terninger
void YatzyGame(void){
    int number_Of_Dice;
    static int score_Board[SCORE_BOARD_SIZE];
    int game_counter = 1;
    while (1) {
        printf("\n=== Game %d ===\n", game_counter++);
        number_Of_Dice = Validate_Dice_Input();
        if (number_Of_Dice < 5) {
            break;
        }

        Play_Rounds(number_Of_Dice,score_Board);
        Display_Calculated_Score(score_Board);
        printf("\n================\n");
    }
    printf("Terminating the game.\n");

}

// Implementerer input validering med scanf for antal terninger
// Indeholder sikkerhedstjek for store tal (>10000) med y/n bekræftelse 
int Validate_Dice_Input(void) {
    int number_of_dice;
    char confirmation;
    
    printf("Enter the amount of dice (5 or more, less than 5 to quit): ");
    // Tjekker om input er et gyldigt tal og større end 5
    while (!scanf("%i", &number_of_dice)) {
        printf("Invalid input! Please enter a number 5 or greater: ");
        clear_input();
    }
    // Bekræfter hvis brugeren har valgt et usædvanligt stort antal terninger
    if (number_of_dice > CONFIRMATION_THRESHOLD) {
        clear_input();  
        printf("Are you sure you want to play with %d dice? (y/n): ", number_of_dice);
        scanf("%c", &confirmation);
        if (confirmation != 'y' && confirmation != 'Y') {
            clear_input();
            return Validate_Dice_Input(); // Spørger igen, hvis brugeren ikke bekræfter
        }
    }
    
    clear_input();
    return number_of_dice;
}
// funktionen Rydder inputbufferen ved at slet tegn indtil linjeskift
// Forhindrer uønskede tegn fra scanf operationen i at være læst 
void clear_input() {
    while (getchar() != '\n');
}

// Allokerer dynamisk int array til terningekast med malloc
// Genererer tilfældige tal 1-6 ved brug af rand() modulo operation
int* Roll_Multiple_Dice(int number_Of_Dice){
    int* dice_Results = malloc(number_Of_Dice * sizeof(int));

    for(int current_die = 0;current_die < number_Of_Dice; current_die++){
        dice_Results[current_die] =  (rand() % DIE_MAX_EYES) + 1;
    } 

    return dice_Results;   
}

//Itererer gennem 15 runder ved brug af enum-baseret runde sporing  
// Håndterer hukommelsesallokering/deallokering af terningekast og pointberegning
void Play_Rounds(int number_Of_Dice,int* score_Board){
    for(int round = ONES; round <= YATZY_ROUNDS; round++){
        int* dice_Rolls = Roll_Multiple_Dice(number_Of_Dice);
        int best_Score = Calculate_Best_Score(dice_Rolls, round,number_Of_Dice);
        score_Board[round] = best_Score;
        // Udskriver navnet på den aktuelle runde
        switch(round) {
            case ONES: printf("1-ere   : "); break;
            case TWOS: printf("2-ere   : "); break;
            case THREES: printf("3-ere   : "); break;
            case FOURS: printf("4-ere   : "); break;
            case FIVES: printf("5-ere   : "); break;
            case SIXES: printf("6-ere   : "); break;
            case PAIR: printf("Et par  : "); break;
            case TWO_PAIRS: printf("To par  : "); break;
            case THREE_OF_A_KIND: printf("Tre ens : "); break;
            case FOUR_OF_A_KIND: printf("Fire ens: "); break;
            case SMALL_STRAIGHT: printf("Lille   : "); break;
            case LARGE_STRAIGHT: printf("Stor    : "); break;
            case FULL_HOUSE: printf("Fuld hus: "); break;
            case CHANCE: printf("Chance  : "); break;
            case YATZY: printf("YATZY   : "); break;
        }
        Print_Dice_Results(dice_Rolls, number_Of_Dice);
        printf("Best possible score: %d\n", best_Score);
        free(dice_Rolls); // Frigiver den allokerede hukommelse for terningekastene
    }
}



// Tager pointer til terning-array og antal som parametre
// Udskriver terningeværdier i lineært format
void Print_Dice_Results(int* dice_Results, int number_Of_Dice) {
    printf("Dice results: ");
    for (int current_die_result = 0; current_die_result < number_Of_Dice; current_die_result++) {
        printf("%d ", dice_Results[current_die_result]);
    }
}

// Bruger frekvenstællingsarray counts[] til at spore terningeforekomster
// Implementerer switch-case til at route til passende scoringsfunktion
int Calculate_Best_Score(int* dice_Rolls, int round, int number_Of_Dice){
    int score = 0;
    int counts[DIE_MAX_EYES] = {0}; // Array til at tælle antallet af hver terningværdi
    // Tæller hvor mange gange hver terningværdi forekommer i kastet med maksimalt værdi af 5
    for (int current_die_roll = 0; current_die_roll < number_Of_Dice; current_die_roll++) {
        if (counts[dice_Rolls[current_die_roll] - 1] < MAX_OF_SAME_FACE ){
            counts[dice_Rolls[current_die_roll] - 1]++;
        }
    }
    // Vælger den relevante scoreberegning baseret på runden
    switch (round) {
        case ONES:
            score = Sum_Of_Same_Value(counts, ONES);
            break;
        case TWOS:
            score = Sum_Of_Same_Value(counts, TWOS);
            break;
        case THREES:
            score = Sum_Of_Same_Value(counts, THREES);
            break;
        case FOURS:
            score = Sum_Of_Same_Value(counts, FOURS);
            break;
        case FIVES:
            score = Sum_Of_Same_Value(counts, FIVES);
            break;
        case SIXES:
            score = Sum_Of_Same_Value(counts, SIXES);
            break;
        case PAIR:
            score = Best_Pair_Score(counts);
            break;
        case TWO_PAIRS:
            score = Best_Two_Pairs_Score(counts);
            break;
        case THREE_OF_A_KIND:
            score = Best_Three_Of_A_Kind_Score(counts);
            break;
        case FOUR_OF_A_KIND:
            score = Best_Four_Of_A_Kind_Score(counts);
            break;
        case SMALL_STRAIGHT:
            score = Small_Straight_Score(counts);
            break;
        case LARGE_STRAIGHT:
            score = Large_Straight_Score(counts);
            break;
        case FULL_HOUSE:
            score = Full_House_Score(counts);
            break;
        case CHANCE:
            score = Sum_Five_Dice(counts);
            break;
        case YATZY:
            score = Yatzy_Score(counts);
            break;
        default:
            break;
    }
    return score; // Returnerer den beregnede score for runden
}

// Tager frekvensarray og målværdi som parametre
// Multiplicerer antal af specifik værdi med værdien selv for øvre sektion scoring
int Sum_Of_Same_Value(int counts[], int value) {
    return counts[value - 1] * value;
}

// Itererer frekvensarray baglæns for at finde højeste par
// Returnerer fordoblet værdi af højest fundne par eller 0 hvis ingen findes
int Best_Pair_Score(int counts[]) {
    for (int die_face_desc = DIE_MAX_EYES - 1; die_face_desc >= 0; die_face_desc--) {
        if (counts[die_face_desc] >= 2) {
            return (die_face_desc + 1) * 2;
        }
    }
    return 0;
}

// Bruger to pass frekvensarray tjek for distinkte par
// Returnerer sum af begge par-værdier eller 0 hvis kriterier ikke er opfyldt
int Best_Two_Pairs_Score(int counts[]) {
    int score = 0;
    int pairs_found = 0;
    for (int die_face_desc = DIE_MAX_EYES - 1; die_face_desc >= 0; die_face_desc--) {
        if (counts[die_face_desc] >= 2) {
            score += (die_face_desc + 1) * 2;
            pairs_found++;
            if (pairs_found == 2) {
                return score;
            }
        }
    }
    return 0;
}

// Scanner frekvensarray baglæns for antal >= 3
// Returnerer tredoblet værdi af højest fundne tre ens eller 0
int Best_Three_Of_A_Kind_Score(int counts[]) {
    for (int die_face_desc = DIE_MAX_EYES - 1; die_face_desc >= 0; die_face_desc--) {
        if (counts[die_face_desc] >= 3) {
            return (die_face_desc + 1) * 3;
        }
    }
    return 0;
}
// Scanner frekvensarray baglæns for antal >= 4
// Returnerer firedoblet værdi af funden fire ens  eller 0
int Best_Four_Of_A_Kind_Score(int counts[]) {
    for (int die_face_desc = DIE_MAX_EYES - 1; die_face_desc >= 0; die_face_desc--) {
        if (counts[die_face_desc] >= 4) {
            return (die_face_desc + 1) * 4;
        }
    }
    return 0;
}

// Tjekker frekvensarray positioner 0-4 for >= 1 hver
// Returnerer 15 hvis sekvens findes, ellers 0
int Small_Straight_Score(int counts[]) {
    if (counts[0] >= 1 && counts[1] >= 1 && counts[2] >= 1 && counts[3] >= 1 && counts[4] >= 1) {
        return SMALL_STRAIGHT_SCORE;
    }
    return 0;
}

// Tjekker frekvensarray positioner 1-5 for >= 1 hver
// Returnerer 20 hvis sekvens findes, ellers 0
int Large_Straight_Score(int counts[]) {
    if (counts[1] >= 1 && counts[2] >= 1 && counts[3] >= 1 && counts[4] >= 1 && counts[5] >= 1) {
        return LARGE_STRAIGHT_SCORE;
    }
    return 0;
}

// Bruger to pass algoritme til at finde tre ens og separat par
// Returnerer kombineret score af tre ens og par-værdier hvis begge findes ellers 0
int Full_House_Score(int counts[]) {
    int three_of_a_kind_value = 0;
    int pair_value = 0;

    // finder højeste af tre ens
    for(int die_face_desc = DIE_MAX_EYES - 1; die_face_desc >= 0; die_face_desc--) {
        if(counts[die_face_desc] >= 3) {
            three_of_a_kind_value = die_face_desc + 1;
            break;
        }
    }

    // Finer højeste af par hvis tre ens er fundet
    if(three_of_a_kind_value != 0) {
        for(int die_face_desc = DIE_MAX_EYES - 1; die_face_desc >= 0; die_face_desc--) {
            if(counts[die_face_desc] >= 2 && (die_face_desc + 1) != three_of_a_kind_value) {
                pair_value = die_face_desc + 1;
                break;
            }
        }
    }

    // Retunere scoret kun hvis begge værdier er fandt
    if(three_of_a_kind_value != 0 && pair_value != 0) {
        return three_of_a_kind_value * 3 + pair_value * 2;
    }
    return 0;
}

// Implementerer baglæns frekvensarraysom holder værdien for de højeste 5 terninger
// Akkumulerer score mens der holdes styr på terning-tæller limit
int Sum_Five_Dice(int counts[]) {
    int score = 0;
    int dice_count = 0;
    
    for (int die_face_desc = DIE_MAX_EYES - 1; die_face_desc >= 0 && dice_count < 5; die_face_desc--) {
        int dice_to_add = counts[die_face_desc];

        if (dice_count + dice_to_add > 5) {
            dice_to_add = 5 - dice_count;
        }

        score += dice_to_add * (die_face_desc + 1);
        dice_count += dice_to_add;
    }
    
    return score;
}

// Lineær scanning af frekvensarray for præcis antal 5
// Returnerer 50 point hvis fundet, ellers 
int Yatzy_Score(int counts[]) {
    for (int die_face_desc = 0; die_face_desc < DIE_MAX_EYES; die_face_desc++) {
        if (counts[die_face_desc] == 5) {
            return YATZE_SCORE;
        }
    }
    return 0;
}

// Beregner sektions summer og anvender bonus-regler
// Bruger switch-case til formateret output af hver scoringskategori
int Display_Calculated_Score(int* score_board) {
    int upper_section_sum = 0;
    int lower_section_sum = 0;
    int bonus = 0;
    int total_score = 0;
    // Udskriver og summerer øverste sektion (1'ere til 6'ere)
    printf("\nScore board:\n");
    for(int round = ONES; round <= SIXES; round++) {
        printf("%d-ere  : %d\n", round, score_board[round]);
        upper_section_sum += score_board[round];
    }

    bonus = Is_There_Bonus(upper_section_sum);
    printf("BONUS  : %d\n\n", bonus);

    // Udskriver og summerer nederste sektion
    for(int round = PAIR; round <= YATZY; round++) {
        switch(round) {
            case PAIR: printf("Et par : %d\n", score_board[round]); break;
            case TWO_PAIRS: printf("To par : %d\n", score_board[round]); break;
            case THREE_OF_A_KIND: printf("Tre ens: %d\n", score_board[round]); break;
            case FOUR_OF_A_KIND: printf("Fire ens: %d\n", score_board[round]); break;
            case SMALL_STRAIGHT: printf("Lille  : %d\n", score_board[round]); break;
            case LARGE_STRAIGHT: printf("Stor   : %d\n", score_board[round]); break;
            case FULL_HOUSE: printf("Fuld hus: %d\n", score_board[round]); break;
            case CHANCE: printf("Chance : %d\n", score_board[round]); break;
            case YATZY: printf("YATZY  : %d\n", score_board[round]); break;
        }
        lower_section_sum += score_board[round];
    }

    total_score = upper_section_sum + bonus + lower_section_sum;
    printf("TOTAL  : %d\n", total_score);

    return total_score;
}

// Sammenligner øvre sektions sum med 63 grænseværdi
// Returnerer 50 point bonus hvis grænse nået, ellers 0
int Is_There_Bonus(int upper_section_sum) {
    if (upper_section_sum >= BONUS_THRESHOLD) {
        return BONUS_SCORE;
    } else {
        return 0;
    }
}
