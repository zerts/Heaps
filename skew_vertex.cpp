#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include "vertex.h"
#include "heaps.h"
#include "meld.h"
#include <typeinfo>

MHeap::SVertex::SVertex()
{
	left = NULL;
	right = NULL;
}

MHeap::SVertex::SVertex(int k)
{
	left = NULL;
	right = NULL;
	key = k;
}

bool MHeap::SVertex::operator<(SVertex *another)
{
	return key < another->key;
}

void MHeap::SVertex::swap_children()
{
	std::swap(left, right);
}

void MHeap::SVertex::update()
{
	swap_children();
}
