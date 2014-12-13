#include "meldable_heaps.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <set>

using namespace std;

void testing(int n)
{
    clock_t t;
    int bad = 0, tests = 0, xxx;
    multiset<int> perfect;
    Binomial_heap bheap = Binomial_heap(0);
    Leftist_heap lheap = Leftist_heap(0);
    Skew_heap sheap = Skew_heap(0);
    perfect.insert(0);
    //cout << "+++\n";
    t = clock();
    for (int i = 0; i < n; i++)
        bheap.insert(i);
    cout << "binomial heap works " << (double)(clock() - t) / CLOCKS_PER_SEC << endl;
    t = clock();
    for (int i = 0; i < n; i++)
        lheap.insert(i);
    cout << "leftist heap works " << (double)(clock() - t) / CLOCKS_PER_SEC << endl;
    t = clock();
    for (int i = 0; i < n; i++)
        sheap.insert(i);
    cout << "skew heap works " << (double)(clock() - t) / CLOCKS_PER_SEC << endl;
    cin >> xxx;
    for (int i = 1; i < n; i++)
    {
        int type = n - i;
        if (type > 10000)
        {
            int x = type;// rand();
            perfect.insert(x);
            bheap.insert(x);
            lheap.insert(x);
            sheap.insert(x);
            if (i % 10000 == 0)
                cout << i << endl;
            //cout << "+++\n";
        }
        else
        {
            tests++;
            int b = bheap.extract_min();
            int l = lheap.extract_min();
            int s = sheap.extract_min();
            int p = 0;
            if (!perfect.empty())
            {
                p = *perfect.begin();
                perfect.erase(perfect.begin());
            }
            if (b == p)
            {
                cout << "+";
            }   
            else
            {
                cout << "NO";
                bad++;
            }
            if (l == p)
            {
                cout << "+";
            }
            else
            {
                cout << "NO";
            }
            if (s == p)
            {
                cout << "+";
            }
            else
            {
                cout << "NO";
                cout << ' ' << s << ' ' << l << ' ' << b << ' ' << p;
                cin >> xxx;
            }
            cout << endl;
        }
    }
    cout << bad << " / " << tests << " tests false\n";
}
