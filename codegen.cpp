#include<iostream>
#include<cstdio>
#include<cctype>
#include<cstring>
#include<vector>
#include<map>
#include "semantic.cpp"
using namespace std;
int reg[32];  // ĳһ���Ĵ����Ƿ�ռ�ã���ռ�õ�һ��û�б�д�� 
int label[1000];  
vector<string> regAdress(32);  // ��ĳһ�Ĵ�����ռ�ã����д�ŵı�ʶ�� 
vector<string> obj;  // Ŀ�����
map<string,int> storeReg;  // ��¼Ŀǰ�洢�ڼĴ����еı�ʶ����int��Ӧ��Ĵ����� 
map<string,string> hashLabel;  // ����->��ǩ 

int getFreeReg(){  // ���ؿ��õļĴ����ı�� 
	int i;
	for(int i=0;i<32;i++){
		if(reg[i]==0){
			reg[i] = 1;
			return i;
		}
	}
	return -1;
}

int getLabel(){
	int i;
	for(int i=0;i<1000;i++){
		if(label[i]==0){
			label[i] = 1;
			return i;
		}
	}
	return -1;
}

int ifstoreReg(string id){  // �жϱ�ʶ��id�Ƿ��Ѿ��洢���˼Ĵ����� 
	if(storeReg.find(id)==storeReg.end()){  // û�д浽�Ĵ����� 
		return -1; 
	}else{
		return storeReg[id];  // ���ض�Ӧ�ļĴ����� 
	}
}

int getReg(string id){  // ���id��Ӧ�ļĴ�������û�������� 
	int regNumber;
	if(ifstoreReg(id)==-1){  // idû�д浽�Ĵ����У���Ҫ���� 
		regNumber = getFreeReg();
		storeReg[id] = regNumber;  // ����� 
		regAdress[regNumber] = id;  // ����� 
		string temp;
		if(symbolTable[id].type=="bool"){
			temp = string("    lb ") + string("R") + to_string(regNumber) + string(",") + string("0")
		            	+ string("(") + id +string(")");			
		}else{  // int
			temp = string("    lw ") + string("R") + to_string(regNumber) + string(",") + string("0")
		            	+ string("(") + id +string(")");			
		}
		obj.push_back(temp); 
	}else{  // �Ѿ����ڼĴ����� 
		regNumber = storeReg[id];  
	} 
	return regNumber;	
}

void regToAdd(){  // ����� 
	string temp;
	for(auto it=storeReg.begin();it!=storeReg.end();it++){
		if(symbolTable[it->first].type=="bool"){
			temp = string("    sb ") + string("R") + to_string(it->second) + string(",0") + string("(")
		            + it->first + string(")");			
		}else{  // int
			temp = string("    sw ") + string("R") + to_string(it->second) + string(",0") + string("(")
		            + it->first + string(")");			
		}
		obj.push_back(temp);
	}
}

void OI(fourTumple x){  // line get -- -- result	-> get(result)	line put -- -- result	-> put(result)
	string temp;
	if(x.op=="get"){
		temp = string("    li ") + string("v0") + string(",") + string("5"); 
		obj.push_back(temp);
		temp = string("    syscall");
		obj.push_back(temp);
		// ��ʱ����ֵ�����V0��
		string regX = string("R") + to_string(getReg(x.result));
		temp = string("    move ") + regX + string(",") + string("v0");  
		obj.push_back(temp);
	}else if(x.op=="put"){
		temp = string("    li ") + string("v0") + string(",") + string("1");
		obj.push_back(temp);
		string regX = string("R") + to_string(getReg(x.result));
		temp = string("    move ") + "a0" + string(",") + regX;
		obj.push_back(temp);
		temp = string("    syscall");
		obj.push_back(temp);
	}
} 

void assign(fourTumple x){  // line := arg1 -- result	-> result:=arg1
	string temp;
	if(x.arg1[0]>='0' && x.arg1[0]<='9'){  // �ض���һ�����֣���ʶ�����������ֿ�ͷ 
		int regNumber = getReg(x.result);
		temp = "    li R" + to_string(regNumber) + "," + x.arg1;  // li R,k
		obj.push_back(temp);
		// ��ʱ���ż����Ĵ�����ֵ���ظ�x,ǰ���Ѿ�����˴��x�ļĴ����������Ҫ���ٷŻ� 	 
	}else{  //    move reg(result),reg(arg1)
		int regNumberX = getReg(x.result);
		int regNumberY = getReg(x.arg1);
		temp = string("    move ") + string("R") + to_string(regNumberX) + string(",") + string("R") + to_string(regNumberY);
		obj.push_back(temp);
	}
}

void cal(fourTumple x){  // line op arg1 arg2 result	-> result:=arg1 op arg2  op reg(result),reg(arg1),reg(arg2)
	string regResult,regArg1,regArg2; 
	regResult = string("R") + to_string(getReg(x.result));
	int flag = 0;  // 0��ʾ���Ǳ�ʶ����1��ʾarg1Ϊ���֣�2��ʾarg2Ϊ���֣�3��ʾarg1��arg2��Ϊ���� 
	if(x.arg1[0]>='0' && x.arg1[0]<='9'){
		flag = 1;
		regArg1 = x.arg1;
	}else{
		regArg1 = string("R") + to_string(getReg(x.arg1));
	} 
	if(x.arg2[0]>='0' && x.arg2[0]<='9'){
		if(flag==1) flag = 3;
		else flag = 2;
		regArg2 = x.arg2;
	}else{
		regArg2 = string("R") + to_string(getReg(x.arg2));
	} 
	string temp;
	if(x.op=="+"){  // add
		if(flag==0) temp = string("    add ") + regResult + string(",") + regArg1 + string(",") + regArg2;
		else if(flag==1 || flag==2) temp = string("    addi ") + regResult + string(",") + regArg1 + string(",") + regArg2;
		else{
			int ans = stoi(regArg1) + stoi(regArg2);
			temp = "    li " + regResult + "," + to_string(ans);  // li R,k
		}
	}else if(x.op=="-"){  // sub
		if(flag==0) temp = string("    sub ") + regResult + string(",") + regArg1 + string(",") + regArg2;
		else if(flag==1 || flag==2) temp = string("    subi ") + regResult + string(",") + regArg1 + string(",") + regArg2;
		else{
			int ans = stoi(regArg1) - stoi(regArg2);
			temp = "    li " + regResult + "," + to_string(ans);  // li R,k
		}		
	}else if(x.op=="&"){  // and
		if(flag==0) temp = string("    and ") + regResult + string(",") + regArg1 + string(",") + regArg2;
		else if(flag==1 || flag==2) temp = string("    andi ") + regResult + string(",") + regArg1 + string(",") + regArg2;
		else{
			int ans = stoi(regArg1) & stoi(regArg2);
			temp = "    li " + regResult + "," + to_string(ans);  // li R,k
		}			
	}else if(x.op=="|"){  // or
		if(flag==0) temp = string("    or ") + regResult + string(",") + regArg1 + string(",") + regArg2;
		else if(flag==1 || flag==2) temp = string("    ori ") + regResult + string(",") + regArg1 + string(",") + regArg2;
		else{
			int ans = stoi(regArg1) | stoi(regArg2);
			temp = "    li " + regResult + "," + to_string(ans);  // li R,k
		}	
	}
	obj.push_back(temp); 
}

void directJ(fourTumple x){  // line goto -- -- result	-> goto result
	string temp = string("    j ") + x.result;  // x.result�Ѿ�����Ϊ��ǩ 
	obj.push_back(temp); 
}

void indirectJ(fourTumple x){  // line relop arg1 arg2 result	-> if arg1 relop arg2 goto result
	string op = x.op;
	string regArg1,regArg2;
	if(x.arg1[0]>='0' && x.arg1[0]<='9') regArg1 = x.arg1;  // ��Ϊ����������Ҫ����Ĵ����ٱȽ� 
	else regArg1 = string("R") + to_string(getReg(x.arg1));
	if(x.arg2[0]>='0' && x.arg2[0]<='9') regArg2 = x.arg2;
	else regArg2 = string("R") + to_string(getReg(x.arg2));
	string temp;
	if(op=="=="){
		temp = "    beq ";
	}else if(op=="<>"){
		temp = "    bne ";
	}else if(op==">"){
		temp = "    bgt ";
	}else if(op==">="){
		temp = "    bge ";
	}else if(op=="<"){
		temp = "    blt ";
	}else if(op=="<="){
		temp = "    ble ";
	}
	temp += regArg1 + string(",") + regArg2 + string(",") + x.result;
	obj.push_back(temp);
}

void codegen(){
	memset(reg,0,sizeof(reg));
	memset(label,0,sizeof(label));
	for(int i=0;i<code.size();i++){  // ��������ת��ת��Ϊ��ǩ��֮����Ҫ�ȱ���һ�飬��Ϊ�˴�������������� 
		if(code[i].op=="goto" || code[i].op=="<" || code[i].op=="<=" || code[i].op=="==" || code[i].op=="<>" 
		|| code[i].op==">" || code[i].op==">="){  // ��תָ�� 
			hashLabel[code[i].result] = string("Label") + to_string(getLabel());
			code[i].result = hashLabel[code[i].result];
		}
	}
	for(int i=0;i<code.size();i++){
		if(hashLabel.find(to_string(code[i].codeLine))!=hashLabel.end()){  // ��ӱ�ǩ 
			string temp = hashLabel[to_string(code[i].codeLine)] + string(":");
			obj.push_back(temp);
		}
		if(code[i].op=="get" || code[i].op=="put"){
			OI(code[i]);
		}else if(code[i].op==":="){
			assign(code[i]);
		}else if(code[i].op=="+" || code[i].op=="-" || code[i].op=="&" || code[i].op=="|"){
			cal(code[i]);
		}else if(code[i].op=="goto"){
			directJ(code[i]);
		}else{  // ����Ϊ�Ƚ����������ô������һ��if arg1 relop arg2 goto result��� 
			indirectJ(code[i]);
		}
	}
	regToAdd();
}


int main(){
	semantic();
	//��Ԫʽȫ���������vector<fourTumple> code�У�
	codegen(); 
	cout<<"****************************Ŀ�����*****************************"<<endl<<endl;
	for(int i=0;i<obj.size();i++){
		cout<<obj[i]<<endl;
	}
	return 0;
}
