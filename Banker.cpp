#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

int r;
vector<int> Quantity;
vector<int> Available;
unordered_map<int, vector<int>> Max;
unordered_map<int, vector<int>> Need;

bool is_valid(vector<int>& tmp, vector<int>& compare) {
    for (int i = 0; i < r; i++) {
        if (tmp[i] > compare[i])
            return false;
    }
    return true;
}

bool is_safe(vector<int> &tmp, int pid) {
    int size = Need.size();
    vector<int> work(r);
    vector<bool> finish(size, false);
    for (int i = 0; i < r; ++i) {
        Need[pid][i] -= tmp[i];
        work[i] = Available[i] - tmp[i];
    }
    while (true) {
        int c = -1;
        int j = -1;
        for (auto i = Need.begin(); i != Need.end(); ++i) {
            if (!finish[++j] && is_valid(i->second, work)) {
                finish[j] = true;
                c = i->first;
                break;
            }
        }
        if (c != -1) {
            for (int i = 0; i < r; ++i) {
                work[i] += (Max[c][i] - Need[c][i]);
            }
        }
        else
            break;
    }
    for (int i = 0; i < r; ++i) {
        Need[pid][i] += tmp[i];
    }
    for (int i = 0; i < size; ++i) {
        if (!finish[i])
            return false;
    }
    return true;
}

int main() {
    cin >> r;
    Quantity.resize(r);
    for (int i = 0; i < r; ++i) {
        cin >> Quantity[i];
    }
    Available = Quantity;
    int pid;
    string command;
    while (cin >> pid >> command) {
        if (command == "terminate") {
            if (Max.find(pid) == Max.end()) {
                cout << "NOT OK" << "\n";
            }
            else {
                for (int i = 0; i < r; ++i) {
                    Available[i] += (Max[pid][i] - Need[pid][i]);
                }
                Need[pid].clear();
                Max[pid].clear();
                cout << "OK" << "\n";
            }
        }
        else if (command == "new") {
            vector<int> tmp(r);
            for (int i = 0; i < r; ++i) {
                cin >> tmp[i];
            }
            if (Max.find(pid) != Max.end() || !is_valid(tmp, Quantity)) {
                cout << "NOT OK" << "\n";
            }
            else {
                Max[pid] = tmp;
                Need[pid] = tmp;
                cout << "OK" << "\n";
            }
        }
        else if (command == "request") {
            vector<int> tmp(r);
            for (int i = 0; i < r; ++i) {
                cin >> tmp[i];
            }
            if (Max.find(pid) == Max.end() || !is_valid(tmp, Available) || !is_valid(tmp, Need[pid]) || !is_safe(tmp, pid)) {
                cout << "NOT OK" << "\n";
            }
            else {
                if (is_safe(tmp, pid));
                for (int i = 0; i < r; ++i) {
                    Available[i] -= tmp[i];
                    Need[pid][i] -= tmp[i];
                }
                cout << "OK" << "\n";
            }
        }
        else {
            cout << "Invalid Input Command" << "\n";
            break;
        }
    }
    return 0;
}