#include <stdio.h>

void t1(char const* word, FILE* stream) {
	int state = 1;
	int table[6][2] = {
		{},
		{ 2, 3 },
		{ 5, 5 },
		{ 4, 5 },
		{ 5, 5 },
		{ 5, 5 }
	};

	while(*word != '\0') {
		state = table[state][*word - '0'];
		++word;
	}

	if(state == 2 || state == 4)
		fprintf(stream, "All correct!\n");
	else
		fprintf(stream, "Final state is incorrect\n");
}

void t2(char const* word, FILE* stream) {
	int state = 1;
	int table[5][2] = {
		{},
		{ 3, 2 },
		{ 0, 3 },
		{ 3, 3 },
		{ 4, 4 }
	};

	while(*word != '\0') {
		state = table[state][*word - '0'];
		++word;
	}

	if(state == 0)
		fprintf(stream, "All correct!\n");
	else
		fprintf(stream, "Final state is incorrect\n");
}

void t3(char const* word, FILE* stream) {
	int state = 0;
	
	while(*word != '\0')
		++state, ++word;

	if(state == 3)
		fprintf(stream, "All correct!\n");
	else
		fprintf(stream, "Final state is incorrect\n");
}

int main() {
	int task_id = 2;	
	char const* word = "11011111";

	void(*tasks[4])(char const*, FILE*) = { NULL, t1, t2, t3 };
	if(task_id <= 0 || task_id > 3)
		printf("Invalid task id\n");
	else
		tasks[task_id](word, stdout);
}
