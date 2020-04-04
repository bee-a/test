#include<Windows.h>
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<conio.h>
double v=100;//蛇的初速度
int i, j;//横纵坐标
int dir[4][2] = {
	{0,  1}, { 1, 0},
	{0, -1}, {-1, 0}
};//方向
int DIR = 0;//相对位置
int a, b;//用于定位食物
int food[2];//数组的第一个元素设为横坐标，第二个元素设为纵坐标
char map[50][50] = { ' ' };//初始化地图全是空格
int snake[10][2] = { -1 }, snakelength = 0;

void hidden() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hOut, &cci);
}//隐藏光标函数，防止光标在屏幕上闪动

void creatfood()
{
	a = rand() % 46;
	b = rand() % 46;
	food[0] = a + 2;//地图靠边处比较难吃到，所以把食物限制到2-47之间
	food[1] = b + 2;
}
void snake1()//通过上文的标记来将#与 区分开
{
	printf("请输入蛇的长度");
	scanf_s("%d", &snakelength);
	for (int i = 0; i < snakelength; i++)
	{
		printf("请输入蛇的坐标");
		scanf_s("%d%d", &snake[i][0], &snake[i][1]);//分别表示蛇的横纵坐标
	}
	getchar();//去掉多余的回车
	int tx = snake[0][0] - snake[1][0];//第一节蛇头相对于第二节蛇身的位置
	int ty = snake[0][1] - snake[1][1];
	for (int i = 0; i < 4; i++)//一共四个方向，一个方向一个方向判断
	{
		if (tx == dir[i][0] && ty == dir[i][1])//相对位置是其中一个方向，就将其设置成初始方向。tx跟方向的二维数组的横坐标比较，ty是跟方向的二维数组的纵坐标比较
		{
			DIR = i;//将这个方向赋值给DIR
			break;
		}
	}
}
void  outputsnake()
{
	hidden();
	//输出地图
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 50; j++)                 //对 map 的每一个位置
		{
			bool ifsnake = false;      //如果这一格属于蛇 该变量值为真，否则为假
			bool iffood = false;
			for (int k = 0; k < snakelength; k++)    //对蛇的每一节，
			{
				if (i == snake[k][0] && j == snake[k][1])//如果snake坐标跟i与j相等，那就是蛇。i与j是map坐标
				{
					ifsnake = true;
					break;//已经判断完就停止

				}
			}
			if (i == food[0] && j == food[1])
				printf("@ ");
			else if (ifsnake)
				printf("# ");
			else
				printf("%c ", map[i][j]);
		}
		printf("\n");
	}
}
void map2()//给边框做标记
{
	int i = 0;
	while (i < 50)
	{
		map[i][0] = 1;
		map[i][49] = 1;
		i++;
	}
	int j = 0;
	while (j < 50)
	{
		map[0][j] = 1;
		map[49][j] = 1;
		j++;
	}

}
inline bool failed()//如果游戏失败了，失败这个事是假的
{
	for (int i = 1; i < snakelength; i++)
	{
		if (snake[0][0] == snake[i][0] && snake[0][1] == snake[i][1])
			return true;
	}
	if (snake[0][0] <= 0 || snake[0][0] >= 49 || snake[0][1] <= 0 || snake[0][1] >= 49)
		return true;
	return false;
}
inline bool FOOD()
{
	if (food[0] == snake[0][0] && food[1] == snake[0][1])//食物一个数对可以
		return true;
	return false;
}
void direction()//定义蛇的运动方向
{
	char temp;
	while (1)
	{

		if (!_kbhit())
		{
			int tx, ty;
			tx = snake[snakelength - 1][0];
			ty = snake[snakelength - 1][1];
			for (int i = snakelength - 2; i >= 0; i--)
			{
				snake[i + 1][0] = snake[i][0];//将前一节蛇头的赋给后一节
				snake[i + 1][1] = snake[i][1];//除了蛇头之外的节数向前走一个单位
			}
			snake[0][0] = snake[1][0] + dir[DIR][0];
			snake[0][1] = snake[1][1] + dir[DIR][1];//添加蛇头：原位置加1次方向
			if (failed())
			{
				printf("GAME OVER\n");
				break;
			}
			if (FOOD())
			{
				creatfood();
				snakelength++;
				v *=0.99 ;
				snake[snakelength - 1][0] = tx;
				snake[snakelength - 1][1] = ty;
			}

		}
		else
		{
			temp = getchar();
			switch (temp)
			{case 'w':DIR = 3; break;
			case 'a':DIR = 2; break;
			case 's':DIR = 1; break;
			case 'd':DIR = 0; break;
			default:
				printf("BAD TOKEN!\n");
			}
			getchar();//回车
		}
		COORD point = { 0,0 };
		HANDLE HOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(HOutput, point);
		outputsnake();//用句柄将光标定位在（0，0），然后重新输出地图
		
		Sleep(v);//蛇的速度在不断的增大
	}
}

int main()
{
	srand((unsigned int)time(NULL));
	creatfood();
	map2();
	snake1();
	outputsnake();
	direction();
	return 0;
}
