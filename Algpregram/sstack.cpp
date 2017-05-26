#include<iostream>
#include"sstack.h"
using namespace std;


///////////////////////////////////////////////���캯��
template<class T,int MaxSize>
 SStack<T,MaxSize>::SStack()
{
	top=-1;
}
//��ջ
template<class T,int MaxSize>
void SStack<T, MaxSize>::push(T x)
{
	if(top==MaxSize-1)
	{
		cerr<<"����";exit(0);
	}
	top++;
	data[top]=x;
}
//��ջ
template<class T,int MaxSize>
T SStack<T, MaxSize>::Pop()
{
	T x;
	if(top==-1)
	{
		cerr<<"����";exit(1);
	}
	x=data[top];
	top--;
	return x;
}
//ȡջ��Ԫ�أ�Ԫ�ز�����ջ��
template<class T,int MaxSize>
T SStack<T, MaxSize>::Top()
{
	if(top==-1)
	{
		cerr<<"����";exit(1);
	}

	return data[top];
}

/////////////////////////////////////////////�ж�ջ�Ƿ�Ϊ��
template<class T,int MaxSize>
bool SStack<T, MaxSize>::isEmpty()
{
	if(top==-1)
		 return true;
	return false;
}

/////////////////////////////////////////////����ָ��
template<class T,int MaxSize>
int SStack<T, MaxSize>::gettop()
{
	 return top;
}

