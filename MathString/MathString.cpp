// MathString.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <iostream>
#include<vector>
#include<string>
#include<boost\foreach.hpp>
#include <iostream>
#include <string>
#include <cctype>
#include <stdlib.h>
#include<math.h>
#include <conio.h>

#define Debug(x) cout<<"DEBUG: "#x <<" = " <<x<<endl;
#define and &&
#define or ||
#define Cast stold

using namespace std;

class MathString;
bool isMathToken(char a){
	if (a == '+' or a == '/' or a == '*')return true;
	else return false;
}
string ReplaceAll(string& str, const string& from, const string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
	return str;
}
bool isNum(char s) {
	if (!isalpha(s))
		return false;
	return true;
}
bool ifFunct(string text){
	if (text.length() > 0){
		if (text.at(text.length() - 1) == ')')return true;

	}
	return false;
}
void FunctionParse(string t, string &A, string&B, string&C){
	int a = 0;
	int Brack = -1;
	for (int i = 0; i < t.length() - 1; i++){
		char x = t.at(i);
		if (x == '(')Brack++;
		if (x == ')')Brack--;

		if (x == '(' and a == 0)a = 1;
		else if (x == ',' and a == 1 and Brack == 0)a = 2;
		else{
			if (a == 0)A = A + x;
			if (a == 1)B = B + x;
			if (a == 2)C = C + x;

		}
	}
}
string RemoveZero(string Text){
	string ret;
	int Position = Text.length() - 1;
	for (; Text[Position] == '0' && Text[Position] != '.' and Position > 0; Position--){}
	if (Text[Position] == '.')Position--;
	for (; Position >= 0; Position--){
		ret = Text[Position] + ret;
	}
	return ret;

}
struct Function{
	string Name;
	void(*Pointer)(MathString* Obj);
	Function(string N, void(*P)(MathString* Obj)){
		Name = N;
		Pointer = P;
	}
};
struct Variable{
	string Name;
	double Value;
	Variable(string N, double V){
		Name = N;
		Value = V;
	}
};
class MathString{
	vector<Function> Functions;
	vector<Variable> Vars;
	double Argument;
	double Number;
	double Return;
	string Template;
	bool debug;
	vector<string> Do(string R, int level = 0){
		ReplaceAll(R, " ", "");
		ReplaceAll(R, "++", "+");
		ReplaceAll(R, "--", "+");
		ReplaceAll(R, "+-", "-");
		ReplaceAll(R, "-", "+-");
		ReplaceAll(R, "*+-", "*-");
		ReplaceAll(R, "/+-", "/-");
		ReplaceAll(R, ")(", ")*(");
		ReplaceAll(R, "[", "(");
		ReplaceAll(R, "]", ")");


		vector < string > Row_;
		string tmp;
		int Brackle = 0;
		BOOST_FOREACH(char t, R){
			if (t == '(')Brackle++;
			if (t == ')')Brackle--;

			if (isMathToken(t) and Brackle == 0){
				Row_.push_back(tmp);
				tmp = t;
				Row_.push_back(tmp);
				tmp = "";
			}
			else{
				tmp = tmp + t;
			}
		}
		Row_.push_back(tmp);
		for (int i = 0; i < Row_.size(); i++){
			for (int j = 0; j < Vars.size(); j++){
				if (Row_.at(i) == Vars.at(j).Name){
					Row_.at(i) = to_string(Vars.at(j).Value);

				}
			}
		}
#define lvl for (int i = 0; i < level; i++)cout << "    "
	/*	if (debug){
			//lvl;
			cout << level << endl;
		}*/
		for (int i = 0; i < Row_.size(); i++){
			if (Row_.at(i).length() > 0){
				if (ifFunct(Row_.at(i))){
					string N, R, Arg;
					FunctionParse(Row_.at(i), N, R, Arg);
					
					vector<string> a = Do(R, level + 1);
					Calcule(a);
					
					if (N.length() == 0)Row_.at(i) = a.at(0);

					else{
						Argument = atof(Do(Arg,level+1).at(0).c_str());
						Number = atof(a.at(0).c_str());
						for (int i = 0; i < Functions.size(); i++){
							if (Functions.at(i).Name == N){
								Functions.at(i).Pointer(this);
								break;
							}
						}
						Row_.at(i) = to_string(Return);
					}
				/*	if (debug){
						lvl; cout << "Function:" << endl;
						lvl; cout << " -Name:     " << N << endl;
						lvl; cout << " -Row:      " << R << "  (" << Number << ")" << endl;
						lvl; cout << " -Argument: " << Arg << "  (" << Argument << ")" << endl;
					}*/
					
				}
				//else{
					if (debug){
						lvl; cout << "Token:" << 1 + i << " | " << Row_.at(i) << endl;
					}
				//}

			}
		}
		Calcule(Row_);
		return Row_;
	}
	static vector<string> DeleteEmpty(vector<string> Row){
		vector<string> Ret;
		for (int i = 0; i < Row.size(); i++){
			if (Row.at(i).length() > 0)Ret.push_back(Row.at(i));
		}
		return Ret;
	}
	static void Calcule(vector<string> &Row){
		for (int z = 0; z < 3; z++){
			for (int i = 1; i < Row.size() - 1; i++){
				if (Row.at(i) == "*"){
					double sum =
						atof(Row.at(i - 1).c_str()) *
						atof(Row.at(i + 1).c_str());
					Row.at(i + 1) = "";
					Row.at(i - 1) = "";
					Row.at(i) = to_string(sum);
					Row = DeleteEmpty(Row);
				}
			}
			for (int i = 1; i < Row.size() - 1; i++){
				if (Row.at(i) == "/"){
					double sum =
						atof(Row.at(i - 1).c_str()) /
						atof(Row.at(i + 1).c_str());
					Row.at(i + 1) = "";
					Row.at(i - 1) = "";
					Row.at(i) = to_string(sum);
					Row = DeleteEmpty(Row);
				}
			}
			for (int i = 1; i < Row.size() - 1; i++){
				if (Row.at(i) == "+"){
					double sum =
						atof(Row.at(i - 1).c_str()) +
						atof(Row.at(i + 1).c_str());
					Row.at(i + 1) = "";
					Row.at(i - 1) = "";
					Row.at(i) = to_string(sum);
					Row = DeleteEmpty(Row);
				}
			}
			
		}
	}
public:
	void PushVariable(string Name, double Value){
		PopVariable(Name);
		Vars.push_back(Variable(Name, Value));
	}
	void PopVariable(string Name){
		for (int i = 0; i < Vars.size(); i++){
			if (Name == Vars.at(i).Name)Vars.at(i).Name = "[DELETE]";
		}
	}
	void ReturnNumber(double a){
		Return = a;
	}
	double GetArgument(){
		return Argument;
	}
	double GetNumber(){
		return Number;
	}
	void LinkFunction(void(*Pointer)(MathString* Obj), string Name){
		Functions.push_back(Function(Name, *Pointer));
	}
	MathString(string Row,bool Debug = false){
		Template = Row;
		debug = Debug;
	}
	double GetResluts(){
		vector<string> A = Do(Template);
		if (A.size() > 1)cout << "Syntax Error" << endl;
		return atof(A.at(0).c_str());
	}

};




void sqrt(MathString *Obj){
	Obj->ReturnNumber(sqrt(Obj->GetNumber()));
}
void pow(MathString *Obj){
	Obj->ReturnNumber(pow(Obj->GetNumber(), Obj->GetArgument()));
}
int _tmain(int argc, _TCHAR* argv[])
{
	string a = "pow(1+1,1+1)*2+1";
	MathString 	test(a);
	test.LinkFunction(&sqrt, "sqrt");
	test.LinkFunction(&pow, "pow");
	cout << a << endl;
	cout << test.GetResluts() << endl;


	system("pause");
	return 0;
}