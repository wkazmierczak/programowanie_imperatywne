#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define N_CARDS 52

int table[52];

int pack_A[N_CARDS];
int pack_B[N_CARDS];


int out_A = 0;
int out_B = 0;

int len_A =  N_CARDS/2;
int len_B =  N_CARDS/2;

int t_A = 1;
int t_B = 1;

int curr_num_conflicts = 0;

int seed;
int version = 0;
int max = 100;



int rand_from_interval(int a, int b) {

    if (a>b){
        return INT_MIN;
    }
    if ((b-a) > RAND_MAX){
        return INT_MAX;
    }
    if (a==b){
        return a;
    }

    return a + rand()%(b-a+1);
}

void rand_permutation(int n, int array[]) {
    for (int i = 0; i < n; ++i) {
        array[i] = i;
    }

    for (int j = 0; j < n-1; ++j) {
        int k = rand_from_interval(j, n-1);
        int tmp = array[k];
        array[k] = array[j];
        array[j] = tmp;

    }
}

void dealing_cards(){
    for (int i = 0; i < N_CARDS/2; ++i) {
        pack_A[i]= table[i];
        pack_B[i] = table[N_CARDS/2 + i];
    }
}

void collecting_cards_from_table(int player){
    if (player == 1){
        int x;
        if (t_A - out_A>0){
            x = t_A - out_A;
        }
        else{
            x = t_A - out_A + N_CARDS;
        }
        for (int i = 0; i < x; ++i) {
            pack_A[(out_A+len_A+i)%N_CARDS] = pack_A[(out_A+i)%N_CARDS];
            //pack_A[(out_A+i)%N_CARDS] = -1;
        }
        out_A = t_A;
        t_A= (t_A+1)%N_CARDS;
        int y;
        if (t_B - out_B>0){
            y = t_B - out_B;
        }
        else{
            y = t_B - out_B + N_CARDS;
        }
        for (int i = 0; i < y; ++i) {
            if (len_A == 0 || len_B == 0) {
                printf("%d ", 1);
                printf("%d ", len_A);
                printf("%d", len_B);
                return;
            }
            pack_A[(out_A+len_A)%N_CARDS] = pack_B[(out_B+i)%N_CARDS];
            //pack_B[(out_B+i)%N_CARDS] = -1;
            len_A+=1;
            len_B-=1;
        }
        out_B = t_B;
        t_B = (t_B+1)%N_CARDS;

    }
    else if (player == 2){
        int val;
        if (t_B - out_B > 0){
            val = t_B - out_B;
        }
        else{
            val = t_B - out_B + N_CARDS;
        }
        for (int i = 0; i < val; ++i) {
            pack_B[(out_B+len_B+i)%N_CARDS] = pack_B[(out_B+i)%N_CARDS];
            //pack_B[(out_B+i)%N_CARDS] = -1;
        }
        out_B = t_B;
        t_B = (t_B+1)%N_CARDS;

        int val_2;

        if (t_A - out_A > 0){
            val_2 = t_A - out_A;
        }
        else{
            val_2 = t_A - out_A + N_CARDS;
        }
        for (int i = 0; i < val_2; ++i) {
            pack_B[(out_B+len_B)%N_CARDS] = pack_A[(out_A+i)%N_CARDS];
            //pack_A[(out_A+i)%N_CARDS] = -1;
            len_B++;
            len_A--;

        }
        out_A = t_A;
        t_A = (t_A+1)%N_CARDS;
    }
}


void war_game(){
    while (1){
        int a;
        int b;
        if (t_A-1>=0) {
            a = pack_A[(t_A - 1) % N_CARDS] / 4;
            b = pack_B[(t_B - 1) % N_CARDS] / 4;
        }
        else{
            a = pack_A[(t_A - 1) % N_CARDS + N_CARDS] / 4;
            b = pack_B[(t_B - 1) % N_CARDS + N_CARDS] / 4;
        }

        if (a > b){
            collecting_cards_from_table(1);
            curr_num_conflicts++;
        }
        else if (a < b){
            collecting_cards_from_table(2);
            curr_num_conflicts++;
        }
        else if (a == b){
            curr_num_conflicts++;
            int required_a;

            switch(version) {
                case 0:

                    if (t_A + 2 - out_A >= 0){
                        required_a = t_A + 2 - out_A;
                    }
                    else{
                        required_a = (t_A + 2 - out_A + N_CARDS) % N_CARDS;
                    }
                    if (len_A >= required_a && len_B >= required_a ) {
                        t_A = (t_A + 2) % N_CARDS;
                        t_B = (t_B + 2) % N_CARDS;
                    } else {
                        printf("%d ", 1);
                        printf("%d ", len_A);
                        printf("%d", len_B);
                        return;
                    }
                    break;
                case 1:
                    pack_A[out_A + len_A] = pack_A[out_A];
                    pack_B[out_B + len_B] = pack_B[out_B];
                    out_A = t_A;
                    out_B = t_B;
                    t_A = (t_A + 1) % N_CARDS;
                    t_B = (t_B + 1) % N_CARDS;
                    break;
            }

        }
        if (len_A == N_CARDS){
            printf("%d ", 2);
            printf("%d ", curr_num_conflicts);
            return;
        }else if (len_B == N_CARDS){
            printf("%d\n", 3);
            for (int i = 0; i < len_B; ++i) {
                printf("%d ", pack_B[(out_B + i) % N_CARDS]);
            }
//            printf("\n");
            return;
        }

        else if (curr_num_conflicts >= max){
            printf("%d ", 0);
            printf("%d ", len_A);
            printf("%d ", len_B);
            return;
        }
    }
}


int main(){
    scanf("%d", &seed);
    scanf("%d", &version);
    scanf("%d", &max);

    srand((unsigned) seed);


    for (int i = 0; i < N_CARDS; ++i) {
        table[i] = i;
    }
    rand_permutation(N_CARDS, table);
    dealing_cards();
    war_game();


    return 0;
}