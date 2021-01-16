#include<iostream>
#include<fstream>
#include<cstdio>
#include<string>
#include<vector>
using namespace std;
struct TwoTumple{
	string str;  // ʶ����ַ�(��)
	string mark;  // �Ǻ�
	string attribute;  // ����
	int line;  // ���� 
	TwoTumple(string a,string b,string c,int l):str(a),mark(b),attribute(c),line(l) {}
	TwoTumple(): str(""),mark(""),attribute(""),line(0) {}
};
// ����ȫ�ֱ���
string text;  // �������Դ����
int state;  // ��ǰ״̬
char C;  // ��ŵ�ǰ������ַ�
string token;  // ����ʶ����ַ���
int iskey;  // �����ж��Ƿ�Ϊ�ؼ���
int forwardP;  // ��ǰָ��
int point;  // �ǹؼ��ֱ�ʶ�������
int lineNum;  // ����
int strNum;  // �������ַ���Ŀ
vector<string> Key;
string finalWords;
vector<TwoTumple> wordSeq;


void get_char(){  // ÿ����һ�Σ�����һ���ַ�����ǰָ������һ��
	C = text[forwardP++];
}

void get_nbc(){  // �����м�Ŀո�
	while(C == ' ' || C == '\t' || C == '\n'){
		if(C=='\n') lineNum++;
		get_char();
	}
}

void cat(){  // ��C���ַ�����token���棬��������ʶ����ַ���
	token += C;
}

bool letter(){  // �жϵ�ǰ�ַ�C�Ƿ�Ϊ��ĸ
	if((C>='A' && C<='Z') || (C>='a'&& C<='z')) return true;
	else return false;
}

bool digit(){  // �ж��Ƿ�Ϊ����
	if(C>='0' && C<='9') return true;
	else return false;
}

void retract(){  // ��ǰָ���ڱ�Ҫ��ʱ����Ҫ����һ����Ϊ��ʱ����Ҫ��ǰ�ж�
	forwardP--;
}

int reverse(){  // �����ж��Ƿ�Ϊ�ؼ���
	int i;
	for(i=0;i<Key.size();i++){
		if(Key[i]==token) break;
	}
	if(i==Key.size()) return 0;  // ��ʾ���ǹؼ���
	else return 1;  // ��ʾ�ǹؼ���
}

void error(){  // ���������
	string newstr = token + C;
	cout<<"�ַ���:"<<newstr<<"\t\t\t"<<"���󣡸õ����޷�ʶ��"<<endl;
}

string toUp(string str){  // �ַ�����д
	for(int i=0;i<str.size();i++) str[i] = toupper(str[i]);
	return str;
}

void initKey(){  // ��ʼ���ؼ����б�
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

void printTumple(TwoTumple temp){  // ���<�Ǻ�,����>��Ԫ��
	cout<<"�ַ���:"<<temp.str<<"\t\t\t<"<<temp.mark<<","<<temp.attribute<<">"<<"\t"<<temp.line<<endl;
	finalWords = finalWords + temp.mark;  // ���ɷ�����
}

void lex(){
	// ��һ�����Ƚ�������Դ�������text���ַ����У��ȴ�����
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

	//��2����ʼ����

	// ��ʼ��һЩ����
	initKey();  // �ؼ���
	state = 0;  // ״̬��ʼ��Ϊ0
	lineNum = 1;
	C = ' ';
	token = "";
	forwardP = 0;  // ��ǰָ�����ʼ�ĵط�
	cout<<"--------------------ʶ����Ϣ--------------------"<<endl;
	while(forwardP<text.size()){  // ����ǰָ��δ�����ļ����
		switch(state){
		case 0:  // ��ʼ״̬
			token = "";
			get_char();
			get_nbc();  // �Ե�ǰλ�����ҵ���һ���ǿո��ַ�
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
			case'Z': state = 1;break;  // ����ʶ���ʶ��״̬
			case'0':
			case'1':
			case'2':
			case'3':
			case'4':
			case'5':
			case'6':
			case'7':
			case'8':
			case'9': state = 2;break;  // ����ʶ������״̬
			case'<': state = 8;break;  // ����ʶ��<��<=״̬
			case'>': state = 9;break;  // ����ʶ��>��>=״̬
			case'=': state = 10;break;  // ����ʶ��=��==״̬
			case'|': state = 11;break;  // ����ʶ��|��||״̬
			case'&': state = 12;break;  // ����ʶ��&��&&״̬
			case'/': state = 13;break;  // ����ʶ��/��ע��״̬
			// һ����Щ�����ֱ���ж�������ֱ���������
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
			default:state = 15;break;  // ������������״̬
			}
			break;
		case 1:  // ��ʶ��ʶ��
			cat();
			get_char();
			if(letter() || digit()) state = 1;  // ѭ�����룬ֱ����ʶ�����
			else{
				retract();  // ��Ҫ����ǰָ�����1��(��Ϊ��Ҫ��ǰ�ж�)
				state = 0;  // ʶ����ɣ���ʼ��һ��
				strNum++;
				// �жϴ�ʱtoken������ַ����Ƿ��ǹؼ���
				iskey = reverse();
				TwoTumple temp;
				if(iskey==1){  // ��ʾ�ǹؼ���
					temp.str = token;
					// temp.mark = toUp(token);           //-------------------------�޸�---------------------------------
					if(token=="int") temp.mark = "i";
					else if(token=="if") temp.mark = "f";
					else if(token=="while") temp.mark = "w";
					else if(token=="get") temp.mark = "g";
					else if(token=="put") temp.mark = "p";
					else if(token=="bool") temp.mark = "b";
					else temp.mark = toUp(token);
 
					temp.attribute = "--";
					temp.line = lineNum;
				}else{  // ��ʾ���ǹؼ���
					temp.str = token;
					temp.mark = "d";  //-------------------------�޸�---------------------------------
//					temp.attribute = "("+ token + ",���:" + to_string(point) +")";
					temp.attribute = token;
					temp.line = lineNum;
					point++;
				}
				printTumple(temp);wordSeq.push_back(temp);
			}
			break;
		case 2:  // ����ʶ��״̬
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
			case'.': state = 3;break;  // ��ʾ�Ǹ�����
			case'E': state = 5;break;  // ��ָ��
			default:
				retract();
				state = 0;
				strNum++;
				printTumple(TwoTumple(token,"n",token,lineNum));  //-------------------------�޸�---------------------------------
				wordSeq.push_back(TwoTumple(token,"n",token,lineNum));
				break;
			}
			break;
		case 3:  // ������ʶ��״̬
			cat();
			get_char();
			if(digit()) state = 4;
			else{
				error();  // �Ƿ��ַ�
				state = 0;
				strNum++;
			}
			break;
		case 4:  // ����������ʶ��״̬
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
			case'E': state = 5;break;  // ָ��ʶ��
			default:
				retract();
				state = 0;
				strNum++;
				printTumple(TwoTumple(token,"n",token,lineNum));  //-------------------------�޸�---------------------------------
				wordSeq.push_back(TwoTumple(token,"n",token,lineNum));
				break;
			}
			break;
		case 5:  // ָ��ʶ��״̬
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
		case 6:  // ָ��ʶ��
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
				printTumple(TwoTumple(token,"n",token,lineNum));  //-------------------------�޸�---------------------------------
			    wordSeq.push_back(TwoTumple(token,"n",token,lineNum));
			}
			break;
		case 8:  // �ж��� < ���� <= ���� <>
			cat();
			get_char();
			switch(C){
			case'=': cat();state=0;strNum++;printTumple(TwoTumple(token,"r","LE",lineNum));wordSeq.push_back(TwoTumple(token,"r","LE",lineNum));break;  //-------------------------�޸�---------------------------------
			case'>': cat();state=0;strNum++;printTumple(TwoTumple(token,"r","NE",lineNum));wordSeq.push_back(TwoTumple(token,"r","NE",lineNum));break;  //-------------------------�޸�---------------------------------
			default: retract();state=0;strNum++;printTumple(TwoTumple(token,"r","LT",lineNum));wordSeq.push_back(TwoTumple(token,"r","LT",lineNum));break;  //-------------------------�޸�---------------------------------
			}
			break;
		case 9:  // �ж��� > ���� >=
			cat();
			get_char();
			if(C=='='){
				cat();
				state = 0;
				strNum++;
				printTumple(TwoTumple(token,"r","GE",lineNum));  //-------------------------�޸�---------------------------------
				wordSeq.push_back(TwoTumple(token,"r","GE",lineNum));
			}else{
				retract();
				state = 0;
				strNum++;
				printTumple(TwoTumple(token,"r","GT",lineNum));  //-------------------------�޸�---------------------------------
				wordSeq.push_back(TwoTumple(token,"r","GT",lineNum));
			}
			break;
		case 10:  // �ж��� = ���� ==
			cat();
			get_char();
			if(C=='='){
				cat();
				state = 0;
				strNum++;
				printTumple(TwoTumple(token,"r","EQ",lineNum));  //-------------------------�޸�---------------------------------
				wordSeq.push_back(TwoTumple(token,"r","EQ",lineNum));
			}else{
				retract();
				state = 0;
				strNum++;
				printTumple(TwoTumple(token,"=","--",lineNum));  //-------------------------�޸�---------------------------------
				wordSeq.push_back(TwoTumple(token,"=","--",lineNum));
			}
			break;
		case 11:  // �ж��� | ���� ||
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
		case 12:  // �ж��� & ���� &&
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
		case 13:   // �ж��� / ���� ע��(/*)
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
		case 14:   // ע��ʶ��״̬
			get_char();
			while(C!='*') get_char();
			get_char();
			if(C=='/') state = 0;  // ��ʾע���Ѿ����������Կ�ʼ��һ�ֽ���
			else state = 14;
			break;
		case 15:  // �Ƿ�����״̬
			error();
			state = 0;
			strNum++;
			break;
		case 16:  // ����ע��
			get_char();
			while(C!='\n') get_char();
			lineNum++;
			state = 0;
			break;
		}
	}
	cout<<"--------------------ͳ����Ϣ--------------------"<<endl;
	cout<<"�ó�����"<<lineNum-1<<"�У��ܹ�����"<<strNum<<"���ַ���"<<endl;
	ofstream outfile("result.txt");
	outfile<<finalWords;
	outfile.close();
}

