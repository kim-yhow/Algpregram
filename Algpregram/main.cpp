#include<iostream>
#include<string>
#include"Algpregram.h"
using namespace std;

int main()
{
	Algpregram alg("wenfa.txt");	
	alg.displayWenfa();
	alg.displayFirstVT();
	alg.displaylastVT();
	alg.displayPriority();
	cout<<endl;
	cout<<"����һ���ķ����в���,��#����"<<endl;
	string s;
	cin>>s;
	alg.Calcu(s);
	return 0;
}