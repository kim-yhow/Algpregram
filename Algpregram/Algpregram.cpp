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
//初始化数组
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


//加载文法
bool Algpregram::LoadWenfa(char *filename)
{
	string temp="";
	ifstream file(filename);
	if(!file.is_open())
	{
		cout<<filename<<"文件无法打开"<<endl;
		exit(1);
	}
	while(file>>temp)
	{
		mSource.push_back(temp);
	}
	return true;
}

//打印出所有文法
void Algpregram::displayWenfa()
{
	cout<<"本次测试所有文法为："<<endl;
	for(unsigned i=0;i<mSource.size();i++)	
	{
		cout<<mSource[i]<<endl;
	}
}


//判断是否是终结符
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


//判断是否是非终结符
bool Algpregram::isVn(char tmp)
{
	if(tmp>='A'&&tmp<='Z')
	{
		return true;
	}
	return false;
}


//加载非终结符文件	
bool Algpregram::LoadMyVn()
{	
	myVn Vn;
	int i=0;
	//itr为文法的迭代器
	for(vector<string>::iterator itr=mSource.begin();itr!=mSource.end();itr++)
	{
		//用于判断Vn是否是重复的
		if(!mflag[(*itr)[0]-'A'].flag)
		{
			//表示已经加载
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
			//如果以Vn开头的文法中Vn已经出现过，则先通过index查找到具体的位置
			mVns[mflag[(*itr)[0]-'A'].index].RelateWenf.push_back(*itr);
		}	
	}
	return true;
}

//入栈
void Algpregram::pushInStack(int Vn_index,int Vt_index)
{
	Newp temp(Vn_index,Vt_index);
	newps.push_back(temp);
}

//判断该非终结符是否已经保存
bool Algpregram::isVtSaved(char vt)
{
	for(vector<myVt>::iterator itr=mVts.begin();itr<mVts.end();itr++)
	{
		if(vt==itr->value)
			return true;
	}
	return false;
}

//设置Vtmp 
void Algpregram::setVtmp(myVt &Vtmp,unsigned i,string wentemp)
{
		if((i+1)<wentemp.size())
		{
			//如果文法后面一个是终结符，则两者优先级相等
			if(isVt(wentemp[i+1]))
				Vtmp.equalVT.push_back(wentemp[i+1]);
			//如果后面是非终结符，则优先级中，所有该非终结符的firstVT都比此时这个vt大			
			else 
				if(isVn(wentemp[i+1]))
				{
					Vtmp.firstVN.push_back(wentemp[i+1]);
					//如果后面有一个非终结符，同时非终结符后面还有一个vt，则这两者相等
					if((i+2)<wentemp.size() && isVt(wentemp[i+2]))
							Vtmp.equalVT.push_back(wentemp[i+2]);					
				}
		}
		if((i-1)>=0)
		{
			//如果是前面一位是非终结符则该非终结符的所有LastVt都比 vt优先级大
			if(isVn(wentemp[i-1]))
			{
				Vtmp.lastVN.push_back(wentemp[i-1]);
			}
		}

}

//保存vt ,i是 该vt在文法中的位置，wentemp为该vt 所在文法
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


//返回Vt的下表
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
	cout<<Vt<<"不合法！！，请输入正确的文法终结符号，包括";
	for(unsigned i=0;i<mVts.size();i++)
	{
		cout<<mVts[i].value<<" ";
	}
	cout<<endl;
	exit(1);
}

//打印firstVT
void Algpregram::displayFirstVT()
{
	for(vector<myVn>::iterator it=mVns.begin();it<mVns.end();it++)
	{
		cout<<it->Value<<" 的FirstVT集为"<<":";
		for(vector<char>::iterator i=it->firstVT.begin();i<it->firstVT.end();i++)
		{
			cout<<*i<<" ";
		}
		cout<<endl;
	}
}
//得到FirstVT
void Algpregram::GetFirstVT()
{
	initial();
	for(vector<myVn>::iterator itr=mVns.begin();itr<mVns.end();itr++)
										AnalyFirstVT(&(*itr));		
	//如果栈不为空
	while(mindexp<newps.size())
	{
								//mVns(vector类)[在newps数组[下表].非终结符的下标]
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


//分析FirstVT
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
		//重新开始的一句话
		if(wenTmp[i]==',' || wenTmp[i]=='|')
		{
			FindVt=false;					
		}
		else if(!FindVt)	
		{
		//如果是非终结符，直接加入该非终结符的firstVt中
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
				//如果不是本身 则保存生成wenTmp的父辈 
				if(wenTmp[i]!=mVn->Value)
					mVns[mflag[wenTmp[i]-'A'].index].ParentVn.push_back(mVn);
				//判断此时之后的以为是否已经超过了该文法长度,没有就可以继续
				if(++i<wenTmp.length())
				{					
				//判断是否是E->Ta...形式
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
							cout<<"不是算符文法！";
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



//得到LastVt
void Algpregram::GetLastVT()
{
	initial();
	for(vector<myVn>::iterator itr=mVns.begin();itr<mVns.end();itr++)
										AnalyLastVT(&(*itr));	
	while(mindexp<newps.size())
	{
									//mVns(vector类)[在newps数组[下表].非终结符的下标]
		for(vector<myVn*>::iterator it=mVns[newps[mindexp].Vn_index].ParentVn.begin();										
										it<mVns[newps[mindexp].Vn_index].ParentVn.end();it++)
		{			
			if(!m_isLastVT[(*it)->index][newps[mindexp].Vt_index])
			{

				(*it)->lastVT.push_back(mVts[newps[mindexp].Vt_index].value);
				//将此时（Vn，Vt）设置为true
				m_isLastVT[(*it)->index][newps[mindexp].Vt_index]=true;
				//将此时生成的true点进栈（Vn，Vt）
				pushInStack((*it)->index,newps[mindexp].Vt_index);
			}			
		}
			mindexp++;
	}

}


//打印lastVT
void Algpregram::displaylastVT()
{
	for(vector<myVn>::iterator it=mVns.begin();it<mVns.end();it++)
	{
		cout<<it->Value<<" 的LastVT集为"<<":";
		for(vector<char>::iterator i=it->lastVT.begin();i<it->lastVT.end();i++)
		{
			cout<<*i<<" ";
		}
		cout<<endl;
	}
}


//分析LastVT		
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
			//重新开始的一句话
			if((*it)[i]==',' || (*it)[i]=='|')
			{
				FindVt=false;					
			}
			else if(!FindVt)	
			{
			//如果是非终结符，直接加入该非终结符的lastVt中
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
					//如果不是本身 则保存生成wenTmp的父辈 
					if(wenTmp[i]!=mVn->Value)
						mVns[mflag[wenTmp[i]-'A'].index].ParentVn.push_back(mVn);
					//判断此时之后的以为是否已经超过了该文法长度,没有就可以继续
					if(--i>-1)
					{					
					//判断是否是E->...aT形式
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
							cout<<"不是算符文法！";
							exit(1);
						}
					}
					}
				}
			i--;
		}
	}
}

//设置优先级
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
			//m为位于终结符后面的非终结符的迭代器如 .....aT....中的T 下面的操作是将T中的所有FirstVT都比a优先级高
			for(vector<char>::iterator mx=mVns[mflag[(*m)-'A'].index].firstVT.begin();mx<mVns[mflag[(*m)-'A'].index].firstVT.end();mx++)
			{
				mVT_priority[i][getIndexOf(*mx)]=SMALLER;
			}
		}

		for(vector<char>::iterator n=it->lastVN.begin();n<it->lastVN.end();n++)
		{
			//n为位于终结符前面的非终结符的迭代器如 .....Ta....中的T 下面的操作是将T中的所有LastVT都比a优先级高
			for(vector<char>::iterator nx=mVns[mflag[(*n)-'A'].index].lastVT.begin();nx<mVns[mflag[(*n)-'A'].index].lastVT.end();nx++)
			{
				mVT_priority[getIndexOf(*nx)][i]=BIGGER;
			}
		}
	}
}
//打印优先级
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


//测试并计算	
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



//测试文法是否合法
void Algpregram::Testwenfa(string str)
{
	int tempv;
	Item temp(OPERATE,'#');
	WENFA.push(temp);
	unsigned i=0;
	//算法不结束，一直运行
	while(i<mItems.size())
	{	
		Item chis=WENFA.Top();
		temp=mItems[i];
		//如果读到了E?
		/*if(chis.flag=='E')
		{
			SUM.push(WENFA.Pop());				
			chis=WENFA.Top();
			if(chis.flag=='E')
			{
				cout<<str.substr(0,str.size()-1)<<"文法不合法"<<endl;
				exit(0);
			}
		}*/
		//判断终结符的优先级
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
				//当第一个出现优先级小于时
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
					case ')':cout<<"没有足够的左括号与右括号匹配"<<endl;exit(0);break;
					case '(':cout<<"没有足够的右括号与左括号匹配"<<endl;exit(1);break;
					break;
					}
				}
				break;
	
			default: 
					if(mItems[i].flag=='#' && WENFA.gettop()==0)
					{					
						cout<<str.substr(0,str.size()-1)<<"是合法文法"<<endl;
						if(!SUM.isEmpty())
						cout<<"计算结果:"<<SUM.Pop().value<<endl;
					}
					else
					{	
						cout<<str.substr(0,str.size()-1)<<"不合法！"<<endl;
						exit(1);
					}
			}
		i++;
	}	
}