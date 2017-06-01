#include "Algpregram.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

Algpregram::Algpregram(char * filename)
{	
		for(int m=0;m<SUM_VN;m++)
			for(int n=0;n<SUM_VT;n++)			
		{
			m_isFirstVT[m][n]=false;
			m_isLastVT[m][n]=false;
		}
	for(int j=0;j<SUM_VT;j++)
		for(int t=0;t<SUM_VT;t++)
		{
			if(j==SUM_VT-1 && t!=SUM_VT-1)
				mVT_priority[j][t]=SMALLER;
			else if(t==SUM_VT-1 && j!=SUM_VT-1)
				mVT_priority[j][t]=BIGGER;
			else
				mVT_priority[j][t]=ERROR;
		}
		
	for(int i=0;i<26;i++)
	{
		mflag[i].flag=false;
		mflag[i].index=0;
	}
	vt_i=0;
	FindVt=false;
	LoadWenfa(filename);
	LoadMyVn();
	GetFirstVT();
	GetLastVT();
	setPriority();
}
//��ʼ������
void Algpregram::initial()
{
	FindVt=false;

	for(vector<myVn>::iterator itr=mVns.begin();itr<mVns.end();itr++)
	{
		itr->ParentVn.clear();
	}
	newps.clear();
	mindexp=0;
}


//�����ķ�
bool Algpregram::LoadWenfa(char *filename)
{
	string temp="";
	ifstream file(filename);
	if(!file.is_open())
	{
		cout<<filename<<"�ļ��޷���"<<endl;
		exit(1);
	}
	while(file>>temp)
	{
		mSource.push_back(temp);
	}
	return true;
}

//��ӡ�������ķ�
void Algpregram::displayWenfa()
{
	cout<<"���β��������ķ�Ϊ��"<<endl;
	for(unsigned i=0;i<mSource.size();i++)	
	{
		cout<<mSource[i]<<endl;
	}
}


//�ж��Ƿ����ս��
bool Algpregram::isVt(char tmp)
{
	if(tmp>='A'&& tmp<='Z')
	{
		return false;
	}
	else if(tmp==',' || tmp== '|' || tmp==':' ||tmp=='\0' )
	{
		return false;
	}
	return true;
}


//�ж��Ƿ��Ƿ��ս��
bool Algpregram::isVn(char tmp)
{
	if(tmp>='A'&&tmp<='Z')
	{
		return true;
	}
	return false;
}


//���ط��ս���ļ�	
bool Algpregram::LoadMyVn()
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
			Vn.index=i;
			Vn.Value=(*itr)[0];
			(Vn.RelateWenf).push_back(*itr);
			mVns.push_back(Vn);			
			(Vn.RelateWenf).clear();
			i++;
		}		
		else
		{
			//�����Vn��ͷ���ķ���Vn�Ѿ����ֹ�������ͨ��index���ҵ������λ��
			mVns[mflag[(*itr)[0]-'A'].index].RelateWenf.push_back(*itr);
		}	
	}
	return true;
}

//��ջ
void Algpregram::pushInStack(int Vn_index,int Vt_index)
{
	Newp temp(Vn_index,Vt_index);
	newps.push_back(temp);
}

//�жϸ÷��ս���Ƿ��Ѿ�����
bool Algpregram::isVtSaved(char vt)
{
	for(vector<myVt>::iterator itr=mVts.begin();itr<mVts.end();itr++)
	{
		if(vt==itr->value)
			return true;
	}
	return false;
}

//����Vtmp 
void Algpregram::setVtmp(myVt &Vtmp,unsigned i,string wentemp)
{
		if((i+1)<wentemp.size())
		{
			//����ķ�����һ�����ս�������������ȼ����
			if(isVt(wentemp[i+1]))
				Vtmp.equalVT.push_back(wentemp[i+1]);
			//��������Ƿ��ս���������ȼ��У����и÷��ս����firstVT���ȴ�ʱ���vt��			
			else 
				if(isVn(wentemp[i+1]))
				{
					Vtmp.firstVN.push_back(wentemp[i+1]);
					//���������һ�����ս����ͬʱ���ս�����滹��һ��vt�������������
					if((i+2)<wentemp.size() && isVt(wentemp[i+2]))
							Vtmp.equalVT.push_back(wentemp[i+2]);					
				}
		}
		if((i-1)>=0)
		{
			//�����ǰ��һλ�Ƿ��ս����÷��ս��������LastVt���� vt���ȼ���
			if(isVn(wentemp[i-1]))
			{
				Vtmp.lastVN.push_back(wentemp[i-1]);
			}
		}

}

//����vt ,i�� ��vt���ķ��е�λ�ã�wentempΪ��vt �����ķ�
void Algpregram::setVt(char vt,unsigned i,string wentemp)
{	
	
	if(!isVtSaved(vt))
	{  	
		myVt Vtmp(vt,vt_i++);
		setVtmp(Vtmp,i,wentemp);
		mVts.push_back(Vtmp);
	}
	else
		setVtmp (mVts[getIndexOf(vt)],i,wentemp);

}


//����Vt���±�
int Algpregram::getIndexOf(char Vt)
{
	if(Vt=='#')
		return FSH;
	for(vector<myVt>::iterator itr=mVts.begin();itr<mVts.end();itr++)
	{
		if(Vt==itr->value)
		{
			return itr->index;
		}
	}
	cout<<Vt<<"���Ϸ���������������ȷ���ķ��ս���ţ�����";
	for(unsigned i=0;i<mVts.size();i++)
	{
		cout<<mVts[i].value<<" ";
	}
	cout<<endl;
	exit(1);
}

//��ӡfirstVT
void Algpregram::displayFirstVT()
{
	for(vector<myVn>::iterator it=mVns.begin();it<mVns.end();it++)
	{
		cout<<it->Value<<" ��FirstVT��Ϊ"<<":";
		for(vector<char>::iterator i=it->firstVT.begin();i<it->firstVT.end();i++)
		{
			cout<<*i<<" ";
		}
		cout<<endl;
	}
}
//�õ�FirstVT
void Algpregram::GetFirstVT()
{
	initial();
	for(vector<myVn>::iterator itr=mVns.begin();itr<mVns.end();itr++)
										AnalyFirstVT(&(*itr));		
	//���ջ��Ϊ��
	while(mindexp<newps.size())
	{
								//mVns(vector��)[��newps����[�±�].���ս�����±�]
		for(vector<myVn*>::iterator it=mVns[newps[mindexp].Vn_index].ParentVn.begin();										
			it<mVns[newps[mindexp].Vn_index].ParentVn.end();it++)
		{			
			if(!m_isFirstVT[(*it)->index][newps[mindexp].Vt_index])
			{
				(*it)->firstVT.push_back(mVts[newps[mindexp].Vt_index].value);
				m_isFirstVT[(*it)->index][newps[mindexp].Vt_index]=true;
				pushInStack((*it)->index,newps[mindexp].Vt_index);
			}			
		}
		mindexp++;
		
	}
}


//����FirstVT
void Algpregram::AnalyFirstVT(myVn* mVn)
{
	string wenTmp;
	unsigned i=2;
	int tmp=0;
	for(vector<string>::iterator it=mVn->RelateWenf.begin();it<mVn->RelateWenf.end();it++)
	{
		FindVt=false;
		wenTmp=*it;
		while(i<wenTmp.length())
		{
		//���¿�ʼ��һ�仰
		if(wenTmp[i]==',' || wenTmp[i]=='|')
		{
			FindVt=false;					
		}
		else if(!FindVt)	
		{
		//����Ƿ��ս����ֱ�Ӽ���÷��ս����firstVt��
			if(isVt(wenTmp[i]))
			{	
				setVt(wenTmp[i],i,wenTmp);			
				tmp=getIndexOf(wenTmp[i]);
				if(!m_isFirstVT[mVn->index][tmp])
				{
					mVn->firstVT.push_back(wenTmp[i]);						
					m_isFirstVT[mVn->index][tmp]=true;
				}
				pushInStack(mVn->index,tmp);
				FindVt=true;				
			}
			else if(isVn(wenTmp[i]))
			{
				//������Ǳ��� �򱣴�����wenTmp�ĸ��� 
				if(wenTmp[i]!=mVn->Value)
					mVns[mflag[wenTmp[i]-'A'].index].ParentVn.push_back(mVn);
				//�жϴ�ʱ֮�����Ϊ�Ƿ��Ѿ������˸��ķ�����,û�оͿ��Լ���
				if(++i<wenTmp.length())
				{					
				//�ж��Ƿ���E->Ta...��ʽ
					if(isVt(wenTmp[i]))			  
					{
						setVt(wenTmp[i],i,wenTmp);					
						tmp=getIndexOf(wenTmp[i]);
						if(!m_isFirstVT[mVn->index][tmp])
						{
							mVn->firstVT.push_back(wenTmp[i]);						
							m_isFirstVT[mVn->index][tmp]=true;
						}
						pushInStack(mVn->index,tmp);
						FindVt=true;
					}	
					else	
						if(wenTmp[i]=='|' ||wenTmp[i]==',')
							continue;

						else 
						{
							cout<<"��������ķ���";
							exit(1);
						}
				}
			}
			}
			if(isVt(wenTmp[i]))			 
			{
				setVt(wenTmp[i],i,wenTmp);
			}
			i++;
		}
	}
}



//�õ�LastVt
void Algpregram::GetLastVT()
{
	initial();
	for(vector<myVn>::iterator itr=mVns.begin();itr<mVns.end();itr++)
										AnalyLastVT(&(*itr));	
	while(mindexp<newps.size())
	{
									//mVns(vector��)[��newps����[�±�].���ս�����±�]
		for(vector<myVn*>::iterator it=mVns[newps[mindexp].Vn_index].ParentVn.begin();										
										it<mVns[newps[mindexp].Vn_index].ParentVn.end();it++)
		{			
			if(!m_isLastVT[(*it)->index][newps[mindexp].Vt_index])
			{

				(*it)->lastVT.push_back(mVts[newps[mindexp].Vt_index].value);
				//����ʱ��Vn��Vt������Ϊtrue
				m_isLastVT[(*it)->index][newps[mindexp].Vt_index]=true;
				//����ʱ���ɵ�true���ջ��Vn��Vt��
				pushInStack((*it)->index,newps[mindexp].Vt_index);
			}			
		}
			mindexp++;
	}

}


//��ӡlastVT
void Algpregram::displaylastVT()
{
	for(vector<myVn>::iterator it=mVns.begin();it<mVns.end();it++)
	{
		cout<<it->Value<<" ��LastVT��Ϊ"<<":";
		for(vector<char>::iterator i=it->lastVT.begin();i<it->lastVT.end();i++)
		{
			cout<<*i<<" ";
		}
		cout<<endl;
	}
}


//����LastVT		
void Algpregram::AnalyLastVT(myVn* mVn)
{

	int i=0;
	string wenTmp;
	for(vector<string>::iterator it=mVn->RelateWenf.begin();it<mVn->RelateWenf.end();it++)
	{
		FindVt=false;
		int tmp;
		i=(int)(*it).size();
		wenTmp=(*it);
		while(i>-1)
		{
			//���¿�ʼ��һ�仰
			if((*it)[i]==',' || (*it)[i]=='|')
			{
				FindVt=false;					
			}
			else if(!FindVt)	
			{
			//����Ƿ��ս����ֱ�Ӽ���÷��ս����lastVt��
				if(isVt(wenTmp[i]))
				{	
					tmp=getIndexOf(wenTmp[i]);
					if(!m_isLastVT[mVn->index][tmp])
					{
						mVn->lastVT.push_back(wenTmp[i]);						
						m_isLastVT[mVn->index][tmp]=true;
					}
					pushInStack(mVn->index,tmp);
					FindVt=true;				
				}
				else if(isVn(wenTmp[i]))
					{
					//������Ǳ��� �򱣴�����wenTmp�ĸ��� 
					if(wenTmp[i]!=mVn->Value)
						mVns[mflag[wenTmp[i]-'A'].index].ParentVn.push_back(mVn);
					//�жϴ�ʱ֮�����Ϊ�Ƿ��Ѿ������˸��ķ�����,û�оͿ��Լ���
					if(--i>-1)
					{					
					//�ж��Ƿ���E->...aT��ʽ
						if(isVt(wenTmp[i]))			  
						{
							tmp=getIndexOf(wenTmp[i]);
							if(!m_isLastVT[mVn->index][tmp])
							{
								mVn->lastVT.push_back(wenTmp[i]);						
								m_isLastVT[mVn->index][tmp]=true;
							}
							pushInStack(mVn->index,tmp);
							FindVt=true;
						}		
						else	if(wenTmp[i]=='|' ||wenTmp[i]==',')
								continue;
						else 
						{
							cout<<"��������ķ���";
							exit(1);
						}
					}
					}
				}
			i--;
		}
	}
}

//�������ȼ�
void Algpregram::setPriority()
{
	int i;
	for(vector<myVt>::iterator it=mVts.begin();it<mVts.end();it++)
	{
		i=it->index;
		for(vector<char>::iterator x=it->equalVT.begin();x<it->equalVT.end();x++)
					mVT_priority[i][getIndexOf(*x)]=EQUAL;
	
		for(vector<char>::iterator m=it->firstVN.begin();m<it->firstVN.end();m++)
		{
			//mΪλ���ս������ķ��ս���ĵ������� .....aT....�е�T ����Ĳ����ǽ�T�е�����FirstVT����a���ȼ���
			for(vector<char>::iterator mx=mVns[mflag[(*m)-'A'].index].firstVT.begin();mx<mVns[mflag[(*m)-'A'].index].firstVT.end();mx++)
			{
				mVT_priority[i][getIndexOf(*mx)]=SMALLER;
			}
		}

		for(vector<char>::iterator n=it->lastVN.begin();n<it->lastVN.end();n++)
		{
			//nΪλ���ս��ǰ��ķ��ս���ĵ������� .....Ta....�е�T ����Ĳ����ǽ�T�е�����LastVT����a���ȼ���
			for(vector<char>::iterator nx=mVns[mflag[(*n)-'A'].index].lastVT.begin();nx<mVns[mflag[(*n)-'A'].index].lastVT.end();nx++)
			{
				mVT_priority[getIndexOf(*nx)][i]=BIGGER;
			}
		}
	}
}
//��ӡ���ȼ�
void Algpregram::displayPriority()
{
	cout<<"\t";
	for(unsigned i=0;i<mVts.size();i++)
	{
		cout<<mVts[i].value<<"\t";
	}
	cout<<endl;
	for(unsigned i=0;i<mVts.size();i++)
	{
		cout<<mVts[i].value<<"\t";
		for(unsigned j=0;j<mVts.size();j++)
		{
			switch(mVT_priority[i][j])
			{
			case EQUAL:
				cout<<"="<<"\t";
				break;
			case BIGGER:
				cout<<">"<<"\t";
				break;
			case SMALLER:
				cout<<"<"<<"\t";
				break;
			default:
				cout<<" "<<"\t";
				break;

			}
		}
		cout<<endl;

	}
}


//���Բ�����	
int Algpregram::Calcu(string str)
{
	Item s;
	int res=0;
	unsigned t=0;
	while(t<str.size())
	{
		switch (str[t])
		{
		case '+':
			s.set(OPERATE,'+');
			mItems.push_back(s);			
			break;
		case '*':
			s.set(OPERATE,'*');
			mItems.push_back(s);	
			break;
		case '(':
			s.set(OPERATE,'(');
			mItems.push_back(s);	
			break;
		case ')':
			s.set(OPERATE,')');
			mItems.push_back(s);	
			break;
		case '#':
			s.set(OPERATE,'#');
			mItems.push_back(s);	
			break;
		default:
		if(str[t]>='0' && str[t]<='9')
			{
				unsigned start=t;
				while(str[t]>='0' && str[t]<='9')
				{
					++t;
				}
				unsigned end =t;
				Item s(atoi(str.substr(start,end).c_str()),'i');
				mItems.push_back(s);
				t--;				
			}
		break;
		}
		++t;
	}
	Testwenfa(str);
	return res;
}



//�����ķ��Ƿ�Ϸ�
void Algpregram::Testwenfa(string str)
{
	int tempv;
	Item temp(OPERATE,'#');
	WENFA.push(temp);
	unsigned i=0;
	//�㷨��������һֱ����
	while(i<mItems.size())
	{	
		Item chis=WENFA.Top();
		temp=mItems[i];
		//���������E?
		/*if(chis.flag=='E')
		{
			SUM.push(WENFA.Pop());				
			chis=WENFA.Top();
			if(chis.flag=='E')
			{
				cout<<str.substr(0,str.size()-1)<<"�ķ����Ϸ�"<<endl;
				exit(0);
			}
		}*/
		//�ж��ս�������ȼ�
		switch( mVT_priority[getIndexOf(chis.flag)][getIndexOf(temp.flag)])
		{
		case SMALLER: 
			WENFA.push(temp);
				break;
		case EQUAL: WENFA.Pop();
				break;
		case BIGGER:
			temp=WENFA.Top();
			SUM.push(WENFA.Pop());
			chis=WENFA.Top();
				//����һ���������ȼ�С��ʱ
			while(mVT_priority[getIndexOf(chis.flag)][getIndexOf(temp.flag)]!=SMALLER)
				{
					temp=chis;
					SUM.push(WENFA.Pop());
					chis=WENFA.Top();
				}				
				i--;
				if(SUM.Top().value==OPERATE)
				{
					switch(SUM.Pop().flag)
					{
					case '+':tempv=SUM.Pop().value+SUM.Pop().value;
						     temp.set(tempv,'i');
							 SUM.push(temp);break;						
					case '*':tempv=SUM.Pop().value*SUM.Pop().value;
							 temp.set(tempv,'i');
							 SUM.push(temp);break;
					case ')':cout<<"û���㹻����������������ƥ��"<<endl;exit(0);break;
					case '(':cout<<"û���㹻����������������ƥ��"<<endl;exit(1);break;
					break;
					}
				}
				break;
	
			default: 
					if(mItems[i].flag=='#' && WENFA.gettop()==0)
					{					
						cout<<str.substr(0,str.size()-1)<<"�ǺϷ��ķ�"<<endl;
						if(!SUM.isEmpty())
						cout<<"������:"<<SUM.Pop().value<<endl;
					}
					else
					{	
						cout<<str.substr(0,str.size()-1)<<"���Ϸ���"<<endl;
						exit(1);
					}
			}
		i++;
	}	
}