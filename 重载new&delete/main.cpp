#include <iostream>
using namespace std;

class Test {
public:
    void* operator new(size_t sz) {
        cout << "chongzai new" << endl;
        void* p = malloc(sz);
        return p;
    }
    void operator delete(void* p) {
        cout << "chongzai delete" << endl;
        free(p);
    }
    void* operator new[](size_t sz) {
        cout << "chongzai new[]" << endl;
        void* p = malloc(sz);
        return p;
    }
    void operator delete[](void* p) {
        cout << "chongzai delete[]" << endl;
        free(p);
    }
};

int main() {
    Test *p = new Test();
    delete p;
    Test *q = new Test[10];
    delete[] q;
}
