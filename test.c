#include <stdio.h>

void linspace(double v[], double start, double stop, int n) {

    double diff;

    if (n == 0){
        return;
    }
    if (n == 1){
        v[0] = start;
        return;
    }
    diff = (stop - start) / (n-1);

    for (int i = 0; i < n; i++){
        v[i] = start + i*diff;
    }

}

void print_vector(const double v[], int n) {
    for (int i = 0; i < n; ++i) {
        printf("%.2f ", v[i]);
    }
    printf("\n");
}

int main(){
    double v[100];
    double start = -10;
    double stop = 10;
    double n = 11;
    linspace(v, start, stop, n);
    print_vector(v, n);
}