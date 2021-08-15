#include<stdio.h>   //printf등 함수
#include<windows.h> //gotoxy등 함수
#include<conio.h>	//getch등 함수	
#include<time.h>   //랜덤값 받기
#include<stdlib.h> //rand함수

#define LEFT 75 //좌로 이동 
#define RIGHT 77 //우로 이동 
#define UP 72 //회전 
#define DOWN 80 //한칸씩 떨어지기
#define SPACE 32 //한번에 떨어지기
#define p 112 //일시정지 
#define P 80 //일시정지
#define ESC 27 //게임종료 

#define false 0
#define true 1

#define ACTIVE_BLOCK -2 // 게임판배열에 저장될 블록의 상태
#define CEILLING -1     // 블록이 이동할 수 있는 공간은 0 또는 음의 정수
#define EMPTY 0         // 블록이 이동할 수 없는 공간은 양수
#define WALL 1
#define INACTIVE_BLOCK 2 // 이동이 완료된 블록값 

#define MAIN_X 11 //게임판 가로크기 
#define MAIN_Y 23 //게임판 세로크기 
#define MAIN_X_ADJ 3 //게임판 위치조정 
#define MAIN_Y_ADJ 1 //게임판 위치조정 
#define STATUS_X_ADJ MAIN_X_ADJ+MAIN_X+1 //게임정보표시 위치조정 

int GOAL; //GOAL 위치
int LEVEL; //LEVEL 위치
int SCORE; //SCORE 위치

int blocks[7][4][4][4] = {

    //ㅁ
0,0,0,0,
0,1,1,0,
0,1,1,0,
0,0,0,0,

0,0,0,0,
0,1,1,0,
0,1,1,0,
0,0,0,0,

0,0,0,0,
0,1,1,0,
0,1,1,0,
0,0,0,0,

0,0,0,0,
0,1,1,0,
0,1,1,0,
0,0,0,0,

    //ㅡ
0,0,0,0,
0,0,0,0,
1,1,1,1,
0,0,0,0,

0,1,0,0,
0,1,0,0,
0,1,0,0,
0,1,0,0,

0,0,0,0,
0,0,0,0,
1,1,1,1,
0,0,0,0,

0,1,0,0,
0,1,0,0,
0,1,0,0,
0,1,0,0,

    //ㄱㄴ
0,0,0,0,
1,1,0,0,
0,1,1,0,
0,0,0,0,

0,0,0,0,
0,0,1,0,
0,1,1,0,
0,1,0,0,

0,0,0,0,
1,1,0,0,
0,1,1,0,
0,0,0,0,

0,0,0,0,
0,0,1,0,
0,1,1,0,
0,1,0,0,

    //ㄱㄴ 반대로
0,0,0,0,
0,1,1,0,
1,1,0,0,
0,0,0,0,

0,0,0,0,
1,0,0,0,
1,1,0,0,
0,1,0,0,

0,0,0,0,
0,1,1,0,
1,1,0,0,
0,0,0,0,

0,0,0,0,
1,0,0,0,
1,1,0,0,
0,1,0,0,

//ㄴ 반대로
0,0,0,0,
0,0,1,0,
1,1,1,0,
0,0,0,0,

0,0,0,0,
1,1,0,0,
0,1,0,0,
0,1,0,0,

0,0,0,0,
0,0,0,0,
1,1,1,0,
1,0,0,0,

0,0,0,0,
0,1,0,0,
0,1,0,0,
0,1,1,0,

//ㄴ
0,0,0,0,
1,0,0,0,
1,1,1,0,
0,0,0,0,

0,0,0,0,
0,1,0,0,
0,1,0,0,
1,1,0,0,

0,0,0,0,
0,0,0,0,
1,1,1,0,
0,0,1,0,

0,0,0,0,
0,1,1,0,
0,1,0,0,
0,1,0,0,

//ㅗ
0,0,0,0,
0,1,0,0,
1,1,1,0,
0,0,0,0,

0,0,0,0,
0,1,0,0,
0,1,1,0,
0,1,0,0,

0,0,0,0,
0,0,0,0,
1,1,1,0,
0,1,0,0,

0,0,0,0,
0,1,0,0,
1,1,0,0,
0,1,0,0
}; // block [블록의 종류][블록의 회전개수][  ][블록의 가로최대 길이]

int b_type; //블록 종류 저장 
int b_rotation; //블록 회전 저장 
int b_type_next; //블록 저장 

int main_org[MAIN_Y][MAIN_X]; //메인 게임판
int main_cpy[MAIN_Y][MAIN_X]; //maincpy는 게임판이 모니터에 표시되기 전의 정보를 가지고 있음, main_cpy와 배열을 비교해서 값이 달라진 곳만 모니터에 고침 
int bx, by; //이동중인 블록의 게임판상의 x,y좌표를 저장 

int key; //키보드로 입력받은 키값

int speed; //블록 떨어지는 속도
int level; //현재 level 
int level_goal; //다음레벨로 넘어가기 위한 목표점수 
int cnt; //현재 레벨에서 제거한 줄 수
int score; //현재 점수 
int last_score = 0; //마지막게임점수 
int best_score = 0; //최고게임점수 

int new_block_on = 0; //새로운 블럭이 필요함을 알려줌
int crush_on = 0; //현재 이동중인 블록이 충돌상태인지 알려줌
int level_up_on = 0; //현재 레벨 목표 완료여부를 알려줌
int space_key_on = 0; //hard drop상태임을 알려줌 

void title(void); //게임시작화면 -
void reset(void); //게임판 초기화 -
void reset_main(void); //메인 게임판(main_org[][]를 초기화)
void reset_main_cpy(void); //copy 게임판(main_cpy[][]를 초기화)
void draw_map(void); //게임 전체 인터페이스를 표시 
void draw_main(void); //게임판을 그림 
void new_block(void); //새로운 블록을 하나 만듦 
void check_key(void); //키보드로 키를 입력받음 
void drop_block(void); //블록을 아래로 떨어트림 
int check_crush(int bx, int by, int rotation); //bx, by위치에 rotation회전값을 갖는 경우 충돌 판단 
void move_block(int dir); //dir방향으로 블록을 움직임 
void check_line(void); //줄이 가득찼는지를 판단하고 지움 
void check_level_up(void); //레벨목표가 달성되었는지를 판단하고 levelup시킴 -
void check_game_over(void); //게임오버인지 판단하고 게임오버를 진행 -
void pause(void);//게임을 일시정지시킴 -

void gotoxy(int x, int y) { //gotoxy함수 
    COORD pos = { 2 * x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //커서숨기는 함수에 사용
void setcursortype(CURSOR_TYPE c) { //커서숨기는 함수 
    CONSOLE_CURSOR_INFO CurInfo;

    switch (c) {
    case NOCURSOR:
        CurInfo.dwSize = 1;
        CurInfo.bVisible = FALSE;
        break;
    case SOLIDCURSOR:
        CurInfo.dwSize = 100;
        CurInfo.bVisible = TRUE;
        break;
    case NORMALCURSOR:
        CurInfo.dwSize = 20;
        CurInfo.bVisible = TRUE;
        break;
    }
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

int main() {
    int i;

    srand((unsigned)time(NULL)); //난수표생성 
    setcursortype(NOCURSOR); //커서 없앰 
    title(); //메인타이틀 호출 
    reset(); //게임판 리셋 

    while (1) {
        for (i = 0; i < 5; i++) { //블록이 한칸떨어지는동안 5번 키입력받을 수 있음 
            check_key(); //키입력확인 
            draw_main(); //화면을 그림 
            Sleep(speed); //게임속도조절 
            if (crush_on && check_crush(bx, by + 1, b_rotation) == false) Sleep(100);
            //블록이 충돌중인경우 추가로 이동및 회전할 시간을 갖음 
            if (space_key_on == 1) { //스페이스바를 누른경우(hard drop) 추가로 이동및 회전할수 없음 break; 
                space_key_on = 0;
                break;
            }
        }
        drop_block(); // 블록을 한칸 내림 
        check_level_up(); // 레벨업을 체크 
        check_game_over(); //게임오버를 체크 
        if (new_block_on == 1) new_block(); // 뉴 블럭 flag가 있는 경우 새로운 블럭 생성 
    }
}

void title(void) {
    int x = 5; //타이틀화면이 표시되는 x좌표 
    int y = 4; //타이틀화면이 표시되는 y좌표 
    int cnt; //타이틀 프레임을 세는 변수  

    gotoxy(x, y + 0); printf("■□□□■■■□□■■□□■■"); Sleep(100);
    gotoxy(x, y + 1); printf("■■■□  ■□□    ■■□□■"); Sleep(100);
    gotoxy(x, y + 2); printf("□□□■              □■  ■"); Sleep(100);
    gotoxy(x, y + 3); printf("■■□■■  □  ■  □□■□□"); Sleep(100);
    gotoxy(x, y + 4); printf("■■  ■□□□■■■□■■□□"); Sleep(100);
    gotoxy(x + 5, y + 2); printf("T E T R I S"); Sleep(100);
    gotoxy(x, y + 7); printf("Please Enter Any Key to Start..");
    gotoxy(x, y + 9); printf("  ↑   : Shift");
    gotoxy(x, y + 10); printf("←  → : Left / Right");
    gotoxy(x, y + 11); printf("  ↓   : Soft Drop");
    gotoxy(x, y + 12); printf(" SPACE : Hard Drop");
    gotoxy(x, y + 13); printf("   P   : Pause");
    gotoxy(x, y + 14); printf("  ESC  : Quit");
    gotoxy(x, y + 16); printf("BONUS FOR HARD DROPS / COMBOS");

    for (cnt = 0;; cnt++) { //cnt를 1씩 증가시키면서 계속 반복
        if (kbhit()) break; //키입력이 있으면 무한루프 종료 
    }

    while (kbhit()) getch(); //버퍼에 기록된 키값을 버림 

}

void reset(void) {


    level = 1; //각종변수 초기화 
    score = 0;
    level_goal = 1000;
    key = 0;
    crush_on = 0;
    cnt = 0;
    speed = 100;

    system("cls"); //화면지움 
    reset_main(); // main_org를 초기화 
    draw_map(); // 게임화면을 그림
    draw_main(); // 게임판을 그림 

    b_type_next = rand() % 7; //다음번에 나올 블록 종류를 랜덤하게 생성 
    new_block(); //새로운 블록을 하나 만듦  

}

void reset_main(void) { //게임판을 초기화  
    int i, j;

    for (i = 0; i < MAIN_Y; i++) { // 게임판을 0으로 초기화  
        for (j = 0; j < MAIN_X; j++) {
            main_org[i][j] = 0;
            main_cpy[i][j] = 100;
        }
    }

    for (j = 1; j < MAIN_X; j++) {  // 벽 만들기
        main_org[3][j] = CEILLING;
    }
    for (i = 1; i < MAIN_Y - 1; i++) { 
        main_org[i][0] = WALL;
        main_org[i][MAIN_X - 1] = WALL;
    }
    for (j = 0; j < MAIN_X; j++) { 
        main_org[MAIN_Y - 1][j] = WALL;
    }
}

void reset_main_cpy(void) { //main_cpy를 초기화 
    int i, j;

    for (i = 0; i < MAIN_Y; i++) {
        for (j = 0; j < MAIN_X; j++) {
            main_cpy[i][j] = 100;
        }
    }
}

void draw_map(void) { //게임 상태 표시를 나타냄 
    int y = 3;
    gotoxy(STATUS_X_ADJ, LEVEL = y); printf(" LEVEL : %5d", level);
    gotoxy(STATUS_X_ADJ, GOAL = y + 1); printf(" GOAL  : %5d", 10 - cnt);
    gotoxy(STATUS_X_ADJ, y + 2); printf("+-  N E X T  -+ ");
    gotoxy(STATUS_X_ADJ, y + 3); printf("|             | ");
    gotoxy(STATUS_X_ADJ, y + 4); printf("|             | ");
    gotoxy(STATUS_X_ADJ, y + 5); printf("|             | ");
    gotoxy(STATUS_X_ADJ, y + 6); printf("|             | ");
    gotoxy(STATUS_X_ADJ, y + 7); printf("+-- -  -  - --+ ");
    gotoxy(STATUS_X_ADJ, y + 8); printf(" YOUR SCORE :");
    gotoxy(STATUS_X_ADJ, SCORE = y + 9); printf("        %6d", score);
    gotoxy(STATUS_X_ADJ, y + 10); printf(" LAST SCORE :");
    gotoxy(STATUS_X_ADJ, y + 11); printf("        %6d", last_score);
    gotoxy(STATUS_X_ADJ, y + 12); printf(" BEST SCORE :");
    gotoxy(STATUS_X_ADJ, y + 13); printf("        %6d", best_score);
    gotoxy(STATUS_X_ADJ, y + 15); printf("  ↑   : Shift        SPACE : Hard Drop");
    gotoxy(STATUS_X_ADJ, y + 16); printf("←  → : Left / Right   P   : Pause");
    gotoxy(STATUS_X_ADJ, y + 17); printf("  ↓   : Soft Drop     ESC  : Quit");
}

void draw_main(void) { //게임판 그리기
    int i, j;

    for (j = 1; j < MAIN_X - 1; j++) { //천장 그리기
        if (main_org[3][j] == EMPTY) main_org[3][j] = CEILLING;
    }
    for (i = 0; i < MAIN_Y; i++) {
        for (j = 0; j < MAIN_X; j++) {
            if (main_cpy[i][j] != main_org[i][j]) { //cpy랑 비교해서 값이 달라진 부분 다시 그리기
                gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
                switch (main_org[i][j]) {
                case EMPTY:
                    printf("  ");
                    break;
                case CEILLING:
                    printf(". ");
                    break;
                case WALL:
                    printf("▩");
                    break;
                case INACTIVE_BLOCK:
                    printf("□");
                    break;
                case ACTIVE_BLOCK:
                    printf("■");
                    break;
                }
            }
        }
    }
    for (i = 0; i < MAIN_Y; i++) { //게임판을 그린 후 main_cpy에 복사  
        for (j = 0; j < MAIN_X; j++) {
            main_cpy[i][j] = main_org[i][j];
        }
    }
}

void new_block(void) { //새로운 블록 생성  
    int i, j;

    bx = (MAIN_X / 2) - 1; //블록 생성 위치x좌표
    by = 0;  //블록 생성위치 y좌표
    b_type = b_type_next; //다음블럭값
    b_type_next = rand() % 7; //다음 블럭을 만듦 
    b_rotation = 0;  //회전 0으로 초기화

    new_block_on = 0;

    for (i = 0; i < 4; i++) { //게임판 bx, by위치에 블럭생성  
        for (j = 0; j < 4; j++) {
            if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = ACTIVE_BLOCK;
        }
    }
    for (i = 1; i < 3; i++) { //다음블럭 표시
        for (j = 0; j < 4; j++) {
            if (blocks[b_type_next][0][i][j] == 1) {
                gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
                printf("■");
            }
            else {
                gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
                printf("  ");
            }
        }
    }
}

void check_key(void) {
    key = 0;

    if (kbhit()) {
        key = getch(); //키값을 받음
        if (key == 224) { //방향킨지 확인
            do { key = getch(); } while (key == 224);
            switch (key) {
            case LEFT: //왼쪽 방향키
                if (check_crush(bx - 1, by, b_rotation) == true) move_block(LEFT);
                break;
            case RIGHT: //오른쪽 방향키
                if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT);
                break;
            case DOWN: //아래쪽 방향키
                if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
                break;
            case UP: //위쪽 방향키 
                if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_block(UP);
                else if (crush_on == 1 && check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_block(100);//바닥에 닿은 경우 위쪽으로 한칸띄워서 회전이 가능한지 확인
            }
        }
        else {
            switch (key) {
            case SPACE: //스페이스바
                space_key_on = 1;
                while (crush_on == 0) { //바닥에 닿을때까지 이동시킴
                    drop_block();
                    score += level; // hard drop 보너스
                    gotoxy(STATUS_X_ADJ, SCORE); printf("        %6d", score);
                }
                break;
            case P:
            case p:
                pause(); //일시정지 
                break;
            case ESC: 
                system("cls"); //화면 지우기
                exit(0); //게임종료 
            }
        }
    }
    while (kbhit()) getch(); //키버퍼를 비우기
}

void drop_block(void) {
    int i, j;

    if (crush_on && check_crush(bx, by + 1, b_rotation) == true) crush_on = 0;
    if (crush_on && check_crush(bx, by + 1, b_rotation) == false) {
        for (i = 0; i < MAIN_Y; i++) { //현재 조작중인 블럭을 굳힘 
            for (j = 0; j < MAIN_X; j++) {
                if (main_org[i][j] == ACTIVE_BLOCK) main_org[i][j] = INACTIVE_BLOCK;
            }
        }
        crush_on = 0;
        check_line();
        new_block_on = 1; //새로운 블럭생성 flag 켬 
        return;
    }
    if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN); //밑이 비어있으면 밑으로 한칸 이동 
    if (check_crush(bx, by + 1, b_rotation) == false) crush_on++; //밑으로 이동이 안되면  crush flag를 켬
}

int check_crush(int bx, int by, int b_rotation) { //충돌이 있는지 검사 
    int i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) { //게임판과 블럭모양을 비교해서 겹치면 false를 리턴 
            if (blocks[b_type][b_rotation][i][j] == 1 && main_org[by + i][bx + j] > 0) return false;
        }
    }
    return true; //하나도 안겹치면 true리턴 
};

void move_block(int dir) { //블록을 이동시킴 
    int i, j;

    switch (dir) {
    case LEFT: //왼쪽방향 
        for (i = 0; i < 4; i++) { //현재좌표의 블럭을 지움 
            for (j = 0; j < 4; j++) {
                if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
            }
        }
        for (i = 0; i < 4; i++) { //왼쪽으로 한칸가서 active block을 찍음 
            for (j = 0; j < 4; j++) {
                if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j - 1] = ACTIVE_BLOCK;
            }
        }
        bx--; //좌표값 이동 
        break;

    case RIGHT:    //오른쪽 방향. 왼쪽방향이랑 같은 원리로 동작 
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
            }
        }
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j + 1] = ACTIVE_BLOCK;
            }
        }
        bx++;
        break;

    case DOWN:    //아래쪽 방향. 왼쪽방향이랑 같은 원리로 동작
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
            }
        }
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i + 1][bx + j] = ACTIVE_BLOCK;
            }
        }
        by++;
        break;

    case UP: //키보드 위쪽 눌렀을때 회전시킴. 
        for (i = 0; i < 4; i++) { //현재좌표의 블럭을 지움  
            for (j = 0; j < 4; j++) {
                if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
            }
        }
        b_rotation = (b_rotation + 1) % 4; //회전값을 1증가시킴(3에서 4가 되는 경우는 0으로 되돌림) 
        for (i = 0; i < 4; i++) { //회전된 블록을 찍음 
            for (j = 0; j < 4; j++) {
                if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = ACTIVE_BLOCK;
            }
        }
        break;

    case 100: //블록이 바닥, 혹은 다른 블록과 닿은 상태에서 한칸위로 올려 회전이 가능한 경우 
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
            }
        }
        b_rotation = (b_rotation + 1) % 4;
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i - 1][bx + j] = ACTIVE_BLOCK;
            }
        }
        by--;
        break;
    }
}

void check_line(void) {
    int i, j, k, l;

    int    block_amount; //한줄의 블록갯수 저장
    int combo = 0; //콤보갯수 저장

    for (i = MAIN_Y - 2; i > 3;) {
        block_amount = 0; //블록갯수 저장 변수 초기화 
        for (j = 1; j < MAIN_X - 1; j++) {
            if (main_org[i][j] > 0) block_amount++;
        }
        if (block_amount == MAIN_X - 2) { //블록이 가득 찬 경우 
            if (level_up_on == 0) {
                score += 100 * level; //점수추가 
                cnt++; //지운 줄 갯수 카운트 증가 
                combo++; //콤보수 증가  
            }
            for (k = i; k > 1; k--) { //윗줄을 한칸씩 모두 내림(윗줄이 천장이 아닌 경우에만) 
                for (l = 1; l < MAIN_X - 1; l++) {
                    if (main_org[k - 1][l] != CEILLING) main_org[k][l] = main_org[k - 1][l];
                    if (main_org[k - 1][l] == CEILLING) main_org[k][l] = EMPTY;
                    //윗줄이 천장인 경우에는 천장을 한칸 내리면 안되니까 빈칸을 넣음 
                }
            }
        }
        else i--;
    }
    if (combo) {
        if (combo > 1) { //2콤보이상인 경우 경우 보너스및 메세지를 게임판에 띄웠다가 지움 
            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 1, MAIN_Y_ADJ + by - 2); printf("%d COMBO!", combo);
            Sleep(500);
            score += (combo * level * 100);
            reset_main_cpy();
        }
        gotoxy(STATUS_X_ADJ, GOAL); printf(" GOAL  : %5d", (cnt <= 10) ? 10 - cnt : 0);
        gotoxy(STATUS_X_ADJ, SCORE); printf("        %6d", score);
    }
}

void check_level_up(void) {
    int i, j;

    if (cnt >= 10) { //레벨별로 10줄씩 없애야함. 10줄이상 없앤 경우 
        draw_main();
        level_up_on = 1; //레벨업 flag를 띄움 
        level += 1; //레벨을 1 올림 
        cnt = 0; //지운 줄수 초기화   

        for (i = 0; i < 4; i++) {
            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
            printf("             ");
            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
            printf("             ");
            Sleep(200);

            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
            printf("☆LEVEL UP!☆");
            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
            printf("☆SPEED UP!☆");
            Sleep(200);
        }
        reset_main_cpy(); //텍스트를 지우기 위해 main_cpy을 초기화.

        for (i = MAIN_Y - 2; i > MAIN_Y - 2 - (level - 1); i--) { //레벨업보상으로 각 레벨-1의 수만큼 아랫쪽 줄을 지워줌 
            for (j = 1; j < MAIN_X - 1; j++) {
                main_org[i][j] = INACTIVE_BLOCK; // 줄을 블록으로 모두 채우고 
                gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
                printf("★");
                Sleep(20);
            }
        }
        Sleep(100);
        check_line();    
        switch (level) { //레벨별로 속도 조절
        case 2:
            speed = 50;
            break;
        case 3:
            speed = 35;
            break;
        case 4:
            speed = 20;
            break;
        case 5:
            speed = 15;
            break;
        case 6:
            speed = 10;
            break;
        case 7:
            speed = 5;
            break;
        case 8:
            speed = 2;
            break;
        case 9:
            speed = 1;
            break;
        case 10:
            speed = 0;
            break;
        }
        level_up_on = 0; //레벨업 flag꺼줌

        gotoxy(STATUS_X_ADJ, LEVEL); printf(" LEVEL : %5d", level); //레벨표시 
        gotoxy(STATUS_X_ADJ, GOAL); printf(" GOAL  : %5d", 10 - cnt); // 레벨목표 표시 

    }
}

void check_game_over(void) {
    int i;

    int x = 5;
    int y = 5;

    for (i = 1; i < MAIN_X - 2; i++) {
        if (main_org[3][i] > 0) { //천장(위에서 세번째 줄)에 inactive가 생성되면 게임 오버 
            gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤"); //게임오버 메세지 
            gotoxy(x, y + 1); printf("▤                              ▤");
            gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");
            gotoxy(x, y + 3); printf("▤  |  G A M E  O V E R..   |   ▤");
            gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");
            gotoxy(x, y + 5); printf("▤   YOUR SCORE: %6d         ▤", score);
            gotoxy(x, y + 6); printf("▤                              ▤");
            gotoxy(x, y + 7); printf("▤  Press any key to restart..  ▤");
            gotoxy(x, y + 8); printf("▤                              ▤");
            gotoxy(x, y + 9); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
            last_score = score; //게임점수를 옮김 

            if (score > best_score) { //최고기록 갱신시 
                best_score = score;

                gotoxy(x, y + 6); printf("▤  ★★★ BEST SCORE! ★★★   ▤  ");
                printf("%d", score);
            }
            Sleep(1000);
            while (kbhit()) getch();
            key = getch();
            reset();
        }
    }
}

void pause(void) { //게임 일시정지 함수 
    int i, j;

    int x = 5;
    int y = 5;

    for (i = 1; i < MAIN_X - 2; i++) { //게임 일시정지 메세지 
        gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
        gotoxy(x, y + 1); printf("▤                              ▤");
        gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");
        gotoxy(x, y + 3); printf("▤  |       P A U S E       |   ▤");
        gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");
        gotoxy(x, y + 5); printf("▤  Press any key to resume..   ▤");
        gotoxy(x, y + 6); printf("▤                              ▤");
        gotoxy(x, y + 7); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
    }
    getch(); //키입력시까지 대기 

    system("cls"); //화면 지우고 새로 그림 
    reset_main_cpy();
    draw_main();
    draw_map();

    for (i = 1; i < 3; i++) { // 다음블록 그림 
        for (j = 0; j < 4; j++) {
            if (blocks[b_type_next][0][i][j] == 1) {
                gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
                printf("■");
            }
            else {
                gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
                printf("  ");
            }
        }
    }
} //끝!  