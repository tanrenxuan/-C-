#include<iostream>
#include<cstdio>
#include<map>
#include<map>
#include<set>
#include<vector>
#include<iomanip>
#include<cstring>
#include "lex.cpp"
using namespace std;
struct record{  // 符号表的字段 
	string name;  // 名字
	string type;  // 类型
	int address;  // 相对地址
	int line;  // 声明行 
	record(string n,string t,int a,int l): name(n),type(t),address(a),line(l) {}
	record(): name(""),type(""),address(0),line(0) {}
};
struct typeAndWidth{
	string type;
	int width;
	typeAndWidth(string t,int w): type(t),width(w) {}
	typeAndWidth(): type(""),width(0) {}
};
struct numAndName{
	int num;
	set<string> name;
	numAndName(int n,set<string> na): num(n),name(na) {}
	numAndName(){
		num = 0;
		set<string> temp;
		name = temp;
	}
};
struct entryAndType{
	string entry;
	string type;
	entryAndType(string e,string t): entry(e),type(t) {}
	entryAndType(): entry(""),type("") {}
};
struct fourTumple{
	int codeLine;
	string op;
	string arg1;
	string arg2;
	string result;
	fourTumple(int c,string o,string a1,string a2,string r): codeLine(c),op(o),arg1(a1),arg2(a2),result(r) {}
	fourTumple(): codeLine(0),op(""),arg1(""),arg2(""),result("") {}; 
};
map<string,record> symbolTable;  // 用来存放符号表
map<string,int> typeTable;  // 根据类型识别大小 
int offset;  // 相对地址 
int Cpoint;  // 遍历wordSeq 
vector<fourTumple> code;  // 存放中间代码，四元式 
string currentWord;
int codeLine;  // 四元式行数 
int T[1000];

void match(){
	Cpoint++;
	if(Cpoint==wordSeq.size()) return;
	currentWord = wordSeq[Cpoint].mark;
}

void gen(int codeLine,string opt,string arg1,string arg2,string result){
	code.push_back(fourTumple(codeLine,opt, arg1, arg2, result));
}

string newTemp(string type){
	int i = 0;
	for(;i<1000;i++){
		if(T[i]==0) break;
	}
	string temp = "T" + to_string(i);
	symbolTable[temp] = record(temp,type,offset,-1);
	T[i] = 1;
	offset += typeTable[type];
	return temp;	
}

void printFourTumple(){
	cout<<"****************************中间代码(四元式)*****************************"<<endl<<endl;
    cout<<setw(15)<<"行号"<<"\t"<<setw(5)<<"op"<<"\t"<<setw(5)<<"arg1"<<"\t"<<setw(5)<<"arg2"<<"\t"<<setw(5)<<"result"<<endl;	
    for(int i=0;i<code.size();i++){
    	cout<<setw(15)<<code[i].codeLine<<"\t"<<setw(5)<<code[i].op<<"\t"<<setw(5)
		    <<code[i].arg1<<"\t"<<setw(5)<<code[i].arg2<<"\t"<<setw(5)<<code[i].result<<endl;	
	}
}

void printCode(){
	cout<<"****************************中间代码*****************************"<<endl<<endl;
	for(int i=0;i<code.size();i++){
		cout<<"("<<code[i].codeLine<<")";
		if(code[i].op=="get" || code[i].op=="put"){
			cout<<code[i].op<<"("<<code[i].result<<")"<<endl;
		}else if(code[i].op==":="){
			cout<<code[i].result<<":="<<code[i].arg1<<endl;
		}else if(code[i].op=="+" || code[i].op=="-" || code[i].op=="&" || code[i].op=="|"){
			cout<<code[i].result<<":="<<code[i].arg1<<code[i].op<<code[i].arg2<<endl;
		}else if(code[i].op=="goto"){
			cout<<"goto "<<code[i].result<<endl;
		}else{  // 否则为比较运算符，把么他就是一个if arg1 relop arg2 goto result语句 
			cout<<"if "<<code[i].arg1<<code[i].op<<code[i].arg2<<" goto "<<code[i].result<<endl;
		}
	}
}

void printSymbolTable(){
	cout<<"****************************符号表*****************************"<<endl<<endl;
    cout<<setw(15)<<"序号"<<setw(15)<<"名字"<<"\t"<<setw(3)<<"类型"<<"\t"<<setw(8)<<"存储地址"<<"\t"<<"行号"<<endl;
    int i = 1;
    for(auto it=symbolTable.begin();it!=symbolTable.end();it++,i++){
    	record tempRecord= it->second;
    	cout<<setw(15)<<i<<setw(15)<<tempRecord.name<<"\t"<<setw(3)<<tempRecord.type<<"\t"<<setw(8)<<tempRecord.address<<"\t"<<tempRecord.line<<endl;
	}
}

// 利用LL(1)文法的递归预测分析进行语法制导翻译
// ****************************变量声明语句*****************************
typeAndWidth fxQ(){  // Q->i    Q->b
	if(currentWord=="i"){  // int类型 
		match();
		return typeAndWidth("int",4);
	}else if(currentWord=="b"){  // bool类型
		match(); 
		return typeAndWidth("bool",1);
	}else{
		cout<<"第"<<wordSeq[Cpoint].line<<"行错误，此时需要变量类型"<<endl;
		exit(1);
	}
}

numAndName fxN(){  // N->,dN1  N->$
	if(currentWord==","){
		match();
		if(currentWord=="d"){
			string dname = wordSeq[Cpoint].str;  // d.name
			match();
			numAndName temp = fxN();
			set<string> nameSet = temp.name;
			nameSet.insert(dname);
			int num = temp.num + 1; 
			return numAndName(num,nameSet); 
		}else{
			cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误，','后缺少标识符"<<endl;
			exit(1);
		}
	}else{
		// 若不为',',表示使用 N->$
		set<string> tempSet;
		return numAndName(0,tempSet); 
	}
}

numAndName fxF(){  // F->dN
	if(currentWord=="d"){
		string dname = wordSeq[Cpoint].str;  // d.name
		match();
		numAndName temp = fxN();
		set<string> nameSet = temp.name;
		nameSet.insert(dname);
		int num = temp.num + 1;
		return numAndName(num,nameSet);		
	}else{
//		cout<<Cpoint<<" "<<currentWord<<endl;
		cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误，类型后缺少标识符"<<endl;
		exit(1);
	}
}


void fxD(){  // D->QF;
	typeAndWidth tempA = fxQ();
	numAndName tempB = fxF();
	for(auto it=tempB.name.begin();it!=tempB.name.end();it++){
		string tempName = *it;  // 声明的标识符名字
		if(symbolTable.find(tempName)!=symbolTable.end()){  // 表示找到 
			cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误，标识符"<<tempName<<"重复定义"<<endl;
			// 此处可不退出，对于重复定义的标识符，不用管即可 
		}else{  // 没有找到标识符 
			symbolTable[tempName] = record(tempName,tempA.type,offset,wordSeq[Cpoint].line);
			offset += tempA.width;
		} 
	}
	if(currentWord!=";"){
		cout<<"第"<<wordSeq[Cpoint-1].line<<"行出现错误，缺少';'"<<endl;
		exit(1); 
	}else{
		match();
	}
} 

// ****************************赋值语句*****************************
entryAndType fxT(entryAndType Ti);
entryAndType fxE(){ //E->dT E->nT E->(E)T
	if(currentWord=="d"){
		string dname = wordSeq[Cpoint].str;  // d.name
		match();
		string T_entry;
		string T_type;
		if(symbolTable.find(dname)!=symbolTable.end()){
			T_entry = dname;
			T_type = symbolTable[dname].type;
			entryAndType E_entry = fxT(entryAndType(T_entry,T_type));
			if(E_entry.type!=T_type){
				cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,变量类型不一致，无法计算"<<endl;
				exit(1);		
			}
			return E_entry;
			
		}else{  // 没找到标识符 
			cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,变量"<<dname<<"未声明"<<endl;
			exit(1);			
		}
	}else if(currentWord=="n"){
		string nname = wordSeq[Cpoint].str;  // n.entry
		match();
		string T_entry = nname;
		string T_type = "int";
		entryAndType E_entry = fxT(entryAndType(T_entry,T_type));
		if(E_entry.type!=T_type){
			cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,变量类型不一致，无法计算"<<endl;
			exit(1);		
		}
		return E_entry;
	}else if(currentWord=="("){
		match();
		entryAndType E_entry1 = fxE();
		if(currentWord==")"){
			match();
		}else{
			cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,缺失右括号"<<endl;
			exit(1);
		}
		string T_entry = E_entry1.entry;
		string T_type = E_entry1.type;
		entryAndType E_entry = fxT(entryAndType(T_entry,T_type));
		if(E_entry.type!=T_type){
			cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,变量类型不一致，无法计算"<<endl;
			exit(1);		
		}
		return E_entry; 
	}else{
		cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误，丢失操作数"<<endl;
		exit(1); 
	}
} 

entryAndType fxT(entryAndType Ti){  // 返回entry  T->+ET T->-ET T->&ET T->|ET T->$
	entryAndType Ts;
	entryAndType E;
	if(currentWord=="+"){
		match();
		E = fxE();
		string T1i = newTemp(E.type);
		gen(++codeLine,"+",Ti.entry,E.entry,T1i);
//		cout<<"("<<codeLine<<")"<<T1i<<":="<<Ti.entry<<"+"<<E.entry<<endl;
		Ts = fxT(entryAndType(T1i,E.type));
	}else if(currentWord=="-"){
		match();
		E = fxE();
		string T1i = newTemp(E.type);
		gen(++codeLine,"-",Ti.entry,E.entry,T1i);
//		cout<<"("<<codeLine<<")"<<T1i<<":="<<Ti.entry<<"-"<<E.entry<<endl;
		Ts = fxT(entryAndType(T1i,E.type));;		
	}else if(currentWord=="&"){
		match();
		E = fxE();
		string T1i = newTemp(E.type);
		gen(++codeLine,"&",Ti.entry,E.entry,T1i);
//		cout<<"("<<codeLine<<")"<<T1i<<":="<<Ti.entry<<"&"<<E.entry<<endl;
		Ts = fxT(entryAndType(T1i,E.type));		
	}else if(currentWord=="|"){
		match();
		E = fxE();
		string T1i = newTemp(E.type);
		gen(++codeLine,"|",Ti.entry,E.entry,T1i);
//		cout<<"("<<codeLine<<")"<<T1i<<":="<<Ti.entry<<"|"<<E.entry<<endl;
		Ts = fxT(entryAndType(T1i,E.type));
	}else if(currentWord=="d"){
		cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,丢失运算符"<<endl;
		exit(1);		
	}else{
		Ts = Ti;
	}
	return Ts;
}

 

void fxC(){  // C->d=E 
	if(currentWord=="d"){
		string dname = wordSeq[Cpoint].str;  // d.name
		match();
		if(symbolTable.find(dname)==symbolTable.end()){
			cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,变量"<<dname<<"未声明"<<endl;
			exit(1);
		}
		if(currentWord=="="){
			match();
			entryAndType E_entry = fxE();;
			if(currentWord==";"){
				match();
				string dtype = symbolTable[dname].type;
				if(dtype!=E_entry.type){
					cout<<"第"<<wordSeq[Cpoint-1].line<<"行出现错误，变量类型"<<dtype<<"和"<<E_entry.type<<"不一致，无法赋值"<<endl;
					exit(1);
				}
				gen(++codeLine,":=",E_entry.entry,"--",dname); 
//				cout<<"("<<codeLine<<")"<<dname<<":="<<E_entry.entry<<endl;
			}else{
				cout<<"第"<<wordSeq[Cpoint-1].line<<"行出现错误，缺少';'"<<endl;
				exit(1); 				
			}
		}else{
			cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误，缺少'='"<<endl;
			exit(1);
		}
	}else{
		cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误，缺少标识符"<<endl;
		exit(1);
	}
} 

// ****************************get语句*****************************
void fxJ(){  // J->g(F)
	if(currentWord=="g"){
		match();
		if(currentWord=="("){
			match();
			numAndName temp = fxF();
			for(auto it=temp.name.begin();it!=temp.name.end();it++){
				if(symbolTable.find(*it)==symbolTable.end()){  // 没有声明 
					cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,变量"<<*it<<"没有声明"<<endl;
					exit(1);
				}
				gen(++codeLine,"get","--","--",*it);
//				cout<<"("<<codeLine<<")"<<"get"<<"("<<*it<<")"<<endl;
			}
			if(currentWord==")"){
				match();
				if(currentWord==";"){
					match();
				}else{
					cout<<"第"<<wordSeq[Cpoint-1].line<<"行出现错误,此处缺少';'"<<endl;
					exit(1);
				}
			}else{
				cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,此处缺少')'"<<endl;
				exit(1);				
			}
		}else{
			cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,此处缺少'('"<<endl;
			exit(1);
		} 
	}else{
		cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误"<<endl;
		exit(1);
	}
}
// ****************************put语句*****************************
void fxK(){  // K->p(F)
	if(currentWord=="p"){
		match();
		if(currentWord=="("){
			match();
			numAndName temp = fxF();
			for(auto it=temp.name.begin();it!=temp.name.end();it++){
				if(symbolTable.find(*it)==symbolTable.end()){  // 没有声明 
					cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,变量"<<*it<<"没有声明"<<endl;
					exit(1);
				}
				gen(++codeLine,"put","--","--",*it);
//				cout<<"("<<codeLine<<")"<<"put"<<"("<<*it<<")"<<endl;
			}
			if(currentWord==")"){
				match();
				if(currentWord==";"){
					match();
				}else{
					cout<<"第"<<wordSeq[Cpoint-1].line<<"行出现错误,此处缺少';'"<<endl;
					exit(1);
				}
			}else{
				cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,此处缺少')'"<<endl;
				exit(1);				
			}
		}else{
			cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,此处缺少'('"<<endl;
			exit(1);
		} 
	}else{
		cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误"<<endl;
		exit(1);
	}
}

// ****************************条件判断语句*****************************
string fxG(){ // G->ErE
	entryAndType temp1 = fxE();
	if(currentWord=="r"){
		string relop = wordSeq[Cpoint].str;
		match();
		entryAndType temp2 = fxE();
		string G_entry = newTemp("bool");
		++codeLine;
		gen(codeLine,relop,temp1.entry,temp2.entry,to_string(codeLine+3));
//		cout<<"("<<codeLine<<")"<<"if "<<temp1.entry<<relop<<temp2.entry<<" goto "<<codeLine+3<<endl;
		gen(++codeLine,":=","0","--",G_entry);
//		cout<<"("<<codeLine<<")"<<G_entry<<":="<<"0"<<endl;
		++codeLine;
		gen(codeLine,"goto","--","--",to_string(codeLine+2));
//		cout<<"("<<codeLine<<")"<<"goto "<<codeLine+2<<endl;
		gen(++codeLine,":=","1","--",G_entry);
//		cout<<"("<<codeLine<<")"<<G_entry<<":="<<"1"<<endl;
		return G_entry;
	}else{
		cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,缺少比较运算符"<<endl;
		exit(1);		
	}
}

// ****************************if语句*****************************
void fxP();
void fxH(){  // H->f(G){P};
	if(currentWord=="f"){
		match();
		if(currentWord=="("){
			match();
			string G_entry = fxG();
			gen(++codeLine,"==",G_entry,"0","UN");  // 暂时存放undefined 
			int currentLine = codeLine;
//			cout<<"("<<codeLine<<")"<<"if "<<G_entry<<"==0"<<" goto "<<"undifined"<<endl;
			if(currentWord==")"){
				match();
				if(currentWord=="{"){
					match();
					fxP();
					if(currentWord=="}"){
						match();
						if(currentWord==";"){
							match();
							// 修改当时的"undifined"
							for(int i=0;i<code.size();i++){
								if(code[i].codeLine==currentLine){
									code[i].result = to_string(codeLine+1);
									break;
								}
							} 
//							cout<<"第("<<currentLine<<")的undifined修改为"<<codeLine+1<<endl;
						}else{
							cout<<"第"<<wordSeq[Cpoint-1].line<<"行出现错误,缺少';'"<<endl;
							exit(1);								
						}
					}else{
						cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,缺少'}'"<<endl;
						exit(1);						
					} 
				}else{
					cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,缺少'{'"<<endl;
					exit(1);						
				}
			}else{
				cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,缺少')'"<<endl;
				exit(1);					
			}
		}else{
			cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,缺少'('"<<endl;
			exit(1);			
		}
	}else{
		cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误"<<endl;
		exit(1);		
	}
}

// ****************************while语句*****************************
void fxI(){  // I->w(G){P};
	if(currentWord=="w"){
		match();
		if(currentWord=="("){
			match();
			int lastLine = codeLine + 1;
			string G_entry = fxG();
			gen(++codeLine,"==",G_entry,"0","UN");  // 暂时存放undefined 
			int currentLine = codeLine;
//			cout<<"("<<codeLine<<")"<<"if "<<G_entry<<"==0"<<" goto "<<"undifined"<<endl;
			if(currentWord==")"){
				match();
				if(currentWord=="{"){
					match();
					fxP();
					if(currentWord=="}"){
						match();
						if(currentWord==";"){
							match();
							gen(++codeLine,"goto","--","--",to_string(lastLine));
							// 修改当时的"undifined"
							for(int i=0;i<code.size();i++){
								if(code[i].codeLine==currentLine){
									code[i].result = to_string(codeLine+1);
									break;
								}
							}
//							cout<<"第("<<currentLine<<")的undifined修改为"<<codeLine+1<<endl;
															
						}else{
							cout<<"第"<<wordSeq[Cpoint-1].line<<"行出现错误,缺少';'"<<endl;
							exit(1);								
						}
					}else{
						cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,缺少'}'"<<endl;
						exit(1);						
					} 
				}else{
					cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,缺少'{'"<<endl;
					exit(1);						
				}
			}else{
				cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,缺少')'"<<endl;
				exit(1);					
			}
		}else{
			cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误,缺少'('"<<endl;
			exit(1);			
		}
	}else{
		cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误"<<endl;
		exit(1);		
	}
}


// ****************************程序构成语句*****************************
void fxL(){  // L->D|C|J|K|H|I
	if(currentWord=="i" || currentWord=="b"){
		fxD();
	}else if(currentWord=="d"){
		fxC();
	}else if(currentWord=="g"){
		fxJ();
	}else if(currentWord=="p"){
		fxK();
	}else if(currentWord=="f"){
		fxH();
	}else if(currentWord=="w"){
		fxI();
	}else{
		cout<<"第"<<wordSeq[Cpoint].line<<"行出现错误"<<endl;
		exit(1);
	}	
}

void fxO(){  // O->LO | $
	if(currentWord=="i" || currentWord=="b" || currentWord=="d"
	|| currentWord=="g" || currentWord=="f" || currentWord=="p" || currentWord=="w"){
		fxL();
		fxO();
	}
}

void fxP(){  // P->LO
	fxL();
	fxO();
}

int semantic(){
	lex();
	//结果存放在了vector<TwoTumple> wordSeq中
	memset(T,0,sizeof(T)); 
	typeTable["int"] = 4;
	typeTable["bool"] = 1;
	Cpoint = 0;
	codeLine = 0;
	currentWord = wordSeq[Cpoint].mark;
	fxP();
	printSymbolTable();
	printFourTumple();
	printCode();
	if(Cpoint!=wordSeq.size()) cout<<"不符合程序定义要求，请检查"<<endl;
	return 0;
} 
