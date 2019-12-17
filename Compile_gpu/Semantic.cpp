#include "stdafx.h"
#include <iostream>
#include <vector>
#include "lex.h"
#include "Semantic.h"
using namespace std;

void Semantic::ident_rec() {
	//CUR_ID = &(*IDENTS)[CUR_ID_index + 1];//下一个id
	//CUR_ID_index++;
	//CUR_ID在此之前就已经更新 更新id不需要查找 直接下一个
	CUR_ID->type = type;
	if (token == '(') {  //此时的token为下一个token
		CUR_ID->class_ = Fun;
		CUR_ID->value = int(code_text + 1);		//！！！！函数地址,暂时未确定形式
												//！！！！代码地址自增
	}
	else {
		CUR_ID->class_ = Glo; //全局变量
		CUR_ID->value = (int)cur_data;		//！！！！该数据的地址
											//！！！！数据地址自增
		cur_data++;
	}
	while (type >= PTR) {
		type -= PTR;
	}
}
void Semantic::type_rec() {							//更新类型，在Type->char <update_type>|void <update_type>|int <update_type> ，识别类型后，类型变为pre_token
	if (prev_token.id == Int) {
		type = INT;
	}
	else if (prev_token.id == Char) {
		type = CHAR;
	}
	else if (prev_token.id == Void) {
		type = VOID;
	}
}
void Semantic::add_ptr() {							//添加指针
	type += PTR;
}
void Semantic::id_to_glo() {  //变量回归调用之前的值
							  //！！！！这里之后需要改进
	vector<Identifier>::iterator iter;
	for (iter = IDENTS->begin(); iter != IDENTS->end(); iter++) {
		if (iter->class_ == Loc) {
			/*iter->class_ = iter->Bcalss;
			iter->type = iter->Btype;
			iter->value = iter->Bvalue;*/
			;
		}
	}
}
void Semantic::parameter_rec() {  //记录参数
								  //CUR_ID = &(*IDENTS)[CUR_ID_index + 1];//下一个id
								  //CUR_ID_index++;
	CUR_ID->Bcalss = CUR_ID->class_;
	CUR_ID->class_ = Loc;
	CUR_ID->Btype = CUR_ID->type;
	CUR_ID->type = type;
	CUR_ID->Bvalue = CUR_ID->value;
	CUR_ID->value = params;  //
	params++;
}
void Semantic::loc_index_rec() {
	pos_local = params + 1;
}
void Semantic::loc_var_rec() {
	//CUR_ID = &(*IDENTS)[CUR_ID_index + 1];//下一个id
	//CUR_ID_index++;
	CUR_ID->Bcalss = CUR_ID->class_;
	CUR_ID->class_ = Loc;
	CUR_ID->Btype = CUR_ID->type;
	CUR_ID->type = type;
	CUR_ID->Bvalue = CUR_ID->value;
	CUR_ID->value = pos_local;
	pos_local++;  //是否有必要换变量，或者是否有必要新建这个函数
}
void Semantic::func_enter() {
	//写入函数开始标记
	code_text++;
	*code_text = ENT;
	code_text++;
	*code_text = pos_local - params;
}
void Semantic::leave() {
	code_text++;
	*code_text = LEV;
}
void Semantic::if_begin() {
	code_text++;
	*code_text = JZ;
	code_text++;
	mark_b = code_text;//记录if初始位置
}
void Semantic::else_begin() {
	*mark_b = (int)(code_text + 3);
	code_text++;
	*code_text = JMP;
	code_text++;
	mark_b = code_text;
}
void Semantic::if_end() {
	*mark_b = (int)(code_text + 1);
}
void Semantic::after_while() {
	mark_a = code_text + 1;
}
void Semantic::while_begin() {
	code_text++;
	*code_text = JZ;
	code_text++;
	mark_b = code_text;
}
void Semantic::while_end() {
	//跳转到mark_a 迭代 而非循环结束
	code_text++;
	*code_text = JMP;
	code_text++;
	*code_text = (int)mark_a;
	*mark_b = (int)(code_text + 1);
}
void Semantic::imm_num() {
	code_text++;
	*code_text = IMM;
	code_text++;
	*code_text = prev_token.value;
	expr_type = INT;
}
void Semantic::pk_assign() {
	if (prev_token.id >= level.top()) {
		if (*code_text == LC || *code_text == LI) {
			*code_text = PUSH; // save the lvalue's pointer
		}
		else {
			throw "赋值错误";
		}
		level.push(Assign);
	}
}

void Semantic::pk_add() {
	code_text++;
	*code_text = PUSH;
	level.push(Add);
}
void Semantic::pk_mul() {
	code_text++;
	*code_text = PUSH;
	level.push(Mul);
}
void Semantic::back() {
	while (!level.empty()&&level.top()>token.id) {
		int temp = level.top();
		level.pop();
		if (temp == Assign) {
			//expr_type
			code_text++;
			*code_text = (expr_type == CHAR) ? SC : SI;
		}
		else if (temp == Add) {
			code_text++;
			*code_text = ADD;
		}
		else if (temp == Mul) {
			code_text++;
			*code_text = Mul;
		}
	}
}

void Semantic::gpu_parameter_rec() {
	CUR_ID->Bcalss = CUR_ID->class_;
	CUR_ID->class_ = Dev;
	CUR_ID->Btype = CUR_ID->type;
	CUR_ID->type = type;
	CUR_ID->Bvalue = CUR_ID->value;
	CUR_ID->value = params;  //
	params++;
}

void Semantic::call_init() {
	adj_size = 0;
	the_func_id = CUR_ID;
}
void Semantic::load_param() {
	if (the_func_id->class_ == Gpu) {

	}
	code_text++;
	*code_text = PUSH;
	adj_size++;
}
void Semantic::var_value() {
	if (CUR_ID->type == Num) {
		code_text++;
		*code_text = IMM;
		code_text++;
		*code_text = CUR_ID->value;
	}
	else if (CUR_ID->class_ == Loc) {
		code_text++;
		*code_text = LEA;
		code_text++;
		*code_text = CUR_ID->value - pos_local;
	}
	else if (CUR_ID->class_ == Glo) {
		code_text++;
		*code_text = IMM;
		code_text++;
		*code_text = CUR_ID->value;
	}
}
void Semantic::call() {
	if (the_func_id->class_ == Sys) {
		code_text++;
		*code_text = the_func_id->value;
	}
	else if (the_func_id->class_ == Fun) {
		code_text++;
		*code_text = CALL;
		code_text++;
		*code_text = the_func_id->value;
	}
	else if (the_func_id->class_ == Gpu) {
		
	}
	else {
		throw "非法函数";
	}
	if(adj_size>0){
		code_text++;
		*code_text = ADJ;
		code_text++;
		*code_text = adj_size;
	}
	expr_type = the_func_id->type;//??????
}

void Semantic::gpu_load() {

}


void Semantic::push() {
	func.push_back(&Semantic::add_ptr);					//0
	func.push_back(&Semantic::ident_rec);				//1
	func.push_back(&Semantic::id_to_glo);				//2
	func.push_back(&Semantic::parameter_rec);			//3
	func.push_back(&Semantic::loc_var_rec);				//4
	func.push_back(&Semantic::loc_index_rec);			//5
	func.push_back(&Semantic::type_rec);				//6
	func.push_back(&Semantic::func_enter);				//7
	func.push_back(&Semantic::leave);					//8
	func.push_back(&Semantic::if_begin);				//9
	func.push_back(&Semantic::else_begin);				//10
	func.push_back(&Semantic::if_end);					//11
	func.push_back(&Semantic::after_while);				//12
	func.push_back(&Semantic::while_begin);				//13
	func.push_back(&Semantic::while_end);				//14
	func.push_back(&Semantic::imm_num);
	func.push_back(&Semantic::var_value);
	func.push_back(&Semantic::pk_assign);
	func.push_back(&Semantic::pk_add);
	func.push_back(&Semantic::pk_mul);
	func.push_back(&Semantic::back);
}


