#pragma once
#include <iostream>
#include <vector>
#include "lex.h"
using namespace std;

class Semantic {
	int type;
	int *code_text;
	int *cur_data;
	Token token;
	Token prev_token;
	//Identifier *cur_id; //存储活跃的id信息
	vector<Identifier> *IDENTS;
	int cur_id_index;
	int params;
	int index_of_bp;
	int pos_local;
	int *mark_a;
	int *mark_b; //只用两个标记 if和while共用 所以不能嵌套

public:
	Semantic() {
		cur_id_index = -1;
		code_text = asm_text;
		/////////////
		cur_data = asm_data;
	}
	void set_ident(vector<Identifier> *i) {
		IDENTS = i;
	}
	void set_prev_token(Token t) {
		prev_token = t;
	}
	void set_cur_token(Token t) {
		token = t;
	}
	vector<void(Semantic::*)()> func;//语义函数向量
	void ident_rec();
	void type_rec();
	void add_ptr();
	void id_to_glo();//变量回归调用之前的值   //！！！！这里之后需要改进
	void parameter_rec();//记录参数
	void loc_index_rec();
	void loc_var_rec();
	void func_enter();//写入函数开始标记
	void leave();
	void if_begin();
	void else_begin();
	void if_end();
	void after_while();
	void while_begin();
	void while_end();
	void load_num();
	void push();
};