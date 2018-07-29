#include <iostream>
#include <vector>
#include <algorithm>
#include <mutex>
#include <thread>

using namespace std;
/*
 * 优先级队列 用大顶堆/小顶堆实现
 *
 * 实现最大优先级队列 <大顶堆>
 */

template <typename T>
class priorityQueue {
public:
	priorityQueue(int s=10):maxsize(s),size(0) {}
	bool getHead(pair<int,T> &x); //读取队头(具有最高优先级的元素) 并删除对头元素
	bool insertEle(const pair<int,T> &x); //插入新元素到队尾
	bool isEmpty() const; //判断队列是否为空
	bool isFull() const; //判断队列是否满
	void makeEmpty(); //队列置空
	int getSize() const;  //获取队列中元素的个数
public:
	template <typename Tt>
	friend ostream& operator<<(ostream& out, const priorityQueue<Tt>& p);
private:
    int size;  //当前队列长度
    int maxsize;   //队列最大长度
    std::vector<pair<int,T>> heap; //堆 元素为K-V形式K为优先级
	std::mutex mu; //互斥锁	
};
template <typename Tt>
ostream& operator<< (ostream& out, const priorityQueue<Tt>& p)
{
    for(int i=0;i<p.heap.size();i++)
    {
        out<<"["<<p.heap[i].first<<","<<p.heap[i].second<<"] ";
    }
    return out;
}

/*
 * 获取队列中元素个数
 */
template <typename T>
int priorityQueue<T>::getSize() const
{
    return heap.size();
}
/*
 * 队列置空
 */
template <typename T>
void priorityQueue<T>::makeEmpty() { heap.clear(); }
/*
 * 判空
 */
template <typename T>
bool priorityQueue<T>::isEmpty() const { return heap.size()==0?true:false; }
/*
 * 判满
 */
template <typename T>
bool priorityQueue<T>::isFull() const { return heap.size()>=maxsize?true:false; }
/*
 * 插入新元素
 */
template <typename T>
bool priorityQueue<T>::insertEle(const pair<int,T> &x) {
	std::unique_lock<std::mutex> lock(mu);
	//队列为空直接插入数据
	if(isEmpty()) {
		heap.push_back(x);
		return true;
	} else if(isFull()) return false; //队列已满不允许插入新元素
	else {
		heap.push_back(x);
		push_heap(heap.begin(),heap.end(),
			[](const pair<int,T> &x1, const pair<int,T> &x2) {
				return x1.first < x2.first;
			}
		);
	}
	return true;
}
/*
 * 获取队列头元素 并删除队头元素，相当于出队
 */
template <typename T>
bool priorityQueue<T>::getHead(pair<int, T> &x) {
 	std::unique_lock<std::mutex> lock(mu);
	if(isEmpty()) return false;
	pop_heap(heap.begin(),heap.end(),
		[](const pair<int,T> &x1, const pair<int,T> &x2) {
			return x1.first < x2.first;
		}
	);
	x = *heap.rbegin();
	heap.pop_back();
	return true;
}

int main(int argc, char *argv[])
{
    priorityQueue<string> Queue(50);
    for(int i=0;i<10;i++)
    {
        Queue.insertEle(make_pair(i,"a"));
        cout<<Queue<<endl;
    }

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	
	for(int i=0;i<10;i++)
    {
        pair<int,string> ss;
        Queue.getHead(ss);
        cout<<"{"<<ss.first<<","<<ss.second<<"}"<<endl;
	}
	return 0;
}	
