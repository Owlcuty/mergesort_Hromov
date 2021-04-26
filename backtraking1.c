#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define CHB_S 8

#define CHB_Q 2 // Queen here
#define CHB_D 1 // Dangerous for another queen
#define CHB_E 0 // Empty

void chb_append_point(int16_t* points, uint16_t x, uint16_t y)
{
	for (int16_t* cur = points; cur < points + CHB_S * 2; ++cur)
	{
		if (*cur == -1)
		{
			*cur = x;
			*(cur + 1) = y;
			return;
		}
	}
}

void chb_set_point(uint16_t* chb, uint16_t x, uint16_t y, uint16_t ind)
{
	*(chb + 2 * (ind - 1)) = x;
	*(chb + 2 * (ind - 1) + 1) = y;
}

void chb_get_point(uint16_t* chb, uint16_t ind, uint16_t* x, uint16_t* y)
{
	*x = *(chb + 2 * (ind - 1));
	*y = *(chb + 2 * (ind - 1) + 1);
}

int chb_check(int x, int y) {
	return x >= 0 && x < CHB_S && y >= 0 && y < CHB_S;
}

uint16_t chb_get_yx(uint16_t* buf, uint16_t y, uint16_t x)
{
	return *(buf + CHB_S * y + x);
}

void chb_set_yx(uint16_t* buf, uint16_t y, uint16_t x, uint16_t value)
{
	*(buf + CHB_S * y + x) = value;
}

void _chb_queen_mark(uint16_t* buf, uint16_t x, uint16_t y) {
	assert(x >= 0 && x < CHB_S);
	assert(y >= 0 && y < CHB_S);

	uint16_t b1 = y - x;
	uint16_t b2 = y + x;

	for(uint16_t i = 0; i < CHB_S; ++i) {
		chb_set_yx (buf, y, i, CHB_D);
		chb_set_yx (buf, i, x, CHB_D);
	}
	for(uint16_t i = 0; i < CHB_S; ++i) {
		// y = x + b
		// b = y_0 - x_0
		uint16_t y1 = i + b1;
		uint16_t y2 = -i + b2;

		if(chb_check(i, y1))
			chb_set_yx(buf, y1, i, CHB_D);

		if(chb_check(i, y2))
			chb_set_yx(buf, y2, i, CHB_D);
	}

	chb_set_yx(buf, y, x, CHB_Q);
}

void chb_queen_mark(int16_t* points, uint16_t* buf)
{
	memset(buf, 0, CHB_S * CHB_S * sizeof(*buf));
	for (int16_t* cur = points; *cur != -1 && cur < points + CHB_S * 2; cur += 2)
	{
		uint16_t x = *cur;
		uint16_t y = *(cur + 1);
		_chb_queen_mark(buf, x, y);
	}
}

int _place_queen(int16_t* points, uint16_t* buf, uint8_t count)
{
	chb_queen_mark(points, buf);

	if (count == CHB_S) {
		for(int i = 0; i < CHB_S; ++i) {
			for(int j = 0; j < CHB_S; ++j) 
				printf("%c ", chb_get_yx(buf, i, j) == CHB_Q ? '1' : '0');
			printf("\n");
		}

		free (points);
		free (buf);
		exit(0);
	}

	for(int i = 0; i < CHB_S; ++i)
		for(int j = 0; j < CHB_S; ++j) {
			if(chb_get_yx(buf, i, j) == CHB_E) {
				chb_set_point(points, j, i, count + 1);
				if(_place_queen(points, buf, count + 1))
				{
					return 1;
				}
				chb_set_point(points, -1, -1, count + 1);
				chb_queen_mark(points, buf);
			}
		}

	return 0;
}

void place_queen(int16_t* points)
{
	uint16_t* buf = (uint16_t*)calloc(64, sizeof(*buf));

	_place_queen(points, buf, 1);
	free (buf);
}

int main() {
	int x = 4, y = 5;

	int16_t* queen_points = (int16_t*)malloc(16 * sizeof(*queen_points));
	memset (queen_points, -1, 16 * sizeof(*queen_points));
	chb_append_point(queen_points, x, y);

	place_queen(queen_points);

	free (queen_points);
}

