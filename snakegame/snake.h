#include <time.h>
#include <list>
#include <stdlib.h>
#include <ncursesw/curses.h>
using namespace std;

class Snake
{
    public:
        int x,y; //뱀 머리의 좌표

        int timeForItem;
        int timeForGate;
        static bool stageClear[4];
        int snakeLength;    //뱀의 몸길이
        int countGrowth;    //총 먹은 growth item 갯수
        int countPoison;    //총 먹은 poison item 갯수
        int countUsingGate; //gate를 사용한 횟수
        int showTime = 0;

        char direction;

        list<pair<int, int>> items;
        list<pair<int, int>> gates; //2개의 gate의 좌표를 저장하는 list
        list<pair<int, int>> snake;

        Snake();
        bool move();
        void drawMap(int stage);
        void isPlaying(int stage);
        int isCollision();
        void endGame(int stage);
        void makeItem();
        void makeGate();
        void updateScoreBoard();
        bool updateMissionBoard(int stage);
        bool getDir();//혜정수정v1
};
