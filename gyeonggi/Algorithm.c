#include<stdio.h>
#include<stdlib.h> // rand() 함수 포함 라이브러리
#include<time.h>

char Map[32] = {0,};
char Completion[16];
char queue[520];

void preprocess() {
	char location;
	srand(time(NULL));
	
	for (int k = 0; k < 3; k++) {
		for (int i = 0; i < 3; i++) {
			while (1) {
				location = rand() % 4;
				if (Map[location + 4 + (k * 8)] == 0) {
					Map[location + 4 + (k * 8)] = (rand() % 3) + 1;
					break;
				}
			}
		}
	}
	for (int i = 0; i < 2; i++) {
		while (1) {
			location = rand() % 4;
			if (Map[location + 28] == 0) {
				Map[location + 28] = (rand() % 3) + 1;
				break;
			}
		}
	}

	Completion[0] = Map[7];
	Completion[1] = Map[6];
	Completion[2] = Map[5];
	Completion[3] = Map[4];
	Completion[4] = Map[15];
	Completion[5] = Map[14];
	Completion[6] = Map[13];
	Completion[7] = Map[12];
	Completion[8] = Map[23];
	Completion[9] = Map[22];
	Completion[10] = Map[21];
	Completion[11] = Map[20];
	Completion[12] = Map[31];
	Completion[13] = Map[30];
	Completion[14] = Map[29];
	Completion[15] = Map[28];

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			printf("%d ", Map[j + (i * 8)]);
		}
		printf("\n");
	}
	printf("=========================================\n");
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%d ", Completion[j + (4 * i)]);
		}
		printf("\n");
	}
}

int main(void) {
	preprocess();
	


	return 0;
}
