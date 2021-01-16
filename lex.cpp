#include<iostream>
#include<fstream>
#include<cstdio>
#include<string>
#include<vector>
using namespace std;
struct TwoTumple{
	string str;  // 识别的字符(串)
	string mark;  // 记号
	string attribute;  // 属性
	int line;  // 行数 
	TwoTumple(string a,string b,string c,int l):str(a),mark(b),attribute(c),line(l) {}
	TwoTumple(): str(""),mark(""),attribute(""),line(0) {}
};
// 定义全局变量
string text;  // 存放整个源程序
int state;  // 当前状态
char C;  // 存放当前读入的字符
string token;  // 正在识别的字符串
int iskey;  // 用来判断是否为关键字
int forwardP;  // 向前指针
int point;  // 非关键字标识符的序号
int lineNum;  // 行数
int strNum;  // 解析的字符数目
vector<string> Key;
string finalWords;
vector<TwoTumple> wordSeq;


void get_char(){  // 每调用一次，读入一个字符，向前指针往后一个
	C = text[forwardP++];
}

void get_nbc(){  // 过滤中间的空格
	while(C == ' ' || C == '\t' || C == '\n'){
		if(C=='\n') lineNum++;
		get_char();
	}
}

void cat(){  // 将C的字符放在token后面，构成正在识别的字符串
	token += C;
}

bool letter(){  // 判断当前字符C是否为字母
	if((C>='A' && C<='Z') || (C>='a'&& C<='z')) return true;
	else return false;
}

bool digit(){  // 判断是否为数字
	if(C>='0' && C<='9') return true;
	else return false;
}

void retract(){  // 向前指针在必要的时候需要回退一格，因为有时候需要超前判断
	forwardP--;
}

int reverse(){  // 用来判断是否为关键字
	int i;
	for(i=0;i<Key.size();i++){
		if(Key[i]==token) break;
	}
	if(i==Key.size()) return 0;  // 表示不是关键字
	else return 1;  // 表示是关键字
}

void error(){  // 错误处理程序
	string newstr = token + C;
	cout<<"字符串:"<<newstr<<"\t\t\t"<<"错误！该单词无法识别"<<endl;
}

string toUp(string str){  // 字符串大写
	for(int i=0;i<str.size();i++) str[i] = toupper(str[i]);
	return str;
}

void initKey(){  // 初始化关键字列表
	Key.push_back("int");
	Key.push_back("if");
	Key.push_back("while");
	Key.push_back("get");
	Key.push_back("put");
	Key.push_back("return");
	Key.push_back("else");
	Key.push_back("include");
	Key.push_back("bool");
}

void printTumple(TwoTumple temp){  // 输出<记号,属性>二元组
	cout<<"字符串:"<<temp.str<<"\t\t\t<"<<temp.mark<<","<<temp.attribute<<">"<<"\t"<<temp.line<<endl;
	finalWords = finalWords + temp.mark;  // 生成符号流
}

void lex(){
	// 第一步，先将待分析源程序存入text的字符串中，等待解析
	ifstream infile("test.txt");
	string line;
	if(infile){
		while(getline(infile,line)){
			text += line;
			text += "\n";
		}
	}else{
		cout<<"no such file!"<<endl;
	}

	//第2步开始分析

	// 初始化一些参数
	initKey();  // 关键字
	state = 0;  // 状态初始化为0
	lineNum = 1;
	C = ' ';
	token = "";
	forwardP = 0;  // 向前指针在最开始的地方
	cout<<"--------------------识别信息--------------------"<<endl;
	while(forwardP<text.size()){  // 当向前指针未到达文件最后
		switch(state){
		case 0:  // 初始状态
			token = "";
			get_char();
			get_nbc();  // 自当前位置起找到第一个非空格字符
			switch(C){
			case'a':
			case'b':
			case'c':
			case'd':
			case'e':
			case'f':
			case'g':
			case'h':
			case'i':
			case'j':
			case'k':
			case'l':
			case'm':
			case'n':
			case'o':
			case'p':
			case'q':
			case'r':
			case's':
			case't':
			case'u':
			case'v':
			case'w':
			case'x':
			case'y':
			case'z':
			case'A':
			case'B':
			case'C':
			case'D':
			case'E':
			case'F':
			case'G':
			case'H':
			case'I':
			case'J':
			case'K':
			case'L':
			case'M':
			case'N':
			case'O':
			case'P':
			case'Q':
			case'R':
			case'S':
			case'T':
			case'U':
			case'V':
			case'W':
			case'X':
			case'Y':
			case'Z': state = 1;break;  // 进入识别标识符状态
			case'0':
			case'1':
			case'2':
			case'3':
			case'4':
			case'5':
			case'6':
			case'7':
			case'8':
			case'9': state = 2;break;  // 进入识别数字状态
			case'<': state = 8;break;  // 进入识别<或<=状态
			case'>': state = 9;break;  // 进入识别>或>=状态
			case'=': state = 10;break;  // 进入识别=或==状态
			case'|': state = 11;break;  // 进入识别|或||状态
			case'&': state = 12;break;  // 进入识别&或&&状态
			case'/': state = 13;break;  // 进入识别/或注释状态
			// 一下这些情况可直接判定，所以直接输出即可
			case '+': printTumple(TwoTumple("+","+","--",lineNum));wordSeq.push_back(TwoTumple("+","+","--",lineNum));strNum++;break;
			case '-': printTumple(TwoTumple("-","-","--",lineNum));wordSeq.push_back(TwoTumple("-","-","--",lineNum));strNum++;break;
			case '!': printTumple(TwoTumple("!","!","--",lineNum));wordSeq.push_back(TwoTumple("!","!","--",lineNum));strNum++;break;
			case ',': printTumple(TwoTumple(",",",","--",lineNum));wordSeq.push_back(TwoTumple(",",",","--",lineNum));strNum++;break;
			case ';': printTumple(TwoTumple(";",";","--",lineNum));wordSeq.push_back(TwoTumple(";",";","--",lineNum));strNum++;break;
			case '(': printTumple(TwoTumple("(","(","--",lineNum));wordSeq.push_back(TwoTumple("(","(","--",lineNum));strNum++;break;
			case ')': printTumple(TwoTumple(")",")","--",lineNum));wordSeq.push_back(TwoTumple(")",")","--",lineNum));strNum++;break;
			case '{': printTumple(TwoTumple("{","{","--",lineNum));wordSeq.push_back(TwoTumple("{","{","--",lineNum));strNum++;break;
			case '}': printTumple(TwoTumple("}","}","--",lineNum));wordSeq.push_back(TwoTumple("}","}","--",lineNum));strNum++;break;
			case '#': printTumple(TwoTumple("#","#","--",lineNum));wordSeq.push_back(TwoTumple("#","#","--",lineNum));strNum++;break;
			default:state = 15;break;  // 进入解决错误处理状态
			}
			break;
		case 1:  // 标识符识别
			cat();
			get_char();
			if(letter() || digit()) state = 1;  // 循环输入，直到标识符完成
			else{
				retract();  // 需要将向前指针回退1格(因为需要超前判断)
				state = 0;  // 识别完成，开始下一轮
				strNum++;
				// 判断此时token里面的字符串是否是关键字
				iskey = reverse();
				TwoTumple temp;
				if(iskey==1){  // 表示是关键字
					temp.str = token;
					// temp.mark = toUp(token);           //-------------------------修改---------------------------------
					if(token=="int") temp.mark = "i";
					else if(token=="if") temp.mark = "f";
					else if(token=="while") temp.mark = "w";
					else if(token=="get") temp.mark = "g";
					else if(token=="put") temp.mark = "p";
					else if(token=="bool") temp.mark = "b";
					else temp.mark = toUp(token);
 
					temp.attribute = "--";
					temp.line = lineNum;
				}else{  // 表示不是关键字
					temp.str = token;
					temp.mark = "d";  //-------------------------修改---------------------------------
//					temp.attribute = "("+ token + ",入口:" + to_string(point) +")";
					temp.attribute = token;
					temp.line = lineNum;
					point++;
				}
				printTumple(temp);wordSeq.push_back(temp);
			}
			break;
		case 2:  // 数字识别状态
			cat();
			get_char();
			switch(C){
			case'0':
			case'1':
			case'2':
			case'3':
			case'4':
			case'5':
			case'6':
			case'7':
			case'8':
			case'9': state = 2;break;
			case'.': state = 3;break;  // 表示是浮点数
			case'E': state = 5;break;  // 有指数
			default:
				retract();
				state = 0;
				strNum++;
				printTumple(TwoTumple(token,"n",token,lineNum));  //-------------------------修改---------------------------------
				wordSeq.push_back(TwoTumple(token,"n",token,lineNum));
				break;
			}
			break;
		case 3:  // 浮点数识别状态
			cat();
			get_char();
			if(digit()) state = 4;
			else{
				error();  // 非法字符
				state = 0;
				strNum++;
			}
			break;
		case 4:  // 浮点数持续识别状态
			cat();
			get_char();
			switch(C){
			case'0':
			case'1':
			case'2':
			case'3':
			case'4':
			case'5':
			case'6':
			case'7':
			case'8':
			case'9': state = 4;break;
			case'E': state = 5;break;  // 指数识别
			default:
				retract();
				state = 0;
				strNum++;
				printTumple(TwoTumple(token,"n",token,lineNum));  //-------------------------修改---------------------------------
				wordSeq.push_back(TwoTumple(token,"n",token,lineNum));
				break;
			}
			break;
		case 5:  // 指数识别状态
			cat();
			get_char();
			switch(C){
			case'0':
			case'1':
			case'2':
			case'3':
			case'4':
			case'5':
			case'6':
			case'7':
			case'8':
			case'9': state = 7;break;
			case'+':
			case'-': state = 6;break;
			default:
				retract();
				error();
				state = 0;
				strNum++;
				break;
			}
			break;
		case 6:  // 指数识别
			cat();
			get_char();
			if(digit()) state = 7;
			else{
				retract();
				error();
				state = 0;
				strNum++;
			}
			break;
		case 7:
			cat();
			get_char();
			if(digit()) state = 7;
			else{
				retract();
				state = 0;
				strNum++;
				printTumple(TwoTumple(token,"n",token,lineNum));  //-------------------------修改---------------------------------
			    wordSeq.push_back(TwoTumple(token,"n",token,lineNum));
			}
			break;
		case 8:  // 判断是 < 还是 <= 还是 <>
			cat();
			get_char();
			switch(C){
			case'=': cat();state=0;strNum++;printTumple(TwoTumple(token,"r","LE",lineNum));wordSeq.push_back(TwoTumple(token,"r","LE",lineNum));break;  //-------------------------修改---------------------------------
			case'>': cat();state=0;strNum++;printTumple(TwoTumple(token,"r","NE",lineNum));wordSeq.push_back(TwoTumple(token,"r","NE",lineNum));break;  //-------------------------修改---------------------------------
			default: retract();state=0;strNum++;printTumple(TwoTumple(token,"r","LT",lineNum));wordSeq.push_back(TwoTumple(token,"r","LT",lineNum));break;  //-------------------------修改---------------------------------
			}
			break;
		case 9:  // 判断是 > 还是 >=
			cat();
			get_char();
			if(C=='='){
				cat();
				state = 0;
				strNum++;
				printTumple(TwoTumple(token,"r","GE",lineNum));  //-------------------------修改---------------------------------
				wordSeq.push_back(TwoTumple(token,"r","GE",lineNum));
			}else{
				retract();
				state = 0;
				strNum++;
				printTumple(TwoTumple(token,"r","GT",lineNum));  //-------------------------修改---------------------------------
				wordSeq.push_back(TwoTumple(token,"r","GT",lineNum));
			}
			break;
		case 10:  // 判断是 = 还是 ==
			cat();
			get_char();
			if(C=='='){
				cat();
				state = 0;
				strNum++;
				printTumple(TwoTumple(token,"r","EQ",lineNum));  //-------------------------修改---------------------------------
				wordSeq.push_back(TwoTumple(token,"r","EQ",lineNum));
			}else{
				retract();
				state = 0;
				strNum++;
				printTumple(TwoTumple(token,"=","--",lineNum));  //-------------------------修改---------------------------------
				wordSeq.push_back(TwoTumple(token,"=","--",lineNum));
			}
			break;
		case 11:  // 判断是 | 还是 ||
			cat();
			get_char();
			if(C=='|'){
				cat();
				state = 0;
				strNum++;
				printTumple(TwoTumple(token,"||","--",lineNum));
				wordSeq.push_back(TwoTumple(token,"||","--",lineNum));
			}else{
				retract();
				state = 0;
				strNum++;
				printTumple(TwoTumple(token,"|","--",lineNum));
				wordSeq.push_back(TwoTumple(token,"|","--",lineNum));
			}
			break;
		case 12:  // 判断是 & 还是 &&
			cat();
			get_char();
			if(C=='&'){
				cat();
				state = 0;
				strNum++;
				printTumple(TwoTumple(token,"&&","--",lineNum));
				wordSeq.push_back(TwoTumple(token,"&&","--",lineNum));
			}else{
				retract();
				state = 0;
				strNum++;
				printTumple(TwoTumple(token,"&","--",lineNum));
				wordSeq.push_back(TwoTumple(token,"&","--",lineNum));
			}
			break;
		case 13:   // 判断是 / 还是 注释(/*)
			cat();
			get_char();
			if(C=='*') state = 14;
			else if(C=='/') state = 16;
			else{
				retract();
				state = 0;
				strNum++;
				printTumple(TwoTumple(token,"/","--",lineNum));
				wordSeq.push_back(TwoTumple(token,"/","--",lineNum));
			}
			break;
		case 14:   // 注释识别状态
			get_char();
			while(C!='*') get_char();
			get_char();
			if(C=='/') state = 0;  // 表示注释已经跳过，可以开始下一轮解析
			else state = 14;
			break;
		case 15:  // 非法输入状态
			error();
			state = 0;
			strNum++;
			break;
		case 16:  // 单行注释
			get_char();
			while(C!='\n') get_char();
			lineNum++;
			state = 0;
			break;
		}
	}
	cout<<"--------------------统计信息--------------------"<<endl;
	cout<<"该程序共有"<<lineNum-1<<"行，总共解析"<<strNum<<"个字符串"<<endl;
	ofstream outfile("result.txt");
	outfile<<finalWords;
	outfile.close();
}

