#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	int day; 
	int month; 
	int year;
} Date;

// 1 bsearch2

#define FOOD_MAX  30   // max. number of goods
#define ART_MAX   15   // max. length of name's name + 1
#define RECORD_MAX 40  // max. line length

typedef struct {
	char name[20];
	float price;
	int amount;
	Date valid_date;
} Food;

typedef int (*ComparFp)(const void *, const void *);

// compare dates
int cmp_date(const void *d1, const void *d2) {

    Date *date1 = (Date*) d1;
    Date *date2 = (Date*) d2;

    if (date1->year != date2->year) return date1->year - date2->year;
    if (date1->month != date2->month) return date1->month - date2->month;
    if (date1->day != date2->day) return date1->day - date2->day;
    return 0;

}

// compare foods
int cmp(const void *a, const void *b) {

    Food * food1 = (Food*)a;
    Food * food2 = (Food*)b;


    int name_cmp = strcmp(food1->name, food2->name);
    if (name_cmp != 0) return name_cmp;
    if (food1->price > food2->price) return 1;
    if (food1->price < food2->price) return -1;
    return cmp_date(&food1->valid_date, &food2->valid_date);

}


// bsearch returning address where to insert new element
void *bsearch2 (const void *key, const void *base, size_t nmemb, size_t size, ComparFp compar, char *result) {

    size_t p = 0;
    size_t q = nmemb;
    const void * curr;


    while (p < q){
        size_t mid = (p + q)/2;

        curr = base+(mid*size);

        int cmp = compar(key, curr);

        if (cmp == 0){
            *result = 1;
            return (void *)curr;
        }

        if (cmp < 0){
            q = mid;
        }

        else{
            p = mid + 1;
        }

    }
    *result = 0;
    return (void *)base+p*size;
}


// print goods of given name
void print_art(Food *p, int n, char *art) {
    for (int i = 0; i < n; i++){
        if (strcmp(p[i].name, art) == 0){
            printf("%.2f %.2d %02d.%02d.%d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month, p[i].valid_date.year);
        }
    }
}

// add record to table if absent
Food * add_record(Food *tab, int *np, ComparFp compar, Food *new) {
    char result;

    Food *curr = bsearch2(new, tab, *np, sizeof(Food), compar, &result);
//    printf("%p %p\n", tab, curr);


    if (result == 1) {
        curr->amount += new->amount;
    } else {
        for (Food * iter = &(tab[*np-1]); iter >= curr; iter--) {
            *(iter+1) = *iter;
        }
//        memmove(curr+1, curr, (&(tab[*np])-curr)* sizeof(Food));
        *curr = *new;
        (*np)++;
    }
    return curr;

}

// read no lines of data
// calls add_record if sorted = 1 and just adds element if sorted = 0
int read_goods(Food *tab, int no, FILE *stream, int sorted) {
    int num = 0;
//    char * art;

    for (int i = 0; i < no; i++){

        Food new_food;
        fscanf(stream, "%s %f %d %d.%d.%d", new_food.name, &new_food.price, &new_food.amount, &new_food.valid_date.day, &new_food.valid_date.month, &new_food.valid_date.year);
        add_record(tab, &num, cmp, &new_food);
//        print_art(tab, num, art);

    }
    return num;

}


int cmp_qs(const void *a, const void *b) {
	Food *fa = (Food*)a, *fb = (Food*)b;
	return cmp_date(&fa->valid_date, &fb->valid_date);
}

int cmp_bs(const void *a, const void *b) {
	Date *pd = (Date*)a;
	Food *fb = (Food*)b;
	return cmp_date(pd, &fb->valid_date);
}

// finds the value of goods due on 'curr_date'
float value(Food *food_tab, size_t n, Date curr_date, int days) {
    float lost_value = 0;

    qsort(food_tab, n, sizeof(Food), cmp_qs);

    struct tm curr_d;
    memset(&curr_d, 0, sizeof(struct tm));
    curr_d.tm_year = curr_date.year - 1900;
    curr_d.tm_mon = curr_date.month - 1;
    curr_d.tm_mday = curr_date.day;
    time_t curr_time = mktime(&curr_d);


    for (int i = 0; i < n; i++){
        struct tm tm;
        memset(&tm, 0, sizeof(struct tm));
        tm.tm_year = food_tab[i].valid_date.year - 1900;
        tm.tm_mon = food_tab[i].valid_date.month - 1;
        tm.tm_mday = food_tab[i].valid_date.day;

        time_t shelf_life = mktime(&tm);

        if(curr_time+(days*24*60*60) == shelf_life){
            lost_value += ((food_tab[i].amount) * (food_tab[i].price));

        }
    }

    return lost_value;
}

/////////////////////////////////////////////////////////////////
// 3 Succession

#define CHILD_MAX 20

enum Sex {F, M};
enum BOOL {no, yes};

struct Bit_data {
	enum Sex sex:1;
	enum BOOL in_line:1;
};

typedef struct {
	char *name;
	struct Bit_data bits;
	Date born;
	char *parent;
} Person;

typedef struct {
	char *par_name;
	int index;
    int num_of_children;
} Parent;

const Date primo_date = { 28, 10, 2011 }; // new succession act

int cmp_primo(Person *person1, Person *person2) {
	if (person1->bits.sex == person2->bits.sex) return 0;
	if (person1->bits.sex == F && cmp_date(&person1->born, &primo_date) > 0) return 0;
	if (person2->bits.sex == F && cmp_date(&person2->born, &primo_date) > 0) return 0;
	return person2->bits.sex - person1->bits.sex;
}

// compare persons
int cmp_person(const void *a, const void *b) {
    const Person *person_1 = (Person *)a;
    const Person *person_2 = (Person *)b;

    if(person_1->parent == NULL)return -1;
    if(person_2->parent == NULL)return 1;

    int par_comparision = strcmp(person_1->parent, person_2->parent);

    if (par_comparision != 0){
        return par_comparision;
    }

    int birthday_comparision = cmp_date(&person_1->born, &person_2->born);

    if(cmp_date(&person_1->born, &primo_date) < 0 && cmp_date(&person_2->born, &primo_date) < 0 ){
        if(person_1->bits.sex == M && person_2->bits.sex == F)return -1;
        if(person_1->bits.sex == F && person_2->bits.sex == M)return 1;
    }
    if(birthday_comparision != 0) {
        return birthday_comparision;
    }

    return person_1->bits.sex - person_2->bits.sex;

}


int fill_indices_tab(Parent* idx_tab, Person* pers_tab, int size) {
    int num_of_people = 0;
    for (int i = 0; i < size; i++) {
        idx_tab[i].par_name = pers_tab[i].name;
        idx_tab[i].index = 0;
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (idx_tab[i].par_name == pers_tab[j].parent) {
                idx_tab[i].index = j;
                break;
            }
        }
    }

    for (int i = 0; i < size; i++) {
        int cnt = 0;
        for (int j = idx_tab[i].index; j < size; j++) {
            if (pers_tab[j].parent != pers_tab[i].name) {
                break;
            }
            cnt += 1;
        }
        idx_tab[i].num_of_children = cnt;
    }
    for (int i = 0; i < size; i++) {
        if (idx_tab[i].num_of_children > 0) num_of_people += 1;
    }
    return num_of_people;
}

void persons_shifts(Person *person_tab, int size, Parent *idx_tab, int no_parents) {
    for(int i = 0; i < size; i++){
        if(idx_tab[i].num_of_children > 0){

            Person *additional_array = malloc((idx_tab[i].num_of_children) * sizeof(Person));

            memmove(additional_array, &person_tab[idx_tab[i].index], (idx_tab[i].num_of_children) * sizeof(Person));

            int num = idx_tab[i].index - (i + 1);
            int new_idx = idx_tab[i].index - num + idx_tab[i].num_of_children;

            memmove(&person_tab[new_idx],&person_tab[i+1],num*sizeof(Person));
            memmove(&person_tab[i+1], additional_array, idx_tab[i].num_of_children * sizeof(Person));
            free(additional_array);

            no_parents = fill_indices_tab(idx_tab, person_tab, size);
        }
    }
}

int cleaning(Person *person_tab, int n) {
    for (int i = 0; i < n; i++){
        if (person_tab[i].bits.in_line == no){
            memmove(&person_tab[i], &person_tab[i+1], (n-i-1)*sizeof(Person));
            i -= 1;
            n -= 1;
        }
    }
    return n;
}

void print_person(const Person *p) {
	printf("%s\n", p->name);
}

int create_list(Person *person_tab, int n) {
    qsort(person_tab, n, sizeof(Person), cmp_person);
    Parent index_tab[n];
    int num_of_parents = fill_indices_tab(index_tab, person_tab, n);
    persons_shifts(person_tab, n, index_tab, num_of_parents);

    return cleaning(person_tab, n);
}

int main(void) {
	Person person_tab[] = {
		{"Charles III", {M, no}, {14, 11, 1948},"Elizabeth II"},
		{"Anne", {F,yes}, {15, 8, 1950},"Elizabeth II"},
		{"Andrew", {M,yes}, {19, 2, 1960},"Elizabeth II"},
		{"Edward", {M,yes}, {10, 3, 1964} ,"Elizabeth II"},
		{"David", {M,yes}, {3, 11, 1961} ,"Margaret"},
		{"Sarah", {F,yes}, {1, 5, 1964}, "Margaret"},
		{"William", {M,yes}, {21, 6, 1982}, "Charles III"},
		{"Henry", {M,yes}, {15, 9, 1984}, "Charles III"},
		{"Peter", {M,yes}, {15, 11, 1977}, "Anne"},
		{"Zara", {F,yes}, {15, 5, 1981}, "Anne"},
		{"Beatrice", {F,yes}, {8, 8, 1988}, "Andrew"},
		{"Eugenie", {F,yes}, {23, 3, 1990}, "Andrew"},
		{"James", {M,yes}, {17, 12, 2007}, "Edward"},
		{"Louise", {F,yes}, {8, 11, 2003}, "Edward"},
		{"Charles", {M,yes}, {1, 7, 1999}, "David"},
		{"Margarita", {F,yes}, {14, 5, 2002}, "David"},
		{"Samuel", {M,yes}, {28, 7, 1996}, "Sarah"},
		{"Arthur", {M,yes}, {6, 5, 2019}, "Sarah"},
		{"George", {M,yes}, {22, 7, 2013}, "William"},
		{"George VI", {M,no}, {14, 12, 1895}, NULL},
		{"Charlotte", {F,yes}, {2, 5, 2015}, "William"},
		{"Louis", {M,yes}, {23, 4, 2018}, "William"},
		{"Archie", {M,yes}, {6, 5, 2019}, "Henry"},
		{"Lilibet", {F,yes}, {4, 6, 2021}, "Henry"},
		{"Savannah", {F,yes}, {29, 12, 2010}, "Peter"},
		{"Isla", {F,yes}, {29, 3, 2012}, "Peter"},
		{"Mia", {F,yes}, {17, 1, 2014}, "Zara"},
		{"Lena", {F,yes}, {18, 6, 2018}, "Zara"},
		{"Elizabeth II", {F,no}, {21, 4, 1925}, "George VI"},
		{"Margaret", {F,no}, {21, 8, 1930}, "George VI"},
		{"Lucas", {M,yes}, {21, 3, 2021}, "Zara"},
		{"Sienna", {F,yes}, {18, 9, 2021}, "Beatrice"},
		{"August", {M,yes}, {9, 2, 2021}, "Eugenie"}
	};

	int to_do, no;
	int n;
	Food food_tab[FOOD_MAX];
	char buff[ART_MAX];
	fgets(buff, ART_MAX, stdin);
	sscanf(buff, "%d", &to_do);

	switch (to_do) {
		case 1:  // bsearch2
			fgets(buff, ART_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 1);
			scanf("%s",buff);
			print_art(food_tab, n, buff);
			break;
		case 2: // qsort
			fgets(buff, ART_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 0);
			Date curr_date;
			int days;
			scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
			scanf("%d", &days);
			printf("%.2f\n", value(food_tab, (size_t)n, curr_date, days));
			break;
		case 3: // succession
			scanf("%d",&no);
			int no_persons = sizeof(person_tab) / sizeof(Person);
			create_list(person_tab,no_persons);
			print_person(person_tab + no - 1);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}
