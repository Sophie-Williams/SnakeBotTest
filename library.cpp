#include "library.h"
HANDLE h;
void initConsole()
{
    h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CCI;
    CCI.bVisible = 0;
    CCI.dwSize = 1;
    SetConsoleCursorInfo(h, &CCI);
}

void setCursor(int y, int x)
{
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(h, c);
}

void setChar(char t, int y, int x)
{
    setCursor(y, x);
    printf("%c", t);
}

int getRandom(int lower, int upper)
{
    return rand()%(upper + 1 - lower) + lower;
}


Map::Map(char t)
{
        for (int y = 0; y < mapH; y++)
            for (int x = 0; x < mapW; x++)
            {
                prevMap[y][x] = '&';
                curMap[y][x] = t;
            }
}

void Map::draw()
{
        for (int y = 0; y < mapH; y++)
            for (int x = 0; x < mapW; x++)
                if (prevMap[y][x] != curMap[y][x])
                {
                    prevMap[y][x] = curMap[y][x];
                    setChar(curMap[y][x], y + mapY, x + mapX);
                }
}

void dumpToFile(char (&m)[mapH][mapW], char fileName[])
{
    ofstream out(fileName);
    for (int y = 0; y < mapH; y++)
    {
        for (int x = 0; x < mapW; x++)
        {
            out << m[y][x];
        }
        out << "\n";
    }
}

Snake::Snake(int sy, int sx, char wh)
{
    len = 5;
    dir = LEFT;
    who = wh;
    nodes[0].y = sy;
    nodes[0].x = sx;
    for (int i = 1; i < 5; i++)
    {
        nodes[i].x = nodes[i - 1].x + 1;
        nodes[i].y = nodes[i - 1].y;
    }
}

void Snake::movePointToDir(Coord *c)
{
    if (dir == UP)
    {
        c->y--;
    } else if (dir == DOWN)
    {
        c->y++;
    } else if (dir == LEFT)
    {
        c->x--;
    } else if (dir == RIGHT)
    {
        c->x++;
    }
}

void Snake::step()
{
    for (int i = len - 1; i >= 1; i--)
    {
        nodes[i].x = nodes[i - 1].x;
        nodes[i].y = nodes[i - 1].y;
    }
    movePointToDir(&nodes[0]);
}

void Snake::turn(int x)
{
    if (((x == LEFT || x == RIGHT) && (dir == UP || dir == DOWN)) || ((dir == LEFT || dir == RIGHT) && (x == UP || x == DOWN)))
    {
        dir = x;
    }
}

char Snake::fwdChar(char (&m)[mapH][mapW])
{
    Coord t = nodes[0];
    movePointToDir(&t);
    int nx = t.x;
    int ny = t.y;
    if (nx >= 0 && nx < mapW && ny >= 0 && ny < mapH)
    {
        return m[ny][nx];
    } else
    {
        return cNoChar;
    }
}

void Snake::draw(int col, char (&m)[mapH][mapW])
{
    for (int i = 0; i < len; i++)
    {
        int tx = nodes[i].x;
        int ty = nodes[i].y;
        m[ty][tx] = (col ? who : cEmpty);
    }
}

int Snake::getTailX()
{
    return nodes[len - 1].x;
}

int Snake::getTailY()
{
    return nodes[len - 1].y;
}
