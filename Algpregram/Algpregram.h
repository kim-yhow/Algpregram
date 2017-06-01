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
#define OPERATE -99999999		//������



//���ս���ṹ��
struct myVn{
	char Value;
	int index;			//ÿ�����ս���±�
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

//�жϷ��ս���Ƿ�ʹ��
struct SG{
	int index;			//ÿ�����ս���±�
	bool flag;			//�Ƿ��Ѿ����ڸ��ս�����ķ�

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
	bool LoadMyVn();									//���ط��ս���ļ�			
	void displayWenfa();								//��ӡ�������ķ�
	void initial();										//��ʼ��

	bool isVn(char tmp);								//�ж��Ƿ��Ƿ��ս��
	bool isVt(char tmp);								//�ж��Ƿ����ս��
	bool isVtSaved(char vt);							//�ж��ս���Ƿ񱻱���
	int getIndexOf(char Vt);							//����Vt���±�
	
	void GetFirstVT();									//�õ�FirstVT
	void AnalyFirstVT(myVn* mVn);						//����FirstVT
	void pushInStack(int Vn,int Vt);					//��ջ
	void setVt(char vt,unsigned i,string wentemp);			//����vt
	void setVtmp(myVt &Vtmp,unsigned i,string wentemp);						//����Vtmp
	
	void displayFirstVT();								//��ӡfirstVT
		
	void GetLastVT();									//�õ�LastVt
	void AnalyLastVT(myVn* mVn);						//����LastVT					
	void displaylastVT();								//��ӡLastVT

	void setPriority();									//�������ȼ�
	void displayPriority();								//��ӡ���ȼ�

	int Calcu(string str);								//���Բ�����
	void Testwenfa(string str);							//�����ķ��Ƿ�Ϸ�


private:
	bool m_isFirstVT[SUM_VN][SUM_VT];								//���ڱȽ�Vn��Vt���Ƿ���FirstVt
	bool m_isLastVT[SUM_VN][SUM_VT];														//���ڱȽ�Vn��Vt�Ƿ���LastVT
	int mVT_priority[SUM_VT][SUM_VT];							//�ս�������ȱȽ�

	bool FindVt;									//���ӽ���
	SG mflag[26];										//��ʾ26����ĸ
	vector<string> mSource;								//�ķ�		
	vector<myVn> mVns;									//������ս��
	vector<myVt> mVts;									//������ս��
	vector<Newp> newps;						//�����ɵ�FirstVT��ջ
	vector<Item> mItems;					//��ű�ʶ����
	unsigned mindexp;										//��Ϊջ��ָ��
	unsigned vt_i;										//���ս������

	SStack<Item,100> WENFA;						//�ķ�ջ
	SStack<Item,100> SUM;							//��ż�����*/
};
#endif