#include<iostream>
#include"sstack.h"
using namespace std;


///////////////////////////////////////////////构造函数
template<class T,int MaxSize>
 SStack<T,MaxSize>::SStack()
{
	top=-1;
}
//入栈
template<class T,int MaxSize>
void SStack<T, MaxSize>::push(T x)
{
	if(top==MaxSize-1)
	{
		cerr<<"上溢";exit(0);
	}
	top++;
	data[top]=x;
}
//出栈
template<class T,int MaxSize>
T SStack<T, MaxSize>::Pop()
{
	T x;
	if(top==-1)
	{
		cerr<<"下溢";exit(1);
	}
	x=data[top];
	top--;
	return x;
}
//取栈顶元素（元素并不出栈）
template<class T,int MaxSize>
T SStack<T, MaxSize>::Top()
{
	if(top==-1)
	{
		cerr<<"下溢";exit(1);
	}

	return data[top];
}

/////////////////////////////////////////////判断栈是否为空
template<class T,int MaxSize>
bool SStack<T, MaxSize>::isEmpty()
{
	if(top==-1)
		 return true;
	return false;
}

/////////////////////////////////////////////返回指针
template<class T,int MaxSize>
int SStack<T, MaxSize>::gettop()
{
	 return top;
}

