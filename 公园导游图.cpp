/*
问题描述：

*/
#include<iostream>
#include<fstream>
#include<iomanip>
#include<stdlib.h>
#include<windows.h>
using namespace std;
#define MAX_INT 10000  //无穷大
#define MAX 100  //最大值
#define OK 1
#define ERROR 0
#define FLOWOVER -1
int visited[MAX]={0};  //辅助函数(标志数组)0为未访问
int p1[MAX];//前驱顶点
int cost;  //记录总的路径
int Path[MAX][MAX];//用path[][]记录从i到j的最短路径上j的前驱景点的序号
int Shortest[MAX][MAX];//辅助数组(存放数组G.arc[][]中的值)
struct vex
{
	char name[MAX];    //景点的名称
	char serial[MAX];  //景点的编序
	char intro[MAX];  //景点的简介
};
/**************建立图的邻接矩阵存储表示********************/
struct AMGraph
{
	vex v[MAX];        //景点的信息(顶点表)
	int arc[MAX][MAX];  //邻接矩阵
	int vnum;         //景点的个数
};
//**********************向导图类***********************
class Touristmaps
{
public:
	void Display(AMGraph);      //显示所有景点信息的函数
	void Scanf(AMGraph,char*);  //查找景点信息的函数
	void Add(AMGraph &);        //增加景点信息的函数
	void Start(AMGraph);        //初始化景点信息的函数
	int ShortestDistance();     //景点间最短路径的函数
	void Floyed();              //弗洛伊德算法函数（计算最短路径）
	void Print(int i,int j);    //输出函数(打印两个景点间的路径及最短距离)
	void Menu();                //菜单函数
	void DFS(int x,int y);      //深度优先算法
	void Show();                //景点地图显示函数
	void TSP(AMGraph,int n);    //最佳旅游路线
};
/*************显示所有信息函数****************/
void Touristmaps::Display(AMGraph GH)
{
	int i;
	int count=1;
	cout<<"             编序      名称       简介"<<endl;
	ifstream infile("C://C++//graph.txt",ios::in);//以输入方式打开文件
	infile>>GH.vnum;
	for(i=0;i<GH.vnum;i++)
	{
		infile>>GH.v[i].serial>>GH.v[i].name>>GH.v[i].intro;
		cout<<"景点编号"<<count<<":     "<<GH.v[i].serial<<"       "<<
			GH.v[i].name<<"       "<<GH.v[i].intro<<endl;
		cout<<endl;
		count++;
	}
	infile.close();           //关闭文件流
	cout<<endl;
	system("pause");
	system("cls");           //清屏
}

/**************查找函数****************************/
void Touristmaps::Scanf(AMGraph GH,char*Select)
{
	int obj;
	bool exist=0;       //0为未找到，1为找到
	ifstream infile("C://C++//graph.txt",ios::in);
	infile>>GH.vnum;
	for(int i=0;i<GH.vnum;i++)
	{
		infile>>GH.v[i].serial>>GH.v[i].name>>GH.v[i].intro;
		if(strcmp(GH.v[i].name,Select)==0||strcmp(GH.v[i].serial,Select)==0)   //字符串的比较
		{
			obj=i;
			exist=1;
		}
	}
	if(exist==0)
		cout<<"未找到查询的相关信息！"<<endl;
	else
	{
		cout<<"您要查询的信息如下："<<endl;
		cout<<"景点编序："<<GH.v[obj].serial<<endl;
		cout<<"景点名称："<<GH.v[obj].name<<endl;
		cout<<"景点信息："<<GH.v[obj].intro<<endl;
	}
	infile.close();
	system("pause");
	system("cls");
}
/**************增加景点信息********************/
void Touristmaps::Add(AMGraph &GH)
{
	int i,j,count,f;
	ifstream infile("C://C++//graph.txt",ios::in);
	if(!infile)
	{
		cout<<"打开文件失败！"<<endl;
		exit(1);      //非正常运行程序导致退出程序
	}
	infile>>GH.vnum;
	f=GH.vnum;
	for(i=0;i<GH.vnum;i++)
		infile>>GH.v[i].serial>>GH.v[i].name>>GH.v[i].intro;
	ofstream outfile2("C://C++//graph.txt",ios::out);      //以输出的方式打开文件
	cout<<"请输入景点数目：";
	cin>>count;
	if(!cin)throw string("输入的数据有误！");
	if(GH.vnum+count>MAX)
	{
		cout<<"您输入的景点数目超出范围！增加失败！"<<endl;
		system("pause");
		system("cls");
	
	}
	else
	{
		GH.vnum=GH.vnum+count;

	}
	outfile2<<GH.vnum<<endl;
	for(i=0;i<f;i++)
	outfile2<<GH.v[i].serial<<" "<<GH.v[i].name<<" "<<GH.v[i].intro<<endl;
	outfile2.close();
	infile.close();
	ofstream outfile("C://C++//graph.txt",ios::app);     //以输出的方式打开文件，将增加的数据放到文件末尾
	cout<<"请依次输入景点编序，名称，简介："<<endl;
	for(i=f;i<GH.vnum;i++)
	{
		cout<<"第"<<i+1<<"个信息分别为：";
		cin>>GH.v[i].serial>>GH.v[i].name>>GH.v[i].intro;
        if(!cin)throw string("输入的数据有误！");
		outfile<<GH.v[i].serial<<' '<<GH.v[i].name<<' '<<GH.v[i].intro<<endl;
	}
	outfile.close();
	ifstream infile1("C://C++//linjiejuzhen.txt",ios::in);
	for(i=0;i<f;i++)
		for(j=0;j<f;j++)
			infile1>>GH.arc[i][j];     //将文件数据放入邻接矩阵
		infile1.close();
		ofstream outfile1("C://C++//linjiejuzhen.txt",ios::out);
		for(i=f;i<GH.vnum;i++)        //初始化邻接矩阵
			for(j=0;j<GH.vnum;j++)
			{
				GH.arc[i][j]=MAX_INT;
				GH.arc[j][i]=MAX_INT;
			}
			cout<<"与景点相关的信息输入:"<<endl;
			for(i=f;i<GH.vnum;i++)
			{
				cout<<"请输入与"<<GH.v[i].name<<"景点相邻的所有权值(没有路径时输入10000)"<<endl;
				for(j=0;j<GH.vnum&&j!=i;j++)
				{
					cout<<"请输入"<<GH.v[i].name<<"与"<<GH.v[j].name<<"景点权值"<<endl;
					cin>>GH.arc[i][j];
                    if(!cin)throw string("输入的数据有误！");
					GH.arc[j][i]=GH.arc[i][j];     //无向图，邻接矩阵j，i和i，j相等
				}
			}
			for(i=0;i<GH.vnum;i++)
			{
				outfile<<endl;
				for(j=0;j<GH.vnum;j++)
					outfile1<<GH.arc[i][j]<<endl;

			}
			outfile1.close();
			cout<<"景点信息添加成功"<<endl;
			system("pause");
			system("cls");
}
/*****************最短路径****************/
int Touristmaps::ShortestDistance()
{
	AMGraph G;   //创建对象
	ifstream infile("C://C++//graph.txt",ios::in);
	infile>>G.vnum;
	infile.close();
	int i,j;
	cout<<"请输入要查询的两个景点的数字编号（0->"<<G.vnum-1<<"):"<<endl;
	cin>>i>>j;
    if(!cin)throw string("输入的数据有误！");
	if(i>=G.vnum||i<0||j>=G.vnum||j<0)
	{
		cout<<"您输入的信息有误！"<<endl;
		system("pause");
		system("cls");
	}
	else
	{
		Floyed();      //弗洛伊德算法(求最短路径)
		Print(i,j);    //输出最短路径
	}
	system("pause");
	system("cls");
	return 1;
}
/******************弗洛伊德算法(求最短路径)*******************/
void Touristmaps::Floyed()
{
	int i,j,k;
	AMGraph G;
	ifstream infile1("C://C++//graph.txt",ios::in);
	infile1>>G.vnum;
	infile1.close();
	ifstream infile("C://C++//linjiejuzhen.txt",ios::in);
	for(i=0;i<G.vnum;i++)
		for(j=0;j<G.vnum;j++)
			infile>>G.arc[i][j];
		infile.close();
		for(i=0;i<G.vnum;i++)              //各对结点之间初始已知路径及距离
			for(j=0;j<G.vnum;j++)
			{
				Shortest[i][j]=G.arc[i][j];//将g.arc[i][j]的值赋值到shortest数组中
				Path[i][j]=-1;             //i,j之间无路径，将j的前置为-1
			}
			for(k=0;k<G.vnum;k++)
				for(i=0;i<G.vnum;i++)
					for(j=0;j<G.vnum;j++)
						if(Shortest[i][k]!=MAX_INT&&Shortest[j][k]!=MAX_INT&&Shortest[i][j]>(Shortest[i][k]+Shortest[k][j]))   //从i经k到j的一条路径更短
						{

							Shortest[i][j]=Shortest[i][k]+Shortest[k][j];  //更新shortest[i][j]
							Path[i][j]=k;  //用path[][]记录从i到j的最短路径上j的前驱景点的序号
							Path[j][i]=k;  //更改j的前驱

						}
}
/******************最佳旅行路线(贪心算法：TSP)****************************/
//最佳旅行路线从n出发，最后回到n
void Touristmaps::TSP(AMGraph GH,int n)
{
	ifstream infile("C://C++//graph.txt",ios::in);
	infile>>GH.vnum;
	for(int h=0;h<GH.vnum;h++)
		infile>>GH.v[h].serial>>GH.v[h].name>>GH.v[h].intro;
	infile.close();
	ifstream infile1("C://C++//linjiejuzhen.txt",ios::in);
	for(int i=0;i<GH.vnum;i++)
		for(int j=0;j<GH.vnum;j++)
        infile1>>GH.arc[i][j];
		infile1.close();
	int Pathsum=0;       //存放路径的和
	int Minpath;         //用于存放当前找到的最小路径
	int visited[MAX]={1};//用来记录顶点的访问情况，visited[m]=0,表示未访问
	//                     visited[m]=1表示m点已被访问
	int j=0;             //临时存放顶点编号，以便传递给顶点n
	for(int m=1;m<GH.vnum;m++)//除正门外，所有景点都保证未被访问
		visited[m]=0;
	cout<<"<0>正门";      //从正门出发
	for(int flat=0;flat!=GH.vnum;)
	{
		Minpath=MAX_INT; //初始化Minpath
		for(int k=0;k<GH.vnum;k++) //依次读取与n相邻的路径，取未被访问的最小路径
		{
			if(Minpath>GH.arc[n][k]&&visited[k]==0)//相邻的两个点进行路径大小比较
			{
				Minpath=GH.arc[n][k];    //求出最小路径，逐一比较，确保Minpath的值为最小值
				j=k;
			}
		}
		if(Minpath<MAX_INT) //如果存在最小路径，flat++；记录已访问的个数

		{
			flat++;
			visited[j]=1; //表示j点已访问
			Pathsum=Pathsum+Minpath;  //更新最短路径的长度
			n=j;         //将点交还给n，以便进行下一次循环
			             //让现在所在的点成为下一个循环的出发点
			cout<<"--><"<<n<<">"<<GH.v[n].name;//输出该点的信息
		}
		if(flat==GH.vnum-1)//当已经访问了所有点，把出发点的visited改为0，
			               //即表示未访问，以便寻找并返回正门
		{
			visited[0]=0;
		}
		if(flat==GH.vnum)
			cout<<endl<<"全程:"<<Pathsum<<"km"<<endl; //返回始发点，输出路径长度
	}
	cout<<"**************祝您旅游愉快**************"<<endl;
	system("pause");
	system("cls");
}
/******************查询的输出函数********************/
void Touristmaps::Print(int i,int j)
{
	int a,b;
	a=i;
	b=j;
	cout<<"要查询的两景点间的最短路径如下：\n";
	cout<<"逆序输出所经过的景点的路径为：";
	if(Shortest[i][j]!=MAX_INT)    //路径存在
	{
		if(i<j)
		{
			cout<<b;
			while(Path[i][j]>=0)
			{
				cout<<"<--"<<Path[i][j];  //把i到j的路径上所有经过的景点按逆序打印出
				if(i<j)
					j=Path[i][j];
				else
					i=Path[j][i];
			}
			cout<<"<--"<<a<<endl<<endl;
			cout<<a<<"-->"<<b<<"的最短距离是："<<Shortest[a][b]<<endl;
		}
		else
		{
			cout<<a;
			while(Path[i][j]>=0)
			{
				cout<<"-->"<<Path[i][j];  //把i到j的路径上所有经过的景点按顺序打印出来
				if(i<j)
					j=Path[i][j];
				else
					i=Path[j][i];
			}
			cout<<"-->"<<b<<endl;
			cout<<a<<"-->"<<b<<"的最短距离是："<<Shortest[a][b]<<endl<<endl;
		}
	}
	else
		cout<<"输入错误！不存在此路！\n\n";

}
void Touristmaps::Show()
{
   cout<<"***********[大夫山森林公园地图]*******************"<<endl;
   cout<<endl;
   cout<<"                <7>仙姑庙                         "<<endl;
   cout<<"              4   *   *  *  2                      "<<endl;
   cout<<"                *     *3   *                      "<<endl;
   cout<<"               *      *      *                     "<<endl;
   cout<<"          <6>仙姑瀑布  *   <5>猕猴园              "<<endl;
   cout<<"        1  *  *  *      *    * *                  "<<endl;
   cout<<"          *   *   2*    *  3*    *                "<<endl;
   cout<<"    <4>情人林 *4     *   *  *      *  5            "<<endl;
   cout<<"           *   *     <3>方家茶园     *            "<<endl;
   cout<<"          3 * *          *    *       *           "<<endl;
   cout<<"             **       3 *     *        *         "<<endl;
   cout<<"             <2>梅山山庄    2 *       <1>牛头寨  "<<endl;
   cout<<"                       *       *        *        "<<endl;
   cout<<"                       2   *    *    *  1       "<<endl;
   cout<<"                             <0>正门            "<<endl;
   system("pause");
   system("cls");
}
/**************深度优先遍历函数*****************/
void Touristmaps::DFS(int x,int y)
{
	AMGraph G;
	int i,j;
	static int n=0;
	ifstream infile1("C://C++//graph.txt",ios::in);
	infile1>>G.vnum;
	for(i=0;i<G.vnum;i++)
		infile1>>G.v[i].serial>>G.v[i].name>>G.v[i].intro;
	infile1.close();
	ifstream infile("C://C++//linjiejuzhen.txt",ios::in);
	for(i=0;i<G.vnum;i++)
		for(j=0;j<G.vnum;j++)
			infile>>G.arc[i][j];
		infile.close();
		p1[p1[0]++]=x;    //x加入路径
		if(x==y)          //找到一条路径则输出
		{
			++n;
			cout<<"第"<<n<<"条途径："<<endl;
			for(i=1;i<p1[0];i++)
				cout<<G.v[p1[i]].name<<"-->";
			cout<<"   "<<"其路径长度为："<<cost;
			cout<<endl;
			return;
		}
		for(j=0;j<G.vnum;j++)
		{
			if(G.arc[x][j]!=MAX_INT&&!visited[x]) //其路径存在并且未访问
			{
				cost=cost+G.arc[x][j];   //记录总的路径
				visited[x]=1;            //标记已访问
				DFS(j,y);
				p1[0]--;                 //去掉j
				cost=cost-G.arc[x][j];   //恢复
				visited[x]=0;            //恢复未访问
			}
		}
}
/*****************菜单函数*************************/
void Touristmaps::Menu()
{
    //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_BLUE);
    cout<< "****************************************************";
	cout<< "\n--------欢迎使用大夫山森林公园公园导游图------------" <<endl;
	cout<< "****************************************************" <<endl;
	cout<< "||================================================||" <<endl;
	cout<< "||1.查看所有景点信息   ||    2.查某景点信息       ||" <<endl;
	cout<< "||3.添加景点信息       ||    4.两点间所有路径     ||" <<endl;
	cout<< "||5.两点间最短路径     ||    6.初始化景点信息     ||" <<endl;
	cout<< "||7.公园景点地图       ||    8.最佳旅游路线       ||" <<endl;
	cout<< "||0.退出导游图         ||                         ||" <<endl;
	cout<< "||================================================||" <<endl;
	cout<< "====================================================" <<endl;
	cout<<endl;
}
/*****************景点信息初始化(即清除景点信息)****************************/
void Touristmaps::Start(AMGraph G)
{
	char x;
	cout<<"你是否想初始化（Y/N）?";
	cin>>x;
    if(!cin)throw string("输入的数据有误！");
	if(x=='y'||x=='Y')
	{
		ofstream outfile("C://C++//graph.txt",ios::out);
		G.vnum=0;
		outfile<<G.vnum;
		outfile.close();
		ofstream outfile1("C://C++//linjiejuzhen.txt",ios::out);
		outfile.close();
		cout<<"初始化成功！"<<endl;
	}
	else
		cout<<"你没有初始化！"<<endl;
	    system("pause");
		system("cls");
}
void main()
{
	AMGraph GH;
	int x,y,Password=0;
	char Select[10];
	Touristmaps Map;
	cout<<"正在加载程序，请稍后";
	for(int i=0;i<30;i++)
	{
	cout<<"..";
	Sleep(100);
	}
	system("cls");
    Sleep(1000);
    Map.Menu();
	while(Password==0)
	{
		int m;
		cout<<"\t\t请输入密码：";
		cin>>m;
		if(!cin)throw("您输入的数据不正确！");
		if(m==123456)
		{
			Password=1;
			cout<<endl;
			system("cls");
			break;
		}
		else
		{
			cout<<"\n\t\t您输入的密码不正确，请重新输入！\n"<<endl;
		}
	}
	while(Password==1)
	{
		Map.Menu();
		ifstream infile("C://C++//graph.txt",ios::in);
		infile>>GH.vnum;
        cout<<"                 公园景点概括                  "<<endl;
		for(int i=0;i<GH.vnum;i++)
		{
			infile>>GH.v[i].serial>>GH.v[i].name>>GH.v[i].intro;
			cout<<setw(21)<<"<"<<i<<">"<<GH.v[i].name<<endl;
		}
		infile.close();
		cout<<" *****************************************************"<<endl;
		cout<<"请输入你需要功能按钮（0-8）：";
		int choose;
		cin>>choose;
		switch(choose)
		{
		case 1:
			{
				Map.Display(GH);
				break;
			}
		case 2:
		{
			cout<<"请输入你需要查看的选项(景点的编序或者名称)：";
			cin>>Select;
            if(!cin)throw string("输入的数据有误！");
			Map.Scanf(GH,Select);
			break;
		}
	case 3:
		{
			Map.Add(GH);
			break;
		}
	case 4:
		{
			ifstream infile("C://C++//graph.txt",ios::in);
			infile>>GH.vnum;
			infile.close();
			cout<<"请输入要查询的两个景点的数字编号：（0->"<<GH.vnum-1<<")："<<endl;
			cin>>x>>y;
            if(!cin)throw string("输入的数据有误！");
			if(x>=GH.vnum||x<0||y>=GH.vnum||y<0)
			{
				cout<<"输入错误或景点重复！\n\n";
				system("pause");
				system("cls");
				break;

			}
			else
			{
				p1[0]=1;
				Map.DFS(x,y);  //调用DFS函数遍历路径
				system("pause");
				system("cls");
				break;
			}
		}
	case 5:
		{
			Map.ShortestDistance();
			break;
		}
	case 6:
		{
			Map.Start(GH);
			break;
		}
	case 7:
		{
			Map.Show();
			break;
		}
	case 8:
		{
            cout<<"其最佳旅游路线为(不重复访问各个景点并回到起点的最佳路径)："<<endl;
			Map.TSP(GH,0);
			break;
		}
	case 0:
		system("cls");
		cout<<"┏━━━━━━━━━━━━━━━━━━━━━━┓"<<endl;
		cout<<"  谢谢使用，祝您旅途愉快！"<<endl;
		cout<<"┗━━━━━━━━━━━━━━━━━━━━━━┛"<<endl;
		exit(1);
		system("pause");
	default:
		cout<<"您的输入有误，请重新输入！"<<endl;
		system("pause");
		system("cls");
		}
	}
}