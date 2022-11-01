#include <iostream>
#include <string>
#include <unistd.h>
#include "game.h"
#include "snake.h"
#include <ncursesw/curses.h>
using namespace std;

//혜정수정 v1
//혜정수정 version2
char init(){
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, true);
    WINDOW * initwin;
    start_color();
    int b = 1,letter = 2, white = 3;
    init_pair(b, COLOR_BLACK, COLOR_BLACK);
    init_pair(white, COLOR_WHITE, COLOR_WHITE);
    init_pair(letter, COLOR_BLACK, COLOR_WHITE);
    border('|','|','_','_','_','_','_','_');

    initwin = newwin(30,50,5,70);
    bkgd(COLOR_PAIR(b));
    attron(COLOR_PAIR(letter));
    mvprintw(3,70, "   S   N   A   K   E     G   A   M   E   ");
    attroff(COLOR_PAIR(letter));
    wbkgd(initwin, COLOR_PAIR(white));
    box(initwin, '|', '-');

    wattron(initwin, COLOR_PAIR(letter));
    int i = 1;
    mvwprintw(initwin,i,1,"______________________________________");
    mvwprintw(initwin,i+2,1,"CONTROL WITH DIRECTION KEY");
    mvwprintw(initwin,i+4,1," > UP, DONW, RIGHT, LEFT ");
    mvwprintw(initwin,i+6,1," > W,A,S,D ");
    mvwprintw(initwin,i+8,1,"______________________________________");

    mvwprintw(initwin,i+10,1,"CLEAR MISSIONS AND COMPLETE ALL STAGES");
    mvwprintw(initwin,i+12,1,"NUM IS STAGE LEVEL");
    mvwprintw(initwin,i+14,1,"EXIT :");
    mvwprintw(initwin,i+16,1,"PRESS Q  OR  PRESS ESC");
    mvwprintw(initwin,i+18,1,"______________________________________");
    if(Snake::stageClear[0]) {mvwprintw(initwin,i+20,1,"PRESS 1 TO START STAGE (CLEAR)");}
    else {mvwprintw(initwin,i+20,1,"PRESS 1 TO START STAGE");}

    if(Snake::stageClear[1]) {mvwprintw(initwin,i+21,1,"PRESS 2 TO START STAGE (CLEAR)");}
    else {mvwprintw(initwin,i+21,1,"PRESS 2 TO START STAGE");}

    if(Snake::stageClear[2]) {mvwprintw(initwin,i+22,1,"PRESS 3 TO START STAGE (CLEAR)");}
    else {mvwprintw(initwin,i+22,1,"PRESS 3 TO START STAGE");}

    if(Snake::stageClear[3]) {mvwprintw(initwin,i+23,1,"PRESS 4 TO START STAGE (CLEAR)");}
    else {mvwprintw(initwin,i+23,1,"PRESS 4 TO START STAGE");}
    wattroff(initwin, COLOR_PAIR(letter));

    refresh();
    wrefresh(initwin);
    
    while(1){
        char key;
        key = getch();
        if(key == '1' || key == '2' || key == '3' || key == '4' || key == 'q' )
            return key;
    }
    clear();
}

int main()
{
    char playORquit = init();
    while(playORquit != 'q')
    {
    endwin();
    Snake snake;
    switch(playORquit){
        case '1':
            snake.isPlaying(1);
            break;
        case '2':
            snake.isPlaying(2);
            break;
        case '3':
            snake.isPlaying(3);
            break;
        case '4' :
            snake.isPlaying(4);
            break;
        case 'q' :
            endwin();
            break;
    }
    playORquit = init();
  }
  endwin();
    return 0;
}
