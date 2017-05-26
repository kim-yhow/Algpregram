#ifndef _ALG_H_
#define _ALG_H_
#include "sstack.cpp"
#include<iostream>
#include<vector>



using namespace std;
#define SUM_VN	 5
#define SUM_VT   7
#define FSH		 6
#define ERROR   -1
#define BIGGER   1
#define SMALLER  2
#define EQUAL    3
#define OPERATE -99999999		//操作符



//非终界符结构体
struct myVn{
	char Value;
	int index;			//每个非终结符下标
	vector<string> RelateWenf;
	vector<myVn*> ParentVn;
	vector<char> firstVT;
	vector<char> lastVT;
};


struct myVt{
	char value;
	int index;
	

	vector<char> firstVN;
	vector<char> lastVN;
	vector<char> equalVT;
	myVt(char Va,unsigned i):value(Va),index(i){}
};

//判断非终结符是否被使用
struct SG{
	int index;			//每个非终结符下标
	bool flag;			//是否已经存在该终结符的文法

};

class Item
{
public:
	int value;
	char flag;
	void set(int i,char d)
	{
		value=i;
		flag=d;
	}
	Item (){};
	Item(int i,char d):value(i),flag(d){}
};

struct Newp
{
	int Vn_index;
	int Vt_index;
	Newp(int vn,int vt):Vn_index(vn),Vt_index(vt){}
};



class Algpregram
{
public:
	Algpregram(char *filename);
	bool LoadWenfa(char *filename);
	bool LoadMyVn();									//加载非终结符文件			
	void displayWenfa();								//打印出所有文法
	void initial();										//初始化

	bool isVn(char tmp);								//判断是否是非终结符
	bool isVt(char tmp);								//判断是否是终结符
	bool isVtSaved(char vt);							//判断终结符是否被保存
	int getIndexOf(char Vt);							//返回Vt的下表
	
	void GetFirstVT();									//得到FirstVT
	void AnalyFirstVT(myVn* mVn);						//分析FirstVT
	void pushInStack(int Vn,int Vt);					//入栈
	void setVt(char vt,unsigned i,string wentemp);			//保存vt
	void setVtmp(myVt &Vtmp,unsigned i,string wentemp);						//设置Vtmp
	
	void displayFirstVT();								//打印firstVT
		
	void GetLastVT();									//得到LastVt
	void AnalyLastVT(myVn* mVn);						//分析LastVT					
	void displaylastVT();								//打印LastVT

	void setPriority();									//设置优先级
	void displayPriority();								//打印优先级

	int Calcu(string str);								//测试并计算
	void Testwenfa(string str);							//测试文法是否合法


private:
	bool m_isFirstVT[SUM_VN][SUM_VT];								//用于比较Vn与Vt对是否是FirstVt
	bool m_isLastVT[SUM_VN][SUM_VT];														//用于比较Vn与Vt是否是LastVT
	int mVT_priority[SUM_VT][SUM_VT];							//终结符的优先比较

	bool FindVt;									//句子结束
	SG mflag[26];										//表示26个字母
	vector<string> mSource;								//文法		
	vector<myVn> mVns;									//保存非终结符
	vector<myVt> mVts;									//保存非终结符
	vector<Newp> newps;						//新生成的FirstVT点栈
	vector<Item> mItems;					//存放标识内容
	unsigned mindexp;										//作为栈的指针
	unsigned vt_i;										//非终结符计数

	SStack<Item,100> WENFA;						//文法栈
	SStack<Item,100> SUM;							//存放计算结果*/
};
#endif