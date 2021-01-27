#include <iostream>
#include <math.h>
#include <string>
#include <string.h>
#include <vector>
#include <tuple>

std::string variablename(std::string ts){
	std::string name = "";
	int intlast = 0;
	for (unsigned int a = 0; a < ts.size(); a++) if (std::string("1234567890.").find(std::string(1,ts[a])) == std::string::npos) {intlast = a;break;};
	ts.erase(ts.begin(),ts.begin()+intlast);
	return ts;
}

float variablevalue(std::string ts){
	int intlast = 0;
	for (unsigned int a = 0; a < ts.size(); a++) if (std::string("1234567890.").find(std::string(1,ts[a])) == std::string::npos) {intlast = a;break;};
	if (intlast != 0) {ts.erase(ts.begin()+intlast,ts.begin()+(ts.size()-intlast+1));return std::stof(ts);}
	else return 1;
}

bool isvariable(std::string ts){
	bool ret = false;
	for (auto a : ts) if (std::string("1234567890.").find(std::string(1,a)) == std::string::npos) {ret = true;break;};
	return ret;
}

int whereisThevariable(std::string ts, std::vector<std::string>alg){
	int res = 0;
	std::string name = variablename(ts);
	for (unsigned int a = 0; a < alg.size(); a++) {if (name == variablename(alg[a])) {res = a; break;}};
	return res;
}

std::tuple<std::string,std::vector<std::string>> eqshort(std::string op, std::vector<std::string>alg){
	for (unsigned int i = 0; i < alg.size();i++){
		//std::cout<<op<<'\n';
		if (isvariable(alg[i])){
			//if (alg[i] == variablename(alg[i])){
			//	alg[i] = std::to_string(variablevalue(alg[i]))+variablename(alg[i]);
			//}
			int where = whereisThevariable(alg[i],alg);
			if (std::string("*/").find(std::string(1,op[where])) == std::string::npos){
				//std::cout<< where <<" <i\n";
				alg.insert(alg.end(),alg[where]);
				alg.erase(alg.begin()+where);
				if (where != 0){
					op += op[where-1];
					op.erase(where-1,1);
				} else {
					char c;
					if (variablevalue(alg[where])>=0) c = '+';
					else c = '-';
					op += c;
					op.erase(where,1);
				}
			}
		} else {
			if (std::stof(alg[i]) == 0){
				alg.erase(alg.begin()+i);
				if (i != 0){
					op.erase(i,1);
				}
			}
		}
	}
	return std::tuple<std::string,std::vector<std::string>>(op,alg);
}

std::string raw_eval(std::string math_p){
	std::vector<std::string> alg;
	std::string op;
	//math_p = math_p ;
	int q = 0;
	std::string num;
	for (auto a : math_p) {
		if (std::string("+-*/=()").find(std::string(1,a)) == std::string::npos) num += std::string(1,a);
		else {
			op += a;
			if (num != ""){
				alg.push_back(num);
				num = "";
			}
		}
	}
	alg.push_back(num);
	int c = op.size();
	for (unsigned int i = 0; i < c; i++){
		auto nshort = eqshort(op,alg);
		op = std::get<0>(nshort);
		alg = std::get<1>(nshort);
		if (op.find('*') != std::string::npos){
			if (!isvariable(alg[op.find('*')]) && !isvariable(alg[op.find('*')+1])){
				std::string res = std::to_string(std::stof(alg[op.find('*')])*std::stof(alg[op.find('*')+1]));
				int delpos = op.find('*');
				alg[delpos] = res;
				alg.erase(alg.begin()+delpos+1);
				op.erase(delpos,1);
			} else if (isvariable(alg[op.find('*')]) && isvariable(alg[op.find('*')+1])){
				if (variablename(alg[op.find('*')]) == variablename(alg[op.find('*')+1])){
					std::string res = std::to_string(variablevalue(alg[op.find('*')])*variablevalue(alg[op.find('*')+1]))+variablename(alg[op.find('*')+1]);
					int delpos = op.find('*');
					alg[delpos] = res;
					alg.erase(alg.begin()+delpos+1);
					op.erase(delpos,1);
				}
			}
		}
		if (op.find('/') != std::string::npos){
			if (!isvariable(alg[op.find('/')]) && !isvariable(alg[op.find('/')+1])){
				std::string res = std::to_string(std::stof(alg[op.find('/')])/std::stof(alg[op.find('/')+1]));
				int delpos = op.find('/');
				alg[delpos] = res;
				alg.erase(alg.begin()+delpos+1);
				op.erase(delpos,1);
			} else if (isvariable(alg[op.find('/')]) && isvariable(alg[op.find('/')+1])){
				if (variablename(alg[op.find('/')]) == variablename(alg[op.find('/')+1])){
					std::string res = std::to_string(variablevalue(alg[op.find('/')])/variablevalue(alg[op.find('/')+1]))+variablename(alg[op.find('/')+1]);
					int delpos = op.find('/');
					alg[delpos] = res;
					alg.erase(alg.begin()+delpos+1);
					op.erase(delpos,1);
				}
			}
		}
		if (op.find('+') != std::string::npos){
			if (std::string("*/").find(op[op.find('+')+1]) == std::string::npos && std::string("*/").find(op[op.find('+')-1]) == std::string::npos){
				if (!isvariable(alg[op.find('+')]) && !isvariable(alg[op.find('+')+1])){
					std::string res = std::to_string(std::stof(alg[op.find('+')])+std::stof(alg[op.find('+')+1]));
					int delpos = op.find('+');
					alg[delpos] = res;
					alg.erase(alg.begin()+delpos+1);
					op.erase(delpos,1);
				}
				if (isvariable(alg[op.find('+')]) && isvariable(alg[op.find('+')+1])){
					if (variablename(alg[op.find('+')]) == variablename(alg[op.find('+')+1])){
						std::string res = std::to_string(variablevalue(alg[op.find('+')])+variablevalue(alg[op.find('+')+1]))+variablename(alg[op.find('+')+1]);
						int delpos = op.find('+');
						alg[delpos] = res;
						alg.erase(alg.begin()+delpos+1);
						op.erase(delpos,1);
					}
				}
			}
		}
		if (op.find('-') != std::string::npos){
			if (std::string("*/").find(op[op.find('-')+1]) == std::string::npos && std::string("*/").find(op[op.find('-')-1]) == std::string::npos){
				if (!isvariable(alg[op.find('-')]) && !isvariable(alg[op.find('-')+1])){
					std::string res = std::to_string(std::stof(alg[op.find('-')])-std::stof(alg[op.find('-')+1]));
					int delpos = op.find('-');
					alg[delpos] = res;
					alg.erase(alg.begin()+delpos+1);
					op.erase(delpos,1);
				}
				if (isvariable(alg[op.find('-')]) && isvariable(alg[op.find('-')+1])){
					if (variablename(alg[op.find('-')]) == variablename(alg[op.find('-')+1])){
						std::string res = std::to_string(variablevalue(alg[op.find('-')])-variablevalue(alg[op.find('-')+1]))+variablename(alg[op.find('-')+1]);
						int delpos = op.find('-');
						alg[delpos] = res;
						alg.erase(alg.begin()+delpos+1);
						op.erase(delpos,1);
					}
				}
			}
		}
	}
	std::string ret = alg[0];
	for (unsigned int a = 0; a < op.size(); a++) ret += std::string(1,op[a])+alg[a+1];
	return ret;
}

std::string evaluate(std::string input){
	input = '(' + input + ')';
	std::string var;
	while (input.find('(') != std::string::npos && input.find(')') != std::string::npos){
		var = input;
		int t = var.find(')');
		var.erase(var.begin()+var.find(')'),var.end());
		int b = var.find_last_of('(');
		var.erase(var.begin(),var.begin()+var.find_last_of('(')+1);
		var = raw_eval(var);
		input.replace(input.begin()+b,input.begin()+t+1,var);
		std::cout<<input<<'\n';
	}
	return raw_eval(input);
}

// 3+(3A*3A)-3

int main(){
	while (1){
		std::string str;
		std::cout << "> "; std::getline(std::cin, str);
		std::cout << evaluate(str) << '\n';
	}
}