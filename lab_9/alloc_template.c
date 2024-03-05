#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int get(int cols, int row, int col, const int *A) {
    return A[col + row*cols];
}

void set(int cols, int row, int col, int *A, int value) {
    A[col + row*cols] = value;
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB) {
    int x, y;
    int res;

    for (int iter = 0; iter < rowsA; iter++) {
        for (int i = 0; i < colsB; i++) {
            res = 0;
            for (int j = 0; j < colsA; j++){
                x = get(colsA, iter, j, A);
                y = get(colsB, j, i, B);
                res += x*y;
            }
            set(colsB, iter, i, AB, res);
        }
    }
}

void read_mat(int rows, int cols, int *t) {
    int value;

    for (int row = 0; row < rows; row++){
        for (int col = 0; col < cols; col++){
            scanf("%d", &value);
            set(cols, row, col, t, value);
        }
    }
}

void print_mat(int rows, int cols, int *t) {
    for (int row = 0; row < rows; row++){
        for (int col = 0; col < cols; col++){
            printf("%d ", get(cols, row, col, t));
        }
        printf("\n");
    }
}

int read_char_lines(char *array[]) {
    int lines = 0;
    array[lines] = (char *) malloc(BUF_SIZE);
    while(fgets(array[lines], BUF_SIZE, stdin) != NULL){
        lines++;
        array[lines] = (char *)malloc(BUF_SIZE);
    }
    return lines;
}

void write_char_line(char *array[], int n) {

    while (*array[n]!='\0'){
        printf("%c", *array[n]);
        array[n]++;
    }
    
}

void delete_lines(char *array[]) {
//    for (int i = 0; i < BUF_SIZE; i++){
//        free(array[i]);
//    }
}

int read_int_lines_cont(int *ptr_array[]) {
    int value= 0;
    int lines = 0;
    int elem_count = 0;
    int flag = 1;
    char text[1000];
    int buffer[BUF_SIZE];
    ptr_array[lines] = buffer;

    while(fgets(text, 1000, stdin) != NULL){
        int i = 0;
        while(text[i] != '\n'){
            if(text[i] == '-'){
                flag = -1;
            }
            else if (text[i] != ' '){
                value = value * 10 + text[i] - '0';
            }
            else{
                buffer[elem_count] = value * flag;
                value = 0;
                flag = 1;
                elem_count++;
            }
            i++;
        }
        buffer[elem_count] = value * flag;
        flag = 1;
        value = 0;
        lines++;
        elem_count++;
        ptr_array[lines] = buffer + elem_count;
    }
    return lines;
}

void write_int_line_cont(int *ptr_array[], int n) {
    while(ptr_array[n] != ptr_array[n+1]){
        printf("%d ", *ptr_array[n]);
        ptr_array[n]++;
    }
}

typedef struct {
	int *values;
	int len;
	double average;
} line_type;

int read_int_lines(line_type lines_array[]) {
    int line_count = 0;
    char buffer[BUF_SIZE];

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        char *token = strtok(buffer, " ");
        int *values = malloc(sizeof(int));
        int sum = 0;
        int cnt = 0;

        while (token != NULL) {
            int value = atoi(token);
            values = realloc(values, (cnt + 1) * sizeof(int));
            values[cnt] = value;
            sum += value;
            cnt++;

            token = strtok(NULL, " ");
        }


        double average = sum / cnt;

        lines_array[line_count].values = values;
        lines_array[line_count].len = cnt;
        lines_array[line_count].average = average;

        line_count++;
    }

    return line_count;
}

void write_int_line(line_type lines_array[], int n) {
    line_type line = lines_array[n];

    for (int i = 0; i < line.len; i++) {
        printf("%d ", line.values[i]);
    }

    printf("\n%.2f", line.average);
}

void delete_int_lines(line_type array[], int line_count) {
    for (int i = 0; i < line_count; i++){
        free(array[i].values);
    }
}

int cmp (const void *a, const void *b) {
    line_type * line_1 = (line_type*)a;
    line_type * line_2 = (line_type*)b;

    return line_1->average - line_2->average;
}

void sort_by_average(line_type lines_array[], int line_count) {
    qsort(lines_array, line_count, sizeof(line_type), cmp);
}

typedef struct {
	int r, c, v;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets) {
    for (int i = 0; i < n_triplets; i++) {
        scanf("%d %d %d", &triplet_array[i].r, &triplet_array[i].c, &triplet_array[i].v);
    }
    return n_triplets;
}

int cmp_triplets(const void *t1, const void *t2) {
    triplet * triplet_1 = (triplet *)t1;
    triplet * triplet_2 = (triplet *)t2;

    if (triplet_1->r == triplet_2->r){
        if(triplet_1->c==triplet_2->c){
            return triplet_1->v - triplet_2->v;
        }
        return triplet_1->c - triplet_2->c;
    }
    return triplet_1->r - triplet_2->r;
}


void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {
    qsort(triplet_array, n_triplets, sizeof(triplet), cmp_triplets);

    for (int i = 0; i < n_triplets; i++) {
        V[i] = triplet_array[i].v;
        C[i] = triplet_array[i].c;
    }

    int current_row = 0;
    R[0] = 0;

    for (int i = 1; i <= rows; i++) {
        int cnt = 0;

        while (current_row < n_triplets && triplet_array[current_row].r == i - 1) {
            current_row++;
            cnt++;
        }
        R[i] = R[i - 1] + cnt;
    }
}

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) {
    for (int i = 0; i < rows; i++) {

        y[i] = 0;

        for (int j = R[i]; j < R[i + 1]; j++) {
            y[i] += V[j] * x[C[j]];
        }
    }
}

void read_vector(int *v, int n) {
    for (int i = 0; i < n; i++) {
        scanf("%d", &v[i]);
    }
}

void write_vector(int *v, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

int read_int() {
	char c_buf[BUF_SIZE];
	fgets(c_buf, BUF_SIZE, stdin);
	return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
	int to_do = read_int();

	int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
	int n, lines_counter, rowsA, colsA, rowsB, colsB;
	int rows, cols, n_triplets;
	char *char_lines_array[TAB_SIZE] = { NULL };
	int continuous_array[TAB_SIZE];
	int *ptr_array[TAB_SIZE];
	triplet triplet_array[TAB_SIZE];
	int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
	int x[TAB_SIZE], y[TAB_SIZE];
	line_type int_lines_array[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d", &rowsA, &colsA);
			read_mat(rowsA, colsA, A);
			scanf("%d %d", &rowsB, &colsB);
			read_mat(rowsB, colsB, B);
			prod_mat(rowsA, colsA, colsB, A, B, AB);
			print_mat(rowsA, colsB, AB);
			break;
		case 2:
			n = read_int() - 1; // we count from 1 :)
			ptr_array[0] = continuous_array;
			read_int_lines_cont(ptr_array);
			write_int_line_cont(ptr_array, n);
			break;
		case 3:
			n = read_int() - 1;
			read_char_lines(char_lines_array);
			write_char_line(char_lines_array, n);
			delete_lines(char_lines_array);
			break;
		case 4:
			n = read_int() - 1;
			lines_counter = read_int_lines(int_lines_array);
			sort_by_average(int_lines_array, lines_counter);
			write_int_line(int_lines_array, n);
			delete_int_lines(int_lines_array, lines_counter);
			break;
		case 5:
			scanf("%d %d %d", &rows, &cols, &n_triplets);
			n_triplets = read_sparse(triplet_array, n_triplets);
			read_vector(x, cols);
			make_CSR(triplet_array, n_triplets, rows, V, C, R);
			multiply_by_vector(rows, V, C, R, x, y);
			write_vector(V, n_triplets);
			write_vector(C, n_triplets);
			write_vector(R, rows + 1);
			write_vector(y, rows);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

