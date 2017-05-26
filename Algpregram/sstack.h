#ifndef SEQSTACK_H
#define SEQSTACK_H
#include<iostream>
using namespace std;



template<class T,int MaxSize>
class SStack
{
	T data[MaxSize];
	int top;
public:
	SStack();			//构造函数
	void push(T x);		//入栈
	T Pop();			//出栈
	T Top();			//取栈顶元素（元素并不出栈）
	bool isEmpty();		//判断是否为空
	int gettop();		//返回此时指针
};

#endif