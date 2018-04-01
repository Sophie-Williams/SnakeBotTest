#include "library.h"
Map *mp;
bool botMissStep;
bool userMissStep;


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
			res[py][px] = cPath;
        pair<int, int> t = p[py][px];
        py = t.first;
        px = t.second;
    }
	return 1;
}




void moveSnake(Snake *s, Snake *bot, int dir)
{
    s->turn(dir);
    if (s->fwdChar(mp->curMap) == cEmpty)
    {
        s->draw(0, mp->curMap);
        s->step();
    } else if (s->fwdChar(mp->curMap) == cBotTail)
    {
        Coord t;
        t.y = s->nodes[0].y;
        t.x = s->nodes[0].x;
        s->movePointToDir(&t);
        mp->curMap[t.y][t.x] = cEmpty;
		s->step();
		s->incLen();
		bot->decLen();
		userMissStep = true;
    }
}

void moveSnakeBot(Snake *bot, Snake *s)
{
    char mapPath[mapH][mapW];
	int by = bot->nodes[0].y;
	int bx = bot->nodes[0].x;
	s->draw(1, mp->curMap);
	int st = buildShortestPath(by, bx, /*s->nodes[0].y, s->nodes[0].x, */s->getTailY(), s->getTailX(), mapPath);
	dumpToFile(mapPath, "dmp.txt");
	bot->draw(0, mp->curMap);


    if (by - 1 >= 0 && mp->curMap[by - 1][bx] == cUserTail)
	{//cout << "YES";
		bot->turn(UP);
		mp->curMap[by - 1][bx] = cEmpty;
		bot->step();
		bot->incLen();
		s->decLen();
		botMissStep = true;
		return;
	} else if (by + 1 < mapH && mp->curMap[by + 1][bx] == cUserTail)
	{//cout << "YES";
		bot->turn(DOWN);
		mp->curMap[by + 1][bx] = cEmpty;
		bot->step();
		bot->incLen();
		s->decLen();
		botMissStep = true;
		return;
	} else if (bx - 1 >= 0 && mp->curMap[by][bx - 1] == cUserTail)
	{//cout << "YES";
		bot->turn(LEFT);
		mp->curMap[by][bx - 1] = cEmpty;
		bot->step();
		bot->incLen();
		s->decLen();
		botMissStep = true;
		return;
	} else if (bx + 1 < mapW && mp->curMap[by][bx + 1] == cUserTail)
	{//cout << "YES";
		bot->turn(RIGHT);
		mp->curMap[by][bx + 1] = cEmpty;
		bot->step();
		bot->incLen();
		s->decLen();
		botMissStep = true;
		return;
	}


	if (by - 1 >= 0 && mapPath[by - 1][bx] == cPath)
	{
		bot->turn(UP);
		bot->step();
	} else if (by + 1 < mapH && mapPath[by + 1][bx] == cPath)
	{
		bot->turn(DOWN);
		bot->step();
	} else if (bx - 1 >= 0 && mapPath[by][bx - 1] == cPath)
	{
		bot->turn(LEFT);
		bot->step();
	} else if (bx + 1 < mapW && mapPath[by][bx + 1] == cPath)
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
        if (!userMissStep)
        {
            char key = getch();
            if (key == 'w')
            {
                moveSnake(s1, s2, UP);
            }
            if (key == 's')
            {
                moveSnake(s1, s2, DOWN);
            }
            if (key == 'a')
            {
                moveSnake(s1, s2, LEFT);
            }
            if (key == 'd')
            {
                moveSnake(s1, s2, RIGHT);
            }
        } else
        {
            userMissStep = false;
        }
        Sleep(100);
        if (!botMissStep)
        {
            moveSnakeBot(s2, s1);
        } else
        {
            botMissStep = false;
        }

        s1->draw(1, mp->curMap);
        s2->draw(1, mp->curMap);
        mp->draw();
    }
    return 0;
}
