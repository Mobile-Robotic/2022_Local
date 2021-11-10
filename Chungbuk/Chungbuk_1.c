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
int G1 = 0;
int G1wall = 0;
int Barcount=0;
int G2 = 0;
void Side(int rl) {
    Pm(0, 2, 2, 150, 400, 11, 0, 0, rl == -1 ? 300 : 450, 0, 0);
    Pm(0, 2, 2, 150, 400, 10, 1, rl == -1 ? 1: 8, 120, 0, 0);
	FCC(2, 0, rl == -1 ? 8:1, 120);
    
	V1(1, 0, 0, 0, 0, 180, 0, 240, 0, 0, 0);
    for(int i = 0;i<2;i++) {
        if(C_D[i][0] == PUCK) {
            ping(1);
            V1(0, i, 0, 0, 0, 180, 0, 240, 0, 135, 2);
            HM(-pw[1], 300, 11, 90, 0, 0);
			HM(-pw[1], 300, 11, -90, 0, 0);
			LM(1, 300, 100, 11, 0, 0, 0, 0);
            FCC(2, 0, rl == -1 ? 8:1, 130);
		}
    }

    turn(rl, 100, 11, 20, 0, 0);
    Pm(rl == -1? 4:5, 2, 2, 150, 500, 11, 0, 0, 180, 0, 0);
    TD(rl * -20, 300, 11, 200, 0, rl * 71, 0);
    FCC(2, 2, 0, 0);
	
	FCC(2, 0, rl == -1 ? 8:1, 135);
    TD(0, 300, 11, 450, rl * -15, rl * -91, 0);
	
	if (rl == -1) FCC(1, 0, 0, 0);
	else FCC(1, 1, 0, 0);
	HM(0,300,11,0,rl * 10,0);
	FCC(2, 0, rl == -1 ? 8:1, 200);

	V1(1, 0, 0, 0, 0, 180, 0, 240, 0, 0, 0);
	V1(0, 0, 0, 0, 0, 180, 0, 240, 130, 135, 1);

	if(C_D[0][0] == PUCK) {
		HM(0, 200, 11, 50, 0, 0);
		HM(0, 200, 11, -50, 0, 0);
	}
	HM(0, 200, 11, 0, rl * -60, 0);
	if (rl == -1) FCC(1, 0, 0, 0);
	else FCC(1, 1, 0, 0);
	FCC(2, 0, rl == -1 ? 8:1, 200);

	TD(0, 300, 11, -20, rl * -450, rl * 91, 0);
	FCC(2, 2, 0, 0);
	FCC(2, 0, rl == -1 ? 8:1, 135);

	HM(0, 300, 11, -20, rl == -1 ? 120 : -150, 0);
	turn(rl * 1, 100, 11, 91, 0, 0);

}

void StartArea() {
    
	FCC(1, 2, 0, 0);
	FCC(2, 0, 1, 165);
	barc = 0;
	Pm(0, 2, 2, 150, 340, 11, 0, 0, 200, 0, 0);
    turn(1, 100, 11, 20, 0, 0);
	barc = 0;
	turn(-1, 100, 11, 40, 0, 0);
	TD(20, 300, 11, 250 ,0, -50, 0);
	
	PUCK = barc;
	
	if(psd_value[8] < 120) {
		// 맨 앞이 퍽이 없다면
		Pm(5, 0, 0, 150, 500, 11, 0, 0, 200, 0, 0);
		turn(-1, 100, 11, 20, 0, 0);
		FCC(1, 1, 0, 0);
	}	
	else if(psd_value[7] < 100) {
		turn(-1, 100, 11, 20, 0, 0);
		Pm(0, 2, 2, 150, 500, 11, 0, 0, 400, 0, 0);
		FCC(1, 1, 0, 0);
	}
	else if(psd_value[6] < 100) {
		turn(-1, 100, 11, 20, 0, 0);
		Pm(0, 0, 1, 150, 500, 11, 0, 0, 200, 0, 0);
		FCC(1, 1, 0, 0);
	}
	
	/*
    Side(-1);
	
	if(psd_value[1] < 100) Wall = 3;
	Pm(0, 2, 2, 150, 500, 11, 0, 0, 300, 0, 0);
	if(psd_value[1] < 100) Wall = 2;
	dc(0, 0, Wall);
	
	
	Side(1);

	Pm(0, 2, 2, 150, 400, 10, 0, 0, 300 * (Wall-1), 0, 0);
	TD(0, 300, 01, 320, 20, 91, 0);
	FCC(2, 2, 0, 0);
	TD(0, 500, 10, 530, 0, -71, 0);
	Pm(5,2,2,150,500,00,1,5,100,0,0);
	turn(-1,100,11,20,0,0);
	FCC(1,2,3,230);
	*/

}
void BArea(int rl){
	if(rl == 1)	FCC(1,2,3,230);
	else FCC(1,2,6,230);
	if(rl == 1 ? psd_value[7] < 100 : psd_value[2] < 100){
		G1=1;
		TD(0,400,11,rl * -10,rl * 530,rl * -91,0);
		rl == 1 ? FCC(1,0,0,0) : FCC(1,1,0,0);
		TD(0,300,11,-10,rl * 300,rl * -91,0);
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
			TD(0,500,01,350,30,91,0);
			FCC(1,2,3,230);
		}
		
	}else{
		G1=2;
		TD(0,400,10,500,0,rl * 91,0);
		TD(0,400,11,520,rl * 50,rl * 91,0);
		rl == 1 ? FCC(1,2,3,240) : FCC(1,2,6,230); 
		TD(0,400,11,500,rl * -30,rl * -91,0);
		HM(0,300,11,50,0,0);
		rl == 1 ? FCC(1,2,6,230) : FCC(1,2,3,230);
		FCC(2,2,0,0);
		if(rl == 1){
			barc=0;
			HM(0,300,11,200,0,0);
			Barcount=barc;
			HM(0,300,11,-200,0,0);
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

	if(Barcount != PUCK && G1 == 1){
		turn(1,100,11,20,0,0);
		Pm(5,2,2,150,500,10,0,0,470,0,0);
		TD(-20,400,10,0,-300,50,0);
		Pm(4,2,2,150,500,10,1,4,100,0,0);
		turn(1,100,11,20,0,0);
		BArea(-1);
	}else if(Barcount != PUCK && G1 == 2){
		HM(0,300,11,-50,0,0);
		TD(0,400,11,0,-530,71,0);
		TD(20,400,11,-10,500,-51,0);
		TD(-20,400,11,-10,-350,51,0);
		Pm(4,2,2,150,500,10,1,4,100,0,0);
		turn(1,100,11,22,0,0);
		BArea(-1);
	}
	else{
		Pm(0,0,1,150,500,10,1,0,60,0,0);
		TD(0,400,11,300,-20,-91,0);
		FCC(1,2,6,250);

	}
	
}
void Puck6(){
		
}
void M1() {
    StartArea();
	//MidArea();

}
void M2()
{
	FCC(1,2,3,230);
	HM(0,300,11,0,-10,0);

	TD(0,400,11,500,-20,91,0);
	
	FCC(2,0,3,200);
	FCC(2,0,8,100);
	/*V1(1, 1, 0, 0, 0, 180, 0, 240, 0, 0, 0);
	for(int i=0;i<3;i++){
		dc(i,0,C_D[i][0]);
	}
    for(int i = 0;i<3;i++) {
        if(C_D[i][0] == PUCK) {
            ping(1);
            V1(0, 1, i, 0, 0, 180, 0, 240, 0, 135 + 10 * i, 2);
            HM(-pw[1], 300, 11, 190 + 30 * i, 0, 0);
			HM(-pw[1], 300, 11, -190 - 30 * i, 0, 0);
		}
    }
	LM(1, 300, 100, 11, 0, 0, 0, 0);*/
	turn(-1,200,11,70,0,0);
	Pm(5,2,2,150,400,10,1,5,150,0,0);
	turn(-1,100,11,20,0,0);
	FCC(1,2,3,230);
	Pm(0,2,2,150,400,10,1,1,100,0,0);
	HM(0,400,00,200,220,0);
	HM(0,400,01,210,0,0);
	TD(0,250,11,200,-220,92,0);
	FCC(2,0,6,200);
	FCC(2,0,1,100);
	turn(1,200,11,70,0,0);
	Pm(4,2,2,150,400,10,1,4,150,0,0);
	turn(1,100,11,20,0,0);
	FCC(1,2,6,230);
	HM(0,300,11,0,10,0);
	TD(0,300,11,270,30,91,0);
	barc = 0;
	Pm(0,2,2,150,400,10,0,0,250,0,0);
	if(barc == PUCK){
		G2 = 1;
	}else{
		G2 = -1;
		turn(-1,100,11,20,0,0);
		Pm(4,2,2,150,400,10,0,0,250,0,0);
		turn(-1,200,11,50,0,0);
		Pm(5,2,2,150,400,10,1,5,150,0,0);
		turn(-1,100,11,20,0,0);
		FCC(1,2,6,230);
		HM(0,300,11,0,10,0);
		Pm(0,2,2,150,400,10,1,8,100,0,0);
		HM(0,400,00,200,-220,0);
		HM(0,400,01,210,0,0);
		TD(0,400,11,430,300,91,0);
		FCC(2,0,4,185);
		FCC(2,0,1,180);
		
		Pm(0,2,2,150,350,10,0,0,150,0,0);
		TD(0,350,00,300,0,-40,0);
		TD(40,350,01,30,-250,40,0);
		FCC(1,0,4,250);
	}
	
	


			
}
void M3()
{
	FCC(1,2,3,230);
	if(psd_value[7] < 100){
		TD(0,400,10,0,300,-89,1);
		FCC(1,0,0,0);
		
		
	}else{
		TD(0,400,10,500,0,91,1);
		TD(0,400,11,520,40,91,0);
		FCC(1,2,3,250);
		TD(0,400,11,500,0,89,0);
		FCC(1,2,6,250);
	}
	HM(0,300,11,150,0,0);
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
