#include <iostream>
#include <queue>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>
using namespace std;

int cache_size;
int alg;
int N;
int* query;

class FIFO {
    unordered_set<int> cache_set;
    queue<int> cache_queue;
    int size;

    bool isContain(int q) {
        return cache_set.find(q) != cache_set.end();
    }

    bool isFull() {
        return cache_set.size() >= cache_size;
    }

    void pop() {
        int out = cache_queue.front();
        cache_queue.pop();
        cache_set.erase(out);
    }

    void push(int q) {
        cache_queue.push(q);
        cache_set.insert(q);
    }
public:
    FIFO(int size) : size(size) {}

    double run() {
        int hit_count = 0;
        for (int i = 0; i < N; i++) {
            if (!isContain(query[i])) {
                if (isFull()) {
                    pop();
                }
                push(query[i]);
            }
            else {
                hit_count++;
            }
        }
        return (double)100000 * hit_count / N / 1000;
    }
};

class LRU {
    struct node {
        int v;
        node* prev, * next;
        node(int v) : v(v), prev(nullptr), next(nullptr) {}
    };
    unordered_map<int, node*> cache_map;
    node* head, * tail;
    int size;

    bool isContain(int q) {
        return cache_map.find(q) != cache_map.end();
    }

    bool isFull() {
        return cache_map.size() >= cache_size;
    }

    void remove(node* now) {
        if (now->prev == nullptr) {
            head = now->next;
        }
        else {
            now->prev->next = now->next;
        }
        if (now->next == nullptr) {
            tail = now->prev;
        }
        else {
            now->next->prev = now->prev;
        }
    }

    void insert(node* now) {
        now->next = head;
        if (head == nullptr) {
            head = tail = now;
        }
        else {
            head->prev = now;
            now->next = head;
            now->prev = nullptr;
            head = now;
        }
    }

    void pop() {
        node* tmp = tail;
        cache_map.erase(tmp->v);
        remove(tmp);
    }

public:
    LRU(int size) : size(size) {}

    double run() {
        int hit_count = 0;
        node* now;
        for (int i = 0; i < N; i++) {
            if (!isContain(query[i])) {
                if (isFull()) pop();
                now = new node(query[i]);
                cache_map[query[i]] = now;
            }
            else {
                now = cache_map.find(query[i])->second;
                remove(now);
                hit_count++;
            }
            insert(now);
        }
        return (double)100000 * hit_count / N / 1000;
    }
};

class MIN {
    unordered_map<int, int> cache_map;
    vector<int> next_position;
    priority_queue<pair<int, int> > pq;
    int size;

    bool isContain(int q) {
        return cache_map.find(q) != cache_map.end();
    }

    bool isFull() {
        return cache_map.size() >= cache_size;
    }

    void pop() {
        int out = pq.top().second;
        pq.pop();
        cache_map.erase(out);
    }

    void push(int q) {
        cache_map.insert(make_pair(q, 1));
    }
public:
    MIN(int size) : size(size) {}

    double run() {
        for (int i = 0; i < N; i++) {
            next_position.push_back(INT32_MAX);
        }
        for (int i = N - 1; i >= 0; --i) {
            auto pair = cache_map.find(query[i]);
            if (pair != cache_map.end()) {
                next_position[i] = pair->second;
            }
            cache_map[query[i]] = i;
        }
        cache_map.clear();
        int hit_count = 0;
        for (int i = 0; i < N; i++) {
            if (!isContain(query[i])) {
                if (isFull()) pop();
                push(query[i]);
            }
            else {
                hit_count++;
            }
            pq.push(make_pair(next_position[i], query[i]));
        }
        return (double)100000 * hit_count / N / 1000;
    }
};

class CLOCK {
    struct node {
        int v;
        node* prev, * next;
        bool valid;
        node(int v) : v(v), prev(nullptr), next(nullptr), valid(false) {}
        node(int v, bool vd): v(v), prev(nullptr), next(nullptr), valid(vd) {}
    };
    unordered_map<int, node*> cache_map;
    node* ptr;
    int size;

    bool isContain(int q) {
        return cache_map.find(q) != cache_map.end();
    }

    bool isFull() {
        return cache_map.size() >= cache_size;
    }

    void push(int q) {
        node *now = new node(q, true);
        cache_map[q] = now;
        if (ptr == nullptr) {
            now->prev = now->next = now;
            ptr = now;
        }
        else {
            now->next = ptr;
            now->prev = ptr->prev;
            ptr->prev->next = now;
            ptr->prev = now;
        }
    }

    void pop() {
        node* tmp = ptr;
        while (tmp != nullptr && tmp->valid) {
            tmp->valid = false;
            tmp = tmp->next;
        }
        ptr = tmp->next;
        cache_map.erase(tmp->v);
        if (tmp->prev != nullptr)
            tmp->prev->next = tmp->next;
        if (tmp->next != nullptr)
            tmp->next->prev = tmp->prev;
    }

public:
    CLOCK(int size) : size(size) {}

    double run() {
        int hit_count = 0;
        node* now;
        for (int i = 0; i < N; i++) {
            if (!isContain(query[i])) {
                if (isFull()) pop();
                push(query[i]);
            }
            else {
                cache_map.find(query[i])->second->valid = true;
                hit_count++;
            }
        }
        return (double)100000 * hit_count / N / 1000;
    }
};

int main() {
    // freopen("2.in", "r", stdin);
    // freopen("1.out", "w", stdout);
    scanf("%d\n", &cache_size);
    scanf("%d\n", &alg);
    scanf("%d\n", &N);
    query = new int[N];
    for (int i = 0; i < N; i++) {
        scanf("%d", &query[i]);
    }
    double hit_ratio;
    switch (alg) {
        case 0: {
            FIFO* fifo = new FIFO(cache_size);
            hit_ratio = fifo->run();
            break;
        }
        case 1: {
            LRU* lru = new LRU(cache_size);
            hit_ratio = lru->run();
            break;
        }
        case 2: {
            MIN* opt = new MIN(cache_size);
            hit_ratio = opt->run();
            break;
        }
        case 3: {
            CLOCK* clock = new CLOCK(cache_size);
            hit_ratio = clock->run();
            break;
        }
        defaut: {
            hit_ratio = 0.0;
            printf("What is your problem?\n");
            break;
        };
    }
    printf("Hit ratio = %05.2lf%%\n", hit_ratio);
}