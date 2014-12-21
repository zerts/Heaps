#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <typeinfo>
#include "vertex.h"
#include "heaps.h"
#include "meld.h"

MHeap::LVertex::LVertex()
{
	left = NULL;
	right = NULL;
	rang = 0;
};

MHeap::LVertex::LVertex(int k)
{
	left = NULL;
	right = NULL;
	rang = 1;
	key = k;
};

void MHeap::LVertex::update_rang()
{
	if (left == NULL || right == NULL)
		rang = 1;
	else
		rang = 1 + std::min(left->rang, right->rang);
};

bool MHeap::LVertex::bad_leftist_vertex()
{
	if (left == NULL && right == NULL)
		return false;
	if (left == NULL && right != NULL)
		return true;
	if (left != NULL && right == NULL)
		return false;
	return left->rang < right->rang;
};

void MHeap::LVertex::swap_children()
{
	std::swap(left, right);
}

void MHeap::LVertex::update()
{
	update_rang();
	if (bad_leftist_vertex())
		swap_children();
}
