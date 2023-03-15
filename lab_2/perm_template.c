#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define N 100

// Returns an integer from [a,b] using library function rand() and operator %
// if a > b return INT_MIN
// if b - a > RAND_MAX return INT_MAX
// if a == b return a
// else return integer from [a,b]
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

    int val = rand();
    val = a + val%(b-a+1);


    return val;

}

void swap (int array[], int i, int k) {
}

// random permutation of integers from [0, n-1]
void rand_permutation(int n, int array[]) {
    for (int i = 0; i < n; ++i) {
        array[i] = i;
    }

    for (int i = 0; i < n-1; ++i) {
        int k = rand_from_interval(i, n-1);
        int tmp = array[k];
        array[k] = array[i];
        array[i] = tmp;
        
    }
}

// bubble sort (increasing order)
// returns number of iterations of the external loop
// after which the array is sorted
// for { 0 1 2 3 7 4 5 6 } -> 1,
// for { 1 2 3 7 4 5 6 0 } -> 7,
// for { 0 1 2 3 4 5 6 7 } -> 0.
int bubble_sort(int n, int array[]) {
    for (int loop = 0; loop < n; ++loop) {
        printf("%d", array[loop]);
    }
    printf("\n");
    for (int i = 0; i < n-1; ++i) {
        int flag = 1;
        for (int j = 0; j < n-1; ++j) {
            if (array[j] > array[j+1]){
                int tmp = array[j];
                array[j] = array[j+1];
                array[j+1] = tmp;
                flag = 0;
            }
        }
        if (flag==1){
            for (int loop = 0; loop < n; ++loop) {
                printf("%d", array[loop]);
            }
            printf("\n");
            return i;
        }

    }
}

int main(void) {
	int to_do, seed;
	int a, b, m, n;
	int array[N];

	scanf("%d %d", &to_do, &seed);
	srand((unsigned) seed); // set the seed

	switch(to_do) {
		case 1:
			scanf("%d %d %d",&a, &b, &m);
			for(int i = 0; i < m; ++i) {
				printf("%d ", rand_from_interval(a, b));
			}
			printf("\n");
			break;
		case 2:
			scanf("%d", &n);
			rand_permutation(n, array);
			for(int i = 0; i < n; ++i) printf("%d ", array[i]);
			printf("\n");
			break;
		case 3:
			scanf("%d", &n);
			rand_permutation(n, array);
			printf("%d\n", bubble_sort(n, array));
			break;
		default:
			printf("NOTHING TO DO!\n");
			break;
	}
	return 0;
}

