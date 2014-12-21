#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include "vertex.h"

namespace MHeap
{
	template <class RandHeap>
	RandHeap * merge_ls_heaps(RandHeap * base, RandHeap * another)
	{
		if (base == NULL)
			return another;
		if (another == NULL)
			return base;
		if (*another < base)
			std::swap(*base, *another);
		base->right = merge_ls_heaps(another, base->right);
		base->update();
		return base;
	};

	template<class RandHeap>
	void meld(RandHeap * first, RandHeap * second)
	{
		if (first->get_heap_type() == second->get_heap_type())
		{
			first->meld(second);
		}
	}
}
