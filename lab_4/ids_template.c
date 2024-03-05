#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char identifiers[MAX_IDS][MAX_ID_LEN];

const char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

int find_idents() {
    int c;
    int d;
    int x = 0;
    int y = 0;
    int similar;
    char ind[MAX_ID_LEN];

    while((c=getchar()) != EOF){
        if (c == '_' || ('a' <= c && c <= 'z' ) || ( 'A' <= c && c <= 'Z' ))
        {
            ind[x] = (char)c;
            x++;
            while((c=getchar())!= EOF && (( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) || ( c >= '0' && c <= '9' ) ||  c == '_' ))
            {
                ind[x] = (char)c;
                x++;
            }
            similar = 0;
            for(int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++)
            {
                if(x == strlen(keywords[i]))
                {
                    similar = 1;
                    for(int j = 0; j < x; j++ )
                    {
                        if(keywords[i][j] != ind[j] )
                        {
                            similar = 0;
                            break;
                        }
                    }
                    if(similar == 1)
                        break;
                }
            }
            if(similar == 0)
            {
                for( int k = 0; k < y; k++)
                {
                    if(x == strlen(identifiers[k]))
                    {
                        similar = 1;
                        for(int i = 0; i < x; i++ )
                        {
                            if(identifiers[k][i] != ind[i])
                            {
                                similar = 0;
                                break;
                            }
                        }
                        if(similar == 1)
                            break;
                    }
                }
                if(similar == 0)
                {
                    for(int i = 0; i < x; i++)
                    {
                        identifiers[y][i] = ind[i];
                    }
                    y++;
                }
            }
            x = 0;
        }
        if(c == '/')
        {
            c = getchar();

            if(c =='/')
            {
                while((c = getchar()) != EOF)
                {
                    if(c == '\n')
                        break;
                }
            }
            else if(c == '*')
            {
                d = getchar();
                while((c = getchar()) != EOF)
                {
                    if( d =='*' && c == '/')
                        break;
                    d = c;
                }
            }
        }
        if(c == '"')
        {
            d = getchar();
            if(d != '"')
            {
                while((c = getchar()) != EOF)
                {
                    if(d != '\\' && c == '"')
                        break;
                    d = c;
                }
            }
        }
        if(c == '\'')
        {
            while((c = getchar()) != EOF)
            {
                if(c == '\'')
                    break;
            }
        }
    }

    return y;
}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(identifiers[a], identifiers[b]);
}

int main(void) {
	printf("%d\n", find_idents());
	return 0;
}

