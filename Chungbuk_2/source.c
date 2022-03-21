#include <stdio.h>

#define X 5
#define Y 4
#define N 20
#define MAX 560
#define M 99

char line[12] = {
	1,1,1,
	1,1,0,
	1,1,1,
	1,0,1,
};

char fIP[N] = {
	0,3,0,0,
	0,0,11,0,
	2,0,0,2,
	0,33,1,2,
	0,0,3,0,
};
char sIP[N] = {0,};

char gps[2];
char move_dir[N], move_cnt;
/*
move_dir 
오른쪽 : 1
아래쪽 : 2
왼쪽 : 3
위쪽 : 0
*/

int front, rear, parent[MAX], step_cnt, Path_cnt;
char queue[MAX][2], temp_IP[N], stack[60][2], pose[2];

char IP[N];
char SS[N];

void enqueue(char arr[]) {
	if (rear < MAX) {
		queue[rear][0] = arr[0];
		queue[rear][1] = arr[1];
		rear++;
	}
}
char* dequeue() {
	if (front < rear) return queue[front++];
	return NULL;
}
void push(char x, char y) {
	stack[step_cnt][0] = x;
	stack[step_cnt][1] = y;
	step_cnt++;
}
void swap(char* a, char* b) {
	char tmp = *a;
	*a = *b;
	*b = tmp;
}
void copy(char a[], char b[]) {
	for (int i = 0; i < N; i++) a[i] = b[i];
}
char Change(char value, char mode) {
	if (mode == 0) return value / Y;
	if (mode == 1) return value % Y;
	return 0;
}
void Pose_init(char d) {
	if (d == 0) pose[0]--;
	if (d == 1) pose[1]++;
	if (d == 2) pose[0]++;
	if (d == 3) pose[1]--;
}
char Find_Node(char x, char y) {
	return x * Y + y;
}
char safe(char x, char y) {
	return 0 <= x && x < X && 0 <= y && y < Y;
}
void Path_S(int i) {
	if (stack[i][1] == stack[i + 1][0]) {
		Path_cnt++;
		Path_S(i + 1);
	}
}
void Link(int a, char mode) {
	if (a > 0) Link(parent[a], mode);
	else {
		copy(temp_IP, IP);
		return;
	}
	if (mode) push(queue[a][0], queue[a][1]);
	swap(&temp_IP[queue[a][0]], &temp_IP[queue[a][1]]);
}
char same(char a[], char b[]) {
	for (int i = 0; i < N; i++) if (a[i] != b[i]) return 0;
	return 1;
}
char s_same(char a[]) {
	for (int i = 0; i < rear; i++) {
		Link(i, 0);
		if (same(temp_IP, a)) return 1;
	}
	return 0;
}

/*
도착 퍽 생성시에 고려할 점
1. 대각선 선과 연결되는 부분인가
2. 만약 0이라면 그냥 두면 되는거 아닌가? 와 시발

*/

char compare(char c1, char c2) {
	if (c1 == 0) return c2;
	else if (c2 == 0) return c1;
	else if (c1 == 1) {
		if (c2 == 2) return 3;
		else if (c2 == 3) return 2;
	}
	else if (c1 == 2) {
		if (c2 == 1) return 3;
		else if (c2 == 3) return 1;
	}
	else if (c1 == 3) {
		if (c2 == 2) return 1;
		else if (c2 == 1) return 2;
	}
}

void makeSIP() {
	for (int i = 0; i < N; i++) {
		char color = fIP[i] / 10;
		if (color != 0) {
			char x = i / 4;
			char y = i % 4;
			if (x > 0 && fIP[Find_Node(x - 1, y)]/10 == 0) sIP[Find_Node(x - 1, y)] = compare(sIP[Find_Node(x - 1, y)], color);
			if (x < X - 1 && fIP[Find_Node(x + 1, y)]/10 == 0) sIP[Find_Node(x + 1, y)] = compare(sIP[Find_Node(x + 1, y)], color);
			if (y > 0 && fIP[Find_Node(x, y - 1)]/10 == 0) sIP[Find_Node(x, y - 1)] = compare(sIP[Find_Node(x, y - 1)], color);
			if (y < Y - 1 && fIP[Find_Node(x, y + 1)]/10 == 0) sIP[Find_Node(x, y + 1)] = compare(sIP[Find_Node(x, y + 1)], color);
			if (i == 0 || i == 2 || i == 5 || i == 7 || i == 8 || i == 10 || i == 13 || i == 15 || i == 16 || i == 18) {
				// 대각선 확인
				//왼쪽 위
				if (x > 0 && y > 0 && line[i-4-x] && fIP[Find_Node(x - 1, y - 1)]/10 == 0) sIP[Find_Node(x - 1, y - 1)] = compare(sIP[Find_Node(x - 1, y - 1)], color);
				// 오른쪽 위
				if (x > 0 && y < Y - 1 && line[i-3-x] && fIP[Find_Node(x - 1, y + 1)]/10 == 0) sIP[Find_Node(x - 1, y + 1)] = compare(sIP[Find_Node(x - 1, y + 1)], color);
				// 왼쪽 아래
				if (x < X - 1 && y > 0 && line[i-1-x] && fIP[Find_Node(x + 1, y - 1)]/10==0) sIP[Find_Node(x + 1, y - 1)] = compare(sIP[Find_Node(x + 1, y - 1)], color);
				// 오른쪽 아래
				if (x < X - 1 && y < Y - 1 && line[i-x] && fIP[Find_Node(x + 1, y + 1)]/10 == 0) sIP[Find_Node(x + 1, y + 1)] = compare(sIP[Find_Node(x + 1, y + 1)], color);
			}
		}
	}
}

/*
void makeSIP() {
	for (int i = 0; i < N; i++) {
		char color = fIP[i] / 10;
		if (color != 0) {
			char x = i / 4;
			char y = i % 4;
			if (x > 0 && sIP[Find_Node(x - 1, y)] == 0) {
				sIP[Find_Node(x - 1, y)] = color;
			}
			else if (x > 0 && sIP[Find_Node(x - 1, y)] != 0) {
				sIP[Find_Node(x - 1, y)] = compare(sIP[Find_Node(x - 1, y)], color);
			}
			if (x < X - 1 && sIP[Find_Node(x + 1, y)] == 0) {
				sIP[Find_Node(x + 1, y)] = color;
			}
			else if (x < X - 1 && sIP[Find_Node(x + 1, y)] != 0) {
				sIP[Find_Node(x + 1, y)] = compare(sIP[Find_Node(x + 1, y)], color);
			}
			if (y > 0 && sIP[Find_Node(x, y - 1)] == 0) {
				sIP[Find_Node(x, y - 1)] = color;
			}
			else if (y > 0 && sIP[Find_Node(x, y - 1)] != 0) {
				sIP[Find_Node(x, y - 1)] = compare(sIP[Find_Node(x, y - 1)], color);
			}
			if (y < Y - 1 && sIP[Find_Node(x, y + 1)] == 0) {
				sIP[Find_Node(x, y + 1)] = color;
			}
			else if (y < Y - 1 && sIP[Find_Node(x, y + 1)] != 0) {
				sIP[Find_Node(x, y + 1)] = compare(sIP[Find_Node(x, y + 1)], color);
			}
			if (i == 0 || i == 2 || i == 5 || i == 7 || i == 8 || i == 10 || i == 13 || i == 15 || i == 16 || i == 18) {
				// 대각선 확인
				if (x > 0 && y > 0 && sIP[Find_Node(x - 1, y - 1)] == 0) {
					sIP[Find_Node(x - 1, y - 1)] = color;
				}
				else if (x > 0 && y > 0 && sIP[Find_Node(x - 1, y - 1)] != 0) {
					sIP[Find_Node(x - 1, y - 1)] = compare(sIP[Find_Node(x - 1, y - 1)], color);
				}
				if (x > 0 && y < Y - 1 && sIP[Find_Node(x - 1, y + 1)] == 0) {
					sIP[Find_Node(x - 1, y + 1)] = color;
				}
				else if (x > 0 && y < Y - 1 && sIP[Find_Node(x - 1, y + 1)] != 0) {
					sIP[Find_Node(x - 1, y + 1)] = compare(sIP[Find_Node(x - 1, y + 1)], color);
				}
				if (x < X - 1 && y > 0 && sIP[Find_Node(x + 1, y - 1)] == 0) {
					sIP[Find_Node(x + 1, y - 1)] = color;
				}
				else if (x < X - 1 && y > 0 && sIP[Find_Node(x + 1, y - 1)] != 0) {
					sIP[Find_Node(x + 1, y - 1)] = compare(sIP[Find_Node(x + 1, y - 1)], color);
				}
				if (x < X - 1 && y < Y - 1 && sIP[Find_Node(x + 1, y + 1)] == 0) {
					sIP[Find_Node(x + 1, y + 1)] = color;
				}
				else if (x < X - 1 && y < Y - 1 && sIP[Find_Node(x + 1, y + 1)] != 0) {
					sIP[Find_Node(x + 1, y + 1)] = compare(sIP[Find_Node(x + 1, y + 1)], color);
				}
			}
		}
	}
}
*/

char findway(char fIP[], char from, char to) {
	if (from == to) return 1;
	char a[N], * p, point[2] = { 0, };

	copy(a, fIP);
	point[0] = from;
	point[1] = 0;

	front = rear = 0;
	enqueue(point);
	while (1) {
		p = dequeue();
		if (p == NULL) return 0;
		if (p[0] == to) break;
		for (int D = 0, j = 0; D < 4; D++) {
			pose[0] = Change(p[0], 0);
			pose[1] = Change(p[0], 1);
			Pose_init(D);
			j = Find_Node(pose[0], pose[1]);
			if (safe(pose[0], pose[1]) && !a[j]) {
				a[j] = 1;
				parent[rear] = front - 1;
				point[0] = j;
				point[1] = D;
				enqueue(point);
			}
		}
	}
	char path[N], cnt = 0;
	for (int i = front - 1; i > 0; i = parent[i]) path[cnt++] = queue[i][1];
	for (int i = 0; i < cnt / 2; i++) swap(&path[i], &path[cnt - i - 1]);
	for (int i = 0; i < cnt; i++) move_dir[move_cnt++] = path[i];
	return 1;
}

int main() {
	
	makeSIP();
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			printf("%d ", sIP[Find_Node(i, j)]);
		}
		printf("\n");
	}
	//findway(fIP, 0, 3);
	//for (int i = 0; i < move_cnt; i++) {
	//	printf("%2d", move_dir[i]);
	//}
}
