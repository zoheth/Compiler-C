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
class Synbl {

};
class Identifier {
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
	string get_name() const {
		return name;
	}
	bool operator==(const Identifier & obj2) const
	{
		if (this->get_name().compare(obj2.get_name()) == 0)
			return true;
		else
			return false;
	}
	bool operator==(string str) const
	{
		if (this->get_name().compare(str) == 0)
			return true;
		else
			return false;
	}
	void print() {
		cout << name + " \ttoken: " << token << endl;
	}
	
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