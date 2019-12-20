#include "Grammer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

VM::VM() {
	poolsize = 1000;
	stack = (int *)malloc(poolsize);
	memset(stack, 0, poolsize);
	sp = (int *)((int)stack + poolsize);
	*--sp = EXIT; // call exit if main returns
	*--sp = PUSH; tmp = sp;
	bp = (int *)((int)stack + poolsize);
	pc = main_addr;
	out = asm_text+1;
	flag = 1;
}
void VM::print() {
	int i = *out;
	out++;
	switch (i)
	{
	case PUSH: {
		cout << (int)(out-1)<<">: "<<"PUSH" << endl;
		break;
	}
	case IMM: {
		cout << (int)(out-1)<<">: "<<"IMM\t" << *out << endl;
		out++;
		break;
	}
	case LC: {
		cout << (int)(out-1)<<">: "<<"LC" << endl;
		break;
	}
	case LI: {
		cout << (int)(out-1)<<">: "<<"LI" << endl;
		break;
	}
	case SC: {
		cout << (int)(out-1)<<">: "<<"SC" << endl;
		break;
	}
	case SI: {
		cout << (int)(out-1)<<">: "<<"SI" << endl;
		break;
	}
	case JMP: {
		cout << (int)(out-1)<<">: "<<"JMP\t" << *out << endl;
		out++;
		break;
	}
	case JZ: {
		cout << (int)(out-1)<<">: "<<"JZ\t" << *out << endl;
		out++;
		break;
	}
	case JNZ: {
		cout << (int)(out-1)<<">: "<<"JNZ\t" << *out << endl;
		out++;
		break;
	}
	case ADD: {
		cout << (int)(out-1)<<">: "<<"ADD" << endl;
		out++;
		break;
	}
	case SUB: {
		cout << (int)(out-1)<<">: "<<"SUB" << endl;
		break;
	}
	case MUL: {
		cout << (int)(out-1)<<">: "<<"MUL" << endl;
		break;
	}
	case DIV: {
		cout << (int)(out-1)<<">: "<<"DIV" << endl;
		break;
	}
	case AND: {
		cout << (int)(out-1)<<">: "<<"AND" << endl;
		break;
	}
	case OR: {
		cout << (int)(out-1)<<">: "<<"OR" << endl;
		break;
	}
	case EQ: {
		cout << (int)(out-1)<<">: "<<"EQ" << endl;
		break;
	}
	case NE: {
		cout << (int)(out-1)<<">: "<<"NE" << endl;
		break;
	}
	case MOD: {
		cout << (int)(out-1)<<">: "<<"MOD" << endl;
		break;
	}
	case SHR: {
		cout << (int)(out-1)<<">: "<<"SHR" << endl;
		break;
	}
	case SHL: {
		break;
	}
	case GT: {
		cout << (int)(out-1)<<">: "<<"GT" << endl;
		break;
	}
	case GE: {
		cout << (int)(out-1)<<">: "<<"GE" << endl;
		break;
	}
	case LT: {
		cout << (int)(out-1)<<">: "<<"LT" << endl;
		break;
	}
	case LE: {
		cout << (int)(out-1)<<">: "<<"LE" << endl;
		break;
	}
	case XOR: {
		cout << (int)(out-1)<<">: "<<"XOR" << endl;
		break;
	}
			  //函数调用指令
	case CALL: {
		cout << (int)(out-1)<<">: "<<"CALL\t" << *out << endl;
		out++;
		break;
	}
	case ENT: {
		cout <<endl;
		cout << (int)(out-1)<<">: "<<"ENT\t" << *out << endl;
		out++;
		break;
	}
	case ADJ: {
		cout << (int)(out-1)<<">: "<<"ADJ\t" << *out << endl;
		out++;
		break;
	}
	case LEV: {
		cout << (int)(out-1)<<">: "<<"LEV" << endl;
		out++;
		break;
	}
	case LEA: {
		cout << (int)(out-1)<<">: "<<"LEA\t" << *out << endl;
		out++;
		break;
	}
			  //内置函数
	case EXIT: {
		cout << (int)(out-1)<<">: "<<"EXIT" << endl;
		printf("exit(%d)", *sp);
		return;
	}
	case PRTF: {
		cout << (int)(out-1)<<">: "<<"PRTF" << endl;
		break;
	}
	}
	print();
	return;
}
void VM::step(int i) {
	pc++;
	switch (i)
	{
		//case HLT: run = false; break;
	case PUSH: {
		//sp++;
		//Stack[sp] = asm_text[++pc];
		*--sp = ax;
		break;
	}
			   //case POP: {
			   //int popvalue = Stack[--sp];
			   //cout << popvalue << endl;
			   //int popvlaue = S.top();
			   //S.pop();
			   //pc++;
			   //break;
			   //}
	case IMM: {
		ax = *pc++;
		break;
	}
	case LC: {
		ax = *(char *)ax; //将对应地址中的字符存入ax中，要求ax中存放地址 char类型转换
		break;
	}
	case LI: {
		ax = *(int *)ax; //将对应地址中的字符存入ax中，要求ax中存放地址 int类型转换
		break;
	}
	case SC: {
		ax = *(char *)*sp++ = ax;
		break;
	}
	case SI: {
		*(int *)*sp++ = ax;
		break;
	}
	case JMP: {
		pc = (int *)*pc;
		break;
	}
	case JZ: {
		pc = ax ? pc + 1 : (int *)*pc;
		break;
	}
	case JNZ: {
		pc = ax ? (int *)*pc : pc + 1;
		break;
	}
			  //运算符指令
			  /*case ADD: {
			  //int a = Stack[--sp];
			  //int b = Stack[--sp];
			  //int res = a + b;
			  //sp++;
			  //Stack[sp] = res;
			  int a = S.top();
			  S.pop();
			  int b = S.top();
			  S.pop();
			  int res = a + b;
			  S.push(res);
			  pc++;
			  break;
			  }*/
	case ADD: {
		ax = *sp++ + ax;
		break;
	}
	case SUB: {
		ax = *sp++ - ax;
		break;
	}
	case MUL: {
		ax = *sp++ * ax;
		break;
	}
	case DIV: {
		ax = *sp++ / ax;
		break;
	}
	case AND: {
		ax = *sp++ & ax;
		break;
	}
	case OR: {
		ax = *sp++ | ax;
		break;
	}
	case EQ: {
		ax = *sp++ == ax;
		break;
	}
	case NE: {
		ax = *sp++ != ax;
		break;
	}
	case MOD: {
		ax = *sp++ % ax;
		break;
	}
	case SHR: {
		ax = *sp++ >> ax;
		break;
	}
	case SHL: {
		ax = *sp++ << ax;
		break;
	}
	case GT: {
		ax = *sp++ > ax;
		break;
	}
	case GE: {
		ax = *sp++ >= ax;
		break;
	}
	case LT: {
		ax = *sp++ < ax;
		break;
	}
	case LE: {
		ax = *sp++ <= ax;
		break;
	}
	case XOR: {
		ax = *sp++ ^ ax;
		break;
	}
			  //函数调用指令
	case CALL: {
		*--sp = (int)(pc + 1);
		pc = (int *)*pc;
		break;
	}
	case ENT: {
		*--sp = (int)bp;
		bp = sp;
		sp = sp - *pc++;
		break;
	}
	case ADJ: {
		sp = sp + *pc++;
		break;
	}
	case LEV: {
		sp = bp;
		bp = (int *)*sp++;
		pc = (int *)*sp++;
		break;
	}
	case LEA: {
		ax = (int)(bp + *pc++);
		break;
	}
			  //内置函数
	case EXIT: {
		printf("exit(%d)", *sp);
		flag = 0;
		break;
	}
	//case OPEN: {
	//	ax = open((char *)sp[1], sp[0]);
	//	break;
	//}
	//case CLOS: {
	//	ax = close(*sp);
	//	break;
	//}
	//case READ: {
	//	ax = read(sp[2], (char *)sp[1], *sp);
	//	break;
	//}
	case PRTF: {
		tmp = sp + pc[1];
		ax = printf("程序print输出：%d\n", tmp[-1]);
		break;
	}
	case MALC: {
		ax = (int)malloc(*sp);
		break;
	}
	case MSET: {
		ax = (int)memset((char *)sp[2], sp[1], *sp);
		break;
	}
	case MCMP: {
		ax = memcmp((char *)sp[2], (char *)sp[1], *sp);
		break;
	}
	}
}
int VM::fetch() {
	if (*pc == EXIT) {
		int a;
	}
	if (*pc == ENT) {
		int a;
		int b;
		a + b;
	}
	return *pc;
}
int VM::run() {
	while (flag) {
		step(fetch());
	}
	return 0;
}