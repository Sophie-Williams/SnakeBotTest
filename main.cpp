#include "library.h"
Map *mp;



int buildShortestPath(int by, int bx, int ey, int ex, char (&res)[mapH][mapW])
{
	for (int y = 0; y < mapH; y++)
		for (int x = 0; x < mapW; x++)
			res[y][x] = mp->curMap[y][x];
	int used[mapH][mapW];
	for (int y = 0; y < mapH; y++)
		for (int x = 0; x < mapW; x++) used[y][x] = 0;
	pair<int, int> p[mapH][mapW];
    int shx[] = {0, 0, 1, -1};
    int shy[] = {1, -1, 0, 0};
    queue< pair<int, int> > q;
    q.push({by, bx});
    p[by][bx] = {-1, -1};
    used[by][bx] = 1;
    res[ey][ex] = '@';
    int px, py;
    while (!q.empty())
    {
        py = q.front().first;
        px = q.front().second;
        if (py == ey && px == ex) break;
        q.pop();
        for (int i = 0; i < 4; i++)
        {
            int ty, tx;
            ty = py + shy[i];
            tx = px + shx[i];
            if (!used[ty][tx] && (res[ty][tx] == cEmpty || res[ty][tx] == '@'))
            {
                q.push({ty, tx});
                p[ty][tx] = {py, px};
                used[ty][tx] = 1;
            }
        }
    }
    if (!(py == ey && px == ex))
    {
        return 0;
    }
    while (p[py][px] != make_pair(-1, -1))
    {
        if (res[py][px] == cEmpty)
			res[py][px] = '+';
        pair<int, int> t = p[py][px];
        py = t.first;
        px = t.second;
    }
	return 1;
}




void moveSnake(Snake *s, int dir)
{
    s->turn(dir);
    if (s->fwdChar(mp->curMap) == cEmpty)
    {
        s->draw(0, mp->curMap);
        s->step();
    }
}

void moveSnakeBot(Snake *bot, Snake *s)//s1 - bot
{
    char mapPath[mapH][mapW];
	int by = bot->nodes[0].y;
	int bx = bot->nodes[0].x;
	s->draw(1, mp->curMap);
	int st = buildShortestPath(by, bx, s->nodes[0].y, s->nodes[0].x,/*s->getTailY(), s->getTailX(),*/ mapPath);
	dumpToFile(mapPath, "dmp.txt");
	bot->draw(0, mp->curMap);
	if (by - 1 >= 0 && mapPath[by - 1][bx] == '+')
	{
		bot->turn(UP);
		bot->step();
	} else if (by + 1 < mapH && mapPath[by + 1][bx] == '+')
	{
		bot->turn(DOWN);
		bot->step();
	} else if (bx - 1 >= 0 && mapPath[by][bx - 1] == '+')
	{
		bot->turn(LEFT);
		bot->step();
	} else if (bx + 1 < mapW && mapPath[by][bx + 1] == '+')
	{
		bot->turn(RIGHT);
		bot->step();
	}
}

int main()
{
    initConsole();
    mp = new Map(cEmpty);
    Snake *s1 = new Snake(5, 2, cUser);
    Snake *s2 = new Snake(7, 3, cBot);

    s1->draw(1, mp->curMap);
    s2->draw(1, mp->curMap);  //mp->dumpToFile("dmp1.txt");
    mp->draw();
    while (1)
    {
        char key = getch();
        if (key == 'w')
        {
            moveSnake(s1, UP);
        }
        if (key == 's')
        {
            moveSnake(s1, DOWN);
        }
        if (key == 'a')
        {
            moveSnake(s1, LEFT);
        }
        if (key == 'd')
        {
            moveSnake(s1, RIGHT);
        }
        Sleep(100);
        moveSnakeBot(s2, s1);

        s1->draw(1, mp->curMap);
        s2->draw(1, mp->curMap);
        mp->draw();
    }
    return 0;
}
