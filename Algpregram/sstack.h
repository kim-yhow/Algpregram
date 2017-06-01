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
	SStack();			//���캯��
	void push(T x);		//��ջ
	T Pop();			//��ջ
	T Top();			//ȡջ��Ԫ�أ�Ԫ�ز�����ջ��
	bool isEmpty();		//�ж��Ƿ�Ϊ��
	int gettop();		//���ش�ʱָ��
};

#endif