#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define YATZY_ROUNDS 15
#define DIE_MAX_EYES 6
#define SCORE_BOARD_SIZE 16
#define MAX_OF_SAME_FACE 5
#define BONUS_THRESHOLD 63
#define BONUS_SCORE 50 
#define SMALL_STRAIGHT_SCORE 15
#define LARGE_STRAIGHT_SCORE 20
#define YATZE_SCORE 50

typedef enum {ONES = 1, TWOS, THREES, FOURS, FIVES, SIXES, PAIR, TWO_PAIRS, THREE_OF_A_KIND, FOUR_OF_A_KIND, SMALL_STRAIGHT, LARGE_STRAIGHT, FULL_HOUSE, CHANCE, YATZY} Rounds;


void YatzyGame(void);
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
int Is_There_Bonus(int* score_board);

int main (void){
    srand(time(NULL));
    YatzyGame();
    return(0);
}

void YatzyGame(void){
    int number_Of_Dice;
    static int score_Board[SCORE_BOARD_SIZE];
    int game_counter = 1;
    while (1) {
        printf("\n=== Game %d ===\n", game_counter++);
        printf("Yatzy with how many dies (a number less than 5 terminates) \n");
        scanf("%d", &number_Of_Dice);

        if (number_Of_Dice < 5) {
            break;
        }

        Play_Rounds(number_Of_Dice,score_Board);
        Display_Calculated_Score(score_Board);
        printf("\n================\n");
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
    for (int current_die_roll = 0; current_die_roll < number_Of_Dice; current_die_roll++) {
        if (counts[dice_Rolls[current_die_roll] - 1] < MAX_OF_SAME_FACE ){
            counts[dice_Rolls[current_die_roll] - 1]++;
        }
    }

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
            // Handle invalid round
            break;
    }
    return score;
}

int Sum_Of_Same_Value(int counts[], int value) {
    return counts[value - 1] * value;
}

int Best_Pair_Score(int counts[]) {
    for (int die_face_desc = DIE_MAX_EYES - 1; die_face_desc >= 0; die_face_desc--) {
        if (counts[die_face_desc] >= 2) {
            return (die_face_desc + 1) * 2;
        }
    }
    return 0;
}

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

int Best_Three_Of_A_Kind_Score(int counts[]) {
    for (int die_face_desc = DIE_MAX_EYES - 1; die_face_desc >= 0; die_face_desc--) {
        if (counts[die_face_desc] >= 3) {
            return (die_face_desc + 1) * 3;
        }
    }
    return 0;
}

int Best_Four_Of_A_Kind_Score(int counts[]) {
    for (int die_face_desc = DIE_MAX_EYES - 1; die_face_desc >= 0; die_face_desc--) {
        if (counts[die_face_desc] >= 4) {
            return (die_face_desc + 1) * 4;
        }
    }
    return 0;
}

int Small_Straight_Score(int counts[]) {
    if (counts[0] >= 1 && counts[1] >= 1 && counts[2] >= 1 && counts[3] >= 1 && counts[4] >= 1) {
        return SMALL_STRAIGHT_SCORE;
    }
    return 0;
}

int Large_Straight_Score(int counts[]) {
    if (counts[1] >= 1 && counts[2] >= 1 && counts[3] >= 1 && counts[4] >= 1 && counts[5] >= 1) {
        return LARGE_STRAIGHT_SCORE;
    }
    return 0;
}

int Full_House_Score(int counts[]) {
    int three_of_a_kind_value = 0;
    int pair_value = 0;

    for (int die_face_desc = DIE_MAX_EYES - 1; die_face_desc >= 0; die_face_desc--) {
        if (counts[die_face_desc] >= 3 && three_of_a_kind_value == 0) {
            three_of_a_kind_value = die_face_desc + 1;
        } else if (counts[die_face_desc] >= 2 && (die_face_desc + 1) != three_of_a_kind_value) {
            pair_value = die_face_desc + 1;
        }
        if (three_of_a_kind_value != 0 && pair_value != 0) {
            return three_of_a_kind_value * 3 + pair_value * 2;
        }
    }
    return 0;
}

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

int Yatzy_Score(int counts[]) {
    for (int die_face_desc = 0; die_face_desc < DIE_MAX_EYES; die_face_desc++) {
        if (counts[die_face_desc] == 5) {
            return YATZE_SCORE;
        }
    }
    return 0;
}

int Display_Calculated_Score(int* score_board) {
    int upper_section_sum = 0;
    int lower_section_sum = 0;
    int bonus = 0;
    int total_score = 0;

    // Display upper section with Danish names
    printf("\nScore board:\n");
    for(int round = ONES; round <= SIXES; round++) {
        printf("%d-ere  : %d\n", round, score_board[round]);
        upper_section_sum += score_board[round];
    }

    bonus = Is_There_Bonus(score_board);
    printf("BONUS  : %d\n\n", bonus);

    // Display lower section with Danish names using switch
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

int Is_There_Bonus(int* score_board) {
    int upper_section_sum = 0;
    for(int i = ONES; i <= SIXES; i++) {
        upper_section_sum += score_board[i];
    }
    if (upper_section_sum >= BONUS_THRESHOLD) {
        return BONUS_SCORE;
    } else {
        return 0;
    }
}
