// https://www.cnblogs.com/zhizhan/p/4876093.html
#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

class String {
public:
	String(const char *str = NULL);  // 通用构造函数
	String(const String &another);  // 复制构造函数
	~String();
	String& operator = (const String &rhs); // 赋值函数
	String operator+(const String &str) const;	//重载+
	friend istream& operator>>(istream &is, String &str);//输入
	friend ostream& operator<<(ostream &os, String &str);//输出
	bool operator==(const String &str); //相等
	String& operator+=(const String &str);
private:
	char *m_data;
};
String& String::operator+=(const String &str) {
	*this = *this + str;
	return *this;
}
bool String::operator==(const String &str) {
	if(strcmp(m_data, str.m_data)==0) return true;
	else return false;
}
String::String(const char *str) {
	if(str == NULL) {
		m_data = new char[1];
		m_data[0] = '\0';
	} else {
		m_data = new char[strlen(str)+1];
		strcpy(m_data, str);
	}
}
String::String(const String &another) {
	m_data = new char[strlen(another.m_data)+1];
	strcpy(m_data, another.m_data);
}
String& String::operator = (const String &rhs) {
	if(this == &rhs) return *this;
	delete[] m_data;
	m_data = new char[strlen(rhs.m_data)+1];
	strcpy(m_data, rhs.m_data);
	return *this;
}
istream& operator>>(istream &is, String &str)//输入
{
	char tem[1000];  //简单的申请一块内存
	is >> tem;
	str.m_data = new char[strlen(tem) + 1];
	strcpy(str.m_data, tem);
	return is;
}
ostream& operator<<(ostream &os, String &str)//输出
{
	os << str.m_data;
	return os;
}
String String::operator+(const String &str) const//重载+
{
	String newString;
	newString.m_data = new char[strlen(m_data) + strlen(str.m_data) + 1];
	strcpy(newString.m_data, m_data);
	strcat(newString.m_data, str.m_data);
	return newString;
}
String::~String() {
	delete[] m_data;
}


int main(int argc, char *argv[]) {
	String a("abc");
	cout << a << endl;
	String b(a);
	cout << b << endl;
	String c = b;
	cout << c << endl;
	String d("zw");
	String e("z1w");
	if(d==e) cout << "equal" << endl;
	else cout << "not equal" << endl;
	d += e;
	cout << d << endl;
//	String f;
//	cin >> f;
//	cout << f << endl;
	return 0;
}
