#include <iostream>
#include <vector>
#include <memory>

using namespace std;
// shared_ptr unique_ptr weak_ptr相关操作
class A {
public:
    int data = 1;
    A(int v):data(v) {}
    A() = default;
    ~A() {
        cout << "delete" << endl;
    }
};

int main() {
//    std::shared_ptr<A> tmp1 = new A; //error
    std::shared_ptr<A> tmp1(new A);
    std::shared_ptr<A> tmp2{new A};
    std::shared_ptr<A> tmp3 = std::make_shared<A>();

    // shared_ptr操作动态数组,需要手动指定删除器
    std::shared_ptr<A> tmp4(new A[5], std::default_delete<A[]>()); //shared_ptr不能直接支持动态数组
    std::shared_ptr<A> tmp5(new A[5], [](A* p) {delete[] p;});
    A* p = tmp5.get(); // 获得原始指针
    cout << p[2].data << endl;

    // unique_ptr操作动态数组
    std::unique_ptr<A[]> tmp6(new A[5]);
    cout << tmp6[2].data << endl;
//    std::shared_ptr<A[]> p(new A[5]); //error
    typedef void(*f)(A*);
    std::unique_ptr<A,f> tmp7(new A[5], [](A* p) {delete[] p;});
//    std::unique_ptr<A> tmp8(new A[5]); //error 只调用一次析构函数
    A* p2 = tmp7.get();
    cout << p2[2].data << endl;

    // weak_ptr
    std::weak_ptr<A> tmp8 = tmp3;
    cout << tmp8.use_count() << endl; // 返回有多少个shared_ptr对象指向同一被管理对象
//    tmp3.reset(); // 重置shared_ptr对象指向null
    if(tmp8.expired()) {
        cout << "该weak ptr不指向任一对象" << endl; // 可用于判断shared_ptr对象是否释放
    }
    std::shared_ptr<A> tmp9 = tmp8.lock(); // 返回share_ptr对象
    std::shared_ptr<A> tmp10(tmp8);
    cout << tmp8.use_count() << endl;
    if(tmp10.unique()) {
         cout << "唯一拥有该物体" << endl;
    } else {
        cout << "非唯一" << endl;
    }
}
