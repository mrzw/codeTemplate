#include <iostream>
#include <unordered_map>
#include <list>
using namespace std;

class LRU {
public:
    int capacity_;
    typedef list<pair<int,string>>::iterator LIST_ITER;
    unordered_map<int,LIST_ITER> mp_;
    list<pair<int,string>> list_;

    LRU(int capacity):capacity_(capacity) {}
    string get(int key) {
        auto found_iter = mp_.find(key);
        if(found_iter==mp_.end()) return "";
        string res = found_iter->second->second;
        list_.erase(mp_[key]);  //先删除原来的数据
        list_.push_front(make_pair(key,res)); //将数据插入到头部
        mp_[key] = list_.begin(); // 更新hashmap
        return res;
    }
    void set(int key, string value) {
        auto found_iter = mp_.find(key);
        if(found_iter!=mp_.end()) {
            string res = found_iter->second->second;
            list_.erase(mp_[key]);  //先删除原来的数据
            list_.push_front(make_pair(key,res)); //将数据插入到头部
            mp_[key] = list_.begin(); // 更新hashmap
        } else {
            if(mp_.size()>=capacity_) {
                int key_to_del = list_.back().first;
                list_.pop_back(); // 删除链表尾部数据
                mp_.erase(key_to_del); // 删除链表尾部元素对应的hashmap
            }
            list_.push_front(make_pair(key,value)); // 更新插入的元素到链表
            mp_.insert(make_pair(key,list_.begin())); // 更新插入的元素到hashmap
        }
    }
};

int main() {

    return 0;
}
