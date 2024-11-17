#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define YATZY_ROUNDS 15
#define DIE_MAX_EYES 6
#define SCORE_BOARD_SIZE 18
#define MAX_OF_SAME_FACE 5

typedef enum {ONES = 1, TWOS, THREES, FOURS, FIVES, SIXES, PAIR, TWO_PAIRS, THREE_OF_A_KIND, FOUR_OF_A_KIND, SMALL_STRAIGHT, LARGE_STRAIGHT, FULL_HOUSE, CHANCE, YATZY} Rounds;


void YatzyGame(void);
int* Roll_Multiple_Dice(int number_Of_Dice);
void Print_Dice_Results(int* dice_Results, int number_Of_Dice);
void Play_Rounds(int number_Of_Dice, int* score_Board);
int Calculate_Best_Score(int* dice_Rolls, int round, int number_Of_Dice);
int Sum_Of_Same_Value(int counts[],int round );
int Second_Half_Of_Yatzy(int counts[],int round);

int main (void){
    srand(time(NULL));
    YatzyGame();
    return(0);
}

void YatzyGame(void){
    int number_Of_Dice;
    static int score_Board[SCORE_BOARD_SIZE];
    while (1) {
        printf("Yatzy with how many dies (a number less than 5 terminates) \n");
        scanf("%d", &number_Of_Dice);

        if (number_Of_Dice < 5) {
            break;
        }

        // Call other functions here
        /*
        int* dice_Results = Roll_Multiple_Dice(number_Of_Dice);
        Print_Dice_Results(dice_Results, number_Of_Dice);
        */
        Play_Rounds(number_Of_Dice,score_Board);
        // Example: calculateScore();
    }
    printf("Terminating the game.\n");

}

int* Roll_Multiple_Dice(int number_Of_Dice){
    int* dice_Results = malloc(number_Of_Dice * sizeof(int));

    for(int current_die = 0;current_die < number_Of_Dice; current_die++){
        dice_Results[current_die] =  (rand() % DIE_MAX_EYES) + 1;
    } 

    return dice_Results;   
}
void Print_Dice_Results(int* dice_Results, int number_Of_Dice) {
    printf("Dice results: ");
    for (int current_die_result = 0; current_die_result < number_Of_Dice; current_die_result++) {
        printf("%d ", dice_Results[current_die_result]);
    }
    //printf("\n");
}

void Play_Rounds(int number_Of_Dice,int* score_Board){
    for(int round = ONES; round <= YATZY_ROUNDS; round++){
        int* dice_Rolls = Roll_Multiple_Dice(number_Of_Dice);
        int best_Score = Calculate_Best_Score(dice_Rolls, round,number_Of_Dice);
        score_Board[round] = best_Score;

        printf("Round %d: ", round );
        Print_Dice_Results(dice_Rolls, number_Of_Dice);
        printf("Best score for this round: %d\n", best_Score);
        free(dice_Rolls);
    }
}

int Calculate_Best_Score(int* dice_Rolls, int round, int number_Of_Dice){
    int score = 0;
    int counts[DIE_MAX_EYES] = {0};
    for (int current_die_roll = 0; current_die_roll <  number_Of_Dice; current_die_roll++) {
        if (counts[dice_Rolls[current_die_roll] - 1] < MAX_OF_SAME_FACE ){
            counts[dice_Rolls[current_die_roll] - 1]++;
        }

    }
    //printf("ignore message after this:");
    //Print_Dice_Results(counts, DIE_MAX_EYES);
    switch (round) {
        case ONES:
            score = Sum_Of_Same_Value(counts,ONES);
            break;
        case TWOS:
            score = Sum_Of_Same_Value(counts,TWOS);
            break;
        case THREES:
            score = Sum_Of_Same_Value(counts,THREES);
            break;
        case FOURS:
            score = Sum_Of_Same_Value(counts,FOURS);
            break;
        case FIVES:
            score = Sum_Of_Same_Value(counts,FIVES);
            break;
        case SIXES:
            score = Sum_Of_Same_Value(counts,SIXES);
            break;
        case PAIR:
            score = Second_Half_Of_Yatzy(counts, PAIR);
            break;
        case TWO_PAIRS:
            score = Second_Half_Of_Yatzy(counts, TWO_PAIRS);
            break;
        case THREE_OF_A_KIND:
            score = Second_Half_Of_Yatzy(counts, THREE_OF_A_KIND);
            break;
        case FOUR_OF_A_KIND:
            score = Second_Half_Of_Yatzy(counts, FOUR_OF_A_KIND);
            break;
        case SMALL_STRAIGHT:
            score = Second_Half_Of_Yatzy(counts, SMALL_STRAIGHT);
            break;
        case LARGE_STRAIGHT:
            score = Second_Half_Of_Yatzy(counts, LARGE_STRAIGHT);
            break;
        case FULL_HOUSE:
            score = Second_Half_Of_Yatzy(counts, FULL_HOUSE);
            break;
        case CHANCE:
            score = Second_Half_Of_Yatzy(counts, CHANCE);
            break;
        case YATZY:
            score = Second_Half_Of_Yatzy(counts, YATZY);
            break;
        default:
            // Handle invalid round
            break;
    }
    return score;
}
int Sum_Of_Same_Value(int counts[],int round ){
       return (counts[round-1] * round);
}
int Second_Half_Of_Yatzy(int counts[],int round){
    int score = 0;
    int first_pair_value = 0;
    int three_of_a_kind_value = 0;
    int dice_count = 0;
    switch (round) {
        case PAIR:
            for(int die_face_desc = DIE_MAX_EYES -1; die_face_desc >= 0; die_face_desc-- ){
                if (counts[die_face_desc] >= 2){
                    score = (die_face_desc+1)*2;
                    break;
                }
            }        
            break;
        case TWO_PAIRS:
            for (int die_face_desc = DIE_MAX_EYES - 1; die_face_desc >= 0; die_face_desc--) {
                if (counts[die_face_desc] >= 2) {
                    if (first_pair_value == 0) {
                        first_pair_value = die_face_desc + 1;
                        score += first_pair_value * 2;
                        dice_count += 2;
                    } else if (first_pair_value > 0){
                        score += (die_face_desc + 1) * 2;
                        dice_count += 2;
                        break;
                    }
                }
            }
            if (dice_count < 4) {
                score = 0; // Not a TWO_PAIRS
            }          
            break;
        case THREE_OF_A_KIND:
            for(int die_face_desc = DIE_MAX_EYES -1; die_face_desc >= 0; die_face_desc-- ){
                if (counts[die_face_desc] >= 3){
                    score = (die_face_desc+1)*3;
                    break;
                }
            }   
            break;
        case FOUR_OF_A_KIND:
            for(int die_face_desc = DIE_MAX_EYES -1; die_face_desc >= 0; die_face_desc-- ){
                if (counts[die_face_desc] >= 4){
                    score = (die_face_desc+1)*4;
                    break;
                }
            }   
            break;
        case SMALL_STRAIGHT:
            if (counts[0] >= 1 && counts[1] >= 1 && counts[2] >= 1 && counts[3] >= 1 && counts[4] >= 1) {
                score = 15; 
            }
            break;
        case LARGE_STRAIGHT:
            if (counts[1] >= 1 && counts[2] >= 1 && counts[3] >= 1 && counts[4] >= 1 && counts[5] >= 1) {
                score = 20; 
            }
            break;
        case FULL_HOUSE:
            for (int die_face_desc = DIE_MAX_EYES - 1; die_face_desc >= 0; die_face_desc--) {
                if (counts[die_face_desc] >= 3 && three_of_a_kind_value == 0) {
                    three_of_a_kind_value = die_face_desc + 1;
                    score += three_of_a_kind_value * 3;
                } else if (counts[die_face_desc] >= 2 && first_pair_value == 0) {
                    first_pair_value = die_face_desc + 1;
                    score += first_pair_value * 2;
                }
                if (three_of_a_kind_value != 0 && first_pair_value != 0) {
                    break;
                }
            }
            if (three_of_a_kind_value == 0 || first_pair_value == 0) {
                score = 0; // Not a valid full house
            }            
            break;
        case CHANCE:
            for (int die_face_desc = DIE_MAX_EYES - 1; die_face_desc >= 0 && dice_count < 5; die_face_desc--) {
                int dice_to_add = counts[die_face_desc];
                if (dice_count + dice_to_add > 5) {
                    dice_to_add = 5 - dice_count;
                }
                score += dice_to_add * (die_face_desc + 1);
                dice_count += dice_to_add;
            }
            break;
        case YATZY:
            for(int die_face_desc = DIE_MAX_EYES -1; die_face_desc >= 0; die_face_desc-- ){
                if (counts[die_face_desc] == 5){
                    score = 50;
                    break;
                }
            }  
            break;
        default:
            break;
    }
    return score; 
}
