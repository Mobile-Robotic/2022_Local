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

#define N 16

char gate[4][2] = {  //각각의 게이트가 지나갈 수 있는 색
	{3, 3},
	{3, 2},
	{2, 1},
	{1, 1}
};

char Map[N] = {   // 위에서부터 순서대로
	0, 0, 2, 2,
	1, 3, 3, 0,
	1, 2, 2, 0,
	1, 3, 0, 3
};

char dap[N];

char scanGate() {
	for (int i = 0; i < 4; i++) {
		if (Map[3 + (4 * i)] == gate[i][0] || Map[3 + (4 * i)] == gate[i][1]) return i;
	}
	return 9;
}

void start() {
	char po = scanGate();

	if (po == 9) {
		
	}
	else {
		
	}
}

void first() {
	dap[0] = Map[3];
	dap[1] = Map[2];
	dap[2] = Map[1];
	dap[3] = Map[0];
	dap[4] = Map[7];
	dap[5] = Map[6];
	dap[6] = Map[5];
	dap[7] = Map[4];
	dap[8] = Map[11];
	dap[9] = Map[10];
	dap[10] = Map[9];
	dap[11] = Map[8];
	dap[12] = Map[15];
	dap[13] = Map[14];
	dap[14] = Map[13];
	dap[15] = Map[12];

	char po = scanGate();
}

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


void startMotion() {
	int X[4] = {0,40,70,180};//카메라 X값
	int Y[3] = {0,120, 240};
	int m[16] = {0};//퍽 값
	int c[3] = {0};//테이프값

	turn(1,100,11,20,0,0);
	V1(1,2,0,0,0,180,0,240,0,0,0);
	for(int i=0;i<2;i++)c[i] = C_D[i][0];
	turn(-1,100,11,20,0,0);
	HM(0,500,11,500,-700,0);
	FCC(1,0,0,0);
	LS();
	V1(1,2,0,0,0,180,0,240,0,0,0);
	c[2] = C_D[0][0];
	gate[0][0] = c[0];
	gate[0][1] = c[0];
	gate[1][0] = c[1];
	gate[1][1] = c[2];
	gate[2][0] = c[2];
	gate[2][1] = c[3];
	gate[3][0] = c[3];
	gate[3][1] = c[3];
	for(int i=0;i<3;i++) dc(i,16,c[i]);
	HM(0,400,10,400,270,0);
	Hm(0,400,00,500,0,4,0x7C,300,0,0);
	HM(0,400,01,50,0,0);
	for(int i=0;i<3;i++){
		C_D[0][0] = 0;
		V1(1,3,0,0,X[i]+5,X[i+1],Y[0],Y[1],0,0,0);
		
		Map[4*3+i] = C_D[0][0];
		dc(i,0,Map[4*3+i]);
	}

	for(int i=0;i<3;i++){
		C_D[0][0] = 0;
		V1(1,0,0,0,X[i],X[i+1],Y[1],Y[2],0,0,0);
		
		Map[4*2+i] = C_D[0][0];
		dc(i,4,Map[4*2+i]);
	}
	TD(0,500,11,-450,100,-30,0);
	for(int i=0;i<3;i++){
		C_D[0][0]=0;
		V1(1,2,0,0,90,180,0,240,0,0,0);
		Map[15-i*4] = C_D[0][0];
		dc(3,4*i,Map[15-i*4]);
		if(i<2)HM(30,400,i == 0?10:00,0,500,0);
	}
	Hm(30,400,01,0,500,1,6,150,0,100);
	


	turn(1,100,11,30,0,0);
	FCC(1,1,0,0);
	LS();
	C_D[0][0]=0;
	V1(1,2,0,0,100,180,0,240,0,0,0);
	Map[3] = C_D[0][0];
	dc(3,12,Map[3]);
	HM(0,300,10,100,-270,0);
	Hm(0,400,00,500,0,4,0x7C,300,200,0);
	HM(0,400,01,50,0,0);
	for(int i=0;i<3;i++){
		C_D[0][0] = 0;
		V1(1,3,0,0,X[i]+5,X[i+1],Y[0],Y[1],0,0,0);
		
		Map[4*1+i] = C_D[0][0];
		dc(i,8,Map[4*1+i]);
	}

	for(int i=0;i<3;i++){
		C_D[0][0] = 0;
		V1(1,0,0,0,X[i],X[i+1],Y[1],Y[2],0,0,0);
		
		m[4*0+i] = C_D[0][0];
		dc(i,12,Map[4*0+i]);
	}
	HM(0,400,10,-250,0,0);
	HM(0,400,01,-100,250,50);
	FCC(1,1,0,0);	
	LS();


}

void M1() {
	startMotion();
	first();
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
