
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "lex.h"
using namespace std;

//enum {
//	Num=128,Fun,Sys,Glo,Loc,
//	Id,Char,String,Else,Enum,If,Int,Return,Sizeof,While,  //对应关键字查询表 Id定位
//	Assign,Add,Sub,Mul,Div,Mod,Xor,Brak,Cond,   //对应界符查询表 Assign定位
//	Lor,Lan,Or,And,Eq,Ne,Lt,Gt,Le,Ge,Shl,Shr,
//	Inc,Dec
//};


string Identifier::get_name() const {
	return name;
}
bool Identifier::operator==(const Identifier & obj2) const
{
	if (this->get_name().compare(obj2.get_name()) == 0)
		return true;
	else
		return false;
}
bool Identifier::operator==(string str) const
{
	if (this->get_name().compare(str) == 0)
		return true;
	else
		return false;
}
void Identifier::print() {
	cout << name + " \ttoken: " << token << endl;
}
Token::Token(int i, int v) {
	id = i;
	value = v;
}
Token::Token() {
	id = 0;
	value = 0;
}
void Token::set(int i) {
	id = i;
	value = 0;
}
void Token::set(int i, int v) {
	id = i;
	value = v;
}

//vector<string> I, C, S, N, K, P;  
vector<Token> token_s;
void Lex::init() {
	//这里的次序必须和enum中的相同
	//Id,Char,String,Else,Enum,If,Ine,Return,Sizeof,While,
	K.push_back("char");
	K.push_back("string");
	K.push_back("else");
	K.push_back("enum");
	K.push_back("if");
	K.push_back("int");
	K.push_back("return");
	K.push_back("sizeof");
	K.push_back("while");
	P.push_back("+");
	P.push_back("-"); 
	P.push_back("*");
	P.push_back("/");
	P.push_back("%");
	P.push_back("^");
	P.push_back("[");
	P.push_back("?");
}
Lex::Lex() {
	text = "";
	pos = 0;
	line = 1;
	init();
}
Lex::Lex(string str) {
	text = str;
	pos = 0;
	line = 1;
	init();
}
void Lex::set(string str) {
	text = str;
	pos = 0;
	line = 1;
}
inline bool Lex::is_i(char c) {
	if ((c <= 'z'&&c >= 'a') || (c <= 'Z'&&c >= 'A') || (c == '_'))
		return true;
	else
		return false;
}
float Lex::to_float(string str) {
	float res = 0;
	int left = 0;
	float right = 0;
	int i;
	for (i = 0; i < str.length() && str[i] != '.'; i++) {
		left *= 10;
		left += (str[i] - '0');
	}
	if (i < str.length()) {
		float ten = 10;
		for (i++; i < str.length(); i++) {
			right += (float)(str[i] - '0') / ten;
			ten *= 10;
		}
	}
	return (float)left + right;
}
Token Lex::number_token() {
	Token the_token;
	int value = 0;
	if (text[pos] > '0') {
		while (text[pos] >= '0'&&text[pos] <= '9') {
			value = value * 10 + text[pos] - '0';
			pos++;
		}
	}
	else {
		pos++;
		if (text[pos] == 'x' || text[pos] == 'X') {
			pos++;
			while ((text[pos] >= '0' && text[pos] <= '9') || (text[pos] >= 'a' && text[pos] <= 'f') || (text[pos] >= 'A' && text[pos] <= 'F')) {
				value = value * 16 + (text[pos] & 15) + (text[pos] >= 'A' ? 9 : 0);
				pos++;
			}
		}
		else {
			while (text[pos] >= '0'&&text[pos] <= '9') {
				value = value * 8 + text[pos] - '0';
				pos++;
			}
		}
	}
	the_token.set(Num,value);
	cout << value << '\t';
	return the_token;
	
	//if (pos == text.length()) {
	//	N.push_back(to_float(str));
	//	the_token.set('N', N.size());		//先push再set, token编号从1开始			
	//	//临时输出
	//	cout << str << '\t';
	//	return the_token;
	//}
	//if ((text[pos] >= '0'&&text[pos] <= '9') || text[pos] == '.') {
	//	str.push_back(text[pos++]);
	//	return number_token(str);
	//}
	//else {									//非数字则可以输出
	//	N.push_back(to_float(str));
	//	//临时输出
	//	cout << str << '\t';
	//	the_token.set('N', N.size());
	//	return the_token;
	//}
}

Token Lex::key_and_i_token(string str) {
	Token the_token;
	if (pos < text.length() && (is_i(text[pos]) || (text[pos] <= '9'&&text[pos] >= '0'))) {
		str.push_back(text[pos++]);
		return key_and_i_token(str);
	}
	else {
		Identifier ident;
		vector<Identifier>::iterator res = find(IDENTS.begin(), IDENTS.end(), str);
		vector<string>::iterator res1 = find(K.begin(), K.end(), str);
		//vector<string>::iterator res2 = find(I.begin(), I.end(), str);
		//临时输出
		cout << str << '\t';
		if (res1 != K.end()) {
			the_token.set(Id+(res1 - K.begin())+1);
			return the_token;
		}
		/*
		else if (res2 != I.end()) {
			the_token.set('I', res2 - I.begin() + 1);
			return the_token;
		}*/
		else if (res != IDENTS.end()) {
			the_token.set(res->token, res->value);
			return the_token;
		}
		else {
			/*I.push_back(str);
			the_token.set('I', I.size());*/
			ident.name = str;
			ident.token = Id;
			the_token.set(Id);//标识符token的value暂时设为0,第二个参数value为空则为0 
			IDENTS.push_back(ident);
			return the_token;
		}
	}

}

Token Lex::char_end(string str) {
	Token the_token;
	int val;
	if (pos >= text.size()) {
		throw "未识别的字符";
		return the_token;
	}
	if (text[pos - 1] == '\\') {
		if (text[pos] == 'n') {
			val = '\n';
			pos++;
		}
		else {
			throw "未识别的字符";
			return the_token;
		}
	}
	else {
		val = text[pos - 1];
	}
	if (text[pos] == '\'') {
		//str.push_back('\'');
		//C.push_back(str);
		the_token.set(Char, val);
		pos++;
		//临时输出
		cout << str << '\t';
		return the_token;
	}
	else {
		pos--;
		throw "未识别的字符";
		return the_token;
	}
}
Token Lex::char_token(string str) {
	Token the_token;
	int value;
	if (pos >= text.size()) {
		throw "未识别的字符";
		return the_token;
	}
	if (text[pos] == '\\'&&text[pos + 1] == 'n') {
		value = '\n';
	}
	else {
		str.push_back(text[pos++]);
		return char_end(str);
	}
}
Token Lex::string_end(string str) {
	Token the_token;
	if (pos >= text.size()) {
		throw "未识别的字符串";
		return the_token;
	}
	if (text[pos] != '"') {
		str.push_back(text[pos++]);
		return string_end(str);
	}
	else {
		str.push_back('"');
		S.push_back(str);
		//临时输出
		cout << str << '\t';
		the_token.set(String);
		pos++;
		return the_token;
	}
}
Token Lex::string_token(string str) {
	Token the_token;
	if (pos >= text.size()) {
		throw "未识别的字符串";
		return the_token;
	}
	else {
		str.push_back(text[pos++]);
		return string_end(str);
	}
}
Token Lex::delimiter_token() {
	Token the_token;
	if (text[pos] == '>') {
		if (pos + 1 < text.size() && text[pos + 1] == '=') {
			pos++;
			the_token.set(Ge);
			//临时输出
			printf(">=\t");
			return the_token;
		}
		else {
			//临时输出
			printf(">\t");
			the_token.set(Gt);
			return the_token;
		}
	}
	else if (text[pos] == '<') {
		if (pos + 1 < text.size() && text[pos + 1] == '=') {
			pos++;
			//临时输出
			printf("<=\t");
			the_token.set(Le);
			return the_token;
		}
		else {
			//临时输出
			printf("<\t");
			the_token.set(Lt);
			return the_token;
		}
	}
	else if (text[pos] == '=') {
		if (pos + 1 < text.size() && text[pos + 1] == '=') {
			pos++;
			//临时输出
			printf("==\t");
			the_token.set(Eq);
			return the_token;
		}
		else {
			//临时输出
			printf("=\t");
			the_token.set(Assign);
			return the_token;
		}
	}
	string str;
	str.push_back(text[pos]);
	vector<string>::iterator res = find(P.begin(), P.end(), str);
	if (res != P.end()) {
		//临时输出
		cout << str << '\t';
		the_token.set(Assign+(res - P.begin()) + 1);
		return the_token;
	}
	else if (text[pos] == '~' || text[pos] == ';' || text[pos] == '{' || text[pos] == '}' || text[pos] == '(' || text[pos] == ')' || text[pos] == ']' || text[pos] == ',' || text[pos] == ':') {
		// directly return the character as token;
		//临时输出
		cout << text[pos] << '\t';
		the_token.set(text[pos]);
		return the_token;
	}
	else {
		if (text[pos] == '\n')
			line++;
		return the_token;
	}
}
///
///
///
Token Lex::next_token() {
	char cur_char;
	string content = text;
	string half;

	if (pos >= text.length()) {
		cur.set('E', 0);
		token_s.push_back(cur);
		return cur;
	}
	cur_char = content[pos];
	try {
		if (cur_char <= '9'&&cur_char >= '0') {
			//pos++;
			cur = number_token();
		}
		else if (is_i(cur_char)) {
			pos++;
			half.push_back(cur_char);
			cur = key_and_i_token(half);
		}
		else if (cur_char == '\'') {
			pos++;
			half.push_back(cur_char);
			cur = char_token(half);
		}
		else if (cur_char == '"') {
			pos++;
			half.push_back(cur_char);
			cur = string_token(half);
		}
		else {
			cur = delimiter_token();
			pos++;					//界符判断较特殊，故而需要pos自增，其他自增在token识别中完成。
		}
	}
	catch (const char*msg) {

		printf("\n\n***Error ! \npos:%d , line: %d , cur_char:%c\n", pos, line, cur_char);
		cerr << msg << endl;
		return cur;
	}
	if (cur.id != 0) {
		printf(" :{ %d, %d }\n", cur.id, cur.value);
		token_s.push_back(cur);
	}
	//next_token();
	return cur;
}

void print() {
	/*vector<Identifier>::iterator iter;
	for (iter = IDENTS.begin(); iter != IDENTS.end(); iter++) {
		iter->print();
	}*/
	for (int i = 0; i < IDENTS.size(); i++) {
		IDENTS[i].print();
	}
}
//void print() {
//	printf("标识符\tC字符\tS字符串\tN常数\tK关键字\tP界符\n");
//	
//	for (unsigned i = 0; i < 16; i++) {
//		if (i < I.size())
//			cout << I[i] << '\t';
//		else
//			cout << '\t';
//		if (i < C.size())
//			cout << C[i] << '\t';
//		else
//			cout << '\t';
//		if (i < S.size())
//			cout << S[i] << '\t';
//		else
//			cout << '\t';
//		if (i < N.size())
//			cout << N[i] << '\t';
//		else
//			cout << '\t';
//		if (i < K.size())
//			cout << K[i] << '\t';
//		else
//			cout << '\t';
//		if (i < P.size())
//			cout << P[i] << '\t';
//		else
//			cout << '\t';
//		cout << endl;
//	}
//}

//int main() {
//	string text;
//	fstream in("text.txt");
//	stringstream ss;
//	ss << in.rdbuf();
//	text = ss.str();
//	cout << text;
//	Lex my_lex(text);
//	cout << endl;
//	my_lex.next_token();
//	cout << endl;
//	print();
//	return 0;
//}