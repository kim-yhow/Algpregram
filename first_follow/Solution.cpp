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

//加载文法文件
bool Solution::LoadWenFile(char *filename)
{
	ifstream file(filename);
	string temp="";
	if(!file.is_open())
	{
		cout<<"警告：无法打开目标文件，请确认文件是否存在！"<<endl;
		exit(1);
		return false;
	}
	while(file>>temp)
	{
		mSource.push_back(temp);
	}
	return true;
}

//加载非终结符
bool Solution::LoadMyVn()
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
			Vn.Value=(*itr)[0];
			Vn.hasnull=false;
			//将Vn为开头的文法保存在这个结构体中
			Vn.RelateWenf.clear();
			Vn.RelateWenf.push_back(*itr);
			mVns.push_back(Vn);
			i++;
		}
		else
		{
			//如果以Vn开头的文法中Vn已经出现过，则先通过index查找到具体的位置
			mVns[mflag[(*itr)[0]-'A'].index].RelateWenf.push_back(*itr);
		}	

	}
	//y用于记录此时读到文法的行数
	unsigned y=0;
	//记录非终结符的位置
	pos temPos;
	for(vector<string>::iterator itr=mSource.begin();itr!=mSource.end();itr++)
	{
		for(unsigned x=0;x<(*itr).size();x++)
		{
			//如果读到的值是非终结符
			if(isVn((*itr)[x]))
			{
				temPos.x=x;
				temPos.y=y;
				mVns[mflag[(*itr)[x]-'A'].index].wPs.push_back(temPos);
		
		//		cout<<mVns[mflag[(*itr)[x]-'A'].index].Value<<"："<<"("<<temPos.y<<","<<temPos.x<<")"<<endl;
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

//first集现在每个文法都被保存在每个非终结符的下面，下面要做的是改掉一文法来进行查找终结符这一个做法。要用非终结符进行查找即类如thefirst（Vn）
void Solution::theFirst()
{
	for(vector<myVn>::iterator itr=mVns.begin();itr!=mVns.end();itr++)
	{
		AnalyFirst(*itr);
	}
}

//得到某个非终结符的first集
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
			cout<<VnTmp.Value<<"关联文法出错"<<endl;
			exit(1);
		}
		start=2;
		end=2;
		i=2;
		bool FindVt=false;				//一个标识，用于判断是否是同一个文法，或者说该文法中是否有逗号，有则需要重新查找非终结符		
		wenTmp=*it;
		while(i<wenTmp.length())
		{
			//重新开始的一句话
			if(wenTmp[i]==',' || wenTmp[i]=='|')
			{
				start=i+1;
				FindVt=false;
			}				
			else if(!FindVt && isVt(wenTmp[i]))
			{
				//因为得到的元素是终结符，所有直接加入first集
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
				//得到当前读到的非终结符在非终结符集合中的下标
				xVns=mflag[wenTmp[i]-'A'].index;
				AnalyFirst(mVns[xVns]);
				//将这个非终结符的first集加入我们要求的first集合中
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


//判断是否是终结符
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

//判断是否是非终结符
bool Solution::isVn(char tmp)
{
	if(tmp>='A'&&tmp<='Z')
	{
		return true;
	}
	return false;
}

//将非中介符与自己的first集绑定起来
void Solution::setFirst(char Vn,char Vt,string wf)
{
	int i=mflag[Vn-'A'].index;
	//mVn[下标，有mflag的index值存放].该非终结符的first集.加入一个vt
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

//将非中介符与自己的first集绑定起来
void Solution::setFirst(char Vn,myVn Vntmp,string wf)
{
	bool hasTheVt=false;
	int i=mflag[Vn-'A'].index;
	char Vt;
	//mVn[下标，有mflag的index值存放].该非终结符的first集.加入一个vt
	if(Vntmp.hasnull)
	{
		mVns[i].hasnull=true;
	}
	for(vector<char>::iterator ix=Vntmp.first.begin();ix!=Vntmp.first.end();ix++)
	{
		//比较是否已经有该终结符，
		Vt=*ix;
		for(vector<char>::iterator its=mVns[i].first.begin();its!=mVns[i].first.end();its++)
		{
			if(Vt==*its)
			{
				hasTheVt=true;
				break;
			}
		}
		//没有则加入
		if(!hasTheVt)
		{
			mVns[i].first.push_back(Vt);
			setM(Vt,wf,i);
		}
		hasTheVt=false;
}
}
//用于生成预测表
void Solution::setM(char Vt,string wf,unsigned i)
{
		
		M temp;
		temp.vt=Vt;
		temp.wenfa=wf;
		mVns[i].Fun.push_back(temp);
}

//将某个非终结符的first集合打印出来		
void Solution::displayFirst(char Vn)
{
	cout<<Vn<<"的first集：";
	for(vector<char>::iterator it =mVns[mflag[Vn-'A'].index].first.begin();
		it!=mVns[mflag[Vn-'A'].index].first.end();it++)
	{
		cout<<*it<<" "; 
	}
	cout<<endl;
}


//将所有的first集全部打印出来
void Solution::displayAllFirst()
{
	for(unsigned i=0;i<mVns.size();i++)
	{
		displayFirst(mVns[i].Value);
	}
	cout<<endl;
}


//打印出所有文法
void Solution::displayWenfa()
{
	cout<<"本次测试所有文法为："<<endl;
	for(unsigned i=0;i<mSource.size();i++)	
	{
		cout<<mSource[i]<<endl;
	}
	cout<<endl;
}

//得到follow集
void Solution::theFollow()
{	
	//分析所有非终结符的follow
	for(unsigned itr=0;itr<mVns.size();itr++)
	{
		AnalyFollow(&(mVns[itr]),itr);
	}
	AnalyFollow();

}
//分析follow
void Solution::AnalyFollow(myVn* mVn,unsigned y)
{
	myVn* Vn=mVn;
	char tmp;
	//利用之前已经求出的该非终结符在文法中的位置
	for(vector<pos>::iterator itp=(*mVn).wPs.begin();itp<(*mVn).wPs.end();itp++)
	{
		if((*itp).x==0&&(*itp).y==0)
		{
			setFollow(mVn,'#');
			continue;
		}
		//在文法中，即mSource中找到
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

//将单纯的终结符加入follow集合
void Solution::setFollow(myVn* mVn,char vt)
{
	mVn->follow.push_back(vt);
}

//将非终结符加入follow中 
void Solution::setFollow(myVn* mVn,myVn* VnTmp)
{
	//将后面的非终结符的first集加进来
	mVn->follow.insert(mVn->follow.end(),VnTmp->first.begin(),VnTmp->first.end());
	//如果有空串
	if(VnTmp->hasnull)
	{
	//去掉空串
	 vector<char>::iterator it=find(mVn->follow.begin(),mVn->follow.end(),'@');
	 if(it!=mVn->follow.end()) mVn->follow.erase(it);

	//应该把VnTmp这个非终结符的follow集加到mVn中，但是现在并不知道VnTmp的follow集是啥
	 setinFollowVn(mVn,VnTmp);
	}
	//去掉相同的元素
	sort(mVn->follow.begin(),mVn->follow.end());
	mVn->follow.erase(unique(mVn->follow.begin(),mVn->follow.end()),mVn->follow.end());

}

//将某个非终结符的Follow集合打印出来
void Solution::displayFollow(char Vn)			
{
	cout<<Vn<<"的follow集：";
	for(vector<char>::iterator it =mVns[mflag[Vn-'A'].index].follow.begin();
		it!=mVns[mflag[Vn-'A'].index].follow.end();it++)
	{
		cout<<*it<<" "; 
	}
	cout<<endl;
}


//将所有的Follow集全部打印出来
void Solution::displayAllFollow()
{
	for(unsigned i=0;i<mVns.size();i++)
	{
		displayFollow(mVns[i].Value);
	}
	cout<<endl;
}

//放入followVn中
void Solution::setinFollowVn(myVn* mVn,myVn* VnTmp)
{
	for(unsigned i=0;i<mVn->FollowVn.size();i++)
	{
		if(VnTmp->Value== (*mVn).FollowVn[i]->Value)
			return;
	}
	mVn->FollowVn.push_back(VnTmp);
}



//将第一次入栈而没有进行操作的Vn查找他们的follow集
void Solution::AnalyFollow()
{
	for(unsigned i=0;i<mVns.size();i++)
	{
		for(unsigned j=0;j!=mVns[i].FollowVn.size();j++)
		{
			//将非终结符的follow集合加入该非终结符中
			mVns[i].follow.insert(mVns[i].follow.end(),
							mVns[i].FollowVn[j]->follow.begin(),mVns[i].FollowVn[j]->follow.end());
		}
		//去掉相同的内容
		sort(mVns[i].follow.begin(),mVns[i].follow.end());
		mVns[i].follow.erase(unique(mVns[i].follow.begin(),mVns[i].follow.end()),
														mVns[i].follow.end());
	}

}

//生成预测分析表
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


//得到预测分析表值
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