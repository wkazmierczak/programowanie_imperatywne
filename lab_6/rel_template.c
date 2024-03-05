#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
	int first;
	int second;
} pair;


// Add pair to existing relation if not already there
int add_relation (pair *tab, int n, pair new_pair) {

    for(int i = 0; i < n; i++)
    {
        if (tab[i].first == new_pair.first && tab[i].second == new_pair.second){
            return i;
        }
        else
        {
            if(tab[i].first == -1 && tab[i].second == -1)
            {
                tab[i].first = new_pair.first;
                tab[i].second = new_pair.second;
                return i;
            }
        }
    }

    return n;
}

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(const pair* relation, int size){

    for(int i = 0; i < size; i++)
    {
        if (relation[i].first == relation[i].second){
            return 1;
        }
    }
    return 0;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(const pair* relation, int size){
    if (is_reflexive(relation, size) == 0)
        return 1;

    return 0;
}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(const pair* relation, int size){
    int x;
    int y;
    int flag;

    for(int i = 0; i < size; i++)
    {

        x = relation[i].first;
        y = relation[i].second;
        flag = 0;

        for(int j = 0; j < size; j++)
        {
            if(y == relation[j].first && x == relation[j].second)
            {
                flag = 1;
                break;
            }
        }
        if(flag == 0)
            return 0;
    }
    return 1;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(const pair* relation, int size){
    int x;
    int y;

    for( int i = 0; i < size; i++ )
    {
        x = relation[i].first;
        y = relation[i].second;
        for( int j = 0; j < size; j++ )
        {
            if( y == relation[j].first && x == relation[j].second)
            {
                if( x != y )
                    return 0;
            }
        }
    }
    return 1;
}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(const pair* relation, int size){
    int x;
    int y;

    for(int i = 0; i < size; i++)
    {
        x = relation[i].first;
        y = relation[i].second;
        for( int j = 0; j < size; j++ )
        {
            if( y == relation[j].first && x == relation[j].second)
            {
                return 0;
            }
        }
    }
    return 1;
}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(const pair* relation, int size){
    int x, y, z;
    int flag;

    for(int i = 0; i < size; i++)
    {
        x = relation[i].first;
        y = relation[i].second;
        for( int j = 0; j < size; j++ )
        {
            if( y == relation[j].first )
            {
                z = relation[j].second;
                flag = 0;
                for( int k = 0; k < size; k++ )
                {
                    if( x == relation[k].first && z == relation[k].second)
                    {
                        flag = 1;
                        break;
                    }
                }

                if(flag == 0 )
                    return 0;
            }
        }
    }

    return 1;
}

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(const pair* relation, int size){
    if( is_reflexive(relation, size) == 1 && is_antisymmetric(relation, size) == 1 && is_transitive(relation, size) == 1 )
        return 1;

    return 0;
}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(const pair* relation, int size){
    int X[size];
    int new_size = 0;
    int x;
    int y ;
    int flag;
    int x1, x2, y1, y2;

    for (int i = 0; i < size; i++){
        x = relation[i].first;
        for (int j = 0; j < size; j++){
            y = relation[j].second;
            if(i == j){
                continue;
            }
            flag = 0;
            for (int k = 0; k < size; k++){
                if((relation[k].first == x && relation[k].second == y) || (relation[k].first == y && relation[k].second == x)){
                    flag = 1;
                    break;
                }
            }
            if (flag == 0){
                return 0;
            }
        }
    }

    for(int i = 0; i < size-1; i++){
        x1 = relation[i].first;
        y1 = relation[i].second;
        for (int j = i+1; j < size; j++){
            x2 = relation[j].first;
            y2 = relation[j].second;
            flag = 0;
            for (int k = 0; k < size; k++){
                if((relation[k].first == x1 && relation[k].second == x2) || (relation[k].first == x2 && relation[k].second == x1)){
                    flag = 1;
                    break;
                }
            }
            if (flag == 0){
                return 0;
            }

            flag = 0;
            for (int k = 0; k < size; k++){
                if((relation[k].first == y1 && relation[k].second == y2) || (relation[k].first == y2 && relation[k].second == y1)){
                    flag = 1;
                    break;
                }
            }
            if (flag == 0){
                return 0;
            }

        }
    }
    return 1;
}

// A total order relation is a partial order relation that is connected
int is_total_order(const pair* relation, int size){
    if( is_partial_order(relation, size) == 1 && is_connected(relation, size) == 1)
        return 1;

    return 0;
}



int find_max_elements(const pair* relation, int size, int* max_elements){
    int flag = 1;
    int max_size = 0;
    int tab[size];
    int flag2;


    for (int i = 0; i < size; i++) {
        tab[i] = relation[i].second;
    }

    while (flag == 1)
    {
        flag2 = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++){
                if (relation[j].first == tab[i]){
                    if (relation[j].first != relation[j].second){
                        tab[i] = relation[j].second;
                        flag2 = 1;
                    }
                }
            }
        }
        if (flag2 == 0){
            flag = 0;
        }
    }


    for (int i = 0; i < size; i++){
        flag = 0;
        for (int j = 0; j < max_size; j++){
            if (tab[i] == max_elements[j]){
                flag = 1;
            }
        }
        if (flag == 0){
            max_elements[max_size] = tab[i];
            max_size++;
        }
    }

    for (int i = 0; i < max_size; i++){
        for (int j = i+1; j < max_size; j++){
            if (max_elements[i] > max_elements[j]){
                int tmp = max_elements[i];
                max_elements[i] = max_elements[j];
                max_elements[j] = tmp;
            }
        }
    }
    return max_size;

}

int find_min_elements(const pair* relation, int size, int* min_elements){
    int flag = 1;
    int min_size = 0;
    int tab[size];
    int flag2;


    for (int i = 0; i < size; i++) {
        tab[i] = relation[i].first;
    }

    while (flag == 1)
    {
        flag2 = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++){
                if (relation[j].second == tab[i]){
                    if (relation[j].first != relation[j].second){
                        tab[i] = relation[j].first;
                        flag2 = 1;
                    }
                }
            }
        }
        if (flag2 == 0){
            flag = 0;
        }
    }


    for (int i = 0; i < size; i++){
        flag = 0;
        for (int j = 0; j < min_size; j++){
            if (tab[i] == min_elements[j]){
                flag = 1;
            }
        }
        if (flag == 0){
            min_elements[min_size] = tab[i];
            min_size++;
        }
    }

    for (int i = 0; i < min_size; i++){
        for (int j = i+1; j < min_size; j++){
            if (min_elements[i] > min_elements[j]){
                int tmp = min_elements[i];
                min_elements[i] = min_elements[j];
                min_elements[j] = tmp;
            }
        }
    }
    return min_size;

}
int get_domain(const pair* relation, int size, int* domain){
    int dom_size = 0;
    int flag;

    for (int i = 0; i < size; i++){
        flag = 0;
        for (int j = 0; j < dom_size; j++){
            if (relation[i].first == domain[j]){
                flag = 1;
            }
        }
        if (flag == 0){
            domain[dom_size] = relation[i].first;
            dom_size++;
        }

    }

    for (int i = 0; i < size; i++){
        flag = 0;
        for (int j = 0; j < dom_size; j++){
            if (relation[i].second == domain[j]){
                flag = 1;
            }
        }
        if (flag == 0){
            domain[dom_size] = relation[i].second;
            dom_size++;
        }

    }

    for (int i = 0; i < dom_size; i++){
        for (int j = i+1; j < dom_size; j++){
            if (domain[i] > domain[j]){
                int tmp = domain[i];
                domain[i] = domain[j];
                domain[j] = tmp;
            }
        }
    }
    return dom_size;
}

// Case 3:

int composition (const pair* relation, int size, const pair* relation2, int size2, pair* relation3){

    pair new_pair;
    int new_size = 0;
    int result;

    for( int i = 0; i < MAX_REL_SIZE; i++ )
    {
        relation3[i].first = -1;
        relation3[i].second = -1;
    }

    for( int i = 0; i < size2; i++ )
    {
        for( int j = 0; j < size; j++ )
        {
            if( relation2[i].first == relation[j].second )
            {
                new_pair.first = relation[j].first;
                new_pair.second = relation2[i].second;

                result = add_relation(relation3, size, new_pair );

                if( result > new_size )
                    new_size = result;
            }
        }
    }
    return new_size + 1;
}

// Comparator for pair
int cmp_pair (const void *a, const void *b) {
}

int insert_int (int *tab, int n, int new_element) {
}


// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
    int n;
    pair para;
    int result = 0;
    int max_size = 0;

    scanf("%d",&n);

    for( int i = 0; i < n; i++ )
    {
        relation[i].first = -1;
        relation[i].second = -1;
    }

    for( int i = 0; i < n; i++ )
    {
        scanf("%d",&para.first);
        scanf("%d",&para.second);

        result = add_relation( relation, n, para);

        if( result > max_size )
            max_size = result;
    }

    return max_size+1;
}

void print_int_array(const int *array, int n) {
    printf("%d\n", n);

    for (int i = 0; i < n; ++i)
    {
        printf("%d ", array[i]);
    }

    printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

