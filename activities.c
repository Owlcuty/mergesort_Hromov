#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
	int s;
	int f;
} activity_t;

int comparator(void const* v1, void const* v2) {
	activity_t* a1 = (activity_t*)v1;
	activity_t* a2 = (activity_t*)v2;

	return a1->f - a2->f;
}

void print_activity(activity_t const* a) {
	printf("\tactivity:\n\t\tstart time is %i\n\t\tfinish time is %i\n", a->s, a->f);
}

int main() {
	srand(clock());

	int n = 10;

	activity_t* a = (activity_t*)malloc(sizeof(activity_t) * n);

	printf("Generation:\n");
	for(int i = 0; i < n; ++i) {
		a[i].s = rand() % 100;

		do { a[i].f = rand() % 100; } 
		while(a[i].f <= a[i].s);

		print_activity(&a[i]);
	}

	qsort(a, n, sizeof(activity_t), comparator);

	printf("\n--------------\n");
	printf("Selected activities:\n");

	int k = 1;
	int idx = 0;
	print_activity(&a[0]);

	for(int i = 1; i < n; ++i) 
		if(a[i].s >= a[idx].f) {
			print_activity(&a[i]);
			++k;
			idx = i;
		}
	
	return 0;
}

