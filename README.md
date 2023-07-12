# Three small demo

## SkipList （跳表）
### 用法 
定义变量 ：模板传入key的比较函数，可以用lambda，或者仿函数  
insert(K key, V value) 插入key-value  
erase(K key) 根据key删除key-value  
count(K key) 判断是否存在key  
update(K key, V new_value) 修改对应key的value为new_value
```cpp
// 仿函数和lambda表达式都可以
// 不传入比较函数默认是 operator <
struct cmp {
    bool operator ()(const string & a, const string & b) {
        return a.length() < b.length();
    }
};
auto cmp = [](const string& a, const string& b) {return a.length() < b.length(); };
skip_list<string, int, decltype(cmp)> mp(cmp);
mp.insert("wocao", 132131);
mp.insert("sds", 51532231);
mp.update("sds", 2222);
mp.insert("gao", 61233523);
mp.erase("wocao");
if (mp.count("wocao")) {
    cout << "exist" << endl;
} else cout << "not exist" << endl;

for (auto it = mp.begin(); it != mp.end(); it++) {
    cout << it->key << " " << it->value << endl;
}
```

## Json

### 用法
从json文件中读取完内容后  
调用 json::parser(string str).value() 获取json对象  
重载 opearator[] 可以用来增加修改key-value，也可以该数组中元素的值
```cpp
std::ifstream fin("myjson.json");
std::stringstream ss; ss << fin.rdbuf();
fin.close();
std::string s{ ss.str() };
auto x = json::parser(s).value();
std::cout << x << "\n";
x["configurations"] = {json::Array {}};
x["configurations"].push({true});
x["files.associations"]["sbdx"] = {json::String {"ssss"}};
x["configurations"].push({json::Null {}});
x["configurations"][0] = {json::Int {1321}};
x["version"] = { 114514LL };
std::cout << x << "\n";
std::ofstream fout("myjson.json");
fout << x;
fout.close();
```

## ThreadPool

### 用法
指定线程池大小创建thread_pool对象  
需要当任务来时  
用thread_pool.submit以lambda的形式传入需要处理的逻辑，后边紧接着是lambda的参数
```cpp
thread_pool pool(8);
int n = 20;
for (int i = 1; i <= n; i++) {
    pool.submit([](int id) {
        if (id % 2 == 1) {
            this_thread::sleep_for(0.2s);
        }
        unique_lock<mutex> lc(_m);
        cout << "id : " << id << endl;
        }, i);
}
```