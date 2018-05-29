#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>

#define width 25  //地图的宽
#define height 30 //地图的高
#define up  72    //方向的键值
#define down  80  
#define left  75    
//常用常量宏定义
#define wall   1 
#define food   2
#define over   3

//定义蛇的结构体
typedef struct snake
{
	int size;
	COORD head ;
	COORD body[height*width];
	int direction;
	int isFood;

}Snake;

//定义一个全局数组作为地图
int Map[height][width];

int difty;
//限定控制台窗口大小、缓冲区和名称
void scrnInfo();

//在横坐标为2*x，纵坐标为y的地方打印输入的字符串
void writeChar(int x, int y, char *str, int attr);
//随机生成一个坐标结构体
COORD randCOORD();
//初始化地图
void initMap();
//打印欢迎界面
void start();
//打印地图
void printMap();
//打印蛇的位置
void printSnake(Snake *snake);
//初始化蛇
void initSnake(Snake * snake);
//找出反方向
int udir(int dir);
//改变蛇的方向
void changeSnake(Snake *snake);
//移动蛇
void moveSnake(Snake *snake);
//产生食物
void makeFood(Snake snake);
//判断是否吃到食物
int getFood(Snake *snake);
//判断蛇是否死亡和是否退出游戏
int isLife(Snake snake);


void main()
{
	do{
		int time, flag = 0;
		scrnInfo();
		initMap();
		printMap();
		start();
		time = difty;
		Snake snake;
		initSnake(&snake);
		makeFood(snake);
		while (1)
		{
			if (kbhit())
			{
				changeSnake(&snake);
			}
			time--;
			if (!time)
			{
				if (isLife(snake))
				{
					break;
				}
				if (getFood(&snake))
				{
					makeFood(snake);
				}
				else { moveSnake(&snake); }
				printSnake(&snake);
				time = difty;
			}
		}
		if (difty == over) break;
		writeChar(28, 20, "按任意键重新开始", 10);
		getch();
		system("CLS");
	} while (difty != over);
}

void scrnInfo()
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD bufSize = { 80, 35 };
	SetConsoleScreenBufferSize(hStdOut, bufSize);
	SMALL_RECT   srect = { 0, 0, 80 - 1, 35 - 1 };
	SetConsoleTitleA("snake");
	SetConsoleWindowInfo(hStdOut, 1, &srect);
}
void writeChar(int x, int y, char *str, int attr)
{
	CONSOLE_CURSOR_INFO m_cci = { 1, 0 };
	COORD pos = { x * 2, y };
	//设置光标不可见
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &m_cci);
	//设置文本颜色
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr);
	//设置输出位置
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	printf("%s", str);
}
COORD randCOORD()
{

	COORD x = { rand() % (width - 2) + 1, rand() % (height - 2) + 1 };
	return x;
}
void initMap()
{
	//遍历数组输出墙
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i == 0 || j == 0 || i == height - 1 || j == width - 1)
				Map[i][j] = wall;
			else
				Map[i][j] = 0;
		}
	}
}
void start()
{
	int t;
	writeChar(8, 5, "欢迎来到贪吃蛇:", 12);
	writeChar(9, 9, "1.开始游戏", 7);
	writeChar(9, 14, "2.退出游戏", 7);
	while (t = getch())
	{
		if (t == '1')
		{
			break;
		}
		if (t == '2')
		{
			difty = over;
			return;
		}

	}
	writeChar(8, 5, "               ", 7);
	writeChar(9, 9, "          ", 7);
	writeChar(9, 14, "          ", 7);
	writeChar(8, 5, "请选择难度:", 12);
	writeChar(9, 8, "1.简单", 7);
	writeChar(9, 11, "2.普通", 7);
	writeChar(9, 14, "3.困难", 7);
	writeChar(9, 17, "4.退出游戏", 7);
	while (t = getch())
	{
		if (t == '1')
		{
			difty = 15000;
			break;
		}
		if (t == '2')
		{
			difty = 10000;
			break;
		}
		if (t == '3')
		{
			difty = 5000;
			break;
		}if (t == '4')
		{
			difty = over;
			break;
		}
	}
	writeChar(8, 5, "           ", 7);
	writeChar(9, 8, "      ", 7);
	writeChar(9, 11, "      ", 7);
	writeChar(9, 14, "      ", 7);
	writeChar(9, 17, "          ", 7);

}

void printMap()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (Map[i][j] == wall)
				writeChar(j, i, "■", 2);
		}
	}


}

void printSnake(Snake *snake)
{
	//打印蛇的头
	writeChar(snake->head.X, snake->head.Y, "●", 5);
	//通过body数组打印蛇的身体
	for (int i = 0; i < snake->size; i++)
	{
		writeChar(snake->body[i].X, snake->body[i].Y, "■", 5);
	}
	//擦除尾部
	if (!snake->isFood){
		if (snake->size)
		{
			writeChar(snake->body[snake->size].X, snake->body[snake->size].Y, "  ", 1);
		}
		else
		{
			switch (snake->direction)
			{
			case up:
				writeChar(snake->head.X, snake->head.Y + 1, "  ", 1); break;
			case left:
				writeChar(snake->head.X + 1, snake->head.Y, "  ", 1); break;
			case right:
				writeChar(snake->head.X - 1, snake->head.Y, "  ", 1); break;
			case down:
				writeChar(snake->head.X, snake->head.Y - 1, "  ", 1); break;
			}
		}
	}
	snake->isFood = 0;
	char score[12];
	if (snake->size != 0)
		itoa(snake->size * 1000000 / difty, score, 10);
	else
		itoa(0, score, 10);
	writeChar(30, 2, "分数:", 10);
	writeChar(33, 2, &score, 9);
	writeChar(28, 8, "提示:", 7);
	writeChar(29, 9, "按I键暂停,再", 7);
	writeChar(29, 10, "次按下继续", 7);
}

void initSnake(Snake * snake)
{
	snake->size = 0;
	snake->head = randCOORD();
	snake->direction = up;
	snake->isFood = 0;

}

int udir(int dir)
{
	switch (dir)
	{
	case up:
		return down; break;
	case left:
		return right; break;
	case right:
		return left; break;
	case down:
		return up; break;
	}
}

void changeSnake(Snake *snake)
{
	char dir = getch();
	if (dir != udir(snake->direction))
	{
		switch (dir)
		{
		case 'w':
		case 'W':
		case up:
			snake->direction = up; break;
		case'a':
		case'A':
		case left:
			snake->direction = left; break;
		case'd':
		case'D':
		case right:
			snake->direction = right; break;
		case's':
		case'S':
		case down:
			snake->direction = down; break;
		case'i':
		case'I':
			while (1)
			{
				char x = getch();
				if (x == 'i' || x == 'I') break;
			}
		}
	}

}

void moveSnake(Snake *snake)
{

	for (int i = snake->size; i > 0; i--)
	{
		snake->body[i] = snake->body[i - 1];
	}
	if (snake->size > 0)
	{
		snake->body[0] = snake->head;
	}
	switch (snake->direction)
	{
	case up:
		snake->head.Y--; break;
	case left:
		snake->head.X--; break;
	case right:
		snake->head.X++; break;
	case down:
		snake->head.Y++; break;
	}
}

void makeFood(Snake snake)
{
	COORD x;
	srand(time(NULL));
	boolean flag1, flag2;
	do{
		flag1 = 0, flag2 = 0;
		x = randCOORD();
		if (x.X != snake.head.X&&x.Y != snake.head.Y)
			flag1 = 1;
		if (snake.size == 0)
			flag2 = 1;
		for (int i = 0; i < snake.size; i++)
		{
			if (snake.body[i].X == x.X&&snake.body[i].Y == x.Y)
			{
				flag2 = 0; break;
			}
			flag2 = 1;
		}
	} while (!flag1 || !flag2);
	Map[x.Y][x.X] = food;
	writeChar(x.X, x.Y, "★", 14);
}

int getFood(Snake *snake)
{
	if (Map[snake->head.Y][snake->head.X] == food)
	{
		for (int i = snake->size; i > 0; i--)
		{
			snake->body[i] = snake->body[i - 1];
		}

		snake->body[0] = snake->head;
		Map[snake->head.Y][snake->head.X] = 0;
		switch (snake->direction)
		{
		case up:
			snake->head.Y--; break;
		case left:
			snake->head.X--; break;
		case right:
			snake->head.X++; break;
		case down:
			snake->head.Y++; break;
		}
		snake->size++;
		snake->isFood = 1;

		return 1;
	}
	return 0;
}

int isLife(Snake snake)
{
	if (Map[snake.head.Y][snake.head.X] == wall)
		return 1;
	for (int i = 0; i < snake.size; i++)
	{
		if (snake.head.X == snake.body[i].X&&snake.head.Y == snake.body[i].Y)
			return 1;
	}
	if (difty == over) return 1;
	return 0;
}
