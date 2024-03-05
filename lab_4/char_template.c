#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_BIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_BIGRAMS] = { 0 };

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp (const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    if (count[va] == count[vb]) return va - vb;	return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di (const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    // sort according to second char if counts and the first char equal
    if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
    // sort according to first char if counts equal
    if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
    return count[vb] - count[va];
}

// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc) {
    int c;
    *nc = 0;
    *nl = 0;
    *nw = 0;
    int state = 0;

    while((c = getchar()) != EOF){

        *nc += 1;

        if(c == (int)NEWLINE){
            *nl += 1;
            if(state == 1)
                *nw += 1;
            state = 0;
        }
        else if(c == (int)' ' || c == (int)'	'){
            if(state == 1)
                *nw += 1;
            state = 0;
        }
        else{
            state = 1;
        }
    }

}

void char_count(int char_no, int *n_char, int *cnt) {
    int tab[MAX_CHARS];
    int ind[MAX_CHARS];
    int c;
    int tmp;
    int flag;

    for(int i = 0; i < MAX_CHARS; i++){
        tab[i] = 0;
        ind[i] = i;
    }

    while((c=getchar())!=EOF){
        if (FIRST_CHAR<=c && c<LAST_CHAR){
            tab[c-FIRST_CHAR]++;
        }
    }

    
    for(int i = 0; i < MAX_CHARS-1; i++)
    {
        flag = 0;
        for(int j = 1; j < MAX_CHARS-i; j++)
        {
            if(tab[ind[j-1]] < tab[ind[j]])
            {
                tmp = ind[j-1];
                ind[j-1] = ind[j];
                ind[j] = tmp;
                flag = 1;
            }
        }

        if(flag == 0) {
            break;
        }
    }

    *n_char = ind[char_no-1]+FIRST_CHAR;
    *cnt = tab[*n_char-FIRST_CHAR];
}

void bigram_count(int bigram_no, int bigram[]) {
    int c;
    int dig = 0;
    int tab[MAX_CHARS*MAX_CHARS];
    int ind[MAX_CHARS*MAX_CHARS];
    int tmp;
    int flag;

    for( int i = 0; i < MAX_CHARS*MAX_CHARS; i++)
    {
        tab[i] = 0;
        ind[i] = i;
    }

    while((c=getchar())!=EOF){
        if (FIRST_CHAR<=(int)c && (int)c<LAST_CHAR){
            if( dig == 0)
                dig = MAX_CHARS*(c-FIRST_CHAR);
            else
            {
                dig += (c-FIRST_CHAR);
                tab[dig]++;
                dig = MAX_CHARS*(c-FIRST_CHAR);
            }
        }
        else{
            dig = 0;
        }
    }


    for(int i = 0; i < MAX_CHARS*MAX_CHARS-1; i++)
    {
        flag = 0;
        for(int j = 1; j < MAX_CHARS*MAX_CHARS-i; j++)
        {
            if(tab[ind[j-1]] < tab[ind[j]])
            {
                tmp = ind[j-1];
                ind[j-1] = ind[j];
                ind[j] = tmp;
                flag = 1;
            }
        }

        if(flag == 0)
            break;
    }

    bigram[0] = ind[bigram_no-1]/MAX_CHARS + FIRST_CHAR;
    bigram[1] = ind[bigram_no-1]%MAX_CHARS + FIRST_CHAR;
    bigram[2] = tab[(bigram[0]-FIRST_CHAR)*MAX_CHARS + (bigram[1]-FIRST_CHAR)];
}

void find_comments(int *line_comment_counter, int *block_comment_counter) {
    int c = 0;
    int c_v2 = 0;

    int block = 0;
    int linear = 0;
    int gap = 2;
    *line_comment_counter = 0;
    *block_comment_counter = 0;

    c_v2 = getchar();

    while((c = getchar()) != EOF)
    {
        if(gap < 2 ) {
            gap += 1;
        }

        if(block == 0 && linear == 0)
        {
            if(c_v2 == (int)'/')
            {
                if(c == (int)'/' && gap > 1)
                {
                    *line_comment_counter+=1;
                    gap = 0;
                    linear = 1;
                }
                else if( c == (int)'*' )
                {
                    block = 1;
                    gap = 0;
                }
            }
        }
        if(c_v2 == (int)'*' && c == (int)'/' && block == 1 && gap == 2)
        {
            block = 0;
            *block_comment_counter+=1;
            gap = 0;
        }
        else if(c == (int)NEWLINE)
            linear = 0;
        c_v2 = c;
    }

}

#define MAX_LINE 128

int read_int() {
    char line[MAX_LINE];
    fgets(line, MAX_LINE, stdin); // to get the whole line
    return (int)strtol(line, NULL, 10);
}

int main(void) {
    int to_do;
    int nl, nw, nc, char_no, n_char, cnt;
    int line_comment_counter, block_comment_counter;
    int bigram[3];

    to_do = read_int();
    switch (to_do) {
        case 1: // wc()
            wc (&nl, &nw, &nc);
            printf("%d %d %d\n", nl, nw, nc);
            break;
        case 2: // char_count()
            char_no = read_int();
            char_count(char_no, &n_char, &cnt);
            printf("%c %d\n", n_char, cnt);
            break;
        case 3: // bigram_count()
            char_no = read_int();
            bigram_count(char_no, bigram);
            printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
            break;
        case 4:
            find_comments(&line_comment_counter, &block_comment_counter);
            printf("%d %d\n", block_comment_counter, line_comment_counter);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

