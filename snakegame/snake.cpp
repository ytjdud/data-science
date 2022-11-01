#include <iostream>
#include <string.h>
#include <time.h>
#include <queue>
#include <ctime>
#include <cstdlib>
#include "game.h"
#include "snake.h"
#include <unistd.h>
#include <clocale>
#include <ncursesw/curses.h>
using namespace std;

static bool missionComplete = false; //목표달성 여부
bool Snake::stageClear[4] = {false, false, false, false}; //각 스테이지 클리어 여부
char map[21][38]; //Stage 저장 용 char타입 2차원 배열

//snake 생성자
Snake::Snake()
{
    initscr();
    noecho();
    curs_set(0);
    cbreak();
    nodelay(stdscr, true);
    keypad(stdscr, true);
    x = 15;
    y = 10;

    timeForGate = 0;
    timeForItem = 0;

    snakeLength = 3;
    countGrowth = 0;
    countPoison = 0;
    countUsingGate = 0;

    direction = 'l'; //왼쪽을 기준으로 시작

    snake.clear();
    for(int i = 0 ; i < snakeLength ; i++) {
        snake.push_back({y,x});
    }

    items.clear();
    gates.clear();
}

//게임 종료
//혜정수정 version2
void Snake::endGame(int stage){
    clear();
    initscr();
    start_color();
    int blue = 1,bletter = 2, white = 3, red = 4, rletter = 5;
    init_pair(blue, COLOR_BLUE, COLOR_BLUE);
    init_pair(white, COLOR_WHITE, COLOR_WHITE);
    init_pair(bletter, COLOR_BLUE, COLOR_WHITE);
    init_pair(rletter, COLOR_RED, COLOR_WHITE);
    init_pair(red, COLOR_RED, COLOR_RED);
    border('|','|','_','_','_','_','_','_');
    WINDOW * endgamewin;
    endgamewin = newwin(30,50,5,70);

    if(missionComplete){
        Snake::stageClear[stage-1] = true;
        bkgd(COLOR_PAIR(blue));
        attron(COLOR_PAIR(bletter));
        mvprintw(3,70, "   S   N   A   K   E     G   A   M   E   ");
        attroff(COLOR_PAIR(bletter));
        wbkgd(endgamewin, COLOR_PAIR(white));
        box(endgamewin, '|', '-');
        wattron(endgamewin, COLOR_PAIR(bletter));
        mvwprintw(endgamewin,5,1,"    M I S S O N  C O M P L E T E  ");
        mvwprintw(endgamewin,8,1,"    YOUR SCORE IS  ");
        mvwprintw(endgamewin,11,1,("    " + to_string(countUsingGate * 200 + snakeLength * 150 + countGrowth * 100 - countPoison * 50 )).c_str());
    }else{
        bkgd(COLOR_PAIR(red));
        attron(COLOR_PAIR(rletter));
        mvprintw(3,70, "S   N   A   K   E     G   A   M   E");
        attroff(COLOR_PAIR(rletter));
        wbkgd(endgamewin, COLOR_PAIR(white));
        box(endgamewin, '|', '-');
        wattron(endgamewin, COLOR_PAIR(rletter));
        mvwprintw(endgamewin,5,1,"  M I S S O N  F A I L  ");
    }
    refresh();
    wrefresh(endgamewin);
    sleep(2);
    endwin();
}

//Snake game을 시작
void Snake::isPlaying(int stage)
{
    switch(stage){
        case 1:
            memcpy(map,stage1,sizeof(map));
            break;
        case 2:
            memcpy(map,stage2,sizeof(map));
            break;
        case 3:
            memcpy(map,stage3,sizeof(map));
            break;
        case 4:
            memcpy(map,stage4,sizeof(map));
            break;
    }
    missionComplete = false;
    drawMap(stage);
    while(move())
    {
        drawMap(stage);
        updateScoreBoard();
        missionComplete = updateMissionBoard(stage);
        if(missionComplete) break;
    }
    endGame(stage);
}


// 맵을 그리
//혜정수정 version2
void Snake::drawMap(int stage)
{
    setlocale(LC_ALL, "");
    initscr();
    clear();

    start_color();
    int blue = 0,letter=1,white=2;
    init_pair(blue, COLOR_BLUE, COLOR_BLUE);
    init_pair(letter, COLOR_WHITE, COLOR_BLUE);
    init_pair(white, COLOR_WHITE, COLOR_WHITE);

    bkgd(COLOR_PAIR(blue));

    for(int i = 0 ; i < 21 ; i++)
    {
        for(int j = 0 ; j < 38 ; j++)
        {
            char temp = map[i][j];
            if (temp == '1') //wall
              {printw("🔲");}
            //   {printw("🔳");}
            else if (temp == '0')
              {printw("◾");}
            else if(temp == '2') //immune wall
              {printw("⬜");}
            else if(temp == '3')
              {printw("🔘");}
            else if(temp == '4')
              {printw("💠");}
            else if(temp == '5')
              {printw("\u272A ");} //growth item
            else if(temp == '6')
              {printw("\u2716 ");} //poision item
            else if(temp == '7')
              {printw("🔴");}
            else {printw("  ");}
        }
        printw("\n");
    }
    refresh();
}

int Snake::isCollision()
{
    list<pair<int,int>>::iterator i;
    for(i = snake.begin() ; i != snake.end(); i++)
    {
        if(i->second == x && i->first == y) return 0;
    }
}


//아이템 생성 함수
//혜정수정
void Snake::makeItem()
{
  // 전에 있던 아이템을 지우기
  list<pair<int, int>>::iterator i;
  for(i=items.begin(); i!=items.end(); i++)
  {
    map[i->first][i->second] = '0';
  }
  items.clear();

 srand(time(NULL)); 
 
  int numOfItem = rand() % 3+1; // 이번 턴에 생성할 아이템 갯수 1~3개
  int randXY[6]; int PorG[3]; int temp=0;


  //randXY에 랜덤한 좌표 6개 대입
  for(int k=0; k<6; k+=2)
  {
    randXY[k] = rand() % 19 + 1;
    randXY[k+1] = rand() % 38 + 1;
    while(map[randXY[k]][randXY[k+1]] != '0'){
        randXY[k] = rand() % 19 + 1;
        randXY[k+1] = rand() % 38 + 1;
    }
  }

  // 짝수면 성장 홀수면 독
  for(int k=0; k<3; k++)
  {
    PorG[k] = rand() % 9;
  }

  for(int k=0; k<numOfItem; k++)
  {
    items.push_back({randXY[temp], randXY[temp+1]});
    temp += 2;
  }

  temp=0; //temp다시 0으로 초기화

  for(i=items.begin(); i!=items.end(); i++)
  {
    if(PorG[temp]%2==0)
      map[i->first][i->second] = '5';
    else
      map[i->first][i->second] = '6';
    temp++;
  }
}

//Gate를 생성
void Snake::makeGate()
{
    //기존의 게이트 다시 벽으로
    srand(time(NULL));
    if(gates.size() != 0)
    {
        list<pair<int,int>>::iterator i;
        for(i = gates.begin() ; i != gates.end(); i++)
          {map[i->first][i->second] = '1';}
    }

    gates.clear();

    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;


    do{ 

        {
            x1 = rand() % 38;
            x2 = rand() % 38;
            y1 = rand() % 21;
            y2 = rand() % 21;
        }
    }while((x1 == x2 && y1 == y2) 
    || (map[y1][x1] != '1' || map[y2][x2] != '1'));
    //gate의 좌표를 gates리스트에 넣음
    gates.push_back({y1,x1}); gates.push_back({y2,x2});

    list<pair<int,int>>::iterator i;
    for(i = gates.begin() ; i != gates.end(); i++){
        map[i->first][i->second] = '7';
    }
}

//혜정수정 version2
void Snake::updateScoreBoard()
{
    start_color();
    int red = 1, white = 2, blue = 3, wb = 4, bw=5;
    init_pair(wb,COLOR_WHITE,COLOR_BLUE);
    init_pair(bw,COLOR_BLUE,COLOR_WHITE);
    init_pair(blue,COLOR_BLUE,COLOR_BLUE);
    init_pair(white,COLOR_WHITE,COLOR_WHITE);
    WINDOW *scoreWin;
    WINDOW *upbg;

    upbg = newwin(30,80,0,80);
    scoreWin = newwin(10,38,0,80); // 수정함

    // mvwin(upbg, 0, 50);
    mvwin(scoreWin, 0, 80);

    wbkgd(scoreWin,COLOR_PAIR(white));
    wattron(scoreWin,COLOR_PAIR(bw));
    mvwprintw(scoreWin,2,12,"<< SCORE BOARD >>");

    mvwprintw(scoreWin,4,2,("SNAKE STATE :  curr : "+ to_string(snakeLength) + " / max : 15").c_str());

    mvwprintw(scoreWin,5,2,("Growth Item:  "+ to_string(countGrowth)).c_str());

    mvwprintw(scoreWin,6,2,("Poison Item:  "+ to_string(countPoison)).c_str());

    mvwprintw(scoreWin,7,2,("Using Gates:  " + to_string(countUsingGate)).c_str());
    // mvwprintw(scoreWin,7,16,to_string(countUsingGate).c_str());

    // wborder(scoreWin,'|','|','_','_',' ',' ','|','|');
    touchwin(upbg);
    wrefresh(scoreWin);
}

// 수정함
//혜정version4
bool Snake::updateMissionBoard(int stage)
{
    start_color();
    int red = 1, white = 2, blue = 3, wb = 4, bw=5;
    init_pair(wb,COLOR_WHITE,COLOR_BLUE);
    init_pair(bw,COLOR_BLUE,COLOR_WHITE);
    init_pair(blue,COLOR_BLUE,COLOR_BLUE);
    init_pair(white,COLOR_WHITE,COLOR_WHITE);

    WINDOW *upbg;
    WINDOW *missionWin;
    missionWin = newwin(10,38,11,80); // 수정함
    upbg = newwin(30,80,11,80);

    mvwin(missionWin, 11, 80);

    wbkgd(missionWin,COLOR_PAIR(blue));
    wattron(missionWin,COLOR_PAIR(wb));  
    mvwprintw(missionWin,2,12,"<< MissionBoard >>");
    
    int gi = stage+2;
    mvwprintw(missionWin,4,2,("Growth Item: " + to_string(gi)).c_str());
    if(countGrowth >= gi) {
        mvwprintw(missionWin,4,18,"(V)");
    }else{
        mvwprintw(missionWin,4,18,"( )");
    }
    
    int ug = stage+1;
    mvwprintw(missionWin,5,2,("Using Gates: "+to_string(ug)).c_str());
    if(countUsingGate >= ug) {
        mvwprintw(missionWin,5,18,"(V)");
    }else {
        mvwprintw(missionWin,5,18,"( )");
    }
    
    // wborder(missionWin,'|','|','_','_',' ',' ','|','|');
    touchwin(upbg);
    wrefresh(missionWin);
    
    if(countGrowth >= gi && countUsingGate >= ug) {
        return true;
    }
    return false;
    
}

//방향함수
bool Snake::getDir(){
    int getd;
    getd = getch(); // 방향키 입력
    switch(getd) // 반대방향 게임종료
    {
      case KEY_UP:
      case 'w':
        if(direction != 'd') direction = 'u';
        else return false;
        break;
      case KEY_DOWN:
      case 's':
        if(direction != 'u')direction = 'd';
        else return false;
        break;
      case KEY_LEFT:
      case 'a':
        if(direction != 'r')direction = 'l';
        else return false;
        break;
      case KEY_RIGHT:
      case 'd':
        if(direction != 'l')direction = 'r';
        else return false;
        break;
      case 'q':
      case 27:
          return 0;
    }
    return 1;
}
bool Snake::move()
{
    if(!getDir()) return 0;

    switch(direction)
    {
      case 'u':
          y--;
          break;
      case 'd':
          y++;
          break;
      case 'l':
          x--;
          break;
      case 'r':
          x++;
          break;
    }

    if(timeForItem % 50 == 0) {makeItem();} //아이템 생성

    if(isCollision() == 0) {return false;} // tail과 충돌

    snake.push_front({y,x}); //뱀머리 좌표를 추가
    map[snake.back().first][snake.back().second] ='0'; // 꼬리지우기
    snake.pop_back(); // 꼬리를 버림


    if(timeForGate % 100 == 0)
    {
        bool goThrow = false;
        list<pair<int,int>>::iterator li;
        for(li = gates.begin() ; li != gates.end() ; li++)
        {
            if(map[li->first + 1][li->second] == '4' || map[li->first - 1][li->second] == '4'
                || map[li->first][li->second + 1] == '4' || map[li->first][li->second - 1] == '4')
              {goThrow = true; break;}
        }

        if(goThrow == false) {makeGate();}
    }

    list<pair<int,int>>::iterator i;
    for(i = snake.begin() ; i != snake.end(); i++)
    {
        if(map[i->first][i->second] == '1' || map[i->first][i->second] == '2') return false; //충돌 처리
        else if(i == snake.begin())
        //머리부분일때(Item을 먹, gate에 입장)
            {
              if(map[i->first][i->second] == '5' && snakeLength < 15) // 성장 아이템, maxlength를 15로 지정
              {
                pair<int,int> aa = snake.back();
                switch(direction){
                    case 'r':
                        snake.push_back({aa.first,aa.second + 1});
                        break;
                    case 'd':
                        snake.push_back({aa.first + 1 , aa.second});
                        break;
                    case 'l' :
                        snake.push_back({aa.first,aa.second - 1});
                        break;
                    case 'u' :
                        snake.push_back({aa.first - 1 , aa.second});
                        break;
                }
                  map[i->first][i->second] = '3';
                  snakeLength++; countGrowth++;
              }
              else if(map[i->first][i->second] == '6') //독사과아이템 // 3차 수정
              { 
                  if(snakeLength < 4) 
                    return false; //뱀길이가 2이하이면 게임오버
                  
                  map[snake.back().first][snake.back().second] ='0';
                  snake.pop_back();
                  map[i->first][i->second] = '3';
                  snakeLength--; countPoison++;
              }
              else if(map[i->first][i->second] == '7')
              {
                    int x1 ,y1,x2,y2;
                    for(list<pair<int,int>>::iterator itr = gates.begin(); itr != gates.end(); ++itr)
                    {
                        if(itr == gates.begin())
                          {y1 = itr->first; x1 = itr->second;}
                        else
                          {y2 = itr->first; x2 = itr->second;}
                    }
                    if(i->first == y1 && i->second == x1)
                    {
                        //y1,x1에 head가 쾅 head의 direction을 바꾸기
                        switch(direction)
                        {
                            case 'd' :
                                if(map[y2 + 1][x2] == '0' || map[y2 + 1][x2] == '5' || map[y2 + 1][x2] == '6') direction = 'd';
                                else if(map[y2][x2 - 1] == '0' || map[y2][x2 - 1] == '5' || map[y2][x2 - 1] == '6') direction = 'l';
                                else if(map[y2 - 1][x2] == '0' || map[y2 - 1][x2] == '5' || map[y2 - 1][x2] == '6') direction = 'u';
                                else if(map[y2][x2 + 1] == '0' || map[y2][x2 + 1] == '5' || map[y2][x2 + 1] == '6') direction = 'r';
                                else return false;
                                break;
                            case 'l' :
                                if(map[y2][x2 - 1] == '0' || map[y2][x2 - 1] == '5' || map[y2][x2 - 1] == '6') direction = 'l';
                                else if(map[y2 - 1][x2] == '0' || map[y2 - 1][x2] == '5' || map[y2 - 1][x2] == '6') direction = 'u';
                                else if(map[y2][x2 + 1] == '0' || map[y2][x2 + 1] == '5' || map[y2][x2 + 1] == '6') direction = 'r';
                                else if(map[y2 + 1][x2] == '0' || map[y2 + 1][x2] == '5' || map[y2 + 1][x2] == '6') direction = 'd';
                                else return false;
                                break;
                            case 'r':
                                if(map[y2][x2 + 1] == '0' || map[y2][x2 + 1] == '5' || map[y2][x2 + 1] == '6') direction = 'r';
                                else if(map[y2 + 1][x2] == '0' || map[y2 + 1][x2] == '5' || map[y2 + 1][x2] == '6') direction = 'd';
                                else if(map[y2][x2 - 1] == '0' || map[y2][x2 - 1] == '5' || map[y2][x2 - 1] == '6') direction = 'l';
                                else if(map[y2 - 1][x2] == '0' || map[y2 - 1][x2] == '5' || map[y2 - 1][x2] == '6') direction = 'u';
                                else return false;
                                break;
                            case 'u' :
                                if(map[y2 - 1][x2] == '0' || map[y2 - 1][x2] == '5' || map[y2 - 1][x2] == '6') direction = 'u';
                                else if(map[y2][x2 + 1] == '0' || map[y2][x2 + 1] == '5' || map[y2][x2 + 1] == '6') direction = 'r';
                                else if(map[y2 + 1][x2] == '0' || map[y2 + 1][x2] == '5' || map[y2 + 1][x2] == '6') direction = 'd';
                                else if(map[y2][x2 - 1] == '0' || map[y2][x2 - 1] == '5' || map[y2][x2 - 1] == '6') direction = 'l';
                                else return false;
                                break;
                            default :
                                break;
                        }
                        switch(direction){
                            case 'l':
                                y = y2;
                                x = x2 - 1;
                                break;
                            case 'r':
                                y = y2;
                                x = x2 + 1;
                                break;
                            case 'd' :
                                y = y2 + 1;
                                x = x2;
                                break;
                            case 'u' :
                                y = y2 - 1;
                                x = x2;
                                break;
                            default:
                                break;
                        }
                            i->first = y;
                            i->second = x;
                            map[i->first][i->second] = '3';
                    }
                    else
                    {
                        //y2,x2에 head가 쾅
                        switch(direction)
                        {
                            case 'r' :
                                if(map[y1][x1 + 1] == '0' || map[y1][x1 + 1] == '5' || map[y1][x1 + 1] == '6') direction = 'r';
                                else if(map[y1 + 1][x1] == '0' || map[y1 + 1][x1] == '5' || map[y1 + 1][x1] == '6') direction = 'd';
                                else if(map[y1][x1 - 1] == '0' || map[y1][x1 - 1] == '5' || map[y1][x1 - 1] == '6') direction = 'l';
                                else if(map[y1 - 1][x1] == '0' || map[y1 - 1][x1] == '5' || map[y1 - 1][x1] == '6') direction = 'u';
                                else return false;
                                break;
                            case 'd' :
                                if(map[y1 + 1][x1] == '0' || map[y1 + 1][x1] == '5' || map[y1 + 1][x1] == '6') direction = 'd';
                                else if(map[y1][x1 - 1] == '0' || map[y1][x1 - 1] == '5' || map[y1][x1 - 1] == '6') direction = 'l';
                                else if(map[y1 - 1][x1] == '0' || map[y1 - 1][x1] == '5' || map[y1 - 1][x1] == '6') direction = 'u';
                                else if(map[y1][x1 + 1] == '0' || map[y1][x1 + 1] == '5' || map[y1][x1 + 1] == '6') direction = 'r';
                                else return false;
                                break;
                            case 'u':
                                if(map[y1 - 1][x1] == '0' || map[y1 - 1][x1] == '5' || map[y1 - 1][x1] == '6') direction = 'u';
                                else if(map[y1][x1 + 1] == '0' || map[y1][x1 + 1] == '5' || map[y1][x1 + 1] == '6') direction = 'r';
                                else if(map[y1 + 1][x1] == '0' || map[y1 + 1][x1] == '5' || map[y1 + 1][x1] == '6') direction = 'd';
                                else if(map[y1][x1 - 1] == '0' || map[y1][x1 - 1] == '5' || map[y1][x1 - 1] == '6') direction = 'l';
                                else return false;
                                break;
                            case 'l' :
                                if(map[y1][x1 - 1] == '0' || map[y1][x1 - 1] == '5' || map[y1][x1 - 1] == '6') direction = 'l';
                                else if(map[y1 - 1][x1] == '0' || map[y1 - 1][x1] == '5' || map[y1 - 1][x1] == '6') direction = 'u';
                                else if(map[y1][x1 + 1] == '0' || map[y1][x1 + 1] == '5' || map[y1][x1 + 1] == '6') direction = 'r';
                                else if(map[y1 + 1][x1] == '0' || map[y1 + 1][x1] == '5' || map[y1 + 1][x1] == '6') direction = 'd';
                                else return false;
                                break;
                            default :
                                break;
                        }
                        switch(direction)
                        {
                            case 'l':
                                y = y1;
                                x = x1 - 1;
                                break;
                            case 'r':
                                y = y1;
                                x = x1 + 1;
                                break;
                            case 'd' :
                                y = y1 + 1;
                                x = x1;
                                break;
                            case 'u' :
                                y = y1 - 1;
                                x = x1;
                                break;
                            default:
                                break;
                        }
                        i->first = y;
                        i->second = x;
                        map[i->first][i->second] = '3';
                    }
                  countUsingGate++;
                }
                else {map[i->first][i->second] = '3';} //아무것도 아닌
            }
        else
        {
            if(map[i->first][i->second] == '7') map[i->first][i->second] = '7';
            else map[i->first][i->second] = '4';
        }
    }
    usleep(110000);
    timeForItem ++;
    timeForGate++;

    return true;
}
