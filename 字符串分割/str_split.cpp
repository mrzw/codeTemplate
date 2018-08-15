#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <vector>
#include <sstream>
using namespace std;
///////////////////////方法1////////////////////
// 借助strtok函数分割字符串，只能用字符分割
int main(int argc, char **argv) {
    char s[] = "Golden Global      View,disk * desk";
    const char *d = " ,*"; // 字符串中包含空格，逗号，星号任意一项即进行分割。
    char *p;
    p = strtok(s,d); // 字符串分割函数
    while(p)
    {
            printf("%s\n",p);
            p=strtok(NULL,d);
    }
    
	return 0;
}

////////////////////方法2///////////////////////////
// 可以以字符串进行分割
std::vector<std::string> split(std::string str,std::string pattern)
{
  std::string::size_type pos;
  std::vector<std::string> result;
  str+=pattern;//扩展字符串以方便操作
  int size=str.size();

  for(int i=0; i<size; i++)
  {
    pos=str.find(pattern,i);
    if(pos<size)
    {
      std::string s=str.substr(i,pos-i);
      result.push_back(s);
      i=pos+pattern.size()-1;
    }
  }
  return result;
}

int main(int argc, char **argv) {
	string s = "GoldenGlobaobiew,diskdesk";
	string sp = "ob"; // 以ob分割
	
	vector<string> result = split(s, sp);
	for(auto i:result) cout << i << endl;
	
	return 0;
}

/////////////////////方法3/////////////////////
// getline只能按字符分割
int main(int argc, char **argv) {
    std::vector<std::string> strings;
    std::istringstream f("denmark;sweden;india;us");
    std::string s;    
    while (std::getline(f, s, ';')) {
        std::cout << s << std::endl;
        strings.push_back(s);
    }
}
