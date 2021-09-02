/*
����������

*/
#include<iostream>
#include<fstream>
#include<iomanip>
#include<stdlib.h>
#include<windows.h>
using namespace std;
#define MAX_INT 10000  //�����
#define MAX 100  //���ֵ
#define OK 1
#define ERROR 0
#define FLOWOVER -1
int visited[MAX]={0};  //��������(��־����)0Ϊδ����
int p1[MAX];//ǰ������
int cost;  //��¼�ܵ�·��
int Path[MAX][MAX];//��path[][]��¼��i��j�����·����j��ǰ����������
int Shortest[MAX][MAX];//��������(�������G.arc[][]�е�ֵ)
struct vex
{
	char name[MAX];    //���������
	char serial[MAX];  //����ı���
	char intro[MAX];  //����ļ��
};
/**************����ͼ���ڽӾ���洢��ʾ********************/
struct AMGraph
{
	vex v[MAX];        //�������Ϣ(�����)
	int arc[MAX][MAX];  //�ڽӾ���
	int vnum;         //����ĸ���
};
//**********************��ͼ��***********************
class Touristmaps
{
public:
	void Display(AMGraph);      //��ʾ���о�����Ϣ�ĺ���
	void Scanf(AMGraph,char*);  //���Ҿ�����Ϣ�ĺ���
	void Add(AMGraph &);        //���Ӿ�����Ϣ�ĺ���
	void Start(AMGraph);        //��ʼ��������Ϣ�ĺ���
	int ShortestDistance();     //��������·���ĺ���
	void Floyed();              //���������㷨�������������·����
	void Print(int i,int j);    //�������(��ӡ����������·������̾���)
	void Menu();                //�˵�����
	void DFS(int x,int y);      //��������㷨
	void Show();                //�����ͼ��ʾ����
	void TSP(AMGraph,int n);    //�������·��
};
/*************��ʾ������Ϣ����****************/
void Touristmaps::Display(AMGraph GH)
{
	int i;
	int count=1;
	cout<<"             ����      ����       ���"<<endl;
	ifstream infile("C://C++//graph.txt",ios::in);//�����뷽ʽ���ļ�
	infile>>GH.vnum;
	for(i=0;i<GH.vnum;i++)
	{
		infile>>GH.v[i].serial>>GH.v[i].name>>GH.v[i].intro;
		cout<<"������"<<count<<":     "<<GH.v[i].serial<<"       "<<
			GH.v[i].name<<"       "<<GH.v[i].intro<<endl;
		cout<<endl;
		count++;
	}
	infile.close();           //�ر��ļ���
	cout<<endl;
	system("pause");
	system("cls");           //����
}

/**************���Һ���****************************/
void Touristmaps::Scanf(AMGraph GH,char*Select)
{
	int obj;
	bool exist=0;       //0Ϊδ�ҵ���1Ϊ�ҵ�
	ifstream infile("C://C++//graph.txt",ios::in);
	infile>>GH.vnum;
	for(int i=0;i<GH.vnum;i++)
	{
		infile>>GH.v[i].serial>>GH.v[i].name>>GH.v[i].intro;
		if(strcmp(GH.v[i].name,Select)==0||strcmp(GH.v[i].serial,Select)==0)   //�ַ����ıȽ�
		{
			obj=i;
			exist=1;
		}
	}
	if(exist==0)
		cout<<"δ�ҵ���ѯ�������Ϣ��"<<endl;
	else
	{
		cout<<"��Ҫ��ѯ����Ϣ���£�"<<endl;
		cout<<"�������"<<GH.v[obj].serial<<endl;
		cout<<"�������ƣ�"<<GH.v[obj].name<<endl;
		cout<<"������Ϣ��"<<GH.v[obj].intro<<endl;
	}
	infile.close();
	system("pause");
	system("cls");
}
/**************���Ӿ�����Ϣ********************/
void Touristmaps::Add(AMGraph &GH)
{
	int i,j,count,f;
	ifstream infile("C://C++//graph.txt",ios::in);
	if(!infile)
	{
		cout<<"���ļ�ʧ�ܣ�"<<endl;
		exit(1);      //���������г������˳�����
	}
	infile>>GH.vnum;
	f=GH.vnum;
	for(i=0;i<GH.vnum;i++)
		infile>>GH.v[i].serial>>GH.v[i].name>>GH.v[i].intro;
	ofstream outfile2("C://C++//graph.txt",ios::out);      //������ķ�ʽ���ļ�
	cout<<"�����뾰����Ŀ��";
	cin>>count;
	if(!cin)throw string("�������������");
	if(GH.vnum+count>MAX)
	{
		cout<<"������ľ�����Ŀ������Χ������ʧ�ܣ�"<<endl;
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
	ofstream outfile("C://C++//graph.txt",ios::app);     //������ķ�ʽ���ļ��������ӵ����ݷŵ��ļ�ĩβ
	cout<<"���������뾰��������ƣ���飺"<<endl;
	for(i=f;i<GH.vnum;i++)
	{
		cout<<"��"<<i+1<<"����Ϣ�ֱ�Ϊ��";
		cin>>GH.v[i].serial>>GH.v[i].name>>GH.v[i].intro;
        if(!cin)throw string("�������������");
		outfile<<GH.v[i].serial<<' '<<GH.v[i].name<<' '<<GH.v[i].intro<<endl;
	}
	outfile.close();
	ifstream infile1("C://C++//linjiejuzhen.txt",ios::in);
	for(i=0;i<f;i++)
		for(j=0;j<f;j++)
			infile1>>GH.arc[i][j];     //���ļ����ݷ����ڽӾ���
		infile1.close();
		ofstream outfile1("C://C++//linjiejuzhen.txt",ios::out);
		for(i=f;i<GH.vnum;i++)        //��ʼ���ڽӾ���
			for(j=0;j<GH.vnum;j++)
			{
				GH.arc[i][j]=MAX_INT;
				GH.arc[j][i]=MAX_INT;
			}
			cout<<"�뾰����ص���Ϣ����:"<<endl;
			for(i=f;i<GH.vnum;i++)
			{
				cout<<"��������"<<GH.v[i].name<<"�������ڵ�����Ȩֵ(û��·��ʱ����10000)"<<endl;
				for(j=0;j<GH.vnum&&j!=i;j++)
				{
					cout<<"������"<<GH.v[i].name<<"��"<<GH.v[j].name<<"����Ȩֵ"<<endl;
					cin>>GH.arc[i][j];
                    if(!cin)throw string("�������������");
					GH.arc[j][i]=GH.arc[i][j];     //����ͼ���ڽӾ���j��i��i��j���
				}
			}
			for(i=0;i<GH.vnum;i++)
			{
				outfile<<endl;
				for(j=0;j<GH.vnum;j++)
					outfile1<<GH.arc[i][j]<<endl;

			}
			outfile1.close();
			cout<<"������Ϣ��ӳɹ�"<<endl;
			system("pause");
			system("cls");
}
/*****************���·��****************/
int Touristmaps::ShortestDistance()
{
	AMGraph G;   //��������
	ifstream infile("C://C++//graph.txt",ios::in);
	infile>>G.vnum;
	infile.close();
	int i,j;
	cout<<"������Ҫ��ѯ��������������ֱ�ţ�0->"<<G.vnum-1<<"):"<<endl;
	cin>>i>>j;
    if(!cin)throw string("�������������");
	if(i>=G.vnum||i<0||j>=G.vnum||j<0)
	{
		cout<<"���������Ϣ����"<<endl;
		system("pause");
		system("cls");
	}
	else
	{
		Floyed();      //���������㷨(�����·��)
		Print(i,j);    //������·��
	}
	system("pause");
	system("cls");
	return 1;
}
/******************���������㷨(�����·��)*******************/
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
		for(i=0;i<G.vnum;i++)              //���Խ��֮���ʼ��֪·��������
			for(j=0;j<G.vnum;j++)
			{
				Shortest[i][j]=G.arc[i][j];//��g.arc[i][j]��ֵ��ֵ��shortest������
				Path[i][j]=-1;             //i,j֮����·������j��ǰ��Ϊ-1
			}
			for(k=0;k<G.vnum;k++)
				for(i=0;i<G.vnum;i++)
					for(j=0;j<G.vnum;j++)
						if(Shortest[i][k]!=MAX_INT&&Shortest[j][k]!=MAX_INT&&Shortest[i][j]>(Shortest[i][k]+Shortest[k][j]))   //��i��k��j��һ��·������
						{

							Shortest[i][j]=Shortest[i][k]+Shortest[k][j];  //����shortest[i][j]
							Path[i][j]=k;  //��path[][]��¼��i��j�����·����j��ǰ����������
							Path[j][i]=k;  //����j��ǰ��

						}
}
/******************�������·��(̰���㷨��TSP)****************************/
//�������·�ߴ�n���������ص�n
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
	int Pathsum=0;       //���·���ĺ�
	int Minpath;         //���ڴ�ŵ�ǰ�ҵ�����С·��
	int visited[MAX]={1};//������¼����ķ��������visited[m]=0,��ʾδ����
	//                     visited[m]=1��ʾm���ѱ�����
	int j=0;             //��ʱ��Ŷ����ţ��Ա㴫�ݸ�����n
	for(int m=1;m<GH.vnum;m++)//�������⣬���о��㶼��֤δ������
		visited[m]=0;
	cout<<"<0>����";      //�����ų���
	for(int flat=0;flat!=GH.vnum;)
	{
		Minpath=MAX_INT; //��ʼ��Minpath
		for(int k=0;k<GH.vnum;k++) //���ζ�ȡ��n���ڵ�·����ȡδ�����ʵ���С·��
		{
			if(Minpath>GH.arc[n][k]&&visited[k]==0)//���ڵ����������·����С�Ƚ�
			{
				Minpath=GH.arc[n][k];    //�����С·������һ�Ƚϣ�ȷ��Minpath��ֵΪ��Сֵ
				j=k;
			}
		}
		if(Minpath<MAX_INT) //���������С·����flat++����¼�ѷ��ʵĸ���

		{
			flat++;
			visited[j]=1; //��ʾj���ѷ���
			Pathsum=Pathsum+Minpath;  //�������·���ĳ���
			n=j;         //���㽻����n���Ա������һ��ѭ��
			             //���������ڵĵ��Ϊ��һ��ѭ���ĳ�����
			cout<<"--><"<<n<<">"<<GH.v[n].name;//����õ����Ϣ
		}
		if(flat==GH.vnum-1)//���Ѿ����������е㣬�ѳ������visited��Ϊ0��
			               //����ʾδ���ʣ��Ա�Ѱ�Ҳ���������
		{
			visited[0]=0;
		}
		if(flat==GH.vnum)
			cout<<endl<<"ȫ��:"<<Pathsum<<"km"<<endl; //����ʼ���㣬���·������
	}
	cout<<"**************ף���������**************"<<endl;
	system("pause");
	system("cls");
}
/******************��ѯ���������********************/
void Touristmaps::Print(int i,int j)
{
	int a,b;
	a=i;
	b=j;
	cout<<"Ҫ��ѯ�������������·�����£�\n";
	cout<<"��������������ľ����·��Ϊ��";
	if(Shortest[i][j]!=MAX_INT)    //·������
	{
		if(i<j)
		{
			cout<<b;
			while(Path[i][j]>=0)
			{
				cout<<"<--"<<Path[i][j];  //��i��j��·�������о����ľ��㰴�����ӡ��
				if(i<j)
					j=Path[i][j];
				else
					i=Path[j][i];
			}
			cout<<"<--"<<a<<endl<<endl;
			cout<<a<<"-->"<<b<<"����̾����ǣ�"<<Shortest[a][b]<<endl;
		}
		else
		{
			cout<<a;
			while(Path[i][j]>=0)
			{
				cout<<"-->"<<Path[i][j];  //��i��j��·�������о����ľ��㰴˳���ӡ����
				if(i<j)
					j=Path[i][j];
				else
					i=Path[j][i];
			}
			cout<<"-->"<<b<<endl;
			cout<<a<<"-->"<<b<<"����̾����ǣ�"<<Shortest[a][b]<<endl<<endl;
		}
	}
	else
		cout<<"������󣡲����ڴ�·��\n\n";

}
void Touristmaps::Show()
{
   cout<<"***********[���ɽɭ�ֹ�԰��ͼ]*******************"<<endl;
   cout<<endl;
   cout<<"                <7>�ɹ���                         "<<endl;
   cout<<"              4   *   *  *  2                      "<<endl;
   cout<<"                *     *3   *                      "<<endl;
   cout<<"               *      *      *                     "<<endl;
   cout<<"          <6>�ɹ��ٲ�  *   <5>⨺�԰              "<<endl;
   cout<<"        1  *  *  *      *    * *                  "<<endl;
   cout<<"          *   *   2*    *  3*    *                "<<endl;
   cout<<"    <4>������ *4     *   *  *      *  5            "<<endl;
   cout<<"           *   *     <3>���Ҳ�԰     *            "<<endl;
   cout<<"          3 * *          *    *       *           "<<endl;
   cout<<"             **       3 *     *        *         "<<endl;
   cout<<"             <2>÷ɽɽׯ    2 *       <1>ţͷկ  "<<endl;
   cout<<"                       *       *        *        "<<endl;
   cout<<"                       2   *    *    *  1       "<<endl;
   cout<<"                             <0>����            "<<endl;
   system("pause");
   system("cls");
}
/**************������ȱ�������*****************/
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
		p1[p1[0]++]=x;    //x����·��
		if(x==y)          //�ҵ�һ��·�������
		{
			++n;
			cout<<"��"<<n<<"��;����"<<endl;
			for(i=1;i<p1[0];i++)
				cout<<G.v[p1[i]].name<<"-->";
			cout<<"   "<<"��·������Ϊ��"<<cost;
			cout<<endl;
			return;
		}
		for(j=0;j<G.vnum;j++)
		{
			if(G.arc[x][j]!=MAX_INT&&!visited[x]) //��·�����ڲ���δ����
			{
				cost=cost+G.arc[x][j];   //��¼�ܵ�·��
				visited[x]=1;            //����ѷ���
				DFS(j,y);
				p1[0]--;                 //ȥ��j
				cost=cost-G.arc[x][j];   //�ָ�
				visited[x]=0;            //�ָ�δ����
			}
		}
}
/*****************�˵�����*************************/
void Touristmaps::Menu()
{
    //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_BLUE);
    cout<< "****************************************************";
	cout<< "\n--------��ӭʹ�ô��ɽɭ�ֹ�԰��԰����ͼ------------" <<endl;
	cout<< "****************************************************" <<endl;
	cout<< "||================================================||" <<endl;
	cout<< "||1.�鿴���о�����Ϣ   ||    2.��ĳ������Ϣ       ||" <<endl;
	cout<< "||3.��Ӿ�����Ϣ       ||    4.���������·��     ||" <<endl;
	cout<< "||5.��������·��     ||    6.��ʼ��������Ϣ     ||" <<endl;
	cout<< "||7.��԰�����ͼ       ||    8.�������·��       ||" <<endl;
	cout<< "||0.�˳�����ͼ         ||                         ||" <<endl;
	cout<< "||================================================||" <<endl;
	cout<< "====================================================" <<endl;
	cout<<endl;
}
/*****************������Ϣ��ʼ��(�����������Ϣ)****************************/
void Touristmaps::Start(AMGraph G)
{
	char x;
	cout<<"���Ƿ����ʼ����Y/N��?";
	cin>>x;
    if(!cin)throw string("�������������");
	if(x=='y'||x=='Y')
	{
		ofstream outfile("C://C++//graph.txt",ios::out);
		G.vnum=0;
		outfile<<G.vnum;
		outfile.close();
		ofstream outfile1("C://C++//linjiejuzhen.txt",ios::out);
		outfile.close();
		cout<<"��ʼ���ɹ���"<<endl;
	}
	else
		cout<<"��û�г�ʼ����"<<endl;
	    system("pause");
		system("cls");
}
void main()
{
	AMGraph GH;
	int x,y,Password=0;
	char Select[10];
	Touristmaps Map;
	cout<<"���ڼ��س������Ժ�";
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
		cout<<"\t\t���������룺";
		cin>>m;
		if(!cin)throw("����������ݲ���ȷ��");
		if(m==123456)
		{
			Password=1;
			cout<<endl;
			system("cls");
			break;
		}
		else
		{
			cout<<"\n\t\t����������벻��ȷ�����������룡\n"<<endl;
		}
	}
	while(Password==1)
	{
		Map.Menu();
		ifstream infile("C://C++//graph.txt",ios::in);
		infile>>GH.vnum;
        cout<<"                 ��԰�������                  "<<endl;
		for(int i=0;i<GH.vnum;i++)
		{
			infile>>GH.v[i].serial>>GH.v[i].name>>GH.v[i].intro;
			cout<<setw(21)<<"<"<<i<<">"<<GH.v[i].name<<endl;
		}
		infile.close();
		cout<<" *****************************************************"<<endl;
		cout<<"����������Ҫ���ܰ�ť��0-8����";
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
			cout<<"����������Ҫ�鿴��ѡ��(����ı����������)��";
			cin>>Select;
            if(!cin)throw string("�������������");
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
			cout<<"������Ҫ��ѯ��������������ֱ�ţ���0->"<<GH.vnum-1<<")��"<<endl;
			cin>>x>>y;
            if(!cin)throw string("�������������");
			if(x>=GH.vnum||x<0||y>=GH.vnum||y<0)
			{
				cout<<"�������򾰵��ظ���\n\n";
				system("pause");
				system("cls");
				break;

			}
			else
			{
				p1[0]=1;
				Map.DFS(x,y);  //����DFS��������·��
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
            cout<<"���������·��Ϊ(���ظ����ʸ������㲢�ص��������·��)��"<<endl;
			Map.TSP(GH,0);
			break;
		}
	case 0:
		system("cls");
		cout<<"������������������������������������������������"<<endl;
		cout<<"  ллʹ�ã�ף����;��죡"<<endl;
		cout<<"������������������������������������������������"<<endl;
		exit(1);
		system("pause");
	default:
		cout<<"���������������������룡"<<endl;
		system("pause");
		system("cls");
		}
	}
}