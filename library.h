#ifndef LIBRARY_H_INCLUDED
#define LIBRARY_H_INCLUDED

#include <bits/stdc++.h>
using namespace std;
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

//size of game map
#define mapW 10
#define mapH 10
//the position of the left top corner of the map on the screen
#define mapX 50
#define mapY 3
#define cEmpty '-'
#define cNoChar '$'
#define cUser '#'
#define cBot 'O'
#define maxNodes 15
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

struct Coord
{
    int x, y;
};

class Map
{
    public:
    char prevMap[mapH][mapW], curMap[mapH][mapW];
    /*  prevMap - previous map image
        curMap - current map image*/
    Map(char t);
    void draw();
};

class Snake
{
    public:
    //nodes[0] - head
    //nodes[len - 1] - tail
    Coord nodes[maxNodes];
    int dir, len;
    char who;

    //public:
    Snake(int sy, int sx, char wh);
    void step();
    void turn(int x);
    void movePointToDir(Coord *c);
    char fwdChar(char (&m)[mapH][mapW]);
    void draw(int col, char (&m)[mapH][mapW]);
    int getTailX();
    int getTailY();
};

extern HANDLE h;
void initConsole();
void setCursor(int y, int x);
void setChar(char t, int y, int x);
int getRandom(int lower, int upper);//from [lower, upper]
void dumpToFile(char (&m)[mapH][mapW], char fileName[]);

#endif // LIBRARY_H_INCLUDED
