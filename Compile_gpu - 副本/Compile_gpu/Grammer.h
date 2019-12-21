#ifndef _ANALYSIS_H_
#define _ANALYSIS_H_
#include"lex.h"
#include "Semantic.h"
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <set>
#include <algorithm>
#include <iomanip>
#include <map>
#include <cmath>
#include <stack>
#include <fstream>
#include <stack>
/*
extern enum Token_tag {
	Num = 128, Fun, Sys, Glo, Loc,
	Id, Char, String, Else, Enum, If, Int, Return, Sizeof, While, Void, //对应关键字查询表 Id定位
	Assign, Add, Sub, Open_paren, Close_paren, Open_curly, Close_curly, Comma, Semicolon, Mul, Div, Mod, Xor, Brak, Cond,			  //对应界符查询表 Assign定位
	Lor, Lan, Or, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr,
	Inc, Dec
};*/

enum Action_tag {
	add_ptr=0,				//0
	ident_rec,				//1
	id_to_glo,				//2
	parameter_rec,			//3
	loc_var_rec,			//4
	loc_index_rec,			//5
	type_rec,				//6
	func_enter,				//7
	leave,					//8
	if_begin,				//9
	else_begin,				//10
	if_end,					//11
	after_while,			//12
	while_begin,			//13
	while_end,				//14
	imm_num,
	ptr_init,
	ptr_inc,
	ptr_clear,
	var_value,
	pk_assign,
	pk_paren,
	pk_add,
	pk_mul,
	pk_sub,
	pk_div,
	pk_land,
	pk_lor,
	pk_gt,
	pk_lt,
	pk_eq,
	pk_neq,
	back,
	back_a,
	array_type_rec,
	array_ok,
	call_init,
	call,
	load_param
};
enum ItemType
{VT = 0,VN = 1,Action = 2};
struct Item
{
	ItemType type;
	string name; 
	int tag;		//给语义动作备用
};
struct Production
{
	Item left;			//左部
	vector<Item> right;		//右部			
};

class Grammer
{
protected:
	int T;//文法个数
	Item start;
	Token cur;				
	Token pre;
	Lex lex;
	Semantic semantic;
	Production productions[100]; //文法集合
	set<string> firstSet[50];//first集
	set<string> followSet[50];//follow集
	vector<Item> ter_withoutblack; //去空字终结符
	vector<Item> vt;//终结符
	vector<Item> vn;//非终结符
	vector<Item> action;//语义动作符
	int LL1_Map[50][50];			

public:
	Grammer() :T(0) {  }
	bool isNotTerminal(Item c);//判断是否为非终结符
	bool isTerminal(Item c);//判断是否为非终结符
	bool isAction(Item c);//判断是否为非终结符
	void remove();			  //左递归消除
	int getVTindex(Item target);//获得在终结符集合中的下标
	int getVTindex(string target);
	int getVTindex(Token target);
	int getAction_index(Item target);	//获得在动作符集合中的下标
	int getVNindex(Item target);//获得在非终结符集合中的下标
	void setType();			//设置类型
	void setTag();
	void createVN_VT_ACTION();		//创建终结符和非终结符集
	void createFirstSet();
	void createFollowSet();
	void getFirstSet(Item target); //得到first集合
	void getFollowSet(Item target);//得到follow集合
	void inputAndSolve(); //处理得到first和follow
	void display(); //显示
	bool createLL1_Map(); //得到预测表
	void print_out();//输出
	void manage(); //综合
	void runParserLL1();	//语法分析
	/////////////////////////////动作
	bool match(Item x);			//匹配
	void push_right(stack<Item>&SYN,int index);	    //逆向压栈
	string next();				   //取下一个token

};


class VM {
private:
	int ax = 0;//ax寄存器
	int *bp,//寻址指针
		*pc,//程序指针
		*sp,//堆栈指针
		*out;
	int *tmp;
	int *stack;
	int  poolsize;
	void step(int i);
	int flag;
public:
	VM();
	int fetch();
	int run();
	void print(int i);
};
#endif 
