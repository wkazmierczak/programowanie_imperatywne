#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

struct List;
typedef void (*DataFp)(void*);
typedef void (*ConstDataFp)(const void*);
typedef  int (*CompareDataFp)(const void*, const void*);

typedef struct ListElement {
    struct ListElement *next;
    void *data;
} ListElement;

typedef struct {
    ListElement *head;
    ListElement *tail;
    ConstDataFp dump_data;
    DataFp free_data;
    CompareDataFp compare_data;
    DataFp modify_data;
} List;

void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if(ptr) return ptr;
    printf("malloc error\n");
    exit(EXIT_FAILURE);
}

void *safe_strdup(const char *string) {
    void *ptr = strdup(string);
    if(ptr) return ptr;
    printf("strdup error\n");
    exit(EXIT_FAILURE);
}

// --- generic functions --- for any data type

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data,
               CompareDataFp compare_data, DataFp modify_data) {
    p_list->head = NULL;
    p_list->tail = NULL;
    p_list->dump_data = dump_data;
    p_list->free_data = free_data;
    p_list->compare_data = compare_data;
    p_list->modify_data = modify_data;
}

// Print elements of the list
void dump_list(const List* p_list) {
    ListElement * curr_elem = p_list->head;
    while(curr_elem != NULL){
        p_list->dump_data(curr_elem->data);
        curr_elem = curr_elem->next;
    }
    printf("\n");
}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data) {
    ListElement * curr_elem = p_list->head;
    while(curr_elem != NULL) {
        if (p_list->compare_data(curr_elem->data, data) == 0) {
            p_list->dump_data(curr_elem->data);
        }
        curr_elem = curr_elem->next;
    }
    printf("\n");

}

// Free element pointed by data using free_data() function
void free_element(DataFp free_data, ListElement *to_delete) {
    free_data(to_delete);
    free(to_delete);
}

// Free all elements of the list
void free_list(List* p_list) {
    ListElement *iter = p_list->head;
    ListElement *tmp;

    while (iter != NULL){
        tmp = iter->next;
        free_element(p_list->free_data, iter);
        iter = tmp;
    }
}

// Push element at the beginning of the list
void push_front(List *p_list, void *data){
    ListElement * element = (ListElement*) safe_malloc(sizeof(ListElement));
    element->next = p_list->head;
    element->data = data;
    if (p_list->head == NULL){
        p_list->tail = element;
    }
    p_list->head = element;
}

// Push element at the end of the list
void push_back(List *p_list, void *data) {
    ListElement * element = (ListElement*) safe_malloc(sizeof(ListElement));
    element->next = NULL;
    element->data = data;
    if (p_list->tail != NULL){
        p_list->tail->next = element;
        p_list->tail = element;
    }
    else{
        p_list->head = element;
        p_list->tail = element;
    }

}

// Remove the first element
void pop_front(List *p_list) {
    ListElement * toDelete;
    if(p_list->head==NULL){
        return;
    }
    toDelete = p_list->head;
    p_list->head = p_list->head->next;
    if (p_list->head == NULL){
        p_list->tail = NULL;
    }
    free(toDelete);
}

// Reverse the list
void reverse(List *p_list) {
    ListElement * iter = p_list->head;
    ListElement * prev = NULL;

    p_list->tail = p_list->head;

    while (iter != NULL){
        iter = iter->next;
        p_list->head->next = prev;
        prev = p_list->head;
        if (iter != NULL){
            p_list->head = iter;
        }
    }
}

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List *p_list, ListElement *p_element) {
    ListElement * iter = p_list->head;
    ListElement * prev = NULL;
    while (iter != NULL && p_list->compare_data(iter->data, p_element->data)<0){
        prev = iter;
        iter = iter->next;
    }
    return prev;

}

// Insert element after 'previous'
void push_after(List *p_list, void *data, ListElement *previous) {
    ListElement *elem = (ListElement *) data;

    if (previous != NULL) {
        elem->next = previous->next;
        previous->next = elem;

        if(previous == p_list->tail){
            p_list->tail = elem;
        }
    }
    else{
        elem->next = p_list->head;

        if (p_list->head == NULL){
            p_list->tail = elem;
        }
        p_list->head = elem;
    }
}

// Insert element preserving order
void insert_in_order(List *p_list, void *p_data) {
    ListElement *elem = safe_malloc(sizeof(ListElement));

    elem->data = p_data;

    ListElement *a = find_insertion_point(p_list, elem);
    if (a == NULL && p_list->head == NULL){
        push_after(p_list, elem, a);
    }
    else if (a == NULL && (p_list->compare_data)(p_list->head->data, elem->data) != 0){
        push_after(p_list, elem, a);
    }
    else if (a == NULL && (p_list->compare_data)(p_list->head->data, elem->data) == 0){
        if (p_list->modify_data != NULL){
            (p_list->modify_data)(p_list->head->data);
        }
    }
    else if (a->next == NULL || (p_list->compare_data)(a->next->data, elem->data) != 0){
        push_after(p_list, elem, a);
    }
    else{
        if (p_list->modify_data != NULL){
            (p_list->modify_data)(a->next->data);
        }
    }
}

// -----------------------------------------------------------
// --- type-specific definitions

// int element


void dump_int(const void *d) {
    const int * elem = (const int*)d;
    printf("%d ", *elem);
}

void free_int(void *d) {
    ListElement *elem = (ListElement*)d;
    free(elem->data);
}

int cmp_int(const void *a, const void *b) {
    const int *elem1 = (const int*)a;
    const int *elem2 = (const int*)b;
    return *elem1 - *elem2;
}

int *create_data_int(int v) {
    int *elem = safe_malloc(sizeof(int));
    *elem = v;
    return elem;
}

// Word element

typedef struct DataWord {
    char *word;
    int counter;
} DataWord;

void dump_word (const void *d) {
    DataWord * elem = (DataWord*) d;
    printf("%s ", elem->word);
}

void dump_word_lowercase(const void *d){
    DataWord *elem = (DataWord*)d;
    printf("%s ", elem->word);

}

void free_word(void *d) {
    DataWord * elem = (DataWord*) d;
    free(elem->word);
}

int cmp_word_alphabet(const void *a, const void *b) {
    DataWord * elem1 = (DataWord*) a;
    DataWord * elem2 = (DataWord*) b;

    return strcmp(elem1->word, elem2->word);
}

int cmp_word_counter(const void *a, const void *b) {
    DataWord * elem1 = (DataWord*) a;
    DataWord * elem2 = (DataWord*) b;

    return elem1->counter - elem2->counter;
}

void modify_word(void *p) {
    DataWord * elem = (DataWord*) p;
    elem->counter++;
}

void *create_data_word(const char *string, int counter) {
    DataWord *elem = (DataWord*)safe_malloc(sizeof(DataWord));
    elem->word = safe_strdup(string);
    elem->counter = counter;
    return elem;
}

// read text, parse it to words, and insert those words to the list.
// Order of insertions is given by the last parameter of type CompareDataFp.
// (comparator function address). If this address is not NULL the element is
// inserted according to the comparator. Otherwise, read order is preserved.
void stream_to_list(List *p_list, FILE *stream, CompareDataFp cmp) {
    char delimiters[] = " .,?!:;-\n\t\r\v\f";
    char str[BUFFER_SIZE];
    char * schowek;
    DataWord * elem;
    p_list->compare_data=cmp;
    while (fgets(str, BUFFER_SIZE, stream)!=NULL){
        schowek = strtok(str, delimiters);
        while(schowek!= NULL){
            elem = create_data_word(schowek, 1);
            if(cmp != NULL){
                for (int i = 0; i < strlen(elem->word); i++)
                {
                    elem->word[i] = tolower(elem->word[i]);
                }
                insert_in_order(p_list, elem);
            }
            else{
                push_back(p_list, elem);
            }
            schowek = strtok(NULL,delimiters);
        }
    }

}

// test integer list
void list_test(List *p_list, int n) {
    char op;
    int v;
    for (int i = 0; i < n; ++i) {
        scanf(" %c", &op);
        switch (op) {
            case 'f':
                scanf("%d", &v);
                push_front(p_list, create_data_int(v));
                break;
            case 'b':
                scanf("%d", &v);
                push_back(p_list, create_data_int(v));
                break;
            case 'd':
                pop_front(p_list);
                break;
            case 'r':
                reverse(p_list);
                break;
            case 'i':
                scanf("%d", &v);
                insert_in_order(p_list, create_data_int(v));
                break;
            default:
                printf("No such operation: %c\n", op);
                break;
        }
    }
}

int main(void) {
    int to_do, n;
    List list;

    scanf ("%d", &to_do);
    switch (to_do) {
        case 1: // test integer list
            scanf("%d",&n);
            init_list(&list, dump_int, free_int, cmp_int, NULL);
            list_test(&list, n);
            dump_list(&list);
            free_list(&list);
            break;
        case 2: // read words from text, insert into list, and print
            init_list(&list, dump_word, free_word, NULL, NULL);
            stream_to_list(&list, stdin, NULL);
            dump_list(&list);
            free_list(&list);
            break;
        case 3: // read words, insert into list alphabetically, print words encountered n times
            scanf("%d",&n);
            init_list(&list, dump_word_lowercase, free_word, NULL, modify_word);
            stream_to_list(&list, stdin, cmp_word_alphabet);
            list.compare_data = cmp_word_counter;
            DataWord data = { NULL, n };
            dump_list_if(&list, &data);
            free_list(&list);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

