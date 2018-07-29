#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/pool/object_pool.hpp> //使用boost对象内存池来作为容器内存分配器
/*

 *  C++ STL 迭代器的原理与实现
 *  迭代器（iterator）是一种抽象设计理念，通过迭代器可以在不了解容器内部原理的情况下遍历容器
 *  STL设计的精髓在于，把容器（Containers）和算法（Algorithms）分开，彼此独立设计，最后再用迭代器（Iterator）把他们粘合在一起。
 *  迭代器内部必须保存一个与容器相关联的指针，然后重载各种运算符操作来方便遍历 重载运算符有：   ++  *  ->
 *  迭代器的作用其实相当于一个智能指针，它指向容器内部的数据，可以通过operator *操作符来解指针获得数据的值，
 *  也可以通过operator ->操作符来获取数据的指针，还能够重载++,--等运算符来移动指针。
 */
namespace zw {
	// List Node定义
	template <typename T>
	struct __list_node {
		__list_node* next; //后继指针
		__list_node* prev; //前驱指针
		T data; //节点数据
		__list_node():next(NULL),prev(NULL) {}
		__list_node(const T &ele, __list_node* n, __list_node* p):data(ele),next(n),prev(p) {}
	};
	
	// List 迭代器定义
	template <typename T, typename Ref, typename Ptr> //T：容器中数据的类型  Ref:容器中数据的引用  Ptr:容器中数据的指针
        /*
         * 迭代器分类
         * 1. Input Interator ：只允许作为输入，也就是只读（Read Only）
         * 2. Output Interator ：只允许作为输出，也就是只写（Write Only）
         * 3. Forward Interator ：允许读写，但只能做前向移动
         * 4. Bidirectional Interator ：允许读写，可以做双向移动
         * 5. Random Access Interator ：允许读写，可以任意移动
         */
	struct __list_iterator{
		__list_node<T> *node; //迭代器当前所指节点
		__list_iterator() {}
		__list_iterator(__list_node<T>* x):node(x) {}
		__list_iterator(const __list_iterator &x):node(x.node) {}		
		// 重载== !=
		bool operator==(const __list_iterator &x) const {
			return node==x.node;
		}
		bool operator!=(const __list_iterator &x) const {
			return node!=x.node;
		}
		// 重载operator*()
		Ref operator*() const {
			return (*node).data; //返回当前节点的data
		}
		// 重载operator->()
		Ptr operator->() const {
			return &(*node).data; //返回当前节点data的指针
		}
		// 重载前置++
		__list_iterator& operator++() {
			node = node->next;
			return *this;
		}
		// 重载后置++
		__list_iterator& operator++(int) {
			__list_iterator tmp(node);
			node = node->next;
			return tmp;
		}
		// 重载operator+(int n) 指向下面第n个节点
		__list_iterator& operator+(int n) {
			for(int i=0; i<n; ++i) {
				node = node->next;
			}
			return *this;
		}
	};
	
	// List类定义
	template <typename T, typename alloc= boost::object_pool<__list_node<T>> > //typename T：T数据类型  typename alloc：内存分配器
	class list {
	public:
		typedef __list_iterator<T,T&,T*> iterator; //类型定义
		list():pls(),len(0) { //构造boost对象内存池作为内存分配器Alloc
			HeadNode = pls.construct();
			HeadNode->next = HeadNode;
			HeadNode->prev = HeadNode;
			std::cout<<"List Constructed!"<<std::endl;
		}
		iterator begin() {return iterator(HeadNode->next);} //返回第一个迭代器 即头结点的下一个节点
		iterator end() {return iterator(HeadNode);} //返回头节点的迭代器，其实就是返回链表的结尾
		void push_back(T &ele) { //添加到尾部
			__list_node<T> *tmp = pls.construct();
			tmp->data = ele;
			tmp->next = HeadNode;
			tmp->prev = HeadNode->prev;
			HeadNode->prev->next = tmp;
			HeadNode->prev = tmp;
			++len;
		}
		void erase(iterator pos) { //删除操作
			pos.node->prev->next = pos.node->next;
			pos.node->next->prev = pos.node->prev;
			pls.destroy(pos.node); //内存池中释放pos节点的内存
			--len;
		}
		size_t size() {return len;}
	private:
		__list_node<T> *HeadNode; //头节点指针，该List为一个带头节点的双向循环链表
		alloc pls; //boost内存池：object_pool内存分配器
		size_t len;
	};	
}

using namespace zw;

class demo {  //测试类
public:
	int a,b;
	demo() {}
	demo(int x, int y):a(x),b(y) {}
	demo& operator=(const demo &d) {
		a = d.a;
		b = d.b;
		return *this;
	}
	friend std::ostream& operator<<(std::ostream &out, const demo &d);
};
std::ostream& operator<<(std::ostream &out, const demo &d)
{
    out<<"["<<d.a<<","<<d.b<<"]";
    return out;
}

int main(int argc, char *argv[]) {
	list<demo> a;
	for(int i=0; i<10; ++i) {
		demo tmp(i,i+1);
		a.push_back(tmp);
	}
	list<demo>::iterator iter;
	std::cout<<"befor erase:"<<std::endl;
    for(iter=a.begin();iter!=a.end();++iter)
    {
        std::cout<<*iter<<std::endl;
	}
	a.erase(a.begin()+2); //测试删除
	std::cout<<"after erase:"<<std::endl;
	std::for_each(a.begin(),a.end(),[](const demo& da){std::cout<<da<<std::endl;});		
	return 0;
}
