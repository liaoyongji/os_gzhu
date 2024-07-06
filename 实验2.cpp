#include<iostream>
#include<time.h>
#include<random>
#include<stdlib.h>
using namespace std;
int N = NULL;			//进程数
int M = NULL;			//资源数
int* resource = NULL;	//资源总数
int** Max = NULL;		//每个进程对应的资源需求数
int** Need = NULL;		//每个进程当前需要的资源数
int* available = NULL;  //当前所剩资源数
int** allocation = NULL;//已分配资源
int* processlist = NULL;//安全进程序列
bool safety(bool opt);
void print()
{
	//print
	cout << "-----------------------------------初始状态-----------------------------------" << endl;
	cout << "\tMax";
	for (int i = 0; i < M * 2; ++i)cout << " ";
	cout << "allocation";
	for (int i = 0; i < M * 2 - 7; ++i)cout << " ";
	cout << "Need";
	for (int i = 0; i < M * 2 - 1; ++i)cout << " ";
	cout << "available";
	cout << endl;
	for (int i = 0; i < N; ++i)
	{
		cout << "进程" << i << "\t";
		for (int j = 0; j < M; ++j)
		{
			cout << Max[i][j] << " ";
		}
		cout << "   ";
		for (int j = 0; j < M; ++j)
		{
			cout << allocation[i][j] << " ";
		}
		cout << "   ";
		for (int j = 0; j < M; ++j)
		{
			cout << Need[i][j] << " ";
		}
		cout << "   ";
		for (int j = 0; j < M && i == 0; ++j)
		{
			cout << available[j] << " ";
		}
		cout << endl;
	}
	cout << "------------------------------------------------------------------------------" << endl;
}
void Init()
{
	srand(time(NULL));
	bool is_safe = false;
	N = rand() % 3 + 3;//3到5
	M = rand() % 4 + 4;//4到7
	//分配空间
	resource = new int[M];
	available = new int[M];
	allocation = new int* [N];
	Max = new int* [N];
	Need = new int* [N];
	for (int i = 0; i < M; ++i)
	{ 
		allocation[i] = new int [M];
		Max[i] = new int[M];
		Need[i] = new int[M];
	}
	//随机生成数据
	while (!is_safe)
	{
		for (int i = 0; i < M; ++i)
		{
			resource[i] = rand() % 5 + 5;//5到9 
			//初始化可以资源=总资源
			available[i] = resource[i];
		}
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				int p = 0;
				int sign1 = 1;
				Max[i][j] = rand() % (resource[j] + 1);
				int d = 0;
				int sign2 = 1;
				d = rand() % (Max[i][j] + 1);
				allocation[i][j] = d;
				//需求=最大需求-已经分配资源
				Need[i][j] = Max[i][j] - allocation[i][j];
			}
		}
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				available[j] -= allocation[i][j];
			}
		}
		if (safety(false))
		{//对生成的序列进行安全检验，若安全则生成成功，否则重新生成
			is_safe = 1;
		}
	}
}
bool safety(bool opt)
{
	//初始化
	int* work = NULL;//当前资源所剩数量
	int count = 0;//标记进程顺序
	processlist = new int[N];//安全序列
	for (int i = 0; i < N; ++i)processlist[i] = -1;//初始化安全序列
	work = new int[M];
	for (int i = 0; i < M; ++i)
	{
		work[i] = available[i];
	}
	bool* visited = NULL;
	visited = new bool[N];//判断某进程是否已经得到过全部资源，得到过为true，否则为false
	for (int i = 0; i < N; ++i)visited[i] = false;

	//输出数据=============================================================================================
	if (opt)
	{
		cout << "---------------------------------安全检查过程---------------------------------" << endl;
		cout << "\twork";
		for (int i = 0; i < M * 2-2; ++i)cout << " ";
		cout << "Need";
		for (int i = 0; i < M * 2 -2; ++i)cout << " ";
		cout << "allocation";
		for (int i = 0; i < M * 2 - 8; ++i)cout << " ";
		cout << "work+allocation";
		cout << endl;
	}
	//======================================================================================================

	for(int u = 0;u<N;++u)
	{ 
		//判断是否至少有一条进程可以得到全部资源而完成运行
		bool* list = new bool[N];
		for (int i = 0; i < N; ++i)
		{
			if (visited[i] == false)
			{
				bool sign = true;
				for (int j = 0; j < M; ++j)
				{
					sign *= (Need[i][j] <= work[j]);
				}
				list[i] = sign;
			}
			else list[i] = false;
		}
		int sign0 = 0;
		for (int i = 0; i < N; ++i)if (list[i])++sign0;
		if (sign0>=1)//若存在一个进程可以得到全部资源运行sing0>=1
		{
			//找出当前need小于work的
			for (int i = 0; i < N; ++i)
			{
				if (!visited[i])
				{
					bool sign = true;
					for (int j = 0; j < M; ++j)
					{
						sign *= (Need[i][j] <= work[j]);
					}
					if (sign)//如果进程i可以全部分配
					{
						processlist[count] = i;
						++count;
						visited[i] = true;
						//输出数据=======================================================
						if (opt)
						{
							cout << "进程" << i << "\t";
							for (int j = 0; j < M; ++j)
							{
								cout << work[j] << " ";
							}
							cout << "  ";
							for (int j = 0; j < M; ++j)
							{
								cout << Need[i][j] << " ";
							}
							cout << "  ";
							for (int j = 0; j < M; ++j)
							{
								cout << allocation[i][j] << " ";
							}
							cout << "  ";
							for (int j = 0; j < M; ++j)
							{
								cout << allocation[i][j] + work[j] << " ";
							}
							cout << endl;
						}
						//===============================================================
						for (int j = 0; j < M; ++j)
						{
							work[j] += allocation[i][j];
						}
						break;
					}
				}
			}

		}
		else
		{//处于不安全状态
			return false;
		}
	}
	if (opt)cout << "------------------------------------------------------------------------------" << endl;
	return true;
}
void BANK()
{
	srand(time(NULL));
	Init();
	cout << "进程数:" << N << endl;
	cout << "资源种类:" << M << endl;
	cout << "资源数:";
	for (int i = 0; i < M; ++i)cout << resource[i] << " ";
	cout << endl;
	print();//输出各个进程当前的资源分配状态
	//随机选取一个进程分配资源
	int pid = rand() % N;
	int* request = new int[M];
	for (int i = 0; i < M; ++i)
	{
		request[i] = rand() % (min(Need[pid][i]+1, available[i]+1));
	}
	cout << "现在请求资源分配的进程是:" << pid << endl;
	cout <<"进程"<<pid << "所请求的资源:";
	for (int i = 0; i < M; ++i)
	{
		cout << request[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < M; ++i)
	{
		available[i] -= request[i];
		Need[pid][i] -= request[i];
		allocation[pid][i] += request[i];
	}
	print();//输出各个进程当前的资源分配状态
	bool sign = safety(false);//安全性检查
	if (sign)//若为安全状态
	{
		cout << "安全状态,安全序列为:";
		for (int i = 0; i < N; ++i)
		{
			cout << processlist[i];
		}
		cout << endl;
		safety(true);//输出检查过程
	}
	else
	{
		cout << "状态不安全!";//否则
	}
}
int main()
{
	BANK();
	return 0;
}