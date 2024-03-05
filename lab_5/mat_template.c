#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {

    for (int i = 0; i < m; i++) {
        for (int k=0; k < n; k++){
            double sum = 0;
            for (int j = 0; j < p; j++) {
                sum += A[i][j] * B[j][k];
            }
            AB[i][k] = sum;
        }
    }
}


// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {
    double wyznacznik = 1;
    double div = 0;

    for (int k = 0; k < n-1; k++){
        for (int i = k+1; i<n; i++){

            if (A[k][k] == 0){
                return NAN;
            }
            div = A[i][k]/A[k][k];
            for(int j = k + 1; j <= n; j++){
                A[i][j] = A[i][j] - div*A[k][j];

            }
        }
    }

    for (int l = 0; l < n; l++){
        wyznacznik *= A[l][l];
    }
    return wyznacznik;
}

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n) {
}

// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps) {
    double b_v2[n];
    int ind[n];
//    double abs_v;
    double wyznacznik = 1;
    double div;

    for (int i = 0; i <n; i++){
        b_v2[i] = b[i];
        ind[i] = i;
    }

    for (int k = 0; k<n-1; k++){
        //double max_value = 0;
        double max_value = A[ind[k]][k];
        int max_row;

        for(int w = k; w < n; w++){
//            if (A[ind[w]][k]>=0){
//                abs_v = A[ind[w]][k];
//            }
//            else{
//                abs_v = -A[ind[w]][k];
//            }
            if (fabs(A[ind[w]][k]) > fabs(max_value)){
                max_value = A[ind[w]][k];
                max_row = w;
            }
        }
        if (ind[k]!=max_row) {
            int tmp = ind[k];
            ind[k] = ind[max_row];
            ind[max_row] = tmp;
            wyznacznik *= (-1);

        }

        if(A[ind[k]][k] < eps){
            return 0;
        }

        for (int i = k+1; i<n; i++){


            if(fabs(A[ind[k]][k]) < eps){
                return 0;
            }

            div = A[ind[i]][k]/A[ind[k]][k];

            for(int j = k; j < n; j++){
                A[ind[i]][j] = A[ind[i]][j] - div*A[ind[k]][j];
            }
            b_v2[ind[i]] -= div*b_v2[ind[k]];

        }
    }

    int cnt = 0;

    for (int l = 0; l < n; l++){
        wyznacznik *= A[ind[l]][l];
        if (b[l]==0){
            cnt+=1;
        }
    }
    if (wyznacznik==0 || cnt ==n){
        return 0;
    }


    for (int i = n-1; i >= 0; i--){
        int y = 1;
        double sum = 0;
        while (y < n - i){
            sum += A[ind[i]][n-y] * x[n-y];
            y++;

        }
        if (fabs(A[ind[i]][i]) < eps){
            return 0;
        }
        x[i] = (b_v2[ind[i]] - sum)/ A[ind[i]][i];
    }


    return wyznacznik;
}

// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
    int ind[n];
    double wyznacznik = 1;

    for (int i = 0; i<n; i++){
        B[i][i] = 1;
    }


    for (int i = 0; i<n; i++){
        ind[i] = i;
    }

    for (int k = 0; k<n; k++){
        double max_value = 0;
        int max_row;

        for(int w = k; w < n; w++){
//            if (A[ind[w]][k] >= 0){
//                abs_v = A[ind[w]][k];
//            }
//            else{
//                abs_v = -A[ind[w]][k];
//            }
            if (fabs(A[ind[w]][k]) > fabs(max_value)){
                max_value = A[ind[w]][k];
                max_row = w;
            }
        }
        if (k!=max_row) {
            int tmp = ind[k];
            ind[k] = ind[max_row];
            ind[max_row] = tmp;
            wyznacznik *= -1;
        }

//        if(max_value < eps){
//            return 0;
//        }

        for (int i = k+1; i<n; i++){
            double div;

            if (A[ind[i]][k] == 0 || A[ind[k]][k] == 0){
                continue;
            }
            if( fabs(A[ind[k]][k]) < eps )
            {
                for( int w = 0; w < n; w++ )
                    for( int j = 0; j < n; j++)
                        B[w][j] = 0;
                return 0;
            }

            div = A[ind[i]][k]/A[ind[k]][k];

            for(int j = k; j < n; j++){
                A[ind[i]][j] = A[ind[i]][j] - div*A[ind[k]][j];
            }

            for(int w = 0; w < n; w++){
                B[ind[i]][w] = B[ind[i]][w] - div*B[ind[k]][w];
            }


        }
//        print_mat(A, n, n);
//        printf("-----------kl----\n");
    }

    for (int l = 0; l < n; l++){
        wyznacznik *= A[ind[l]][l];
    }


    for (int i = 0; i < n; i++){
        double div;
        div = 1/A[ind[i]][i];
        for (int j = i; j<n; j++){
            A[ind[i]][j] = div*A[ind[i]][j];
        }
        for(int w = 0; w < n; w++){
            B[ind[i]][w] = B[ind[i]][w]*div;
        }
    }

//    print_mat(A, n, n);
//    printf("---------------\n");

    for (int k = n-1; k > 0; k--) {


        for (int i = k - 1; i >= 0; i--) {
            double div;

            if (A[ind[i]][k] == 0) {
                continue;
            }

            div = A[ind[i]][k] / A[ind[k]][k];

            for (int j = k; j < n; j++) {
                A[ind[i]][j] = A[ind[i]][j] - div * A[ind[k]][j];
            }
            for(int w = 0; w < n; w++){
                B[ind[i]][w] = B[ind[i]][w] - div*B[ind[k]][w];
            }

        }
    }
    double C[n][n];

    for (int i =0; i<n; i++){
        for (int j = 0; j < n; j++){
            C[i][j] = B[ind[i]][j];
        }
    }

    for (int i =0; i<n; i++){
        for (int j = 0; j < n; j++){
            B[i][j] = C[i][j];
        }
    }



//    print_mat(A, n, n);
//    printf("---------------\n");

    return wyznacznik;


}

int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if (det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A, n, n);
			det = matrix_inv(A, B, n, eps);
			printf("%.4f\n", det);
			if (det) print_mat(B, n, n);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

