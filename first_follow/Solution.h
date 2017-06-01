#ifndef _SoL_H_
#define _SoL_H_
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>
using namespace std;
//���ڼ�¼���ս�����ķ��г��ֵ�λ��
struct pos{
	unsigned x;		//��¼���ս����ˮƽλ��
	unsigned y;		//��¼��ֱλ��
};

struct M{
	string wenfa;
	char vt;
};


//���ս���ṹ��
struct myVn{
	char Value;
	bool hasnull;
	vector<pos> wPs;			//��¼���ս�����ķ��е�λ��
	vector<char> first;
	vector<char> follow;
	vector<myVn*> FollowVn;		//���ڱ�����Ҫ����Ԫ�ص�follow
	vector<string> RelateWenf;
	vector<M> Fun;				//һ���õ�Ԥ�������ĺ���

};

struct SG{
	int index;			//ÿ�����ս���±�
	bool flag;			//�Ƿ��Ѿ����ڸ��ս�����ķ�
};


class Solution{
public:
	Solution(char *filename);
	bool LoadWenFile(char* filename);					//�����ķ��ļ�
	void displayWenfa();								//��ӡ�������ķ�

	bool LoadMyVn();				    	//���ط��ս���ļ�			
	bool isVn(char tmp);
	bool isVt(char tmp);								//�ж��Ƿ����ս��
	
	void theFirst();									//�õ�first��
	void AnalyFirst(myVn VnTmp);		
	void setFirst(char Vn,char Vt,string wf);						//���������ս������first����
	void setFirst(char Vn,myVn VnTmp,string wf);					//�����ս�����Լ���first��������
	void displayFirst(char Vn);							//��ĳ�����ս����first���ϴ�ӡ����	
	void displayAllFirst();								//�����е�first��ȫ����ӡ����
	
	void theFollow();									//�õ�follow��
	void AnalyFollow();									//��follow�����뵽��һ����follow��
	void AnalyFollow(myVn* VnTmp,unsigned y);									
	void setFollow(myVn* mVn,char Vt);						//���������ս������follow����
	void setFollow(myVn* mVn,myVn *VnTmp);						//���ս������follow����
	void setinFollowVn(myVn* mVn,myVn *VnTmp);							//����followVn��
	void displayFollow(char Vn);							//��ĳ�����ս����Follow���ϴ�ӡ����	
	void displayAllFollow();								//�����е�Follow��ȫ����ӡ����

	void setM(char vt,string wf,unsigned i);
	string getM(myVn mVns,char Vt);							//�õ�Ԥ�������ֵ
	void getPreAnalysisTable();								//����Ԥ�������

private:
	vector<string> mSource;								//�ķ�						
	vector<myVn> mVns;									//���ս��
	vector<char> mInput;
	SG mflag[26];										//�����жϷ��ս���Ƿ��ظ�

};

#endif