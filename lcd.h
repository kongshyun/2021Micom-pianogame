#ifndef LCD_H_
#define LCD_H_
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#define sbi(x, y) (x |= (1 << y))  // x의 y 비트를 설정(1)
#define cbi(x, y) (x &= ~(1 << y)) // x의 y 비트를 클리어(0)
// CON 포트는 포트C와연결됨을정의
#define LCD_CON      
PORTC
// DATA 포트는 포트 A와연결됨을정의
#define LCD_DATA     
PORTA
// DATA 포트의 출력 방향설정매크로를정의
#define LCD_DATA_DIR DDRA
// DATA 포트의 입력 방향설정매크로를정의
#define LCD_DATA_IN  PINA
// RS 신호의 비트번호정의
#define LCD_RS   0
// RW 신호의비트번호정의
#define LCD_RW   1
// E 신호의 비트번호정의
#define LCD_E    2

// 텍스트 LCD로 부터상태(명령)를읽는함수
unsigned char LCD_rCommand(void){
    unsigned char temp=1;
    LCD_DATA_DIR = 0X00;
    cbi(LCD_CON, LCD_RS); // 0번 비트 클리어, RS = 0, 명령
    sbi(LCD_CON, LCD_RW); // 1번 비트 설정, RW = 1, 읽기
    sbi(LCD_CON, LCD_E);  // 2번 비트 설정, E = 1
    _delay_us(1);
    temp = LCD_DATA_IN;      
    _delay_us(1);
    // 명령 읽기
    cbi(LCD_CON, LCD_E);  // 명령 읽기 동작 끝
    LCD_DATA_DIR = 0XFF;
    _delay_us(1);
    return temp;
}


// 텍스트 LCD의 비지플래그상태를확인하는함수
char LCD_BusyCheck(unsigned char temp){
    if(temp & 0x80)     return 1;
    else                return 0;      
}



// 텍스트 LCD에 명령을 출력하는 함수 - 단, 비지플래그를 체크하지 않음
void LCD_wCommand(char cmd) {
    cbi(LCD_CON, LCD_RS); // 0번 비트 클리어, RS = 0, 명령
    cbi(LCD_CON, LCD_RW); // 1번 비트 클리어, RW = 0, 쓰기
    sbi(LCD_CON, LCD_E);  // 2번 비트 설정, E = 1

    LCD_DATA = cmd;       // 명령 출력
    _delay_us(1);
    cbi(LCD_CON, LCD_E);  // 명령 쓰기 동작 끝

    _delay_us(1);
}

// 텍스트 LCD에 명령을 출력하는 함수 - 단, 비지플래그를 체크함
void LCD_wBCommand(char cmd) {
    while (LCD_BusyCheck(LCD_rCommand())) // 비지 플래그 확인
        _delay_us(1);

    cbi(LCD_CON, LCD_RS); // 0번 비트 클리어, RS = 0, 명령
    cbi(LCD_CON, LCD_RW); // 1번 비트 클리어, RW = 0, 쓰기
    sbi(LCD_CON, LCD_E);  // 2번 비트 설정, E = 1

    LCD_DATA = cmd;       // 명령 출력
    _delay_us(1);
    cbi(LCD_CON, LCD_E);  // 명령 쓰기 동작 끝

    _delay_us(1);
}


// 텍스트 LCD를 초기화하는 함수
void LCD_Init(void) {
    _delay_ms(100);
    // 비지 플래그를 체크하지 않는 Function Set
    LCD_wCommand(0x38);
    _delay_ms(10);
    // 비지 플래그를 체크하지 않는 Function Set
    LCD_wCommand(0x38);
    _delay_us(200);
    // 비지 플래그를 체크하지 않는 Function Set
    LCD_wCommand(0x38);
    _delay_us(200);

    // 비지 플래그를 체크하는 Function Set
    LCD_wBCommand(0x38);
    // 비지 플래그를 체크하는 Display On/Off Control
    LCD_wBCommand(0x0c);
    // 비지 플래그를 체크하는 Clear Display
    LCD_wBCommand(0x01);
}

// 텍스트 LCD에 1바이트 데이터를 출력하는 함수
void LCD_wData(char dat) {
    while (LCD_BusyCheck(LCD_rCommand())) // 비지 플래그 상태 확인
        _delay_us(1);

    sbi(LCD_CON, LCD_RS); // 0번 비트 설정, RS = 1, 데이터
    cbi(LCD_CON, LCD_RW); // 1번 비트 클리어, RW = 0, 쓰기
    sbi(LCD_CON, LCD_E);  // 2번 비트 설정, E = 1

    LCD_DATA = dat;       // 데이터 출력
    _delay_us(1);
    cbi(LCD_CON, LCD_E);  // 데이터 쓰기 동작 끝

    _delay_us(1);
}

// 텍스트 LCD에 문자열을 출력하는 함수
void LCD_wString(char *str) {
    while (*str)
        LCD_wData(*str++); // 문자열의 각 문자 출력
}

#endif /* LCD_H_ */
