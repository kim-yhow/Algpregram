#ifndef _SoL_H_
#define _SoL_H_
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>
using namespace std;
//用于记录非终结符在文法中出现的位置
struct pos{
	unsigned x;		//记录非终结符的水平位置
	unsigned y;		//记录竖直位置
};

struct M{
	string wenfa;
	char vt;
};


//非终界符结构体
struct myVn{
	char Value;
	bool hasnull;
	vector<pos> wPs;			//记录非终结符在文法中的位置
	vector<char> first;
	vector<char> follow;
	vector<myVn*> FollowVn;		//用于保存需要加入元素的follow
	vector<string> RelateWenf;
	vector<M> Fun;				//一个得到预测分析表的函数

};

struct SG{
	int index;			//每个非终结符下标
	bool flag;			//是否已经存在该终结符的文法
};


class Solution{
public:
	Solution(char *filename);
	bool LoadWenFile(char* filename);					//加载文法文件
	void displayWenfa();								//打印出所有文法

	bool LoadMyVn();				    	//加载非终结符文件			
	bool isVn(char tmp);
	bool isVt(char tmp);								//判断是否是终结符
	
	void theFirst();									//得到first集
	void AnalyFirst(myVn VnTmp);		
	void setFirst(char Vn,char Vt,string wf);						//将单纯的终结符加入first集合
	void setFirst(char Vn,myVn VnTmp,string wf);					//将非终结符与自己的first集绑定起来
	void displayFirst(char Vn);							//将某个非终结符的first集合打印出来	
	void displayAllFirst();								//将所有的first集全部打印出来
	
	void theFollow();									//得到follow集
	void AnalyFollow();									//将follow集加入到另一个的follow中
	void AnalyFollow(myVn* VnTmp,unsigned y);									
	void setFollow(myVn* mVn,char Vt);						//将单纯的终结符加入follow集合
	void setFollow(myVn* mVn,myVn *VnTmp);						//非终结符加入follow集合
	void setinFollowVn(myVn* mVn,myVn *VnTmp);							//放入followVn中
	void displayFollow(char Vn);							//将某个非终结符的Follow集合打印出来	
	void displayAllFollow();								//将所有的Follow集全部打印出来

	void setM(char vt,string wf,unsigned i);
	string getM(myVn mVns,char Vt);							//得到预测分析表值
	void getPreAnalysisTable();								//生成预测分析表

private:
	vector<string> mSource;								//文法						
	vector<myVn> mVns;									//非终结符
	vector<char> mInput;
	SG mflag[26];										//用于判断非终结符是否重复

};

#endif