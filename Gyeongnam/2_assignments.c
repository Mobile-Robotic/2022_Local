
int Map[13] = {
	1, 3, 0,
	  2, 3,
	0, 3, 1,
	  2, 1,
	0, 0, 3
};

int MidColor;  // Map[6] 컬러
int rl;        // Loop시 오른쪽, 왼쪽
int gps[2];    //로봇 위치, 로봇 방향
int vertex[2]; // 꼭짓점의 위, 아래에 퍽이 몇개 있는지 확인
int pucktex;   // 각 꼭짓점에 Map[6]이 이미 있는 지 확인
int ud; //1은 위, 2는 아래

void bring() {
	if (gps[0] == 11) {
		if (Map[8] == 0 && Map[10] != 0) {
			Map[8] = Map[10];
			Map[10] = 0;
		}
		else if (Map[9] == 0 && Map[12] != 0) {
			Map[9] = Map[12];
			Map[12] = 0;
		}
	}
	else if (gps[0] == 1) {
		if (Map[3] == 0 && Map[0] != 0) {
			Map[3] = Map[0];
			Map[0] = 0;
		}
		else if (Map[4] == 0 && Map[2] != 0) {
			Map[4] = Map[2];
			Map[2] = 0;
		}
	}
}

int prediction() {
	//왼쪽, 오른쪽 중 어느 곳이 회전률이 더 적나 계산
	int Rcount = 0;
	int Lcount = 0;
	int testMap[13];

	memmove(Map, testMap, sizeof(int) * 13);

	int temp;

	while (testMap[1] == Map[6] || testMap [11]== Map[6]) {
		temp = testMap[1];
		testMap[1] = testMap[4];
		testMap[4] = testMap[7];
		testMap[7] = testMap[9];
		testMap[9] = testMap[11];
		testMap[11] = testMap[8];
		testMap[8] = testMap[5];
		testMap[5] = testMap[3];
		testMap[3] = temp;
		Lcount += 1;
	}
	while (testMap[1] == Map[6] || testMap [11]== Map[6]) {
		temp = testMap[1];
		testMap[1] = testMap[3];
		testMap[3] = testMap[5];
		testMap[5] = testMap[8];
		testMap[8] = testMap[11];
		testMap[11] = testMap[9];
		testMap[9] = testMap[7];
		testMap[7] = testMap[4];
		testMap[4] = temp;
		Rcount += 1;
	}

	// 오른쪽으로 도는 것이 왼쪽으로 도는 것보다 크다면 왼쪽으로 돌아야함
	// 그 외는 오른쪽으로 돈다
	if (Rcount > Lcount) return -1;
	else return 1;
}

void Move(int num) {
	// Map의 num 위치로 로봇을 이동시키는 함수
	if (gps[0] == 0 || gps[0] == 2 || gps[0] == 10 || gps[0] == 12) {
		
	}
	else if (gps[0] == 1 || gps[0] == 11) {
		
	}
	else if (gps[0] == 3 || gps[0] == 4 || gps[0] == 8 || gps[0] == 9) {
		
	}
	else if (gps[0] == 5 || gps[0] == 7) {
		
	}
}

void change(int source) {
	
}

void Loop(int num) {
	// Loop 회전 시키는 함수
	// move 함수를 사용
	int rl = prediction();
	int temp;
	while (ud == 0) {
		if (rl == -1) {
			temp = Map[1];
			Map[1] = Map[4];
			Map[4] = Map[7];
			Map[7] = Map[9];
			Map[9] = Map[11];
			Map[11] = Map[8];
			Map[8] = Map[5];
			Map[5] = Map[3];
			Map[3] = temp;
		}
		else {
			temp = Map[1];
			Map[1] = Map[3];
			Map[3] = Map[5];
			Map[5] = Map[8];
			Map[8] = Map[11];
			Map[11] = Map[9];
			Map[9] = Map[7];
			Map[7] = Map[4];
			Map[4] = temp;
		}
	}
}

void onePuck() {
	while (1) {
		
	}
	twoPuck();
}

void twoPuck() {
	while (1) {
		
	}
}

void zeroPuck() {
	while (ud == 0) { 
		Loop(1); 
		if (Map[0] == 0 && Map[3] == Map[6]) {
			
		}
	}
	
	onePuck();
}

void makeOnePuck(int rl) {
	// 아래에 위치한 퍽을 Loop 안으로 진입시킴
	// rl이 -1이면 왼쪽에 있는것, 1이면 오른쪽에 있는것


}

void solve() {
	if (gps[0] == 10) {
		Map[10] = Map[11];
		Map[11] = 0;
		gps[0] = 11;
		gps[1] = 1;
	}
	if (gps[0] == 12) {
		Map[12] = Map[11];
		Map[11] = 0;
		gps[0] = 11;
		gps[1] = 1;
	}

	if (Map[0] == 0) vertex[0] += 1;
	else if (Map[0] == Map[6]) pucktex += 1;
	if (Map[2] == 0) vertex[0] += 1;
	else if (Map[2] == Map[6]) pucktex += 1;
	if (Map[10] == 0) vertex[1] += 1;
	else if (Map[10] == Map[6]) pucktex += 1;
	if (Map[12] == 0) vertex[1] += 1;
	else if (Map[12] == Map[6]) pucktex += 1;


	if (pucktex == 1) {
		// 미들 퍽 색 1개가 vertex에 이미 위치해 있다.
		// 가능성 총 2가지
		// 위에 하나, 아래에 하나
		if (Map[0] == Map[6] || Map[2] == Map[6]) ud = 1;
		else if (Map[10] == Map[6] || Map[12] == Map[6]) ud = 2;
		onePuck();
	}
	else if (pucktex == 2) {
		// 미들 퍽 색 2개가 vertex에 이미 위치해 있다.
		// 가능성 총 4가지
		// 위에 두개, 아래에 두개, 엇갈림 2가지 경우
		if (Map[0] == Map[6] && Map[2] == Map[6]) { ud = 1; twpPuck(); }        // 위에 2개
		else if (Map[10] == Map[6] || Map[12] == Map[6]) { ud = 2; twpPuck(); }   // 아래에 2개
		else {
			// 미들 퍽이 엇갈리게 위 아래로 위치함
			ud = 1;
			// 아래에 위치한 퍽을 Loop 안으로 진입시킴
			if (Map[10] == Map[6]) makeOnePuck(-1);
			else if (Map[12] == Map[6]) makeOnePuck(1);
			onePuck();
		}
	}
	else {
		// 사전에 일치하는 Map[6] 퍽이 없다
		zeroPuck();
	}
	
}


void Gyeongnam() {
	
	FCC(1, 0, 0, 0);
	FCC(1, 2, 0, 0);
	Pm(0, 0, 0, 150, 650, 11, 0, 0, 1550, 0, 0);
	
	gps[0] = 11;
	gps[1] = 1;
	vertex[0] = 0;
	vertex[1] = 0;
	pucktex = 0;
	ud = 0;
	MidColor = Map[6];

	solve();
}


int main(void) {
	//Busan();
	Gyeongnam();

	return 0;
}
