#include "tests.h"
#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

int main()
{
    clock_t t;
    //Binomial_heap_array heaps;
    //Leftist_heap_array heaps;
    int n, x;
    cin >> n;
    testing(n);
    /*t = clock();
    heaps.add_heap(0);
    for (int i = 1; i < n; i++)
    {
        heaps.insert(0, i);
    }
    for (int i = 0; i < n; i++)
    {
        cout << heaps.extract_min(0) << endl;
    }
    cout << (double)(clock() - t) / CLOCKS_PER_SEC;*/
    system("pause");
    return 0;
}
