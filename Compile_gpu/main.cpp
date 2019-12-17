// main.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"Grammer.h"

int main()
{
	asm_text = (int *)malloc(1000 * sizeof(int));
	asm_data = (int *)malloc(1000 * sizeof(int));
	/*TableStack res;
	res.manage();*/
	Grammer g;
	//g.inputAndSolve();
	g.manage();
	/*int a;
	cin >> a;//等待回车退出
	/*
	ifstream in("Input.txt");			//改动采用自动机实现
	string s;
	getline(in, s);
	cout << s.length();*/

	/*Lex test("int a;");
	cout << endl;
	test.next_token();
	cout << endl;
	test.next_token();
	cout << endl;
	test.next_token();
	cout << endl;
	*/
	cout << "\n+++++++++++++++++++++++++++++++++++++\n" << endl;
	for (int i = 0; i < 50; i++) {
		cout << asm_text[i] << endl;
	}
	return 0;
}

