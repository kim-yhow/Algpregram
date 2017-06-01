#include "Solution.h"
#include <fstream>
#include <cstring>

using namespace std;

Solution::Solution(char *filename)
{
	for(int i=0;i<26;i++)
	{
		mflag[i].flag=false;
		mflag[i].index=0;
	}
	LoadWenFile(filename);
	LoadMyVn();
	theFirst();
	theFollow();

}

//�����ķ��ļ�
bool Solution::LoadWenFile(char *filename)
{
	ifstream file(filename);
	string temp="";
	if(!file.is_open())
	{
		cout<<"���棺�޷���Ŀ���ļ�����ȷ���ļ��Ƿ���ڣ�"<<endl;
		exit(1);
		return false;
	}
	while(file>>temp)
	{
		mSource.push_back(temp);
	}
	return true;
}

//���ط��ս��
bool Solution::LoadMyVn()
{
	myVn Vn;
	int i=0;
	
	//itrΪ�ķ��ĵ�����
	for(vector<string>::iterator itr=mSource.begin();itr!=mSource.end();itr++)
	{

		//�����ж�Vn�Ƿ����ظ���
		if(!mflag[(*itr)[0]-'A'].flag)
		{
			//��ʾ�Ѿ�����
			mflag[(*itr)[0]-'A'].flag=true;
			mflag[(*itr)[0]-'A'].index=i;
			Vn.Value=(*itr)[0];
			Vn.hasnull=false;
			//��VnΪ��ͷ���ķ�����������ṹ����
			Vn.RelateWenf.clear();
			Vn.RelateWenf.push_back(*itr);
			mVns.push_back(Vn);
			i++;
		}
		else
		{
			//�����Vn��ͷ���ķ���Vn�Ѿ����ֹ�������ͨ��index���ҵ������λ��
			mVns[mflag[(*itr)[0]-'A'].index].RelateWenf.push_back(*itr);
		}	

	}
	//y���ڼ�¼��ʱ�����ķ�������
	unsigned y=0;
	//��¼���ս����λ��
	pos temPos;
	for(vector<string>::iterator itr=mSource.begin();itr!=mSource.end();itr++)
	{
		for(unsigned x=0;x<(*itr).size();x++)
		{
			//���������ֵ�Ƿ��ս��
			if(isVn((*itr)[x]))
			{
				temPos.x=x;
				temPos.y=y;
				mVns[mflag[(*itr)[x]-'A'].index].wPs.push_back(temPos);
		
		//		cout<<mVns[mflag[(*itr)[x]-'A'].index].Value<<"��"<<"("<<temPos.y<<","<<temPos.x<<")"<<endl;
			}
			else if(isVt((*itr)[x]) && (*itr)[x]!='@' )
			{
				mInput.push_back((*itr)[x]);				
			}
		}
		y++;
	}
	sort(mInput.begin(),mInput.end());
	mInput.erase(unique(mInput.begin(),mInput.end()),mInput.end());
	mInput.push_back('#');
	return true;
}

//first������ÿ���ķ�����������ÿ�����ս�������棬����Ҫ�����Ǹĵ�һ�ķ������в����ս����һ��������Ҫ�÷��ս�����в��Ҽ�����thefirst��Vn��
void Solution::theFirst()
{
	for(vector<myVn>::iterator itr=mVns.begin();itr!=mVns.end();itr++)
	{
		AnalyFirst(*itr);
	}
}

//�õ�ĳ�����ս����first��
void Solution::AnalyFirst(myVn VnTmp)
{
	int xVns;
	unsigned start,end;
	unsigned i;
	string wenTmp;
	string wf;
	for(vector<string>::iterator it =VnTmp.RelateWenf.begin();it!=VnTmp.RelateWenf.end();it++)
	{
		if((*it)[0]!=VnTmp.Value)
		{
			cout<<VnTmp.Value<<"�����ķ�����"<<endl;
			exit(1);
		}
		start=2;
		end=2;
		i=2;
		bool FindVt=false;				//һ����ʶ�������ж��Ƿ���ͬһ���ķ�������˵���ķ����Ƿ��ж��ţ�������Ҫ���²��ҷ��ս��		
		wenTmp=*it;
		while(i<wenTmp.length())
		{
			//���¿�ʼ��һ�仰
			if(wenTmp[i]==',' || wenTmp[i]=='|')
			{
				start=i+1;
				FindVt=false;
			}				
			else if(!FindVt && isVt(wenTmp[i]))
			{
				//��Ϊ�õ���Ԫ�����ս��������ֱ�Ӽ���first��
				while(wenTmp[end]!=',' && end<wenTmp.length())
				{
					end++;
				}
				wf.assign(wenTmp,start,end-start);
				setFirst(VnTmp.Value,wenTmp[i],wf);
				FindVt=true;
			}
			else if(!FindVt && isVn(wenTmp[i]))
			{
				//�õ���ǰ�����ķ��ս���ڷ��ս�������е��±�
				xVns=mflag[wenTmp[i]-'A'].index;
				AnalyFirst(mVns[xVns]);
				//��������ս����first����������Ҫ���first������
					while(wenTmp[end]!=',' && end<wenTmp.length())
				{
					end++;
				}
				wf.assign(wenTmp,start,end-start);
				setFirst(VnTmp.Value,mVns[xVns],wf);
				if(!mVns[xVns].hasnull)
					FindVt=true;
			}
			i++;
		}
	}
}


//�ж��Ƿ����ս��
bool Solution::isVt(char tmp)
{
	if(tmp>='A'&& tmp<='Z')
	{
		return false;
	}
	else if(tmp==',' || tmp== '|' || tmp==':')
	{
		return false;
	}
	return true;
}

//�ж��Ƿ��Ƿ��ս��
bool Solution::isVn(char tmp)
{
	if(tmp>='A'&&tmp<='Z')
	{
		return true;
	}
	return false;
}

//�����н�����Լ���first��������
void Solution::setFirst(char Vn,char Vt,string wf)
{
	int i=mflag[Vn-'A'].index;
	//mVn[�±꣬��mflag��indexֵ���].�÷��ս����first��.����һ��vt
	if(Vt=='@')
	{
		mVns[i].hasnull=true;
	}
	for(vector<char>::iterator its=mVns[i].first.begin();its!=mVns[i].first.end();its++)
	{
		if(Vt==*its)
			return;
	}
	mVns[i].first.push_back(Vt);
	if(Vt!='@')
	{
		setM(Vt,wf,i);

	}
}

//�����н�����Լ���first��������
void Solution::setFirst(char Vn,myVn Vntmp,string wf)
{
	bool hasTheVt=false;
	int i=mflag[Vn-'A'].index;
	char Vt;
	//mVn[�±꣬��mflag��indexֵ���].�÷��ս����first��.����һ��vt
	if(Vntmp.hasnull)
	{
		mVns[i].hasnull=true;
	}
	for(vector<char>::iterator ix=Vntmp.first.begin();ix!=Vntmp.first.end();ix++)
	{
		//�Ƚ��Ƿ��Ѿ��и��ս����
		Vt=*ix;
		for(vector<char>::iterator its=mVns[i].first.begin();its!=mVns[i].first.end();its++)
		{
			if(Vt==*its)
			{
				hasTheVt=true;
				break;
			}
		}
		//û�������
		if(!hasTheVt)
		{
			mVns[i].first.push_back(Vt);
			setM(Vt,wf,i);
		}
		hasTheVt=false;
}
}
//��������Ԥ���
void Solution::setM(char Vt,string wf,unsigned i)
{
		
		M temp;
		temp.vt=Vt;
		temp.wenfa=wf;
		mVns[i].Fun.push_back(temp);
}

//��ĳ�����ս����first���ϴ�ӡ����		
void Solution::displayFirst(char Vn)
{
	cout<<Vn<<"��first����";
	for(vector<char>::iterator it =mVns[mflag[Vn-'A'].index].first.begin();
		it!=mVns[mflag[Vn-'A'].index].first.end();it++)
	{
		cout<<*it<<" "; 
	}
	cout<<endl;
}


//�����е�first��ȫ����ӡ����
void Solution::displayAllFirst()
{
	for(unsigned i=0;i<mVns.size();i++)
	{
		displayFirst(mVns[i].Value);
	}
	cout<<endl;
}


//��ӡ�������ķ�
void Solution::displayWenfa()
{
	cout<<"���β��������ķ�Ϊ��"<<endl;
	for(unsigned i=0;i<mSource.size();i++)	
	{
		cout<<mSource[i]<<endl;
	}
	cout<<endl;
}

//�õ�follow��
void Solution::theFollow()
{	
	//�������з��ս����follow
	for(unsigned itr=0;itr<mVns.size();itr++)
	{
		AnalyFollow(&(mVns[itr]),itr);
	}
	AnalyFollow();

}
//����follow
void Solution::AnalyFollow(myVn* mVn,unsigned y)
{
	myVn* Vn=mVn;
	char tmp;
	//����֮ǰ�Ѿ�����ĸ÷��ս�����ķ��е�λ��
	for(vector<pos>::iterator itp=(*mVn).wPs.begin();itp<(*mVn).wPs.end();itp++)
	{
		if((*itp).x==0&&(*itp).y==0)
		{
			setFollow(mVn,'#');
			continue;
		}
		//���ķ��У���mSource���ҵ�
		if((*itp).x+1 <mSource[(*itp).y].size() && mSource[(*itp).y][(*itp).x+1]!=',')
		{
			tmp=mSource[(*itp).y][(*itp).x+1];
			if(isVt(tmp))
			{
				setFollow(mVn,tmp);
				continue;
			}
			if(isVn(tmp))
			{
				Vn=&(mVns[mflag[tmp-'A'].index]);
				setFollow(mVn,Vn);
				continue;
			}
		}
		else
		{
			tmp=mSource[(*itp).y][0];
			setinFollowVn(mVn,&(mVns[mflag[tmp-'A'].index]));
		}
		
	}
}

//���������ս������follow����
void Solution::setFollow(myVn* mVn,char vt)
{
	mVn->follow.push_back(vt);
}

//�����ս������follow�� 
void Solution::setFollow(myVn* mVn,myVn* VnTmp)
{
	//������ķ��ս����first���ӽ���
	mVn->follow.insert(mVn->follow.end(),VnTmp->first.begin(),VnTmp->first.end());
	//����пմ�
	if(VnTmp->hasnull)
	{
	//ȥ���մ�
	 vector<char>::iterator it=find(mVn->follow.begin(),mVn->follow.end(),'@');
	 if(it!=mVn->follow.end()) mVn->follow.erase(it);

	//Ӧ�ð�VnTmp������ս����follow���ӵ�mVn�У��������ڲ���֪��VnTmp��follow����ɶ
	 setinFollowVn(mVn,VnTmp);
	}
	//ȥ����ͬ��Ԫ��
	sort(mVn->follow.begin(),mVn->follow.end());
	mVn->follow.erase(unique(mVn->follow.begin(),mVn->follow.end()),mVn->follow.end());

}

//��ĳ�����ս����Follow���ϴ�ӡ����
void Solution::displayFollow(char Vn)			
{
	cout<<Vn<<"��follow����";
	for(vector<char>::iterator it =mVns[mflag[Vn-'A'].index].follow.begin();
		it!=mVns[mflag[Vn-'A'].index].follow.end();it++)
	{
		cout<<*it<<" "; 
	}
	cout<<endl;
}


//�����е�Follow��ȫ����ӡ����
void Solution::displayAllFollow()
{
	for(unsigned i=0;i<mVns.size();i++)
	{
		displayFollow(mVns[i].Value);
	}
	cout<<endl;
}

//����followVn��
void Solution::setinFollowVn(myVn* mVn,myVn* VnTmp)
{
	for(unsigned i=0;i<mVn->FollowVn.size();i++)
	{
		if(VnTmp->Value== (*mVn).FollowVn[i]->Value)
			return;
	}
	mVn->FollowVn.push_back(VnTmp);
}



//����һ����ջ��û�н��в�����Vn�������ǵ�follow��
void Solution::AnalyFollow()
{
	for(unsigned i=0;i<mVns.size();i++)
	{
		for(unsigned j=0;j!=mVns[i].FollowVn.size();j++)
		{
			//�����ս����follow���ϼ���÷��ս����
			mVns[i].follow.insert(mVns[i].follow.end(),
							mVns[i].FollowVn[j]->follow.begin(),mVns[i].FollowVn[j]->follow.end());
		}
		//ȥ����ͬ������
		sort(mVns[i].follow.begin(),mVns[i].follow.end());
		mVns[i].follow.erase(unique(mVns[i].follow.begin(),mVns[i].follow.end()),
														mVns[i].follow.end());
	}

}

//����Ԥ�������
void Solution::getPreAnalysisTable()
{
	string s;
	cout<<"\t";
	for(unsigned i=0; i<mInput.size();i++)
	{
		cout<<mInput[i]<<"\t";
	}

	cout<<endl;
	for(unsigned j=0;j<mVns.size();j++)
	{
		cout<<mVns[j].Value<<"\t";
		for(unsigned i=0;i<mInput.size();i++)
		{
			s=getM(mVns[j],mInput[i]);
			if(s.compare("")==0)
			{
				cout<<"error"<<"\t";
			}
			else{
				cout<<mVns[j].Value<<":"<<s<<"\t";
			}
		}
		cout<<endl;

	}


}


//�õ�Ԥ�������ֵ
string Solution::getM(myVn mVns,char Vt)
{
	string s="";
	for(unsigned i=0;i<mVns.Fun.size();i++)
		{
			if(mVns.Fun[i].vt==Vt)
			{
				s=mVns.Fun[i].wenfa;
			}
	}
	if(mVns.hasnull)
		for(unsigned j=0;j<mVns.follow.size();j++)
		{
			if(mVns.follow[j]==Vt)
			{				
				if(s.compare("")==0)
					return "@";
				else
					return s+","+mVns.Value+":@";
			}
		}
	return s;
}