#include "Interface.h"
#include <stdlib.h>

// 옵션에서(톱니바퀴)	-> libraries -> liba.a & libm.a 추가
// 옵션에서(톱니바퀴)	-> General -> Frequency 14745600 설정
// Interface.c 		 	-> Camera Baud Rate Setting : (USART1 Baud Rate: 115200)(통신속도)(UBRR1L=0x5F -> UBRR1L=0x07)
// Interface.c 			-> ISR Camera_V1 Setting
// Interface.c			-> lcd_clear_screen()을 cls()로 display_char()을 dc()로
// Interface.c 			-> Interface_init()에 putchar1(18); 추가 (카메라 V1 프로토콜 사용)
// Motor.c 			 	-> SetGain 10, 3, 1 Setting
// Sensor.c				-> Sensor_init()에 DDRD  &= ~0x50; 추가
// Sensor.h				-> READ_SENSOR()를 S로 변경
// Sensor.h				-> S에 | (~(PIND<<1)&0x20) | (~PIND&0x40) 추가

void LS() {
	if(S&8) return;
	if(S&0x24) Hm(-90, 100, 10, 0,0, 3, 8, 0, 0, 0);
	else if(S&0x50) Hm(90, 100, 10, 0, 0, 3, 8, 0, 0, 0);
	else {
		HM(90, 500, 11, 70, 0, 0);
		Hm(-90, 100, 10, 0, 0, 3, 8, 0, 0, 0);
	}
	StopMotion(9);
}
void Motion(int i) {
	dc(0, 0, i);
	LS();
	ping(1);
	Line(0, 400, 10, 0, 0, 150, 0, 0);
	LM(1, 400, 100, 01, 500, 0, i * 90, 0);
	LS();
}

#define X 8
#define Y 4
#define N 32
#define MAX 560

char line[12] = {
	1,1,1,
	1,1,0,
	1,1,1,
	1,0,1,
};
char fIP[N] = {
	0,3,0,0,
	0,0,0,0,
	2,0,0,2,
	0,1,0,2,
	0,0,3,0,
	0,0,0,0,
	0,3,1,1,
	3,3,2,1,
};
char sIP[N] = {0,};
/*
0, 1, 2, 3,
4, 5, 6, 7,
8, 9, 10, 11
*/
char paper[2][2] = {{3, 9},{1, 6}};
char papercount = 0;

int front, rear, parent[MAX], step_cnt, Path_cnt;
char queue[MAX][2], temp_IP[N], stack[60][2], pose[2];
int gps[2];
char move_dir[N], move_cnt;

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
	for (int i = 0; i < 19; i++) if (a[i] != b[i]) return 0;
	return 1;
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
	return 0;
}

char dv(char i) {  //대각선 이동이 가능한 위치인지 확인
	if (i == 0 || i == 2 || i == 5 || i == 7 || i == 8 || i == 10 || i == 13 || i == 15 || i == 16 || i == 18) return 1;
	else return 0;
}

void makeSIP() {
	for (char i = 0; i < 2; i++) {  // 3, 9
		char x = paper[i][1] / Y;
		char y = paper[i][1] % Y;
		if (x > 0) sIP[Find_Node(x - 1, y)] = compare(sIP[Find_Node(x - 1, y)], paper[i][0]);
		if (x < X - 1) sIP[Find_Node(x + 1, y)] = compare(sIP[Find_Node(x + 1, y)], paper[i][0]);
		if (y > 0) sIP[Find_Node(x, y - 1)] = compare(sIP[Find_Node(x, y - 1)], paper[i][0]);
		if (y < Y - 1) sIP[Find_Node(x, y + 1)] = compare(sIP[Find_Node(x, y + 1)], paper[i][0]);
		if (dv(paper[i][1])) {
			// 대각선 확인
			//왼쪽 위
			if (x > 0 && y > 0 && line[i - 4 - x]) sIP[Find_Node(x - 1, y - 1)] = compare(sIP[Find_Node(x - 1, y - 1)], paper[i][0]);
			// 오른쪽 위
			if (x > 0 && y < Y - 1 && line[i - 3 - x]) sIP[Find_Node(x - 1, y + 1)] = compare(sIP[Find_Node(x - 1, y + 1)], paper[i][0]);
			// 왼쪽 아래
			if (x < X - 1 && y > 0 && line[i - 1 - x]) sIP[Find_Node(x + 1, y - 1)] = compare(sIP[Find_Node(x + 1, y - 1)], paper[i][0]);
			// 오른쪽 아래
			if (x < X - 1 && y < Y - 1 && line[i - x]) sIP[Find_Node(x + 1, y + 1)] = compare(sIP[Find_Node(x + 1, y + 1)], paper[i][0]);
		}
	}
	for (char i = 0; i < 20; i++) if (paper[0][1] == i || paper[1][1] == i) fIP[i] = 0;
}

void scan(int lo, int d, int dir, int w){
    if(d){
        turn(dir, 200, 11, w, 0, 0);    
        LS();
        V1(1, 2, 0, 0, 135, 180, 90, 155, 0, 0, 0);    
    }else{
        turn(dir, 200, 11, w, 0, 0);    
        V1(1, 3, 0, 0, 90, 180, 90, 155, 0, 0, 0);    
    }
    
    if(C_D[0][4] > 45){
		paper[papercount][0] = C_D[0][0];
		paper[papercount++][1] = lo;
	}
	else fIP[lo] = C_D[0][0];
}
void Bscan() {	
	HM(0, 200, 11, 50, 0, 0);
	FCC(1, 2, 0, 0);
	TD(0, 650, 11, 1000, 0, -90, 0);
	LS();
	Line(0, 350, 11, 0, 0, 340, 0, 0);

	LS();
	V1(1, 0, 0, 0, 50, 120, 100, 130, 0, 0, 0);
	fIP[29] = C_D[0][0];
	V1(1, 0, 0, 0, 120, 180, 100, 130, 0, 0, 0);
	fIP[25] = C_D[0][0];
	if(fIP[25] != 0 && fIP[29] == 0) {
		LS();
		Line(0, 500, 11, 0, 0, 700, 0, 0);
		Line(180, 500, 11, 0, 0, 700, 0, 0);
		fIP[29] = fIP[25];
		fIP[25] = 0;
	}
    Hm(90,300,10,0,0,3,0x40,0,150,0);
	LS();
	
	V1(1, 0, 0, 0, 50, 120, 100, 130, 0, 0, 0);
	fIP[28] = C_D[0][0];
	V1(1, 0, 0, 0, 120, 180, 100, 130, 0, 0, 0);
	fIP[24] = C_D[0][0];

	if(fIP[24] != 0 && fIP[28] == 0) {
		LS();
		Line(0, 500, 11, 0, 0, 700, 0, 0);
		Line(180, 500, 11, 0, 0, 700, 0, 0);
		fIP[28] = fIP[24];
		fIP[24] = 0;
	}	
	Hm(-90,300,10,0,0,3,0x20,0,600,0);
	LS();
	
	V1(1, 0, 0, 0, 50, 120, 100, 130, 0, 0, 0);
	fIP[30] = C_D[0][0];
	V1(1, 0, 0, 0, 120, 180, 100, 130, 0, 0, 0);
	fIP[26] = C_D[0][0];

	if(fIP[26] != 0 && fIP[30] == 0) {
		LS();
		Line(0, 500, 11, 0, 0, 700, 0, 0);
		Line(180, 500, 11, 0, 0, 700, 0, 0);
		fIP[30] = fIP[26];
		fIP[26] = 0;
	}
	Hm(-90,300,10,0,0,3,0x20,0,150,0);
	LS();
	
	V1(1, 0, 0, 0, 50, 120, 100, 130, 0, 0, 0);
	fIP[31] = C_D[0][0];
	V1(1, 0, 0, 0, 120, 180, 100, 130, 0, 0, 0);
	fIP[27] = C_D[0][0];

	if(fIP[27] != 0 && fIP[31] == 0) {
		LS();
		Line(0, 500, 11, 0, 0, 700, 0, 0);
		Line(180, 500, 11, 0, 0, 700, 0, 0);
		fIP[31] = fIP[27];
		fIP[27] = 0;
	}

	//스토리지 확인 끝
	turn(1, 200, 11, 90, 0, 0);
	LS();
	Line(0, 500, 10, 0, 0, 400, 0, 0);
	TD(0, 500, 01, 500, 0, 90, 0);
	
}	

void Ascan() {
	LS();


	Line(0, 500, 10, 4, 0x7C, 0, 0, 0);
    Line(0, 500, 01, 0, 0, 150, 0, 0);
	LS();
	// A 맵 스캔 시작
    scan(16, 1, -1, 90);
    scan(12, 0, 1, 45);
    scan(13, 1, 1, 45);
    scan(14, 0, 1, 45);
    scan(18, 1, 1, 45);
	
	turn(-1, 150, 11, 90, 0, 0);
	LS();

	if(fIP[13] > 0 && fIP[13] < 10) {
		//퍽이 있음
		Line(0, 450, 11, 0, 0, 500, 0, 0);
		turn(1, 70, 11, 180, 0, 0);
		Line(0, 300, 11, 0, 0, 250, 0, 0);
		Line(180, 300, 11, 0, 0, 250, 0, 0);
		turn(1, 100, 11, 180, 0, 0);
		fIP[17] = fIP[13];
		fIP[13] = 0;
	}
	else Line(0, 450, 11, 0, 0, 500, 0, 0);
	LS();
	
	// 두번째 확인 포인트 13번
    scan(8, 0, -1, 45);
    if(S & 0x7C) {line[6] = 1; LS();}
    scan(9, 1, 1, 45);
    scan(10, 0, 1, 45);
    if(S & 0x7C) {line[7] = 1; LS();}
    
    turn(1, 150, 11, 90, 0, 0);
    if(S & 0x7C) {line[10] = 1; LS();}
    
    turn(1, 150, 11, 90, 0, 0);
    if(S & 0x7C) {line[9] = 1; LS();}
    
    turn(-1, 200, 11, 135, 0, 0);
    LS();

	//이동 시작
	if(fIP[14] > 0 && fIP[14] < 10) {
		//퍽이 있음
		Line(0, 450, 11, 0, 0, 500, 0, 0);
		turn(1, 50, 11, 180, 0, 0);
		Line(0, 300, 11, 0, 0, 250, 0, 0);
		Line(180, 300, 11, 0, 0, 250, 0, 0);
		turn(1, 100, 11, 180, 0, 0);
		fIP[13] = fIP[14];
		fIP[14] = 0;
	}
	else Line(0, 450, 11, 0, 0, 500, 0, 0);
	LS();

	
	//세번째 확인 포인트 14번
	scan(11, 0, -1, 45);
	scan(15, 1, 1, 45);
	scan(19, 0, 1, 45);
	
	bar[0] = 0;
	HM(0, 300, 11, 270, 0, 0);
	HM(180, 300, 11, 270, 0, 0);

	if(!bar[0]) line[11] = 1;

	//이동 시작	
	turn(-1, 200, 11, 135, 0, 0);
	LS();
	if(fIP[10] > 0 && fIP[10] < 10) {
		//퍽이 있음
		Line(0, 450, 11, 0, 0, 500, 0, 0);
		turn(1, 50, 11, 180, 0, 0);
		Line(0, 300, 11, 0, 0, 250, 0, 0);
		Line(180, 300, 11, 0, 0, 250, 0, 0);
		turn(1, 100, 11, 180, 0, 0);
		fIP[14] = fIP[10];
		fIP[10] = 0;
	}
	else Line(0, 450, 11, 0, 0, 500, 0, 0);
	LS();

	//네번째 확인 포인트 10번
	scan(5, 0, -1, 45);
	if(S & 0x7C) {line[4] = 1; LS();}
	
	scan(6, 1, 1, 45);
	scan(7, 0, 1, 45);
	if(S & 0x7C) {line[5] = 1; LS();}

	turn(1, 150, 11, 90, 0, 0);
	if(S & 0x7C) {line[8] = 1; LS();}

	//이동 시작
	turn(-1, 150, 11, 135, 0, 0);
	LS();
	if(fIP[6] > 0 && fIP[6] < 10) {
		//퍽이 있음
		Line(0, 450, 11, 0, 0, 500, 0, 0);
		turn(1, 50, 11, 180, 0, 0);
		Line(0, 300, 11, 0, 0, 250, 0, 0);
		Line(180, 300, 11, 0, 0, 250, 0, 0);
		turn(1, 100, 11, 180, 0, 0);
		fIP[10] = fIP[6];
		fIP[6] = 0;
	}
	else Line(0, 450, 11, 0, 0, 500, 0, 0);
	LS();

	// 6번 자리
	scan(1, 0, -1, 45);
	scan(2, 1, 1, 45);
	scan(3, 0, 1, 45);

	bar[0] = 0;
	HM(0, 300, 11, 270, 0, 0);
	HM(180, 300, 11, 270, 0, 0);
	if(!bar[0]) line[2] = 1;

	//이동시작
	turn(-1, 150, 11, 135, 0, 0);
	LS();
	if(fIP[5] > 0 && fIP[5] < 10) {
		//퍽이 있음
		Line(0, 450, 11, 0, 0, 500, 0, 0);
		turn(1, 50, 11, 180, 0, 0);
		Line(0, 300, 11, 0, 0, 250, 0, 0);
		Line(180, 300, 11, 0, 0, 250, 0, 0);
		turn(1, 100, 11, 180, 0, 0);
		fIP[6] = fIP[5];
		fIP[5] = 0;
	}
	else Line(0, 450, 11, 0, 0, 500, 0, 0);
	LS();
	
	//5번 자리
	scan(8, 0, -1, 45);
	if(S & 0x7C) {line[3] = 1; LS();}
	
	scan(4, 1, 1, 45);
	scan(0, 0, 1, 45);
	if(S & 0x7C) {line[0] = 1; LS();}
	
	turn(1, 150, 11, 90, 0, 0);
	if(S & 0x7C) {line[1] = 1; LS();}

	turn(1, 100, 11, 45, 0, 0);
	LS();

	//최종 도착 지점 : 5
	// 최종 방향 : 2(오른쪽)
	gps[0] = 5;
	gps[1] = 2;
}
void Go(int node) {
	if (paper[0][1] == node || paper[1][1] == node) {
		Line(0, 500, 11, 0, 0, 500, 0, 0);
	}
	else if(gps[0] > 19) {
		Line(0, 500, 11, 0, 0, 	425, 0, 0);
	}
	else {
		Line(0, 500, 10, 3, 0x60, 0, 100, 0);
		Line(0, 500, 01, 0, 0, 150, 0, 0);
	}
}
char get_node(int from, int dir) {//8,4
	if (dir == 0) return from - 4;
	if (dir == 1) return from - 3;
	if (dir == 2) return from + 1;
	if (dir == 3) return from + 5;
	if (dir == 4) return from + 4;
	if (dir == 5) return from + 3;
	if (dir == 6) return from - 1;
	if (dir == 7) return from - 5;

	return 99;
}
char findway(char fIP[], char from, char to) {
	move_cnt = 0;
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
			char x = pose[0];   // 현재 위치
			char y = pose[1];  //현재 위치
			char i = Find_Node(x, y);
			Pose_init(D);
			if ((x == 4 && y != 1 && D == 4) || (x == 5 && y != 1 && D == 0)) {}
			else {
				j = Find_Node(pose[0], pose[1]);
				if (dv(i) && D % 2 == 1 && i < 20 && !((x == 0 && (D == 7 || D == 1) || (x == 4 && (D == 3 || D == 5) || (y == 0 && (D == 7 || D == 5)) || (y == 4 && (D == 1 || D == 3)))))) {  // 대각선 이동이 가능하면서 대각선 차례라면
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
	}
	char path[N], cnt = 0;
	for (int i = front - 1; i > 0; i = parent[i]) path[cnt++] = queue[i][1];
	for (int i = 0; i < cnt / 2; i++) swap(&path[i], &path[cnt - i - 1]);
	for (int i = 0; i < cnt; i++) move_dir[move_cnt++] = path[i];
	return 1;
}
void set_dir(int s_dir){
    int t_dir;
    int move = gps[1] - s_dir;
    char flg=0;
    if(move < 0) {t_dir = 1; dc(0, 10, 99);}
    else t_dir = -1;

	if (abs(move) > 4) {
		t_dir *= -1;
		move = 8 - abs(move);
	}
	if (abs(move) == 4) {
		if (gps[0] < 4) {
			if (gps[1] == 2) t_dir = 1;
			else t_dir = -1;
		}
		else if (gps[0] % 4 == 0) {
			if (gps[1] == 0) t_dir = 1;
			else t_dir = -1;
		}
		else if (gps[0] % 4 == 3) {
			if (gps[1] == 0) t_dir = -1;
			else t_dir = 1;
		}
	}
	turn(t_dir, 200, 11, abs(move) * 45, 0, 0);

	LS();
	gps[1] = s_dir;
}
void move(char had) {
	char end = 0;
	for (char i = 0; i < move_cnt; i++) {
		if(end) break;
		set_dir(move_dir[i]);
		int wall = isWall(get_node(gps[0], gps[1]), gps[1]);
		if (had && i == move_cnt -1) {
			if(get_node(gps[0], move_dir[i]) > 19) {
				Line(0, 300, 11, 0, 0, 220, 0, 0);
				Line(180, 300, 11, 0, 0, 200, 0, 0);
			}
			else {
				if(move_dir[i] % 2 == 0) {
					Line(0, 300, 11, 0, 0, 250, 0, 0);
					Line(180, 300, 11, 0, 0, 250, 0, 0);
				}
				else {
					Line(0, 300, 11, 0, 0, 500, 0, 0);
					Line(180, 300, 11, 0, 0, 500, 0, 0);
				}
			}
			Getting = 0;
			end++;
			break;
		}
		else if (wall) {
			if (abs(wall) == 1) {
				Line(0, 500, 10, 0, 0, 400, 0, 0);
				LM(4, 400, 100, 01, 300, 0, 45 * wall, 0);
			}
			if (abs(wall) == 2) {
				Line(0, 500, 11, 0, 0, 200, 0, 0);
				TD(0, 300, 11, 300, 0, 45 * wall, 0);
			}
			if (wall == 3) {
				Line(0, 500, 11, 0, 0, 500, 0, 0);
				turn(1, 60, 11, 45, 0, 0);
				HM(0, 300, 11, 0, -130, 0);
				turn(1, 60, 11, 92, 0, 0);
				Hm(-90, 200, 10, 0, 0, 3, 8, 0, 50, 0);
			}
		}
		else if (gps[1] % 2 == 1) Line(0, 500, 11, 0, 0, 700, 0, 0);
		else if((gps[0] == 17 && gps[1] == 4) || (gps[0] == 21 && gps[1] == 0)) Line(0, 500, 11, 0, 0, 700, 0, 0);
		else Go(get_node(gps[0], gps[1]));gps[0] = get_node(gps[0], gps[1]);
		gps[1] = gps[1] + wall;
		if (gps[1] > 7) gps[1] = gps[1] - 8;
		if (gps[1] < 0) gps[1] = gps[1] + 8;
		
	}
}

int isWall(int node, int w_dir) {
	if (node == 0) {
		if (w_dir == 0) return 2;
		if (w_dir == 6) return -2;
		if (w_dir == 7) return 3;
	}
	if (node == 1 && w_dir == 0) return 2;
	if (node == 2) {
		if (w_dir == 0) return 2;
		if (w_dir == 1) return 1;
		if (w_dir == 7) return -1;
	}
	if (node == 3) {
		if (w_dir == 0) return -2;
		if (w_dir == 2) return 2;
	}
	if (node == 4 && w_dir == 6) return -2;
	if (node == 7) {
		if (w_dir == 1) return -1;
		if (w_dir == 2) return 2;
		if (w_dir == 3) return 1;
	}
	if (node == 8) {
		if (w_dir == 5) return -1;
		if (w_dir == 6) return 2;
		if (w_dir == 7) return 1;
	}
	if (node == 11 && w_dir == 2) return 2;
	if (node == 12 && w_dir == 6) return 2;
	if (node == 15) {
		if (w_dir == 1) return -1;
		if (w_dir == 2) return 2;
		if (w_dir == 3) return 1;
	}
	if (node == 16) {
		if (w_dir == 4) return -2;
		if (w_dir == 5) return 3;
		if (w_dir == 6) return 2;
	}
	if (node == 19) {
		if (w_dir == 2) return -2;
		if (w_dir == 4) return 2;
	}
	if(node == 20) {
		if(w_dir == 6) return -2;
		if(w_dir == 0) return 2;
	}
	if(node == 23) {
		if(w_dir == 0) return -2;
		if(w_dir == 2) return 2;
	}
	if(node == 24 && w_dir == 6) return 2;
	if(node == 27 && w_dir == 2) return -2;
	if(node == 28) {
		if(w_dir == 4) return -2;
		if(w_dir == 6) return 2;
	}
	if(node == 31) {
		if(w_dir == 2) return -2;
		if(w_dir == 4) return 2;
	}
	if((node == 29 || node == 30) && w_dir == 4) return 2;
	return 0;
}
void solve(char fIP[], char from, char to) {
	char A[N];
	copy(A, fIP);
	A[from] = 0;
	findway(A, gps[0], from);

	if(!move_cnt && !Getting) {
		//퍽으로 이동이 안된다면 다른 칸으로 띄어넘기
		//이동 가능한 위치로 이동한다.
		cls();
		dc(3, 10, 99);
		char x = gps[0] / Y;
		char y = gps[0] % Y;
		char moved = 0;
		char dir = -1; //맵지 기준 오른쪽으로 로봇 몸체 이동
		char enddir = 1;
		// 대부분의 경우 왼쪽으로 이동
		set_dir(4);  //방향을 아래로 설정
		for(int i = 0;i<5;i++) {
			if (y-1 >= 0) findway(fIP, gps[0], Find_Node(x+i, y-1));
			else if (y+1 <= Y-1) {findway(fIP, gps[0], Find_Node(x+i, y+1)); enddir=-1;}
			else {
				findway(fIP, gps[0], Find_Node(x+i, y));
				if(y == 3) dir = 1;
			}
			if(move_cnt) {
				HM(90 * dir, 200, 11, 240, 0, 0);
				HM(0, 300, 11, 470 * i, 0, 0);
				HM(90 * enddir, 200, 11, 240, 0, 0);
			}
		}
	}
	//이동 가능 하다면
	//해당 퍽이 있는 곳으로 이동
	else move(0);
	Getting = 1;
	//퍽을 가지고 있는 상태

	move_cnt = 0;
	findway(fIP, from, to);
	
	dc(0, 0, move_dir[0]);
	dc(1, 0, move_dir[1]);
	dc(2, 0, move_dir[2]);

	move(1);
	fIP[to] = fIP[from];
	fIP[from] = 0;
}
void M1() { 
	// chungbuk code
	/*
	Bscan(); 
	dc(0, 0, fIP[31]);
	dc(0, 4, fIP[30]);
	dc(0, 8, fIP[29]);
	dc(0, 12, fIP[28]);
	dc(1, 0, fIP[27]);
	dc(1, 4, fIP[26]);
	dc(1, 8, fIP[25]);
	dc(1, 12, fIP[24]);
	Ascan();
	*/
	makeSIP(); // 맵 완성 완료


	gps[0] = 5;
	gps[1] = 2;
	// 테스트 코드 
	
	char color[3] = { 0,0,0 };
	char fulcolor[3] = { 0,0,0 };
	char puckcount = 0;
	char fulpuck = 0;
	for (char i = 0; i < 20; i++) {
		if (fIP[i] != 0) { color[fIP[i] - 1]++; puckcount++; }
		if (sIP[i] != 0) {fulcolor[sIP[i] - 1]++; fulpuck++;}
	}
	
	cls();
	dc(0, 0, fIP[0]);
	dc(0, 4, fIP[4]);
	dc(0, 8, fIP[8]);
	dc(0, 12, fIP[12]);
	dc(0, 16, fIP[16]);
	
	dc(1, 0, fIP[1]);
	dc(1, 4, fIP[5]);
	dc(1, 8, fIP[9]);
	dc(1, 12, fIP[13]);
	dc(1, 16, fIP[17]);
	
	dc(2, 0, fIP[2]);
	dc(2, 4, fIP[6]);
	dc(2, 8, fIP[10]);
	dc(2, 12, fIP[14]);
	dc(2, 16, fIP[18]);
	
	dc(3, 0, fIP[3]);
	dc(3, 4, fIP[7]);
	dc(3, 8, fIP[11]);
	dc(3, 12, fIP[15]);
	dc(3, 16, fIP[19]);
	/*
	_delay_ms(3000);
	cls();

	dc(0, 0, color[0]);
	dc(0, 4, color[1]);
	dc(0, 8, color[2]);
	dc(1, 0, fulcolor[0]);
	dc(1, 4, fulcolor[1]);
	dc(1, 8, fulcolor[2]);
	dc(2, 0, puckcount);
	dc(3, 0, fulpuck);
	
	_delay_ms(3000);	
	cls();
	*/
	char stop = 0;
	while (puckcount > 5) {  //맵 안의 퍽이 5개 이상이라면 퍽 빼기
		if(fIP[31] && fIP[30] && fIP[29] && fIP[28] && fIP[27] && fIP[26] && fIP[25] && fIP[24]) break;
		for (int i = 19; i >= 0; i--) {
			if (!stop && fIP[i] != 0 && color[fIP[i] - 1] > fulcolor[fIP[i]-1]) {  //픽한 퍽이 필요한 퍽보다 많을때
				for (int j = 31; j >= 24; j--) {
					if (fIP[j] == 0) {
						findway(fIP, i, j);
						if(move_cnt) { 
							solve(fIP, i, j); 
							puckcount--; 
							color[fIP[j]-1]--; 
							stop=1; 
							break;
						}
					}
				}
			}
		}
		for(int i = 19; i >= 0; i--) {  //많은 색의 퍽을 가져올 수 없다면 걍 가까운 퍽 가져옴
			if (fIP[i] && !stop) {
				for (int j = 31; j >= 24; j--) {
					if (fIP[j] == 0) {
						findway(fIP, i, j);
						if (move_cnt == 0 && i == 19) { 
							solve(fIP, 18, j); 
							puckcount--; 
							color[fIP[18]-1]--;
							stop=1;break;
						}
						else if(move_cnt) { 
							solve(fIP, i, j); 
							puckcount--; 
							color[fIP[i]-1]--;
							stop=1;
							break;
						}
					}
				}
			}
		}
		stop=0;
	}

	//맵 안에 퍽 5개만 남겨놓은 상태
	for(int i = 0;i<20;i++) {
		if(fIP[i] != sIP[i]) {
			//현재 있는 퍽이 답지의 퍽과 다를때
			//퍽이 있어야 하는데 퍽이 없다.
			//퍽이 없어야 하는데 있다.
			if(sIP[i] == 0 && color[fIP[i]-1] > fulcolor[fIP[i]-1]){
				// 픽한 퍽의 색이 필요 이상이고 스토리지에 공간이 남으면
				for(int j = 31; j>=24;j--) {
					if(!fIP[j]) {
						findway(fIP, i, j);
						if(move_cnt) solve(fIP, i, j);   // 바로 이동 가능하면 운반 시작
						else {
							//뭔가가  막힌다면

						}
					}
				}
			}
			else {
				if(fIP[i] != 0) {
					//도착점에 다른 퍽이 있다면 치운다
					
				}
				if(fIP[i] == 0 && sIP[i] != 0) {
					//현재는 퍽이 없는데 퍽이 있어야 할때
					for(int k = 0;k<N;k++) {
						if(fIP[k] == sIP[i] && fIP[k] != sIP[k]) {
							findway(fIP, k, i);
							if(move_cnt) solve(fIP, k, i);
						}
					}
				}
			}
		}
	}
}
void M2()
{
	
}
void M3()
{
	
}
void M4()
{
	
}
void M5()
{
	
}
void M6()
{

}
void M7()
{

}
void M8()
{
	
}
void M9()
{

}

int main(void){
	unsigned char M_cnt=0;
    Interface_init();

	while(1)
	{
		if(SW1)
		{
			M_cnt++;
			PORTB=1;
			while(SW1);
			PORTB=0;
		}
		if(SW2)
		{
			M_cnt--;
			PORTB=2;
			while(SW2);
			PORTB=0;
		}
		if(SW3)
		{
			cls();
			WriteCommand(0,DFH);
			WriteCommand(1,DFH);
			WriteCommand(2,DFH);
			ping(0);

			if(!M_cnt)
			{
				M1(); M2(); M3(); M4(); M5(); M6(); M7(); M8(); M9();
				StopMotion(9);
				
				PORTB|=0x0F;
				_delay_ms(2000);
				PORTB&=~0x0F;
			}
			else if(M_cnt==1) M1();
			else if(M_cnt==2) M2();
			else if(M_cnt==3) M3();
			else if(M_cnt==4) M4();
			else if(M_cnt==5) M5();
			else if(M_cnt==6) M6();
			else if(M_cnt==7) M7();
			else if(M_cnt==8) M8();
			else if(M_cnt==9) M9();
			StopMotion(9);
		}
		if(SW4)
		{
			cls();
			WriteCommand(0,DFH);
			WriteCommand(1,DFH);
			WriteCommand(2,DFH);
			ping(0);

			Buzz(3);
			PORTB=1;
			while(SW4)
			{
				if(SW1) cls(), PORTB=1;
				if(SW2) cls(), PORTB=2;
				if(SW3) cls(), PORTB=4;

				if(PORTB&1)
				{	
					ping(0);
					sprintf(str,"    X = (%5d)",(int)px[0]); lcd_display_str(1,0,str);
					sprintf(str,"    Y = (%5d)",(int)py[0]); lcd_display_str(2,0,str);
					sprintf(str,"    W = (%5d)",(int)pw[0]); lcd_display_str(3,0,str);
				}
				if(PORTB&2)
				{
					dc(0,9,psd_value[0]);
					dc(0,4,psd_value[1]);dc(0,14,psd_value[8]);
					dc(1,0,psd_value[2]);dc(1,17,psd_value[7]);
					dc(2,0,psd_value[3]);dc(2,17,psd_value[6]);
					dc(3,4,psd_value[4]);dc(3,14,psd_value[5]);
				}
				if(PORTB&4)
				{
					V1(11,0,0,0,0,180,0,240,0,0,0);
					
					for(int i=0;i<5;i++){
						dc(0,i*4,C_D[i][0]);
						dc(1,i*4,C_D[i][1]);
						dc(2,i*4,C_D[i][2]);
					}

					dc(3,17,C_N);
				}
			}
			PORTB=0;
			cls();
		}
		dc(3,17,M_cnt);
	}
}
