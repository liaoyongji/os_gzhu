#include<iostream>
#include<stdlib.h>
#include<time.h>
using namespace std;
const int N = 320;
const int M = 10;//每k存放10条指令
int Instructions[N];

//生成指令序列
void Init_instructions()
{
	srand(time(NULL));//设置随机数种子
	int i = 0;
	while (i<N)
	{
		int x = rand() % N; 
		while (x > N - 2)
		{
			x = rand() % N;
		}
		if(i<N)Instructions[i++] = x + 1; 
		int y = rand() % (x + 1);
		while (y > N - 3)
		{
			y = rand() % (x + 1);
		}
		if(i<N)Instructions[i++] = y;
		if(i<N)Instructions[i++] = y + 1;
		int z = rand() % (N - (y + 2)) + y + 2; 
		if(i<N)Instructions[i++] = z;
	}
}


//最佳淘汰算法-------------------------------------------------------------------------------------
bool i_in_memery(int i,int n,int memery[])//判断指令i所在页是否在内存中
{
	for (int j = 0; j < n; ++j)
		if (Instructions[i]/M == memery[j])
			return true;//在内存中返回true
	return false;//不在内存中返回false
}
bool memery_full(int n,int memery[])
{
	for (int i = 0; i < n; ++i)
		if (memery[i] == -1)
			return false;//内存未满
	return true;//内存已满
}
int find_max_next_visit(int n,int next_visit[])
{
	int k = 0;
	int max = 0;
	for (int i = 0; i < n; ++i)
	{
		if (next_visit[i] > max)
		{
			k = i;
			max = next_visit[i];
		}
	}
	return k;
}
float OPT(int n)
{
	//初始化
	double count = 0;
	int* memery;
	memery = new int[n];
	int* next_visit;
	next_visit = new int[n];
	for (int i = 0; i < n; ++i)
	{
		memery[i] = -1;
		next_visit[i] = -1;
	}
	for(int i = 0;i<N;++i)//运行指令i
	{
		//若i所在内存页不在内存中
		if (!i_in_memery(i, n, memery))
		{
			//内存已满
			if (memery_full(n, memery))
			{
				//更新next_visit
				//遍历内存
				for (int j = 0; j < n; ++j)
				{
					int k = i;
					int d = 1;
					while (k < N)
					{
						if (memery[j] == Instructions[k]/M)
						{
							next_visit[j] = d;
							break;
						}
						else next_visit[j] = INT32_MAX;
						++d;
						++k;
					}
				}
				//找出next_visit最大值的位置
				memery[find_max_next_visit(n, next_visit)] = Instructions[i]/M;
			}
			else//内存未满
			{
				//找出第一个未满的位置
				int j = 0;
				while (memery[j] != -1)++j;
				memery[j] = Instructions[i]/M;//将指令所在页放入内存
			}
		}
		else ++count;//i所在内存页在内存中
	}
	return count;
}
//end of 最佳淘汰----------------------------------------------------------------------------------

//先进先出算法-------------------------------------------------------------------------------------
bool instruction_in_memery(int n,int i,int memery[])
{
	for (int j = 0; j < n; ++j)
	{
		if (Instructions[i] / M == memery[j])
			//一个页可存储M条指令0....319,Instructions[i]/M->页号
			return true;
	}
	return false;
}
bool memery_is_full(int n,int memery[])
{
	for (int i = 0; i < n; ++i)
		if (memery[i] == -1)return false;
	return true;
}
void upgrade_time(int n,int time[])
{
	for (int i = 0; i < n; ++i)
	{
		if (time[i] != -1)++time[i];
	}
}
void FIFO(int n)
{
	double count = 0;//命中次数
	int* memery = new int[n];//内存
	int* time = new int[n];//页在内存中的时间
	for (int i = 0; i < n; ++i)
	{
		time[i] = -1;
		memery[i] = -1;
	}
	//运行指令
	for (int i = 0; i < N; ++i)
	{
		//指令所在页在内存中
		if (instruction_in_memery(n, i, memery))
		{
			++count;//命中
			//刷新时间
			upgrade_time(n,time);
		}
		//指令所在页不在内存中
		else
		{
			//内存已满
			if (memery_is_full(n, memery))
			{
				//寻找time最大的页，替换
				int j = 0;
				int max_time = 0;
				for (int k = 0; k < n; ++k)
				{
					if (max_time < time[k])
					{
						max_time = time[k];
						j = k;
					}
				}
				memery[j] = Instructions[i] / M;
				time[j] = 0;
				upgrade_time(n, time);
			}
			//内存未满
			else
			{
				//寻找内存中顺序第一个空位
				int j = 0;
				while (memery[j] != -1)++j;
				memery[j] = Instructions[i] / M;
				time[j] = 0;//将指令放入内存
				upgrade_time(n, time);
			}
		}
	}
	cout << "内存为" << n << "k运行成功," << "命中率为:" << count/N << endl;
}
//end of 先进先出----------------------------------------------------------------------------------


//最久未使用算法-----------------------------------------------------------------------------------
void LRU(int n)
{
	//初始化
	float count = 0;
	int* memery = new int[n];
	int* time = new int[n];
	for (int i = 0; i < n; ++i)
	{
		memery[i] = -1;
		time[i] = -1;
	}
	//遍历指令集
	for (int i = 0; i < N; ++i)
	{
		//指令i在内存中
		if (i_in_memery(i, n, memery))
		{
			//指令使用
			//将时间置为0
			//查找指令在memery中的位置
			int j = 0;
			while (memery[j]!=Instructions[i]/10)++j;
			time[j] = 0;
			++count;
			upgrade_time(n,time);
		}
		//指令i不在内存中
		else
		{
			//内存已满
			if (memery_is_full(n, memery))
			{
				//找出time中的最大值
				int j = 0;
				int max = 0;
				int max_time = 0;
				while (j<n)
				{
					if (max_time < time[j])
					{
						max_time = time[j];
						max = j;
					}
					++j;
				}
				memery[max] = Instructions[i] / 10;//将页替换内存
				time[max] = 0;//重置时间
				upgrade_time(n, time);
			}
			//内存未满
			else
			{
				//寻找内存中最前面的空位，将指令放入内存
				int j = 0;
				while (memery[j] != -1)++j;
				memery[j] = Instructions[i] / M;
				//将时间置为0
				time[j] = 0;
				//刷新时间
				upgrade_time(n, time);//++time
			}
		}
	}
	cout << "内存为" << n << "k运行成功," << "命中率为:" << count / N << endl;
}
//end of 最久未使用算法----------------------------------------------------------------------------

int main()
{
	Init_instructions();
	cout << "最佳淘汰算法(OPT):" << endl;
	for(int n = 4;n<33;++n)
	{ 
		double p = OPT(n);
		double p0 = p / N;
		cout << "内存为" << n << "k运行成功," <<"命中率为:"<<p0<< endl;
	}
	cout << endl;
	cout << "先进先出算法(FIFO):" << endl;
	for (int i = 4; i < 33; ++i)
	{
		FIFO(i);
	}
	cout << endl;
	cout << "最久未使用算法(LRU):" << endl;
	for (int i = 4; i < 33; ++i)
	{
		LRU(i);
	}
}