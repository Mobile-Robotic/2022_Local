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

#define N 16
#define MAX 580

char Map[N] = {
	0, 0, 2, 2, 
	1, 3, 3, 9, 
	1, 2, 2, 0, 
	1, 3, 0, 3, 
};  //로봇 위치는 9로 기록


char Sequence[16];
char gate[4][2] = { 0, 0, 0, 0, 0, 0, 0, 0};
char temp[4][4];   //로봇 이동에 따른 맵의 변화를 기록
int rear = 0, front = 0;
int target = 0;  // arrangement에서 몇번째를 맞춰야 하는지 지정
char gps[2];
int parent[MAX];
char queue[MAX][2];  // 로봇의 X, Y 좌표만 기록
char stack[60][2]; //경로 역추적을 위한 stack


void arrangement() {
	//맞춰야 하는 순서 저장 배열
	Sequence[0] = Map[2];
	Sequence[1] = Map[14];
	Sequence[2] = Map[1];
	Sequence[3] = Map[13];
	Sequence[4] = Map[0];
	Sequence[5] = Map[12];
	Sequence[6] = Map[4];
	Sequence[7] = Map[8];
	Sequence[8] = Map[5];
	Sequence[9] = Map[9];
	Sequence[10] = Map[6];
	Sequence[11] = Map[10];
	Sequence[12] = Map[3];
	Sequence[13] = Map[15];
	Sequence[14] = Map[7];
	Sequence[15] = Map[11];
	for (int i = 0; i < 16; i++) printf("%d ", Sequence[i]);
	printf("\n");
}


void start() {
	HM(0, 650, 11, 520, -750, 0);
	FCC(1, 0, 0, 0);
	LS();
}

void Motion() {
	LS();
	turn(-1, 200, 11, 90, 0, 0);
	LS();
	turn(1, 200, 11, 90, 0, 0);
	LS();
	ping(1);
	Line(0, 400, 10, 0, 0, 150, 0, 0);
	LM(1, 400, 100, 01, 500, 0, 90, 0);
	LS();
}

void M1() {
	start();
	//solve();
}
void M2() {
	Motion();	
}
void M3() {
	
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
