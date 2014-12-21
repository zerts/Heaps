#pragma once

#include "i_meldable_heap.h"
#include "vertex.h"
#include "heaps.h"

void MHeap::HeapArray::add_heap(int key, MHeap::HeapType type)
{
	if (type == MHeap::HeapType::BINOMIAL)
		heaps.push_back(new MHeap::BHeap(key));
	if (type == MHeap::HeapType::LEFTIST)
		heaps.push_back(new MHeap::LHeap(key));
	if (type == MHeap::HeapType::SKEW)
		heaps.push_back(new MHeap::SHeap(key));
}

void MHeap::HeapArray::insert(size_t index, int key)
{
	heaps[index]->insert(key);
}

int MHeap::HeapArray::extract_min(size_t index)
{
	return heaps[index]->extract_min();
}

void MHeap::HeapArray::meld(size_t first_index, size_t second_index)
{
	if (heaps[first_index]->get_heap_type() != heaps[second_index]->get_heap_type())
		return;
	if (first_index < heaps.size() && second_index < heaps.size() && first_index != second_index)
	{
		heaps[first_index]->meld(heaps[second_index]);
		if (second_index + 1 != heaps.size())
		{
			heaps[second_index] = heaps.back();
			heaps.pop_back();
		}
	}
}

size_t MHeap::HeapArray::size()
{
	return heaps.size();
}

bool MHeap::HeapArray::empty()
{
	return heaps.size() == 0;
}

MHeap::IHeap * heap_maker(MHeap::HeapType type)
{
	switch (type)
	{
	case MHeap::BINOMIAL:
		return new MHeap::BHeap;
	case MHeap::LEFTIST:
		return new MHeap::LHeap;
	case MHeap::SKEW:
		return new MHeap::SHeap;
	default:
		break;
	}
}
