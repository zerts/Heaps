#pragma once

#include "tests.h"
#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

int main()
{
    int n;
    cin >> n;
	while (n)
	{
		testing(n);
		cin >> n;
	}
    system("pause");
    return 0;
}
