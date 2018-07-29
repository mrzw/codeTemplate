// https://blog.csdn.net/jofranks/article/details/17438955
// c++11 六大构造函数
#include <iostream>
#include <assert.h>
using namespace std;
 
class Temp
{
public:
	Temp(const char* str = nullptr); // 构造函数
	Temp(Temp&& t); // 移动构造函数
	Temp& operator = (Temp&& t); // 移动赋值操作符
	Temp(const Temp& t); // 复制构造函数
	Temp& operator = (Temp& t); // 赋值操作符
	~Temp(void); // 析构函数
private:
	char *m_pData;
};
 
Temp::Temp(const char* str)
{
	if (!str)
	{
		m_pData = nullptr;
	}
	else
	{
		this->m_pData = new char[strlen(str) + 1];
		strcpy(this->m_pData, str);
	}
}
 
Temp::Temp(Temp&& t) :m_pData(move(t.m_pData))
{
	t.m_pData = nullptr;
}
 
Temp& Temp::operator = (Temp&& t)
{
	assert(this != &t);
 
	this->m_pData = nullptr;
	this->m_pData = move(t.m_pData);
	t.m_pData = nullptr;
 
	return *this;
 
}
 
Temp::Temp(const Temp& t)
{
	if (!t.m_pData)
	{
		this->m_pData = nullptr;
	}
	else
	{
		this->m_pData = new char[strlen(t.m_pData) + 1];
		strcpy(this->m_pData, t.m_pData);
	}
}
 
Temp& Temp::operator = (Temp &t)
{
	if (this != &t)
	{
		delete[] this->m_pData;
		if (!t.m_pData)
		{
			this->m_pData = nullptr;
		}
		else
		{
			this->m_pData = new char[strlen(t.m_pData) + 1];
			strcpy(this->m_pData, t.m_pData);
		}
	}
 
	return *this;
}
 
Temp::~Temp(void)
{
	if (this->m_pData)
	{
		delete[] this->m_pData;
		this->m_pData = nullptr;
	}
}

