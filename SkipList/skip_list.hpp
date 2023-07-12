#ifndef _SKIPLIST_HPP_
#define _SKIPLIST_HPP_

#include <set>
#include <string>
#include <time.h>
#include <iostream>

using namespace std;

#define INT_MAX 0x7fffffff 

template<typename T>
struct Less {
    bool operator () (const T & a, const T & b) {
        return a < b;
    }
};

template<typename K, typename V, typename Comp = Less<K>> 
class skip_list{
  private:

    struct skip_list_node {
        int level;
        const K key;
        V value;
        skip_list_node** forward;
        skip_list_node(): key{0}, value{0}, level(0), forward{0} {}
        skip_list_node(K _key, V _value, int _level, skip_list_node* nxt = nullptr): key(_key), value(_value), level(_level) {
            forward = new skip_list_node* [level + 1];
            for (int i = 0; i <= level; i++) {
                forward[i] = nxt;
            }
        }    
        ~skip_list_node() {
            delete[] forward;
        }
    };

    using node = skip_list_node;

    int level;
    int length;
    static const int MAXL = 32;
    static const int P = 4;
    static const int S = 0xFFFF;
    static const int PS = S / P;
    static const int INVALID = INT_MAX;
    node* head, * tail;
    Comp less;

    void init() {
        srand((uint32_t)time(NULL));
        level = length = 0;
        head->forward = new node * [MAXL + 1];
        for (int i = 0; i <= MAXL; i++) {
            head->forward[i] = tail;
        }
    }

    int randomLevel() {
        int lv = 1;
        while ((rand() & S) < PS) lv++;
        return lv < MAXL ? lv : MAXL;
    }

    node* find(const K & key, node** update) {
        node* p = head;
        for (int i = level; i >= 0; i--) {
            while (p->forward[i] != tail && less(p->forward[i]->key, key)) {
                p = p->forward[i];
            }
            update[i] = p;
        }    
        p = p->forward[0];
        return p;
    }
  public:
    struct iterator {
        node* p;
        iterator() : p(nullptr) {};
        iterator(node* rhs) : p(rhs) {}
        node* operator ->()const { return (p);}
        node& operator *() const { return *p;}
        bool operator == (const iterator& rhs) { return rhs.p == p;}
        bool operator != (const iterator& rhs) {return !(rhs.p == p);}
        void operator ++() {p = p->forward[0];}
        void operator ++(int) { p = p->forward[0]; }
    };

    skip_list() : head(new node()), tail(new node()), less{Comp()} {
        init();    
    }

    skip_list(Comp _less) : head(new node()), tail(new node()),  less{_less} {
        init();
    }

    void insertOrUpdate(const K& key, const V& value) {
        node* update[MAXL + 1];
        node* p = find(key, update);
        if (p->key == key) {
            p->value = value;
            return;
        }
        int lv = randomLevel();
        if (lv > level) {
            lv = ++level;
            update[lv] = head;
        }
        node* newNode = new node(key, value, lv);
        for (int i = lv; i >= 0; i--) {
            p = update[i];
            newNode->forward[i] = p->forward[i];
            p->forward[i] = newNode;
        }
        ++length;
    }

    void insert(const K& key, const V& value) {
        insertOrUpdate(key, value);
        cout << "insert successfully!" << endl;
    }

    void update(const K& key, const V& value) {
        if (!count(key)) {
            cout << "The key does not exist!" << endl;
            return ; 
        }
        insertOrUpdate(key, value);
        cout << "update successfully!" << endl;
    }

    bool erase(const K& key) {
        node* update[MAXL + 1];
        node* p = find(key, update);
        if (p->key != key) {
            cout << "The key does not exist" << endl;
            return false; 
        }
        for (int i = 0; i <= p->level; i++) {
            update[i]->forward[i] = p->forward[i];
        }
        delete p;
        while (level > 0 && head->forward[level] == tail) level--;
        length--;
        cout << "erase successfully!" << endl;
        return true;
    }

    iterator find(const K&key) {
        node* p = head;
        for (int i = level; i >= 0; i--) {
            while (p->forward[i] != tail && less(p->forward[i]->key, key)) {
                p = p->forward[i];
            }
        }    
        p = p->forward[0];
        if (p == tail) { 
            return iterator(tail); 
        }
        if (p->key == key) {
            return iterator(p);
        }
        return iterator(tail);
    }

    bool count(const K& key) {
        node* p = head;
        for (int i = level; i >= 0; i--) {
            while (p->forward[i] != tail && less(p->forward[i]->key, key)) {
                p = p->forward[i];
            }
        }    
        p = p->forward[0];
        return p != tail && p->key == key;
    }

    int size() {
        return length;
    }

    iterator end() {
        return iterator(tail);
    }

    iterator begin() {
        return iterator(head->forward[0]);
    }
};


#endif // _SKIPLIST_HPP_