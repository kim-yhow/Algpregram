#include <iostream>
#include <fstream>
#include <vector>
#include "Solution.h"
using namespace std;

int main()
{
	Solution s("wftest.txt");
	s.displayWenfa();
	s.displayAllFirst();
	s.displayAllFollow();
	s.getPreAnalysisTable();
	return 0;
}