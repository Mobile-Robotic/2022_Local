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
void Side(int rl) {
    Pm(0, 2, 2, 150, 500, 10, 1, 0, 60, 0, 0);

    V1(1, 0, 0, 0, 0, 180, 0, 240, 0, 0, 0);
    for(int i = 0;i<2;i++) {
        if(C_D[i][0] == PUCK) {
            ping(1);
            V1(0, i, 0, 0, 0, 180, 0, 240, 135, 125, 3);
            HM(0, 300, 11, 60, 0, 0);
            LM(1, 300, 100, 11, 0, 0, 0, 0);
            FCC(2, 2, 0, 0);
        }
    }

    turn(rl, 100, 11, 20, 0, 0);
    Pm(rl == -1? 4:5, 2, 2, 150, 500, 11, 0, 0, 350, 0, 0);
    TD(rl * -20, 300, 11, 250, 0, rl * 71, 0);
    FCC(2, 2, 0, 0);
    TD(0, 400, 11, 450, 0, rl * -91, 0);
    if (rl == -1) FCC(1, 0, 0, 0);
	else FCC(1, 1, 0, 0);
    FCC(2, 0, rl == -1 ? 5:4, 250);

    V1(1, 0, 0, 0, 0, 180, 0, 240, 0, 0, 0);
    if(C_D[0][0] == PUCK) {
        ping(1);
        Pm(0, 0, 0, 150, 500, 11, 0, 0, 100, 0, 0);
        LM(1, 300, 10, 11, 0, 50, 0, 0);
        FCC(1, 0, 0, 0);
        FCC(2, 0, rl == -1 ? 5:4, 200);
    } 

    TD(0, 300, 11, 0, rl * -450, rl * 91, 0);
    FCC(2, 2, 0, 0);
	HM(0,300,11,-30,0,0);
    HM(0, 300, 11, 0, rl * -150, 0);
    turn(rl * 1, 100, 11, 90, 0, 0);
	FCC(2 ,2, 0, 0);
}

void StartArea() {
    FCC(1, 2, 0, 0);

    TD(0, 400, 11, 230,0, 25, 0);
	turn(-1,200,11,60,0,0);
    TD(35, 300, 11, 220, 0, -55, 0);
    PUCK = barc;

    Side(-1);
	Side(1);

}

void M1() {
    StartArea();



}
void M2()
{
	FCC(1, 2, 0, 0);
	TD(0, 300, 11, 200, 0, 45, 0);
	turn(-1, 100, 11, 90, 0, 0);
	dc(0, 0, barc);
}
void M3()
{
	HM(0,300,11,30,0,0);
	V1(1, 0, 0, 0, 0, 180, 0, 240, 0, 0, 0);
    for(int i = 0;i<2;i++) {
        if(C_D[i][0] == PUCK) {
            ping(1);
            V1(0, i, 0, 0, 0, 180, 0, 240, 125, 135, 3);
            HM(0, 300, 11, 60, 0, 0);
            LM(1, 300, 100, 11, 0, 0, 0, 0);
            FCC(2, 2, 0, 0);
        }
    }

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