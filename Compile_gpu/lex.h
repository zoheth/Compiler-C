#pragma once
#pragma once
#include <string>
#include <vector>
using namespace std;

class Token { 
public:
	int id;
	int value;
	Token(int i, int v);
	Token();
	void set(int i);
	void set(int i, int v);
};
class Synbl { //待定符号表

};
class Identifier {  //与C4结构一样 name改成了string 暂时用作符号表
public:
	int token;
	int hash;
	string name;
	int class_;
	int tpye;
	int value;
	int Bcalss;
	int Btype;
	int Bvalue;
	/*void hash_code(string str) {
		string::iterator ite;
		ite = str.begin();
		for (size_t i = 0; i < str.size(); i++) {
			hash = *ite + 147 * hash;
		}
	}*/
	string get_name() const;
	bool operator==(const Identifier & obj2) const;
	bool operator==(string str) const;
	void print();
};

class Lex {
	string text;
	Token cur;
	unsigned pos;
	unsigned line;
	vector<string> I, C, S, K, P;
	vector<float> N;
public:
	Lex();
	Lex(string str); 
	void set(string str);
	bool is_i(char c);
	void init();
	float to_float(string str);
	Token number_token();
	Token key_and_i_token(string str);
	Token char_end(string str);
	Token char_token(string str);
	Token string_end(string str);
	Token string_token(string str);
	Token delimiter_token();
	Token next_token();
	float get_N(int i) {
		return N[i];
	}
};