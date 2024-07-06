#include<iostream>
#include<time.h>
#include<math.h>
#include<vector>
#include<algorithm>
using namespace std;
int N;//磁道个数
int* track_list;//磁道序列
int head_begin;
//初始化，随机生成磁道序列，磁头位置
void init()
{
	srand(time(NULL));//设置随机数种子
	N = 10;//(rand() % 1001)+1;//随机生成磁道个数
	track_list = new int[N];
	for (int i = 0; i < N; ++i)
		track_list[i] = rand() % 1001;//随机生成磁道序列
	head_begin = rand() % 1001;//初始化磁头位置
}
//先来先服务算法:
void FCFS()
{
	int head = head_begin;
	int next_head = head_begin;
	int n = 0;//总磁道位数
	cout << "FCFS磁头移动顺序:";
	for (int i = 0;i<N; ++i)
	{
		next_head = track_list[i];
		n += abs(next_head - head);
		head = next_head;
		if (i != N - 1)cout << next_head << "->";
		else cout << next_head << endl;
	}
	cout << "FCFS总磁道位数:" << n << endl;
}
//最短寻道时间优先法
//判断是否已经寻道完毕
bool is_visited(bool visited[])
{
	bool res = true;
	for (int i = 0; i < N; ++i)
		res *= visited[i];
	return res;
}
void upgrade_d(int head,int d[])
{
	for (int i = 0; i < N; ++i)
		d[i] = abs(head - track_list[i]);
}
void SSTF()
{
	int n = 0;
	int head = head_begin;
	int next_head = head_begin;
	int* d = new int[N];
	bool* visited = new bool[N];
	for (int i = 0; i < N; ++i)
	{
		d[i] = INT32_MAX;
		visited[i] = false;
	}
	cout << "SSTF磁头移动顺序:";
	//未遍历完成时
	while (!is_visited(visited))
	{
		//计算d[N]
		upgrade_d(head,d);
		//d中最小值的位置k
		int k = 0;
		int min = INT32_MAX;
		for (int i = 0; i < N; ++i)
			if (!visited[i] && min >= d[i])
			{k = i;min = d[i];}
		next_head = track_list[k];
		n += abs(head - next_head);
		visited[k] = true;
		head = next_head;
		if (is_visited(visited))cout << next_head << endl;
		else cout << next_head << "->";
	}
	cout << "SSTF总磁道数:"<<n<<endl;
}
//电梯扫描算法
void SCAN()
{
	bool* visited = new bool[N];
	for (int i = 0; i < N; ++i)visited[i] = false;
	int n = 0;
	//将track_list分为两个序列,一个大于head_begin,一个小于head_begin
	vector<pair<int, int>>list_1;//小于head_begin
	vector<pair<int, int>>list_2;//大于head_begin
	//list.first为head_begin到点track_list[list.second]的距离
	int head = head_begin;
	int next_head = head_begin;
	for (int i = 0; i < N; ++i)
	{
		if (track_list[i] <= head_begin)
			list_1.push_back({ abs(head_begin - track_list[i]),i });
		else
			list_2.push_back({ abs(head_begin - track_list[i]),i });
	}


	sort(list_1.begin(), list_1.end());
	sort(list_2.begin(), list_2.end());

	cout << "SCAN磁头移动顺序:";
	for (int i =0; i<list_1.size(); ++i)
	{
		next_head = track_list[list_1[i].second];
		n += abs(head - next_head);
		head = next_head;
		visited[list_1[i].second] = true;
		if (!is_visited(visited))cout << next_head << "->";
		else cout << next_head;
	}
	for (int i = 0; i < list_2.size(); ++i)
	{
		next_head = track_list[list_2[i].second];
		n += abs(head - next_head);
		head = next_head;
		visited[list_2[i].second] = true;
		if (!is_visited(visited))cout << next_head << "->";
		else cout << next_head;
	}
	cout << endl << "SCAN总磁道数:" << n << endl;
}
int main()
{
	init();
	cout << "磁头初始位置:" << head_begin<<endl;
	cout << "输入磁道序列:";
	for (int i = 0; i < N; ++i)
	{
		cout << track_list[i] << " ";
	}
	cout << endl << endl;
	FCFS();
	cout << endl<<endl;
	SSTF();
	cout << endl << endl;
	SCAN();
	cout << endl << endl;
	return 0;
}