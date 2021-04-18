#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

void print_prices(int* prices, int* lens, int* way, int len)
{
	int cur_len = len;
	while (cur_len > 0)
	{
		int x = way[cur_len];
		printf("%d ", x);

		cur_len -= x;
	}
	fputc('\n', stdout);
}

int* cutrod(int* prices, int len, int* way)
{
	int* dp = (int*)calloc(len + 1, sizeof(*dp));

	for (int j = 1; j <= len; j ++)
	{
		dp[j] = INT_MIN;
		for (int i = 1; i <= j; i ++)
		{
			if (dp[j - i] + prices[i] > dp[j])
			{
				dp[j] = dp[j - i] + prices[i];
				if (way)
					way[j] = i;
			}
		}
	}
	return dp;
}

int main() {
	FILE* ifile = fopen("input.txt", "r");

	int L;
	fscanf(ifile, "%i", &L);
	
	int n;
	fscanf(ifile, "%i", &n);
	
	int* l = (int*)malloc(sizeof(int) * n);
	int* c = (int*)malloc(sizeof(int) * n);

	for(int i = 0; i < n; ++i)
		fscanf(ifile, "%i", &l[i]);

	for(int i = 0; i < n; ++i)
		fscanf(ifile, "%i", &c[i]);


	int* pmine = (int*)calloc(L + 1, sizeof(*pmine));
	for (int i = 0; i < n; i++)
	{
		int cur = c[i];
		if (pmine[l[i]] < cur)
			pmine[l[i]] = cur;
	}
	int* way = (int*)calloc(L + 1, sizeof(*way));
	int* prices = cutrod(pmine, L, way);
	printf("total cost of the cutting out is %i\n", prices[L]);
	printf("sections: ");
	print_prices(prices, l, way, L);

	free(l);
	free(c);
	free(prices);
	free(way);

	return 0;
}

