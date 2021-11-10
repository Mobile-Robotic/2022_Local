#include<stdio.h>

char Map[4][8] = {
	0, 0, 2, 2, 0, 0, 0, 0,
	1, 3, 3, 0, 0, 0, 0, 0,
	1, 2, 2, 0, 0, 0, 0, 0,
	1, 3, 0, 3, 0, 0, 0, 0
};

typedef struct {
	queueNode* parent;
	char data[4][4];
}queueNode;


char Sequence[16];
char gate[3] = { 3, 2, 1 };
queueNode* address[100000];
int length = 0;
queueNode* start;
int temp[4][4];


void arrangement() {
	//맞춰야 하는 순서 저장 배열
	Sequence[0] = Map[0][2];
	Sequence[1] = Map[3][2];
	Sequence[2] = Map[0][1];
	Sequence[3] = Map[3][1];
	Sequence[4] = Map[0][0];
	Sequence[5] = Map[3][0];
	Sequence[6] = Map[1][0];
	Sequence[7] = Map[2][0];
	Sequence[8] = Map[1][1];
	Sequence[9] = Map[2][1];
	Sequence[10] = Map[1][2];
	Sequence[11] = Map[2][2];
	Sequence[12] = Map[0][3];
	Sequence[13] = Map[3][3];
	Sequence[14] = Map[1][3];
	Sequence[15] = Map[2][3];
	for (int i = 0; i < 16; i++) printf("%d ", Sequence[i]);
	printf("\n");
}

void enqueue(char value[4][4]) {
	//새로운 노드를 힙에 추가
	queueNode* new;
	new = (queueNode*)malloc(sizeof(queueNode));
	for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) new->data[i][j] = value[i][j];

	new->parent = address[length-1];
	address[length] = new;
	length++;
}

void dequeue() {
	//address에 있는 모든 노드들을 free 시킨다.
	for (int i = length - 1; i > 0; i--) {
		
	}
}

void print() {
	printf("=====================\n");
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%d ", temp[i][j]);
		}
		printf("\n");
	}

	enqueue(temp);
}

void firstCase() {
	//첫번째 퍽의 이동만을 담당하는 함수
	int count = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					temp[i][j] = Map[i][j];
				}
			}

			if (Map[i][j] != 0) {
				int up = i - 1;
				int down = i + 1;
				int left = j - 1;
				int right = j + 1;

				if (up >= 0 && Map[up][j] == 0) {
					count++;
					temp[up][j] = Map[i][j];
					temp[i][j] = 0;
					print();
				}
				if (down <= 3 && Map[down][j] == 0) {
					count++;
					temp[down][j] = Map[i][j];
					temp[i][j] = 0;
					print();
				}
				if (right <= 3 && Map[i][right] == 0) {
					count++;
					temp[i][right] = Map[i][j];
					temp[i][j] = 0;
					print();
				}
				if (left >= 0 && Map[i][left] == 0) {
					count++;
					temp[i][left] = Map[i][j];
					temp[i][j] = 0;
					print();
				}

				count = 0;
			}
		}
	}
}

void first() {
	//start node 생성
	start = (queueNode*)malloc(sizeof(queueNode));
	start->parent = NULL;
	address[length] = start;

	firstCase();
}

void solution() {
	
}

void solve() {
	arrangement();
	first();
	solution();																																	
} 

void M1() {
	solve();
}

int main() {
	M1();
	return 0;
}
