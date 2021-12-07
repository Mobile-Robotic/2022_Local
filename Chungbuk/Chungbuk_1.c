#include "Interface.h"

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

void LS()
{
    if (S&8) return;
    if (S&0x24) Hm(-90,100,10,0,0,3,8,0,0,0);
    else if (S&0x50) Hm(90,100,10,0,0,3,8,0,0,0);
    else 
    {
        HM(90,500,11,70,0,0);
        Hm(-90,100,10,0,0,3,8,0,0,0);
    }
    StopMotion(9);
}
int PUCK=1;
int Wall = 1;
int B[2] = {0};//B구역 가변벽 오른쪽부터 인덱스 0, 1(가변벽이 양쪽 벽에 붙어있을시 1, 가운데쪽에 붙을시 2)
int G1 = 1;//모든수행후 돌아올때 G1구역 통과해야'할'부분 (오른쪽부터 1, 2)
int Barcount=0;
int G2 = -1;//모든수행후 돌아올때 G2구역 통과해야'할'부분 (오른쪽부터 1, 2)
void CG2(int num){
	if(num == 1)G2 = -1 * G1;
	else G2 = 1 * G1;
}
void Side(int rl) {
    Pm(0, 2, 2, 150, 400, 11, 0, 0, rl == -1 ? 300 : 450, 0, 0);
    Pm(0, 2, 2, 150, 400, 10, 1, rl == -1 ? 1: 8, 120, 0, 0);
	FCC(2, 0, rl == -1 ? 8:1, 120);
    
	V1(1, 0, 0, 0, 0, 180, 0, 240, 0, 0, 0);
    for(int i = 0;i<2;i++) {
        if(C_D[i][0] == PUCK) {
            ping(1);
            V1(0, i, 0, 0, 0, 180, 0, 240, 0, 135, 2);
            HM(0, 300, 11, 90, 0, 0);
			HM(0, 300, 11, -90, 0, 0);
			LM(1, 300, 100, 11, 0, 0, 0, 0);
            FCC(2, 0, rl == -1 ? 8:1, 130);
		}
    }

    turn(rl, 100, 11, 20, 0, 0);
    Pm(rl == -1? 4:5, 2, 2, 150, 500, 11, 0, 0, 180, 0, 0);
    TD(rl * -20, 300, 11, 200, 0, rl * 71, 0);
    FCC(2, 2, 0, 0);
    TD(0, 300, 11, 450, 0, rl * -91, 0);
	
	rl == -1 ? FCC(1,0,0,0) : FCC(1,1,0,0);
	rl == -1 ? FCC(2,0,3,240) : FCC(2,0,6,240);
	FCC(2,2,0,0);

	V1(1, 0, 0, 0, 0, 180, 0, 240, 0, 0, 0);

	if(C_D[0][0] == PUCK) {
		HM(0, 200, 11, 50, 0, 0);
		HM(0, 200, 11, -50, 0, 0);
	}
	HM(0, 200, 11, 0, rl * -60, 0);
	if (rl == -1) FCC(1, 0, 0, 0);
	else FCC(1, 1, 0, 0);
	TD(0, 300, 11, 0, rl * -450, rl * 91, 0);
	FCC(2, 2, 0, 0);
	rl == -1 ? FCC(2,0,5,251) : FCC(2,0,4,251);

	HM(0, 300, 11, -20, rl == -1 ? 120 : -150, 0);
	turn(rl * 1, 100, 11, 91, 0, 0);

}

void StartArea() {
    
	FCC(1, 2, 0, 0);
	FCC(2, 0, 1, 165);
	barc = 0;
    Pm(0, 2, 2, 150, 500, 11, 0, 0, 200, 0, 0);
	turn(1, 100, 11, 20, 0, 0);
	turn(-1, 250, 11, 80, 0, 0);
	TD(60, 250, 11, 250 ,0, -30, 0);
	
	PUCK = barc;
	FCC(2, 0, 1, 190);
	
    Side(-1);
	
	if(psd_value[1] < 100) Wall = 3;
	Pm(0, 2, 2, 150, 500, 11, 0, 0, 300, 0, 0);
	if(psd_value[1] < 100) Wall = 2;
	dc(0, 0, Wall);
	
	
	Side(1);

	Pm(0, 2, 2, 150, 400, 10, 0, 0, 300 * (Wall-1), 0, 0);
	TD(0, 300, 01, 320, 20, 91, 0);
	FCC(2, 2, 0, 0);
	TD(0, 400, 10, 450, 0, -71, 0);
	Pm(5,2,2,150,400,00,1,5,100,0,50);
	turn(-1,100,11,20,0,0);
	FCC(1,2,3,230);
	

}
void BArea(int rl){
	int count = 0;
	if(rl == 1)	FCC(1,2,3,230);
	else FCC(1,2,6,230);
	if(rl == 1 ? psd_value[7] < 100 : psd_value[2] < 100){
		B[count] = 1;
		count++;
		TD(0,400,11,-10,rl * 525,rl * -91,0);
		rl == 1 ? FCC(1,0,0,0) : FCC(1,1,0,0);
		TD(0,300,11,-5,rl * 300,rl * -91,0);
		TD(0,300,11,300,0,rl * -91,0);
		rl == 1 ? FCC(1,1,0,0) : FCC(1,0,0,0);
		FCC(2,2,0,0);
		if(rl == 1){
			barc=0;
			HM(0,300,11,200,0,0);
			Barcount=barc;
			HM(0,300,11,-200,-20,0);
			FCC(1,1,0,0);
		}else{
			Pm(0,2,2,150,500,10,0,0,500,0,0);
			TD(0,500,01,350,10,91,0);
			HM(0,300,11,20,0,0);
			FCC(1,2,3,230);
		}
		
	}else{
		B[count] = 2;
		count++;
		TD(0,400,10,500,0,rl * 91,0);
		TD(0,400,11,520,rl * 50,rl * 91,0);
		rl == 1 ? FCC(1,2,3,240) : FCC(1,2,6,230);
		FCC(2,2,0,0); 
		TD(0,400,11,500,rl * -15,rl * -91,0);
		HM(0,300,11,50,0,0);
		rl == 1 ? FCC(1,2,6,230) : FCC(1,2,3,230);
		FCC(2,2,0,0);
		if(rl == 1){
			barc=0;
			HM(0,300,11,200,0,0);
			Barcount=barc;
			HM(0,300,11,-200,0,0);
			HM(0,300,11,0,-30,0);
			FCC(1,2,6,230);

		}else {
			Pm(0,2,2,150,500,10,0,0,500,0,0);
			TD(0,500,01,350,30,91,0);
			FCC(1,2,3,230);
		}
	}
}

void MidArea(){
	BArea(1);
	if(Barcount == PUCK){
		G1 = 1;//오른쪽일때
		Pm(0,0,1,150,500,10,1,0,60,0,0);
		TD(0,400,11,300,-20,-91,0);
		FCC(1,2,6,250);
		
	}else if(B[0] == 1){//안쪽
		G1 = -1;//왼쪽일때
		turn(1,100,11,20,0,0);
		Pm(5,2,2,150,500,10,0,0,470,0,0);
		TD(-20,400,00,0,-300,50,50);
		Pm(4,2,2,150,500,10,1,4,100,0,0);
		turn(1,100,11,20,0,0);
		BArea(-1);
	}else if(B[0] == 2){//가장자리
		G1 = -1;//왼쪽일때
		HM(0,300,11,-50,0,0);
		TD(0,400,11,0,-530,71,0);
		TD(20,400,11,-10,500,-51,0);
		TD(-20,400,11,-10,-350,51,0);
		Pm(4,2,2,150,500,10,1,4,100,0,0);
		turn(1,100,11,22,0,0);
		BArea(-1);
	}
	
	
}
int first(int num, int a){// rl값, 순서
	if(num == 1){
		if(a == 1) return 1;
		else return 0;
	}
	else {
		if(a == 1)return 0;
		else return 1;
	} 
}
void Puck6(int rl){//왼쪽시작이 1, 오른쪽 시작이 -1
	
	rl == 1 ? FCC(1,2,3,230) : FCC(1,2,6,230);
	HM(0,300,11,0,-10 * rl,0);

	TD(0,400,11,500,-20 * rl,91 * rl,0);
	
	rl == 1 ? FCC(2,0,3,200) : FCC(2,0,6,200);
	rl == 1 ? FCC(2,0,8,100) : FCC(2,0,1,100);
	char tmp[3];
	V1(0,first(rl, 1),0,0,0,140,0,240,0,130,2);
	V1(1,first(rl, 1),0,0,0,140,0,240,0,0,0);
	for(int i=0;i<3;i++) tmp[i]=C_D[i][0];
	
	ping(3);
	for(int i=0;i<3;i++){
		if(tmp[i]==PUCK){
			LM(3,0,100,11,px[3],py[3],20*i*-rl,0);
			HM(-pw[3],500,11,180+40*i,0,0);
			LM(3,400,0,11,0,0,pw[3],0);
		}
	}
	LM(3,0,100,11,px[3],py[3],0,0);
	turn(-1 * rl,200,11,70,0,0);
	Pm(rl == 1 ? 5 : 4,2,2,150,400,10,1,rl == 1 ? 5 : 4,150,0,0);
	turn(-1 * rl,100,11,20,0,0);
	rl == 1 ? FCC(1,2,3,230) : FCC(1,2,6,230);
	Pm(0,2,2,150,400,10,1,rl == 1 ? 1 : 8,90,0,0);
	HM(0,400,00,200,200 * rl,50);
	HM(0,400,01,220,0,50);
	TD(0,250,11,250,-220 * rl,92 * rl,0);
	rl == 1 ? FCC(2,0,6,250) : FCC(2,0,3,250);
	//rl == 1 ? FCC(2,0,1,130) : FCC(2,0,8,130);
	V1(0,first(rl, 2),0,0,0,140,0,240,0,130,2);
	V1(1,first(rl, 2),0,0,0,140,0,240,0,0,0);
	for(int i=0;i<3;i++) tmp[i]=C_D[i][0];
	
	ping(3);
	for(int i=0;i<3;i++){
		if(tmp[i]==PUCK){
			LM(3,0,100,11,px[3],py[3],20*i*rl,0);
			HM(-pw[3],500,11,180+30*i,0,0);
			LM(3,400,0,11,0,0,pw[3],0);
		}
	}
	LM(3,0,100,11,px[3],py[3],0,0);
	rl == 1 ? FCC(2,0,6,230) : FCC(2,0,3,230);
	//rl == 1 ? FCC(2,0,1,130) : FCC(2,0,8,130);
	
	turn(1 * rl,200,11,70,0,0);
	Pm(rl == 1 ? 4 : 5,2,2,150,400,10,1,rl == 1 ? 4 : 5,150,0,0);
	turn(1 * rl,100,11,20,0,0);

	rl == 1 ? FCC(1,2,6,230) : FCC(1,2,3,230);
	HM(0,300,11,0,10 * rl,0);
	TD(0,300,11,270,30 * rl,91 * rl,0);
	ping(1);
	barc = 0;
	Pm(0,2,2,150,400,10,0,0,300,0,0);
	dc(0,0,barc);
	if(barc == PUCK){
		CG2(1);
		HM(180,300,11,200,0,0);
		TD(0,350,10,350,0,40 * rl,0);
		TD(-40 * rl,350,01,30,250,-40 * rl,0);
		rl == 1 ? FCC(1,1,5,250) : FCC(1,0,4,250);
	}else{
		CG2(-1);
		LM(1,300,0,11,-10,0,0,0);
		turn(-1 * rl,200,11,70,0,0);
		Pm(rl == 1 ? 5 : 4,2,2,150,400,10,1,rl == 1 ? 5 : 4,150,0,0);
		turn(-1 * rl,100,11,20,0,0);
		rl == 1 ? FCC(1,2,6,230) : (1,2,3,230);
		HM(0,300,11,0,10 * rl,0);
		Pm(0,2,2,150,400,10,1,rl == 1 ? 8 : 1,90,0,0);
		HM(0,400,00,200,-200 * rl,50);
		HM(0,400,01,250,0,50);
		TD(0,400,11,430,300 * rl,91 * rl,0);
		rl == 1 ? FCC(2,0,4,185) : FCC(2,0,5,185);
		rl == 1 ? FCC(2,0,1,180) : FCC(2,0,8,180);
		
		Pm(0,2,2,150,400,10,0,0,150,0,0);
		TD(0,350,00,230,0,-35 * rl,50);
		TD(40 * rl,350,01,50,-250 * rl,30 * rl,50);
		rl == 1 ? FCC(1,0,4,250) : FCC(1,1,5,250);
	}
}

void LastArea(int rl){//왼쪽이 1 오른쪽이 -1
	rl == 1 ? FCC(1,0,4,250) : FCC(1,1,5,250);
	V1(1,0,0,0,0,140,0,240,0,0,0);
	if(C_D[0][0] == PUCK){
		Pm(0,2,2,150,350,10,0,0,400,0,0);
		turn(1,100,11,20,0,0);
		Pm(5,2,2,150,350,10,1,5,150,0,0);
		rl == 1 ? FCC(1,0,4,250) : FCC(1,1,5,250);
	}
	TD(0,300,11,-100,250 * rl,-30 * rl,0);
	HM(30 * rl,300,11,-50,0,0);
	turn(rl,150,11,60,0,0);
	HM(-30 * rl,300,10,150,0,0);
	TD(-30 * rl,300,01,110,200 * rl,-30 * rl,0);
	FCC(2,2,0,0);
	HM(0,300,11,50,100 * rl,0);
	FCC(1,2,0,0);
	HM(0,300,11,0,-100 * rl,0);
	FCC(2,2,0,0);
	V1(1,0,0,0,0,140,0,200,0,0,0);
	for(int i=0;i<2;i++){
		if(C_D[i][0] == PUCK){
			ping(3);
			LM(3,0,100,11,px[3],py[3],i == 1 ? 13 : -13,0);
			HM(-pw[3],400,11,300,0,0);
			LM(3,400,0,11,0,0,0,0);
		}
	}
	FCC(2,2,0,0);
	HM(0,300,11,-70,-100 * rl,0);
	turn(rl,200,11,90,0,0);
	HM(0,300,11,100,-70 * rl,0);
	Pm(0,2,2,150,400,11,0,0,200,0,0);
	TD(0,300,11,350,0,-91 * rl,0);
	FCC(2,2,0,0);
	HM(0,300,11,20,-110 * rl,0);
	FCC(1,2,0,0);
	HM(0,300,11,0,100 * rl,0);
	FCC(2,2,0,0);
	V1(1,0,0,0,0,140,0,200,0,0,0);
	for(int i=0;i<2;i++){
		if(C_D[i][0] == PUCK){
			ping(3);
			LM(3,0,100,11,px[3],py[3],i == 1 ? 13 : -13,0);
			HM(-pw[3],500,11,300,0,0);
			LM(3,400,0,11,0,0,0,0);
		}
	}
	HM(0,300,11,20,-110 * rl,0);
	FCC(1,2,0,0);
	turn(1 * rl,200,11,90,0,0);
	HM(0,350,10,100,0,0);
	HM(0,350,01,200,150 * rl,50);
	TD(0,350,11,300,0,-91 * rl,0);
	rl == 1 ? FCC(1,1,5,250) : FCC(1,0,4,250);
	V1(1,0,0,0,0,140,0,240,0,0,0);
	if(C_D[0][0] == PUCK){
		Pm(0,2,2,150,350,10,0,0,400,0,0);
		turn(1,100,11,20,0,0);
		Pm(5,2,2,150,350,10,1,5,150,0,0);
		rl == 1 ? FCC(1,1,5,250) : FCC(1,0,4,250);
	}
}
void Back(){//왼쪽으로나올때 G2 = 1 오른쪽이 G2 = -1
	int rl = G2;
	if(G1 == G2){
		rl == 1 ? FCC(1,0,4,250) : FCC(1,1,5,250);
		TD(0,300,11,-100,270 * rl,-30 * rl,0);
		HM(30 * rl,300,10,-400,0,0);
		HM(30 * rl,300,01,0,200 * rl,0);
		turn(-rl,200,11,60,0,0);
		FCC(2,0,rl == 1 ? 1 : 8,150);
		Pm(0,2,2,150,300,10,0,0,200,0,0);
		TD(0,300,01,250,-30 * rl,-90 * rl,0);
		HM(0,300,11,30,0,0);
		rl == 1 ? FCC(1,1,5,230) : FCC(1,0,4,230);
		FCC(2,2,0,0);
	}else{
		rl == 1 ? FCC(1,0,4,250) : FCC(1,1,5,250);
		TD(0,300,11,-100,270 * rl,-30 * rl,0);
		HM(30 * rl,300,11,-50,0,0);
		turn(rl,100,11,30,0,0);
		HM(0,300,11,-370,0,0);
		turn(rl,200,11,70,0,0);
		Pm(rl == 1 ? 4 : 5,2,2,150,350,10,1,rl == 1 ? 4 : 5,150,0,0);
		turn(rl,100,11,20,0,0);
		rl == 1 ? FCC(1,2,3,230) : FCC(1,2,6,230);
		Pm(0,2,2,150,400,10,1,rl == 1 ? 1 : 8,90,0,0);
		HM(0,400,00,200,200 * rl,50);
		TD(0,400,00,350,0, -30 * rl,50);
		TD(30 * rl,400,01,0,-250 * rl,20 * rl,50);
		Pm(0,2,2,150,300,10,0,0,200,0,0);
		TD(0,300,01,300,30 * rl,90 * rl,0);
		HM(0,300,11,30,0,0);
		rl == 1 ? FCC(1,0,4,230) : FCC(1,1,5,230);
		FCC(2,2,0,0);
	}
	rl = -G1;
	rl == 1 ? FCC(1,0,4,230) : FCC(1,1,5,230);
	FCC(2,2,0,0);
	Pm(0,2,2,150,500,10,1,0,60,0,0);
	TD(0,400,01,300,0,92 * rl,0);
	HM(0,300,11,50,0,0);
	rl == 1 ? FCC(1,2,3,230) : FCC(1,2,6,230);
	
	if(rl == 1 ? B[0] : B[1] == 2){//가장자리 (2)일때
		FCC(2,2,0,0);
		TD(0,400,10,350,-15 * rl,-70 * rl,0);
		TD(-20,400,00,450,0,50 * rl,50);
		Pm(rl == 1 ? 4 : 5,2,2,150,400,00,1,rl == 1 ? 4 : 5,150,0,100);
		turn(1 * rl,100,11,20,0,0);
		rl == 1 ? FCC(1,2,3,230) : FCC(1,2,6,230);
	}
	Pm(0,0,rl == 1 ? 0 : 1,150,350,11,0,0,(rl == 1 ? (Wall - 1) : (3 - Wall)) * 300,0,0);
	TD(0,400,11,450,-30 * rl,-93 * rl,0);
	FCC(2,2,0,0); 
	if(Wall == 1){
		Pm(0,2,2,150,300,11,0,0,200,0,0);
		turn(1,100,11,40,0,0);
		HM(-40,300,10,150,0,0);
		TD(-40,300,01,100,350,-35,50);
		FCC(2,2,0,0);
		TD(0,300,11,450,0,-60,0);
	}else if(Wall == 2){
		Pm(0,2,2,150,300,11,0,0,500,0,0);
		FCC(2,2,0,0);
		TD(0,300,11,450,0,-60,0);

	}else if(Wall == 3){
		Pm(0,2,2,150,300,11,0,0,200,0,0);
		turn(-1,100,11,40,0,0);
		HM(40,300,10,170,0,0);
		TD(40,300,01,100,-350,35,50);
		FCC(2,2,0,0);
		TD(0,300,11,450,0,-60,0);
	}
}
void M1() {
    StartArea();
	MidArea();
	Puck6(-G1);
	LastArea(-G2);
	Back();
}

void M2()
{
	LastArea(1);
}
void M3()
{
	StartArea();
	
}
void M4()
{	
	FCC(2,2,0,0);

}
void M5()
{
	Back();
}
void M6()
{
	int rl = 1;

	rl == 1 ? FCC(1,0,0,0) : FCC(1,1,0,0);
	rl == 1 ? FCC(2,0,3,240) : FCC(2,0,6,250);
	FCC(2,2,0,0);
}
void M7()
{
int rl = 1;
	Wall = 2;
	rl == 1 ? FCC(1,2,3,230) : FCC(1,2,6,230);
	Pm(0,0,rl == 1 ? 0 : 1,150,350,11,0,0,(rl == 1 ? (Wall - 1) : (3 - Wall)) * 300,0,0);
	TD(0,400,11,450,-30 * rl,-93 * rl,0);
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
