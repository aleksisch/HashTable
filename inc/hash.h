#include <vector>
#include <fstream>
#include <sstream>
#include "string.h"

class list {
private:
    struct Node {
        Node(const char* data = nullptr, Node* next = nullptr, Node* prev = nullptr) :data(data), next(next), prev(prev){}
        Node* next;
        Node* prev;
        const char* data;
    };
    int l_size;
    Node* start;
    Node* end;
public:
    list() :l_size(0) {
        start = new Node();
        end = new Node();
        start->next = end;
        start->prev = end;
        end->next = start;
        end->prev = start;
    }
    ~list() {
        Node* tmp = start->prev;
        while (tmp->prev != start) {
            tmp = tmp->prev;
            delete tmp;
        }
        delete start;
    }
    void push_front(const char* value) {
        Node* tmp = new Node(value, start->next, start);
        start->next = tmp;
        tmp->next->prev = tmp;
        l_size++;
    }
    void pop_front() {
        Node* tmp = start->next;
        start = tmp->next;
        tmp->next->prev = start;
        l_size++;
        delete tmp;
    }
    int size() {
        return l_size;
    }
    int find(const char* value) {
        Node* tmp = start->next;
        while (tmp != end && strcmp(tmp->data, value) != 0) {
            tmp = tmp->next;
        }
        return tmp != end;
    }

    bool find_asm(const char* value) {
        return FindListAsm(this, value);
    }
};

class HashTable {
private:
    std::vector<list> table;
    int size;
    unsigned int (*hash) (const char*);
public:
    explicit HashTable(unsigned int (*hash) (const char*), int size = 5000)
            : table(size), size(size), hash(hash){}
    void insert(const char* value) {
        unsigned int h = hash(value) % this->size;
        table[h].push_front(value);
    }

    int getSize(int n) {
        return table[n].size();
    }
    bool find(const char* value) {
        unsigned int h = hash(value) % this->size;
        return table[h].find(value);
    }

    bool find_asm(const char* value) {
        unsigned int h = hash(value) % this->size;
        return FindListAsm(&(table[h]), value);//table[h].find_asm(value);
    }
    int GetSize() {
        return size;
    }
};

namespace TestHash {

    unsigned int hash1 (const char* a) {
        return 1;
    }

    unsigned int hash2 (const char* a) {
        return strlen(a);
    }

    unsigned int hash3 (const char* a) {
        int res = 0;
        for (int i = 0; a[i] != 0; i++) {
            res += a[i];
        }
        return res;
    }

    unsigned int hash4(const char* a) {
        return hash3(a) * hash2(a);
    }

    unsigned int hash5(const char* a) {
        int tmp = 0;
        int res = 0;
        for (int i = 0; a[i] != 0; i++) {
            res ^= a[i];
            tmp = (res >> 31) & 1;
            res <<= 1;
            res |= tmp;
        }
        return res;
    }

    unsigned int hash6(const char* a) {
        int res = 0;
        for (int i = 0; a[i] != 0; i++) {
            res += a[i] * (i + 1412313);
        }
        return res;
    }

    unsigned int hash7(const char* a) {
        std::hash<const char*> f;
        return (f(a) >> 4);
    }
    std::vector<unsigned int (*)(const char*)> arr{hash1, hash2, hash3, hash4, hash5, hash6, hash7};

    void TestAllFunction(std::string filename = "hamlet.txt", const int table_size = 2000) {
        std::ifstream input(filename);
        std::stringstream buff;
        buff << input.rdbuf();
        std::vector<std::string> str;
        std::string tmp;
        while (!buff.eof()) {
            std::string tmp1;
            tmp.clear();
            buff >> tmp;
            str.emplace_back(tmp);
        }
        std::stringstream out_buff;

        for (int i = 0; i < TestHash::arr.size(); i++) {
            out_buff.str(std::string());            //clear buff
            HashTable data(TestHash::arr[i], table_size);
            for (auto & cur_str : str) {
                const char* tmp = cur_str.c_str();
                data.insert(tmp);
            }
            for (int l = 0; l < table_size; l++) {
                out_buff << l << " ; " << data.getSize(l) << "\n";
            }
            std::ofstream output("output_" + std::to_string(i) + ".csv");
            output << out_buff.str();
        }
            system("gnuplot -p plot_script");
    }
}