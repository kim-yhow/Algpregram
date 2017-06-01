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
	cout<<"输入一串文法进行测试,以#结束"<<endl;
	string s;
	cin>>s;
	alg.Calcu(s);
	return 0;
}