#pragma GCC optimize ("O3")

#include <iostream>
#include <algorithm>
using namespace std;

int s, m, n;
int *track;

bool compare(int a, int b) {
    if ((a > s && b > s) || (a < s && b < s)) return a > b;
    else return a < b;
}

int main() {
    scanf("%d %d %d", &s, &m, &n);
    track = new int[n];
    for (int i = 0; i < n; i++) {
        scanf("%d", &track[i]);
    }
    sort(track, track + n, compare);
    int prev = s, dis = 0;
    printf("%d ", s);
    for (int i = 0; i < n; i++) {
        printf("%d ", track[i]);
        dis += abs(prev - track[i]);
        prev = track[i];
    }
    printf("\n%d\n", dis);
    return 0;
}