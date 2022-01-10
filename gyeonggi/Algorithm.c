#include<stdio.h>
#include<stdlib.h> // rand() 함수 포함 라이브러리
#include<time.h>

char Map[32] = {0,};
char preMap[16];
char queue[520];

void preprocess() {
	int redCount = 0;
	int blueCount = 0;
	int yellowCount = 0;

	int first = 0;
	int second = 0;
	int third = 0;
	int four = 0;

	srand(time(NULL));
	for (int i = 0; i < 11; i++) {
		while (1) {
			int location = rand() % 16;
			int color = rand() % 3;

			// 개수 제한 확인
			if (preMap[location] != 0) continue;
			else {
				if ((location < 4 && first == 2) || ((4<=location && location < 8) && second == 3) || ((8 <= location && location < 12) && third == 3) || ((12 <= location && location < 16) && four == 3)) continue;
				else {
					//색 개수 체크
					if ((color == 0 && redCount < 4) || (color == 1 && blueCount < 4) || (color == 2 && yellowCount < 4)) {
						if (location < 4) first++;
						else if (location < 8) second++;
						else if (location < 12) third++;
						else four++;
						
						if (color == 0) redCount++;
						if (color == 1) blueCount++;
						if (color == 2) yellowCount++;
						preMap[location] = color;

						for (int i = 0; i < 4; i++) {
							for (int j = 0; j < 4; j++) {
								printf("%d ", preMap[i * 4 + j]);
							}
							printf("\n");
						}
						printf("\n\n");

						break;
					}
				}
			}
		}
	}
	Map[0] = preMap[0];
	Map[1] = preMap[1];
	Map[2] = preMap[2];
	Map[3] = preMap[3];
	Map[8] = preMap[4];
	Map[9] = preMap[5];
	Map[10] = preMap[6];
	Map[11] = preMap[7];
	Map[16] = preMap[8];
	Map[17] = preMap[9];
	Map[18] = preMap[10];
	Map[19] = preMap[11];
	Map[24] = preMap[12];
	Map[25] = preMap[13];
	Map[26] = preMap[14];
	Map[27] = preMap[15];
}

int main(void) {
	preprocess();
	//for (int i = 0; i < 4; i++) {
	//	for (int j = 0; j < 8; j++) {
	//		printf("%d ", Map[i * 8 + j]);
	//	}
	//	printf("\n");
	//}
	return 0;
}
