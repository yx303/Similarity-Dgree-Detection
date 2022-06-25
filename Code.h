#pragma once
#ifndef _CODE_H_
#define _CODE_H_
#include<fstream>
#include<string>
#include<vector>
#include<iostream>
#include<cmath>
#include<sstream>
#include <algorithm>
#include<regex>
using namespace std;
#define K 10//切片长度
#define W 10//特征值滑窗

struct Dict {//避免重复选取特征值
	int value;//元素值
	bool used;//是否已选为特征值
};

string DeleteAnnotation(string path) {//删除注释
	string str = "";
	char c1, c2;
	ifstream in;
	in.open(path, ios::in);
	if (!in) return "文件打开异常!";
	while (in.get(c1)) {
		if (c1 == '/') {
			in.get(c2);
			if (c2 == '/') {
				while (in.get(c1)) {
					if (c1 == '\n') break;
					continue;
				}
			}
			else if (c2 == '*') {
				while (in.get(c1)) {
					if (c1 == '*') {
						in.get(c1);
						if (c1 == '/') {
							in.get(c1);
							break;
						}
					}
					continue;
				}
			}
		}
		else if (c1 == '"') {
			while (in.get(c2)) {
				if (c2 == '"') {
					in.get(c1);
					break;
				}
				continue;
			}
		}
		if (c1 != '\n' && /*c1 != ' ' && */c1 != '\t') str += c1;
	}
	in.close();
	vector<string> List = { "void","int","char","float","double","bool","long",
		"short","unsigned","true","false","const","inline","auto","static","extern",
		"register","for","while","if","else","do","switch","case","default","break",
		"continue","return","goto","new","delete","sizeof","class","struct","enum",
		"union","typedef","this","friend","virtual","mutable","explicit","operator",
		"private","protected","public","template","typename","namespace","using",
		"catch","throw","try","main","cout","cin" ,"return","include","define",
		"std","endl","regex" };
	string aim_str = "", mid_str = "";
	for (int i = 0; i < str.length(); i++) {
		if ((int(str[i]) > 64 && int(str[i]) < 91) || (int(str[i]) > 47 && int(str[i]) < 58)
			|| (int(str[i]) > 96 && int(str[i]) < 123) || int(str[i]) == 95) {
			mid_str += str[i];
		}
		else {
			stringstream in(mid_str);
			double a;
			if (in >> a) aim_str += mid_str;
			else {
				if (find(List.begin(), List.end(), mid_str) == List.end()) aim_str += "A";
				else aim_str += mid_str;
			}
			mid_str = "";
			aim_str += str[i];
		}
	}
	regex s("\\s+");
	aim_str = regex_replace(aim_str, s, "");
	/*cout << aim_str << endl;*/
	return aim_str;
}

vector<int> HashValue(string str) {//将字符串切成长度为K的子串,并求子串对应的哈希值
	int N = str.length();
	vector<string> shingles;
	string s;
	int i = 0, j;
	while (i + K <= N) {
		s = "";
		for (j = i; j < K + i; j++)
			s += str[j];
		shingles.push_back(s);
		i++;
	}
	vector<int> hashvalue;
	i = 0;
	int base = 3, value;
	//char ch;
	while (i < N - K + 1) {
		value = 0;
		for (j = 0; j < K; j++) {
			//ch = shingles[i][j];
			value += (int)shingles[i][j] * pow(base, K - j - 1);
		}

		hashvalue.push_back(value);
		i++;
	}
	return hashvalue;
}

vector<int> FlagValue(vector<int> list) {//求特征值
	int n = list.size();
	Dict* listnum = new Dict[n];
	for (int i = 0; i < n; i++) {
		listnum[i].value = list[i];
		listnum[i].used = false;
	}
	vector<int> flag;
	int i = 0, mark;
	//Dict temp;
	while (i + W <= n) {
		//temp = listnum[i];
		mark = i;
		for (int j = i; j < W + i; j++) {
			if (listnum[mark].value >= listnum[j].value) mark = j;
		}
		if (!listnum[mark].used) {
			flag.push_back(listnum[mark].value);
			listnum[mark].used = true;
		}
		i++;
	}
	return flag;
}

double Simility(vector<int> list1, vector<int> list2) {//求相似度
	sort(list1.begin(), list1.end());
	sort(list2.begin(), list2.end());
	int unionset = 0;
	double intersection = 0.0;
	unionset = list1.size() + list2.size();
	bool flag = false;
	for (size_t i = 0; i < list1.size(); i++) {
		if (i > 0) {
			if (list1[i] == list1[i - 1] && flag) {
				intersection++;
				continue;
			}
			else flag = false;
		}
		for (size_t j = 0; j < list2.size(); j++) {
			if (list2[j] == list1[i]) {
				flag = true;
				intersection++;
			}
		}
		if (flag) intersection++;
	}
	//cout << intersection << endl;
	//cout << unionset << endl;
	double value = intersection / unionset;
	return value;
}


#endif//_CODE_H_