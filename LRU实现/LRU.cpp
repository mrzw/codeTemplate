#include <iostream>
#include <unordered_map>
#include <list>
using namespace std;
// 基于双向链表和hash_map构建LRU算法
class LRUCache {
private:
    int m_capacity;
    unordered_map<int, list<pair<int, int>>::iterator> m_map; // m_map_iter->first: key, m_map_iter->second: list iterator;
    list<pair<int, int>> m_list; // m_list_iter->first: key, m_list_iter->second: value;
public:
    LRUCache(int capacity) : m_capacity(capacity) {}
    // Get the value (will always be positive) of the key if the key exists in the cache, otherwise return -1.
    int get(int key) {
        auto found_iter = m_map.find(key);
        if (found_iter == m_map.end()) return -1;
        m_list.splice(m_list.begin(), m_list, found_iter->second);
        found_iter->second = m_list.begin();
        return found_iter->second->second;
    }
    // Set or insert the value if the key is not already present. When the cache reached its capacity, it should invalidate the least recently used item before inserting a new item.
    void set(int key, int value) {
        auto found_iter = m_map.find(key);
        if(found_iter != m_map.end()) {
            m_list.splice(m_list.begin(), m_list, found_iter->second);
            found_iter->second = m_list.begin();
            found_iter->second->second = value;
            return;
        }
        if(m_map.size() == m_capacity) {
            int key_to_del = m_list.back().first;
            m_list.pop_back();
            m_map.erase(key_to_del);
        }
        m_list.emplace_front(key, value);
        m_map[key] = m_list.begin();
    }
};
