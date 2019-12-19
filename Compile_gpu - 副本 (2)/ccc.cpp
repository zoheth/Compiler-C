#include "stdafx.h"
#include <iostream>
#include <vector>
#include "lex.h"
using namespace std;
/*
class Semantic {
	int type;
	Token token;
	Token prev_token;
	Identifier cur_id; //存储活跃的id信息
	vector<Identifier> IDENTS;
	int cur_id_index;
	int params;
	int index_of_bp;
	int pos_local;
	int *mark_a;
	int *mark_b; //只用两个标记 if和while共用 所以不能嵌套

public:
	vector<void(Semantic::*)()> func;//语义函数向量
	void ident_rec() {

		//cur_id在此之前就已经更新 更新id不需要查找 直接下一个
		cur_id.type = type;
		if (token == '(') {  //此时的token为下一个token
			cur_id.class_ = Fun;
			cur_id.value = int(code_text + 1);//！！！！函数地址,暂时未确定形式
											  //！！！！代码地址自增
		}
		else {
			cur_id.class_ = Glo; //全局变量
			cur_id.value = 0;//！！！！该数据的地址
							 //！！！！数据地址自增
		}
		while (type >= PTR) {
			type -= PTR;
		}
		cur_id = IDENTS[cur_id_index + 1];//下一个id
		cur_id_index++;
	}
	void type_rec() {
		type = prev_token.id;
	}
	void add_ptr() {
		type += PTR;
	}
	void id_to_glo() {  //变量回归调用之前的值
						//！！！！这里之后需要改进
		vector<Identifier>::iterator iter;
		for (iter = IDENTS.begin(); iter != IDENTS.end(); iter++) {
			if (iter->class_ == Loc) {
				iter->class_ = iter->Bcalss;
				iter->type = iter->Btype;
				iter->value = iter->Bvalue;

			}
		}
	}
	void parameter_rec() {  //记录参数
		cur_id.Bcalss = cur_id.class_;
		cur_id.class_ = Loc;
		cur_id.Btype = cur_id.type;
		cur_id.type = type;
		cur_id.Bvalue = cur_id.value;
		cur_id.value = params;  //
		params++;
	}
	void loc_index_rec() {
		pos_local = params + 1;
	}
	void loc_var_rec() {
		cur_id.Bcalss = cur_id.class_;
		cur_id.class_ = Loc;
		cur_id.Btype = cur_id.type;
		cur_id.type = type;
		cur_id.Bvalue = cur_id.value;
		cur_id.value = pos_local;
		pos_local++;  //是否有必要换变量，或者是否有必要新建这个函数
	}

	void func_enter() {
		//写入函数开始标记
		code_text++;
		*code_text = ENT;
		code_text++;
		*code_text = pos_local - params;
	}
	void leave() {
		code_text++;
		*code_text = LEV;
	}
	void if_begin() {
		code_text++;
		*code_text = JZ;
		code_text++;
		mark_a = code_text;//记录if初始位置
	}
	void else_begin() {
		*mark_a = (int)(code_text + 3);
		code_text++;
		*code_text = JMP;
		code_text++;
		mark_a = code_text;
	}
	void if_end() {
		*mark_a = (int)(code_text + 1);
	}
	void after_while() {
		mark_a = code_text + 1;
	}
	void while_begin() {
		code_text++;
		*code_text = JZ;
		code_text++;
		mark_b = code_text;
	}
	void while_end() {
		//跳转到mark_a 迭代 而非循环结束
		code_text++;
		*code_text = JMP;
		code_text++;
		*code_text = (int)mark_a;
		*mark_b = (int)(code_text + 1);
	}
	void load_num() {
		code_text++;
		*code_text = IMM;
		code_text++;

	}
	void push() {
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
	}
};

int main() {
	Semantic a;
	void (Semantic::*xx)() = a.func[0];
	(a.*xx)();
	return 0;
}
*/
