#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define handle_error(msg) \
	do { perror(msg); exit(errno); } while (0)

#ifdef MERGE_SORT_DEBUG
#define ERRPRINTF(format, ...)	fprintf(stderr, "%d::%s::%s__::__ " format "\n", __LINE__, __FILENAME__, __PRETTY_FUNCTION__, ## __VA_ARGS__)
#else
#define ERRPRINTF(format, ...)
#endif

void merge(int* left, int* split, int* right, int* buf)
{
	int* cbuf = buf;
	int* lcur = left;
	int* rcur = split;

	while (lcur < split && rcur < right)
	{
		if (*lcur < *rcur)
		{
			*cbuf = *lcur;
			lcur++;
		}
		else
		{
			*cbuf = *rcur;
			rcur++;
		}
		++cbuf;
	}

	if (lcur < split)
		memcpy (cbuf, lcur, sizeof(int) * (split - lcur));
	if (rcur < right)
		memcpy (cbuf, rcur, sizeof(int) * (right - split));

	memcpy (left, buf, sizeof(int) * (right - left));
}

void merge_sort_ (int* left, int* right, int* buf)
{
	if (left + 1 < right)
	{
		int* split = left + (right - left) / 2;
		int* buf_end = buf + (right - left);

		merge_sort_ (left, split, buf);
		merge_sort_ (split, right, buf_end);
		merge (left, split, right, buf);
	}
}

void merge_sort (int* array, uint32_t size)
{
	if (array == NULL)
	{
		errno = EINVAL;
		handle_error("fail merge_sort: ");
	}

	int* buf = (int*)calloc(size, sizeof(*buf));

	merge_sort_ (array, array + size, buf);

	free (buf);
}

typedef struct {
	int* data;
	size_t size;
	size_t capacity;
} vec_t;


int const vec_init(vec_t* vec, size_t capacity) {
	assert(vec != NULL);
	assert(capacity != 0);

	vec->data = (int*)malloc(sizeof(int) * capacity);
	if(vec->data == NULL)
		return 1;

	vec->size = 0;
	vec->capacity = capacity;
	return 0;
}
void vec_free(vec_t* vec) {
	assert(vec != NULL);
	free(vec->data);
}
int const vec_push(vec_t* vec, int value) {
	assert(vec != NULL);

	if(vec->size == vec->capacity) {
		size_t ncap = vec->capacity * 2;
		
		int* ndata = (int*)realloc(vec->data, sizeof(int) * ncap);
		if(ndata == NULL)
			return 1;

		vec->data = ndata;
		vec->capacity = ncap;
	}

	vec->data[vec->size++] = value;
	return 0;
}


int main(int argc, char* argv[])
{

#define ifkey(key) \
	if(strcmp(argv[argn], key) == 0)

	int argn = 0;
	while(argn != argc) {
		if(argn + 1 != argc) {
			ifkey("-f") {
				freopen(argv[argn + 1], "r", stdin);
				if (stdin == NULL)
					handle_error ("fail freopen '-f': ");
			}
			ifkey("-o") {
				freopen(argv[argn + 1], "w", stdout);
				if (stdout == NULL)
					handle_error ("fail freopen '-o': ");

			}
		}

		++argn;
	}

#undef ifkey
	vec_t v;
	
	if (vec_init(&v, 10) != 0)
	{
		handle_error ("fail vec_init: ");
	}
	
	// check mode
	if(argc == 1) {
		printf("source: ");
		for(size_t i = 0; i < 10; ++i) {
			int val = rand();
			vec_push(&v, val);
			printf("%i ", val);
		}
		printf("\n");
	}
	else {
	// read data
		for(;;) {
			int val;
			size_t r = scanf("%i", &val);
			if(feof(stdin))
				break;

			if (r != 1)
			{
				errno = EINVAL;
				handle_error ("fail reading: ");
			}
			if (vec_push(&v, val) != 0)
			{
				handle_error ("fail vec_push: ");
			}
		}	
	}

	// print sorted data
	for(size_t i = 0; i < v.size; ++i) {
		printf("%i\n", v.data[i]);
	}
	printf("\n--------------------\n");
	

	merge_sort(v.data, v.size);

	// print sorted data
	for(size_t i = 0; i < v.size; ++i) {
		printf("%i\n", v.data[i]);
	}
	
	// clean everything
	vec_free(&v);
	fclose(stdin);
	fclose(stdout);

}

