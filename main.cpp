#include "SkipList/skip_list.hpp"
#include "Json/json.hpp"
// #include "ThreadPool/thread_pool.hpp"

using namespace std;

// mutex _m; 

int main(int argc, char** argv) {
    if (argc == 1) {
        cout << "hello world!!" << endl;
        return 0;
    }
    
    if (argc > 2) {
        cout << "please input skiplist, json or bufferpool" << endl;
        return 0;
    }

    if (string(argv[1]) == "skiplist") {
        auto cmp = [](const string& a, const string& b) {return a.length() < b.length(); };
        skip_list<string, int, decltype(cmp)> mp(cmp);
        mp.insert("wocao", 132131);
        mp.insert("giao", 61233523);
        mp.erase("wocao");
        for (auto it = mp.begin(); it != mp.end(); it++) {
            cout << it->key << " " << it->value << endl;
        }
        return 0;
    }

    if (string(argv[1]) == "json") {
        std::ifstream fin("myjson.json");
        std::stringstream ss; ss << fin.rdbuf();
        fin.close();
        std::string s{ ss.str() };
        auto x = json::parser(s).value();
        // 
        std::cout << x << "\n";
        x["configurations"] = {json::Array {}};
        x["configurations"].push({true});
        x["files.associations"]["sbdx"] = {json::String {"ssss"}};
        x["configurations"].push({json::Null {}});
        x["version"] = { 114514LL };
        std::cout << x << "\n";
        std::ofstream fout("myjson.json");
        fout << x;
        fout.close();
        return 0;
    }

    if (string(argv[1]) == "threadpool") {
        // thread_pool pool(8);
        // int n = 20;
        // for (int i = 1; i <= n; i++) {
        //     pool.submit([](int id) {
        //         if (id % 2 == 1) {
        //             this_thread::sleep_for(0.2s);
        //         }
        //         unique_lock<mutex> lc(_m);
        //         cout << "id : " << id << endl;
        //         }, i);
        // }
        return 0;
    }
    cout << "please input skiplist, json or bufferpool" << endl;
    return 0;
}