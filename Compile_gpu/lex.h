#pragma once
#include <string>
#include <vector>
using namespace std;
enum {
	Num = 128, Fun, Sys, Glo, Loc,
	Id, Char, String, Else, Enum, If, Int, Return, Sizeof, While,  //对应关键字查询表 Id定位
	Assign, Add, Sub, Mul, Div, Mod, Xor, Brak, Cond,   //对应界符查询表 Assign定位
	Lor, Lan, Or, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr,
	Inc, Dec
};

class Token { 
public:
	int id;		//id就是上面的enum ~ ; { } ( ) ] , :的id是本身
	int value;	//大部分value都为0  Num的value为本身数字大小  有待重新定义
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
	string get_name() const;
	bool operator==(const Identifier & obj2) const;  //重载==用于find
	bool operator==(string str) const;
	void print();
};

vector<Identifier> IDENTS;//符号表向量

class Lex {
	string text;
	Token cur;
	unsigned pos;
	unsigned line;
	vector<string> I, C, S, K, P;
	vector<float> N;
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
public:
	Lex();
	Lex(string str);		//传入代码文本初始化
	void set(string str);	//设置代码文本
	Token next_token();	
};