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
/*
char fIP[N] = {
	0,3,0,0,
	0,0,11,0,
	2,33,0,2,
	0,0,1,2,
	0,0,3,0,
};
*/
char fIP[N] = {
	0,3,0,0,
	0,0,11,0,
	2,33,0,2,
	0,0,1,2,
	0,0,3,0,
};

char sIP[N] = {0,};

char gps[2];
char move_dir[N], move_cnt;
/*
move_dir 
  0
 7 1
6   2
 5 3
  4
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
/*
move_dir
  0
 7 1
6   2
 5 3
  4
*/
void Pose_init(char d) {
	if (d == 0) pose[0]--;
	if (d == 2) pose[1]++;
	if (d == 4) pose[0]++;
	if (d == 6) pose[1]--; 
	if (d == 1) { pose[0]--; pose[1]++; }
	if (d == 3) { pose[0]++; pose[1]++; }
	if (d == 5) { pose[0]++; pose[1]--; }
	if (d == 7) { pose[0]--; pose[1]--; }
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

char dv(char i) {  //대각선 이동이 가능한 위치인지 확인
	if (i == 0 || i == 2 || i == 5 || i == 7 || i == 8 || i == 10 || i == 13 || i == 15 || i == 16 || i == 18) return 1;
	else return 0;
}

void makeSIP() {
	for (int i = 0; i < N; i++) {
		char color = fIP[i] / 10;
		if (color != 0) {
			char x = i / Y;
			char y = i % Y;
			if (x > 0 && fIP[Find_Node(x - 1, y)]/10 == 0) sIP[Find_Node(x - 1, y)] = compare(sIP[Find_Node(x - 1, y)], color);
			if (x < X - 1 && fIP[Find_Node(x + 1, y)]/10 == 0) sIP[Find_Node(x + 1, y)] = compare(sIP[Find_Node(x + 1, y)], color);
			if (y > 0 && fIP[Find_Node(x, y - 1)]/10 == 0) sIP[Find_Node(x, y - 1)] = compare(sIP[Find_Node(x, y - 1)], color);
			if (y < Y - 1 && fIP[Find_Node(x, y + 1)]/10 == 0) sIP[Find_Node(x, y + 1)] = compare(sIP[Find_Node(x, y + 1)], color);
			if (dv(i)) {
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
		for (int D = 0, j = 0; D < 8; D++) {
			pose[0] = Change(p[0], 0);
			pose[1] = Change(p[0], 1);
			char x = pose[0];
			char y = pose[1];
			char i = Find_Node(x, y);
			Pose_init(D);
			
			j = Find_Node(pose[0], pose[1]);
			if (dv(j) && D % 2  == 1) {  // 대각선 이동이 가능하면서 대각선 차례라면
				if ((safe(pose[0], pose[1]) && !a[j]) && ((D == 7 && line[i - 4 - x]) || (D == 1 && line[i - 3 - x]) || (D == 5 && line[i - 1 - x]) || (D == 3 && line[i - x]))) {
					a[j] = 1;
					parent[rear] = front - 1;
					point[0] = j;
					point[1] = D;
					enqueue(point);
				}
			}
			if (D % 2 == 0 && safe(pose[0], pose[1]) && !a[j]) {  // 위, 아래, 왼쪽, 오른쪽 검증 차례이라면
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
	printf("%d %d %d %d\n", sIP[0], sIP[1], sIP[2], sIP[3]);
	printf("%d %d %d %d\n", sIP[4], sIP[5], sIP[6], sIP[7]);
	printf("%d %d %d %d\n", sIP[8], sIP[9], sIP[10], sIP[11]);
	printf("%d %d %d %d\n", sIP[12], sIP[13], sIP[14], sIP[15]);
	printf("%d %d %d %d\n", sIP[16], sIP[17], sIP[18], sIP[19]);
	
	
	/*char red = 0;
	char blue = 0;
	char yellow = 0;
	char fulred = 0;
	char fulblue = 0;
	char fulyellow = 0;
	for (int i = 0; i < N; i++) {
		if (fIP[i] == 1) red++;
		else if (fIP[i] == 2) blue++;
		else if (fIP[i] == 3) yellow++;

		if (sIP[i] == 1) fulred++;
		else if (sIP[i] == 2) fulblue++;
		else if (sIP[i] == 3) fulyellow++;
	}
	//findway를 쓰고 답이 없으면 옮길 수 있는 길을 만들 퍽의 이동 경로 계산 코드? bfs?
	
	char piece[N];  // 맞춰야 하는 퍽의 위치들(저장 후 매 이동마다 탈출 가능한지 계산)
	char pieceCnt = 0;
	while (same(fIP, sIP)) {
		for (char i = 0; i < N; i++) {
			if (fIP[i] != sIP[i]) {
				// 둘다 답도, 빈칸도 아닐때
				if (sIP[i] == 0) piece[pieceCnt] = i;  // 퍽이 있지만 맞춰야 하는 자리가 아닐때
				else {
					// 퍽을 맞춰야 하는 자리일때
					char temp[N];

				}
				
				else if (fIP[i] == 0 && sIP[i] != 0) {
					// 퍽이 없지만 맞춰야 하는 자리일때
					for (char j = 0; i < pieceCnt; i++) {
						
					}
				}
				else if (fIP[i] != 0 && sIP[i] != 0) {
					// 퍽이 있고 다른 색의 퍽을 맞춰야 하는 상황일때
				}
				
			}
		}
	}
	*/
	findway(fIP, 12, 3);
	for (int i = 0; i < move_cnt; i++) {
		printf("%2d", move_dir[i]);
	}
}
