#include"stdafx.h"
#include"lex.h"
#include "Grammer.h"
#include <sstream>
bool Grammer::isNotTerminal(Item c)
{
	/*for (int i = 0; i < vn.size(); i++)
		if (c.name == vn[i].name)*/
	if (c.type == ItemType::VN)
		return true;
	return false;
}
bool Grammer::isTerminal(Item c)
{
	if (c.type == ItemType::VT)
		return true;
	return false;
}
bool Grammer::isAction(Item c)
{
	if (c.type == ItemType::Action)
		return true;
	return false;
}
int Grammer::getVNindex(Item target)
{
	for (int i = 0; i < vn.size(); i++)
	{
		if (target.name == vn[i].name)
			return i;
	}
	return -1;
}
int Grammer::getVTindex(Item target)
{
	for (int i = 0; i < vt.size(); i++)
	{
		if (target.name == vt[i].name)
			return i;
	}
	return -1;
}
int Grammer::getVTindex(string target)
{
	for (int i = 0; i < vt.size(); i++)
	{
		if (target == vt[i].name)
			return i;
	}
	return -1;
}
int Grammer::getVTindex(Token target)
{
	for (int i = 0; i < vt.size(); i++)
	{
		if (target.id == vt[i].tag)
			return i;
	}
	return -1;
}
int Grammer::getAction_index(Item target)
{
	{
		for (int i = 0; i < action.size(); i++)
		{
			if (target.name == action[i].name)
				return i;
		}
		return -1;
	}
}
void Grammer::setType()
{
	//cout << T;
	for (int i = 0; i < T; i++)
	{
		int size = productions[i].right.size();
		//cout << productions[i].left.name << productions[i].left.type << "->";
		for (int j = 0; j < size; j++)
		{
			Item temp = productions[i].right[j];
			//cout << temp.name;
			if ((getVNindex(temp)) != -1)
			{
				productions[i].right[j] = vn[getVNindex(temp)];
			}
			else if ((getVTindex(temp)) != -1)
			{
				productions[i].right[j] = vt[getVTindex(temp)];
			}
			else if ((getAction_index(temp)) != -1)
			{
				productions[i].right[j] = action[getAction_index(temp)];
			}
			else
			{
				cout << endl;
				cout << "错误：setType()出错了" << endl;
				throw "出错了";
			}
			//cout << productions[i].right[j].type;
		}
		//cout << endl;
	}
}
void Grammer::createVN_VT_ACTION()			//创建终结符和非终结符集
{
	int terminal_tag = Token_tag::Int_const;
	int action_tag = Action_tag::add_ptr;

	ifstream in1("VN.txt");			//读非终结符
	string s;
	while (getline(in1, s))
	{
		Item temp{ ItemType::VN,s,-1 };
		vn.push_back(temp);
	}
	in1.close();
	start = vn[0];			//设置起始符
	ifstream in2("VT.txt");			//读终结符
	while (getline(in2, s))
	{
		Item temp{ ItemType::VT,s,terminal_tag };
		vt.push_back(temp);
		terminal_tag++;
	}
	in2.close();

	ifstream in3("ACTION.txt");			//读动作符
	while (getline(in3, s))
	{
		Item temp{ ItemType::Action,s,action_tag };
		action.push_back(temp);
		action_tag++;
	}
	in3.close();
	/*cout << "here";
	for (int i = 0; i < vn.size(); i++)
		cout << vn[i].name << " " << vn[i].tag << endl;
	for (int i = 0; i < vt.size(); i++)
		cout << vt[i].name << " " << vt[i].tag << endl;
	for (int i = 0; i < action.size(); i++)
		cout << action[i].name << " " << action[i].tag << endl;
		*/

}
void Grammer::getFirstSet(Item target)
{
	int tag = 0;
	int flag = 0;
	//cout << "T:" << T << endl;
	/*if (target.name == "Body-decl_2")
		cout << "来了！" << endl;
		*/
	for (int i = 0; i < T; i++)
	{
		flag = 0;
		tag = 0;
		if (productions[i].left.name == target.name)//匹配产生式左部
		{

			int first_index = 0;
			for (; first_index < productions[i].right.size(); first_index++)
			{
				if (isAction(productions[i].right[first_index]))
					continue;
				else
					break;
			}
			if (first_index >= productions[i].right.size())
			{
				cout << "错误：产生式错误！" << endl;
					throw "FIRST SET ERROR!";
			}
			//cout << "index:" << first_index << endl;

			if (isTerminal(productions[i].right[first_index]))//对于终结符，直接加入first
			{
				firstSet[getVNindex(target)].insert(productions[i].right[first_index].name);
				continue;
			}
			else if (isNotTerminal(productions[i].right[first_index]))
			{
				for (int j = first_index; j < productions[i].right.size(); j++)
				{
					if (isTerminal(productions[i].right[j]))				//终结符结束
					{
						firstSet[getVNindex(target)].insert(productions[i].right[j].name);
						break;
					}
					else if (isNotTerminal(productions[i].right[j]))			//非终结符递归
					{

						//cout << "递归：" << productions[i].right[j].name << endl;
						//if (getVNindex(target) < getVNindex(productions[i].right[j]))

						getFirstSet(productions[i].right[j]);

						set<string>::iterator it;
						//cout << productions[i].right[j].name << ":";
						for (it = firstSet[getVNindex(productions[i].right[j])].begin(); it != firstSet[getVNindex(productions[i].right[j])].end(); it++)
						{
							//cout <<" "<< *it ;
							if ((*it) == "@")							//有空产生式
							{
								flag = 1;
								tag += 1;
								continue;

							}
							else

								firstSet[getVNindex(target)].insert(*it);//将FIRST(Y)中的非$就加入FIRST(X)
						}
						//cout << endl;

					}
					if (flag == 0)			//如果flag = 0,则说明该非终结符的first集不存在空，则无需再查找递归结束的一个条件
						break;
					else flag = 0;
				}



				int len = 0;
				for (int k = 0; k < productions[i].right.size(); k++)		//用于检查该左部的first集应加入空
				{
					if (productions[i].right[k].type == ItemType::VN)
						len++;

				}
				if (flag == len)//所有右部first(Y)都有#,将$加入FIRST(X)中				//考虑可以将空产生式作为终结符，他的fisrt集为自己
				{

					firstSet[getVNindex(target)].insert("@");
					cout << target.name << flag << endl;;
				}


			}
			else
				return;
		}
	}
	/*
	int index = getVNindex(target);
	cout << target.name << " " << firstSet[index].size() << "--->";

	for (set<string>::iterator it = firstSet[index].begin(); it != firstSet[index].end(); it++)
	{
		cout << (*it) << " ";
	}
	cout << endl;
	*/
}
/*
void Grammer::getFollowSet(Item target)
{
	for (int i = 0; i < T; i++)
	{
		int index = -1;					//获取target在产生式右部的下标
		int len = 0;
		int flag = 0;					//标明target在产生式右部的下一个符号位语义动作符
		len = productions[i].right.size();
		for (int j = 0; j < len; j++)
		{
			if (productions[i].right[j].name == target.name)
			{
				index = j;
				break;
			}
		}

		if (index == -1) continue;
		cout << i<<":"<<productions[i].left.name << ":";
		cout << index<<endl;

		if (index < len - 1)
		{
			//Item nxt = productions[i].right[index + 1]; //考虑动作符
			index++;		//B -> ...Aβ  获取β的index
			while (index < len)
			{

				if (isAction(productions[i].right[index]))
				{
					index++;
					flag = 1;
				}
				else
				{
					flag = 0;
					break;
				}

			}
			//cout << "flag:" << flag << endl;
			if (flag == 0)
			{
				Item nxt = productions[i].right[index];
				//cout << "威慑" << endl;
				if (isTerminal(nxt))
				{
					followSet[getVNindex(target)].insert(nxt.name);
					//cout << "看这里！" << nxt.name<<endl;
				}
				else if( (isNotTerminal(nxt)))
				{

					if (nxt.name == productions[i].left.name)
					{
						int temp_index = index + 1;
						while (temp_index < len)
						{
							if (isAction(productions[i].right[temp_index]) == false)
								  break;
							else temp_index++;
						}
						if (temp_index >= len)
							continue;
					}
					int isExt = 0;
					set<string>::iterator it;
					for (it = firstSet[getVNindex(nxt)].begin(); it != firstSet[getVNindex(nxt)].end(); it++)
					{

						if ((*it) == "@")
							isExt = 1;
						else
							followSet[getVNindex(target)].insert(*it);
					}

					if (isExt && productions[i].left.name != target.name)			//B -> ...Aβ,β取空时
					{
						getFollowSet(productions[i].left);		               //递归求解
						set<string>::iterator it;
						Item tmp = productions[i].left;
						for (it = followSet[getVNindex(tmp)].begin(); it != followSet[getVNindex(tmp)].end(); it++)
							followSet[getVNindex(target)].insert(*it);
					}
				}
			}
			else if(target.name != productions[i].left.name)			//考虑语义动作
			{
				cout <<"进入这里" << endl;
				getFollowSet(productions[i].left);
				set<string>::iterator it;
				Item tmp = productions[i].left;
				for (it = followSet[getVNindex(tmp)].begin(); it != followSet[getVNindex(tmp)].end(); it++)
					followSet[getVNindex(target)].insert(*it);
			}
			else return;
		}

		else if(index == len - 1 && target.name != productions[i].left.name)
		{
			getFollowSet(productions[i].left);
			set<string>::iterator it;
			Item tmp = productions[i].left;
			for (it = followSet[getVNindex(tmp)].begin(); it != followSet[getVNindex(tmp)].end(); it++)
				followSet[getVNindex(target)].insert(*it);
		}

	}
	int index = getVNindex(target);
	cout << target.name << " "<< followSet[index].size()<< "--->";

	for (set<string>::iterator it = followSet[index].begin(); it != followSet[index].end(); it++)
	{
		cout << (*it) << " ";
	}
	cout << endl;
}*/
void Grammer::getFollowSet(Item target)
{
	/*if (target.name == "Expression") {
		int a;
	}*/
	for (int i = 0; i < T; i++)
	{
		int len = productions[i].right.size();
		int index = -1;
		int mark = 0, flag = 0, tag = 0;
		for (int j = 0; j < len; j++)
		{
			if (productions[i].right[j].name == target.name)
			{
				index = j;
				break;
			}
		}
		if (index == -1) continue;
		int next = index + 1;

		while (next < len)
		{
			if (isAction(productions[i].right[next]) == false)
			{
				break;
			}
			else
				next++;
		}
		if (index < len - 1 && next < len)
		{

			for (int k = index + 1; k < len; k++)
			{
				flag = 0;
				Item temp = productions[i].right[k];
				if (isTerminal(temp))
				{
					followSet[getVNindex(target)].insert(temp.name);
					break;
				}
				else if (isNotTerminal(temp))
				{
					set<string>::iterator it;
					for (it = firstSet[getVNindex(temp)].begin(); it != firstSet[getVNindex(temp)].end(); it++)
					{

						if ((*it) == "@")
						{
							flag = 1;
							tag++;
						}
						else
							followSet[getVNindex(target)].insert(*it);
					}
					if (flag == 0)
					{
						break;
					}
				}
				else
					continue;


			}
			for (int j = index + 1; j < len; j++)
			{
				if (productions[i].right[j].type == ItemType::VN)
					mark++;
			}
			if (mark == tag && tag != 0 && target.name != productions[i].left.name)
			{
				//cout << "看这里！！" << productions[i].left.name << endl;
				if (getVNindex(target) < getVNindex(productions[i].left))
					getFollowSet(productions[i].left);
				set<string>::iterator it;
				Item tmp = productions[i].left;
				for (it = followSet[getVNindex(tmp)].begin(); it != followSet[getVNindex(tmp)].end(); it++)
					followSet[getVNindex(target)].insert(*it);
			}
		}
		else if ((next >= len || index == len - 1) && (target.name != productions[i].left.name))
		{
			if (getVNindex(target) < getVNindex(productions[i].left))
				getFollowSet(productions[i].left);
			set<string>::iterator it;
			Item tmp = productions[i].left;
			for (it = followSet[getVNindex(tmp)].begin(); it != followSet[getVNindex(tmp)].end(); it++)
				followSet[getVNindex(target)].insert(*it);
		}
	}
	/*
	int index = getVNindex(target);
	cout << target.name << " " << followSet[index].size() << "--->";

	for (set<string>::iterator it = followSet[index].begin(); it != followSet[index].end(); it++)
	{
		cout << (*it) << " ";
	}
	cout << endl;
	*/
}
void Grammer::createFirstSet()
{
	//cout << "FIRST SET Start!!" << endl;
	for (int i = 0; i < vn.size(); i++)
	{
		//cout << "开始FIRST！" << vn[i].name << endl;

		getFirstSet(vn[i]);

	}

	//cout << "FIRST SET:" << endl;
	/*for (int i = 0; i < vn.size(); i++)
	{
		cout << vn[i].name << ": ";
		set<string>::iterator it;
		for (it = firstSet[i].begin(); it != firstSet[i].end(); it++)
			cout << *it << "  ";
		cout << endl;

	}*/
	//cout << "FIRST SUCCESS!!"<<endl;
}
void Grammer::createFollowSet()
{
	//cout << vn.size();
	//cout << "FOLLOW开始了" << endl;
	int index = getVNindex(start);
	followSet[index].emplace("#");


	/*
	cout << "HERE!" << endl;
	set<string>::iterator f;
	for(f = followSet[index].begin();f!= followSet[index].end();f++)
		cout<<(*f);
	*/
	for (int i = 0; i < vn.size(); i++)
	{
		//cout << "开始建立" << vn[i].name << endl;
		//string name = vn[i].name;
		/*if (name == "Statement" || name == "While-statement" || name == "If-statement" )
			continue;
		/*else if (name == "Expr" || name == "Expression")
		{
			followSet[i].insert(";");
			followSet[i].insert(")");
			continue;
		}*/
		getFollowSet(vn[i]);

	}
	/*cout << "FOLLOW SET:" << endl;
	for (int i = 0; i < vn.size(); i++)
	{
		cout << vn[i].name << "<";
		set<string>::iterator it;
		for (it = followSet[i].begin(); it != followSet[i].end(); it++)
			cout << *it << "  ";
		cout << ">" << endl;
	}
	*/
}
void Grammer::inputAndSolve()
{
	ifstream in("Input.txt");			//改动采用自动机实现
	string s;
	while (getline(in, s))					//读入一行文法
	{
		int state = 0;
		int size = s.length();
		int i = 0;
		string name = "";
		// 在 | 前有空格，后没有，最后有一个空格，箭头后没有空格，< 后没有空格 ，>前没有空格，后也没有

		while (i < size)						//文法读入处理
		{

			switch (state)
			{
			case 0:					//左部
				if (s[i] == '-'&&s[i + 1] == '>') //处理左部
				{
					Item temp{ ItemType::VN,name };
					productions[T].left = temp;
					state = 1;
					name = "";
					i += 2;
				}
				else							//拼左部
				{
					name += s[i];
					i++;
				}
				break;
			case 1:								//右部
				if (s[i] == '|' || i == size - 1 || s[i + 1] == '\n')			//处理右部
				{
					if (name != "")
					{
						Item temp{ ItemType::VT,name };
						productions[T].right.push_back(temp);
					}
					T++;		//代表右部已产生
					if (s[i] == '|')
					{
						productions[T].left = productions[T - 1].left;
						state = 1;
					}
					else
					{
						state = 0;
					}
					i++;
					name = "";
				}
				else if (s[i] == ' ')			//处理右部的一个文法符号
				{
					if (name == "")i++;		//无操作 继续读
					else
					{
						Item temp{ ItemType::VT,name };
						productions[T].right.push_back(temp);
						//productions[T].right[Right_index].name = name;
						name = "";
						i++;
					}

				}
				else if (s[i] == '<')			//读到语义动作
				{
					state = 2;
					i++;
				}
				else
				{
					name += s[i];
					i++;
				}
				break;
			case 2:
				if (s[i] == '>')
				{
					Item temp{ ItemType::Action,name,-1 };
					productions[T].right.push_back(temp);
					name = "";
					state = 1;
					i++;
					//语义动作
				}
				else
				{
					name += s[i];
					i++;
				}
				break;
			default: cout << "输入看这里！！"; break;
			}
		}
	}
	in.close();
	createVN_VT_ACTION();
	setType();
	/*
	cout << "文法读入！" << endl;
	for (int i = 0; i < T; i++)
	{
		cout << productions[i].left.name << "->";
		for (int j = 0; j < productions[i].right.size(); j++)
			cout << productions[i].right[j].name << " ";
		//cout << "  " << productions[i].right.size();
		cout << endl;
	}
	*/
}

void Grammer::display()
{

	cout << "FIRST SET:" << endl;
	for (int i = 0; i < vn.size(); i++)
	{
		cout << vn[i].name << ": ";
		set<string>::iterator it;
		for (it = firstSet[i].begin(); it != firstSet[i].end(); it++)
			cout << *it << "  ";
		cout << endl;
	}
	cout << "FOLLOW SET:" << endl;
	for (int i = 0; i < vn.size(); i++)
	{
		cout << vn[i].name << ": ";
		set<string>::iterator it;
		for (it = followSet[i].begin(); it != followSet[i].end(); it++)
			cout << *it << "  ";
		cout << endl;
	}

}
bool Grammer::createLL1_Map()			//需要增加判断				//有错误！！！！！！！！！！！！！！！！！！！！！！1
{
	for (vector<Item>::iterator it = vt.begin(); it != vt.end(); it++)
	{
		if ((*it).name == "@")
		{
			vt.erase(it);
			break;
		}
	}
	/*
	for (int i = 0; i < vn.size(); i++)
	{
			firstSet[i].erase("@");
	}
	*/
	//vt.push_back(Item{ ItemType::VT,"#" });			//终结符加入#

	for (int i = 0; i < vn.size(); i++)				//初始化
		for (int j = 0; j < vt.size(); j++)
			LL1_Map[i][j] = -1;

	for (int i = 0; i < T; i++)
	{
		int len = productions[i].right.size();
		int flag = 0;			//空标志
		int tag = 0;			//所有右部空标识
		//Item tmp = productions[i].right[index];
		for (int k = 0; k < len; k++)
		{
			flag = 0;
			while (k < len)						//省略语义动作
			{
				if (isAction(productions[i].right[k]) == false)
				{
					break;
				}
				else
				{
					k++;
				}

			}
			if (k >= len)
			{
				cout << "Table error!" << endl;
				return false;
			}

			Item tmp = productions[i].right[k];
			if (isTerminal(tmp))			//终结符 
			{
				if (tmp.name != "@")
				{
					if (LL1_Map[getVNindex(productions[i].left)][getVTindex(tmp)] != -1)
					{
						cout << productions[i].left.name << " " << LL1_Map[getVNindex(productions[i].left)][getVTindex(tmp)];
						cout << "error1 " << endl;
						return false;
					}
					LL1_Map[getVNindex(productions[i].left)][getVTindex(tmp)] = i;
				}
				if (tmp.name == "@")
				{
					set<string>::iterator  it;
					//cout << "就是这里！！！！" << endl;
					for (it = followSet[getVNindex(productions[i].left)].begin(); it != followSet[getVNindex(productions[i].left)].end(); it++)
					{
						if (LL1_Map[getVNindex(productions[i].left)][getVTindex(*it)] != -1)
						{
							cout << productions[i].left.name << " " << LL1_Map[getVNindex(productions[i].left)][getVTindex(*it)];
							cout << "error2 " << endl;
							return false;
						}
						LL1_Map[getVNindex(productions[i].left)][getVTindex(*it)] = i;
					}
				}
				break;
			}
			else if (isNotTerminal(tmp))						//非终结符
			{
				set<string>::iterator it;
				for (it = firstSet[getVNindex(tmp)].begin(); it != firstSet[getVNindex(tmp)].end(); it++)
				{

					if ((*it) == "@")
					{
						flag = 1;
						tag++;
					}
					else
					{
						if (LL1_Map[getVNindex(productions[i].left)][getVTindex(*it)] != -1)
						{
							cout << productions[i].left.name << "->" << *it;
							cout << "error3 " << endl;
							return false;
						}
						LL1_Map[getVNindex(productions[i].left)][getVTindex(*it)] = i;
					}
				}
				if (flag == 0)
				{
					break;
				}
				/*if (firstSet[getVNindex(tmp)].count("@") != 0)				//含空产生式
				{
					set<string>::iterator  it;
					for (it = followSet[getVNindex(productions[i].left)].begin(); it != followSet[getVNindex(productions[i].left)].end(); it++)
					{

						if (LL1_Map[getVNindex(productions[i].left)][getVTindex(*it)] != -1)
						{
							cout << "error4 " << endl;
							return false;
						}
						LL1_Map[getVNindex(productions[i].left)][getVTindex(*it)] = i;
					}
				}
				*/
			}
			else
			{
				cout << "Table error2!" << endl;
				return false;
			}

			int mark = 0;
			for (int j = 0; j < len; j++)
			{
				if (productions[i].right[j].type == ItemType::VN)
					mark++;

			}
			if (mark == tag)
			{
				//cout << "有人在吗！！" << endl;
				set<string>::iterator  it;
				for (it = followSet[getVNindex(productions[i].left)].begin(); it != followSet[getVNindex(productions[i].left)].end(); it++)
				{
					if (LL1_Map[getVNindex(productions[i].left)][getVTindex(*it)] != -1)
					{
						cout << productions[i].left.name << " " << LL1_Map[getVNindex(productions[i].left)][getVTindex(*it)];
						cout << "error4 " << endl;
						return false;
					}
					LL1_Map[getVNindex(productions[i].left)][getVTindex(*it)] = i;
				}
			}

		}
	}
	return true;
}
void Grammer::print_out()
{
	ofstream cout("Table_Output.txt");
	cout << setw(15) << '\0';
	for (int i = 0; i < vt.size(); i++)
	{
		string tercopy;
		tercopy += "  ";
		tercopy += vt[i].name;
		cout << left << setw(15) << tercopy;
	}
	cout << endl;
	for (int i = 0; i < vn.size(); i++)
	{
		string nonter;
		nonter = vn[i].name + ": ";
		cout << setw(10) << nonter;

		for (int j = 0; j < vt.size(); j++)
		{
			if (LL1_Map[i][j] == -1)
				cout << setw(10) << "@";
			else
			{
				/*
				int k = 0;
				string ss;
				ss += vn[i].name;
				ss += "->";
				for (Item temp : productions[LL1_Map[i][j]].right)
				{
					ss += temp.name;
				}
				cout << left << setw(15) << ss;
				*/
				cout << left << setw(10) << LL1_Map[i][j];

			}
		}
		cout << endl;
	}
	//cout.close();
}
void Grammer::manage()
{
	inputAndSolve();
	createFirstSet();
	createFollowSet();
	//display()
	if (createLL1_Map() == false)
	{
		cout << "错误：非LL1文法" << endl;
		return;
	}
	print_out();
	if (runParserLL1() == false)
	{
		cout << "错误：语法分析错误" << endl;
	}
	lex.print();
	return;
}
bool Grammer::runParserLL1()		//语法分析
{
	stack<Item>SYN;		//语法栈
	stack<Item>SEM;
	SYN.push(Item{ ItemType::VT,"#",Token_tag::End });
	SYN.push(start);
	semantic.push();
	Item x = SYN.top();
	//lex.set("void main(){int a;}#");

	fstream fs("text.txt"); // 创建个文件流对象,并打开"file.txt"
	stringstream ss; // 创建字符串流对象
	ss << fs.rdbuf(); // 把文件流中的字符输入到字符串流中
	string str = ss.str(); // 获取流中的字符串 
	lex.set(str + "#");

	semantic.set_prev_token(cur);
	cur = lex.next_token();
	semantic.set_cur_token(cur);
	semantic.set_ident(lex.ptr_ident());
	while (SYN.empty() == false)
	{
		SYN.pop();
		//cout << "\n\nstack:" << x.name << endl;
		while (cur.id == 0 || cur.id == 32 || cur.id == 9)
		{
			cur = lex.next_token();
		}
		if (isTerminal(x))			//终结符
		{
			//cout << x.name << x.tag;
			//cout << "mark 1" << endl;
			cout << "①：匹配				";
			if (match(x) == true)
			{
				if (x.name == "#")
				{
					cout << "语法分析成功！" << endl;
					return true;
				}
			}
			else
			{
				cout << "匹配失败" << endl;
				return false;
			}

		}
		else if (isNotTerminal(x))						//非终结符
		{
			int index = LL1_Map[getVNindex(x)][getVTindex(cur)];
			cout << "②：查表				";
			if (index == -1)
			{
				
				/*cout << cur.id;
				cout << "看这里" << x.name << vt[getVTindex(cur)].name << endl;
				cout << x.name << endl << x.tag;
				cout << "\nerror2!" << endl;
				*/
				cout << "查表失败！" << endl;
				return false;
			}
			else
			{
				push_right(SYN, index);
				//cout<<
				//cout << "mark 2 " << cur.id << endl;
			}
		}
		else if (isAction(x))						//语义动作
		{

			//调用语义动作函数
			/*
			cout << x.name << x.tag << "mark 3" << endl;
			cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << x.tag << endl;
			*/
			cout << "③：执行语义动作			" << x.name << endl;
			try {
				void (Semantic::*xx)() = semantic.func[x.tag];
				(semantic.*xx)();
			}
			catch (const char*msg) {

				printf("\n\n***Error ! 语义错误\n");
				cerr << msg << endl;
				exit(1);
			}
			 

		}
		else
		{
			cout << "异常错误：无类型可匹配" << endl;
			return false;
		}
		x = SYN.top();

	}

}
bool Grammer::match(Item x)
{
	if (x.tag == cur.id)
	{
		
		cout << "match" <<" "<< x.name << endl;
		semantic.set_prev_token(cur);
		cur = lex.next_token();
		semantic.set_cur_token(cur);
		return true;
	}
	else
	{
		/*cout << "x:" << x.name << x.tag << " 待匹配" << cur.id;
		if (x.tag == cur.id)cout << "嘿嘿";
		semantic.set_prev_token(cur);
		cur = lex.next_token();
		semantic.set_cur_token(cur);
		*/
		return false;
	}
}
void Grammer::push_right(stack<Item>&SYN, int index)
{
	vector<Item> temp = productions[index].right;
	cout << "将产生式"<<index<<"压栈: " ;
	if ((temp.size() == 0) || (temp.size() == 1 && temp[0].name == "@"))
	{
		cout << "@"<<endl;
		return;
	}
	for (int i = temp.size() - 1; i >= 0; i--)
	{
		SYN.push(temp[i]);
		cout << temp[i].name << " ";
	}
	cout << endl;
}
/*string Grammer::next()
{
	string s;
	cout << "请输入w:" << endl;
	cin >> s;
	return s;
}*/