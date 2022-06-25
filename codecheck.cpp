#include"Code.h"
#include<string>
#include<iostream>
#include<vector>
#include<iomanip>
using namespace std;

int main(int argc, char* argv[]) {
	string str1, str2;
	vector<int> hashvalue1;
	vector<int> hashvalue2;
	vector<int> Flag1;
	vector<int> Flag2;
	double result;
	str1 = DeleteAnnotation(argv[1]);
	str2 = DeleteAnnotation(argv[2]);
	hashvalue1 = HashValue(str1);
	hashvalue2 = HashValue(str2);
	Flag1 = FlagValue(hashvalue1);
	Flag2 = FlagValue(hashvalue2);
	result = Simility(Flag1, Flag2);
	cout << fixed << setprecision(4) << result << endl;
	return 0;
}