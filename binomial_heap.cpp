#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <typeinfo>
#include "vertex.h"
#include "heaps.h"


void make_binary(int n, std::vector <int> &pows)
{
	while (n)
	{
		pows.push_back(n % 2);
		n /= 2;
	}
}

MHeap::BVertex::BVertex()
{
	key = 0;
	rang = 0u;
	left = NULL;
	right = NULL;
};

MHeap::BVertex::BVertex(int k)
{
	key = k;
	rang = 0u;
	left = NULL;
	right = NULL;
};

MHeap::BVertex::BVertex(int k, unsigned int d, BVertex * p, BVertex * c, BVertex * s)
{
	key = k;
	rang = d;
	left = c;
	right = s;
};

bool MHeap::BVertex::operator<(const BVertex &a)
{
	return key < a.key;
}


void merge_vertex(MHeap::BVertex * base, MHeap::BVertex * another)
{
	if (*another < *base)
		std::swap(*base, *another);
	another->right = base->left;
	base->left = another;
	base->rang++;
}

void MHeap::BHeap::bdestructor(BVertex * curr)
{
	if (curr == NULL)
	{
		delete curr;
		return;
	}
	BVertex * del_ver = curr->left;
	for (unsigned int i = 0u; i < curr->rang; i++)
	{
		BVertex * next = del_ver->right;
		bdestructor(del_ver);
		del_ver = next;
	}
	delete curr;
}

MHeap::BHeap::~BHeap()
{
	for (size_t i = 0; i < vertex.size(); i++)
		bdestructor(vertex[i]);
}

MHeap::BHeap::BHeap(const int key)
{
	BVertex * new_vertex = new BVertex;
	new_vertex->key = key;
	vertex.push_back(new_vertex);
	used.push_back(true);
}

MHeap::BHeap::BHeap(const BVertex * main)
{
	BVertex * curr = main->left, *curr_next;
	delete main;
	do
	{
		curr_next = curr->right;
		used.push_back(true);
		curr->right = NULL;
		vertex.push_back(curr);
		curr = curr_next;
	} while (curr != NULL);
	reverse(vertex.begin(), vertex.end());
}

int MHeap::BHeap::operator[](int index)
{
	return used[index];
}

size_t MHeap::BHeap::size()
{
	return used.size();
}

void MHeap::BHeap::insert(int key)
{
	BVertex * new_ver = new BVertex(key);
	bool finish = false;
	for (size_t i = 0; i < used.size(); i++)
	{
		if (!used[i])
		{
			vertex[i] = new_ver;
			used[i] = true;
			finish = true;
			break;
		}
		else
		{
			merge_vertex(new_ver, vertex[i]);
			vertex[i] = NULL;
			used[i] = false;
		}
	}
	if (!finish)
	{
		used.push_back(true);
		vertex.push_back(new_ver);
	}
}

void MHeap::BHeap::clear()
{
	vertex.clear();
	used.clear();
}

void MHeap::BHeap::meld(IHeap * another)
{
	BHeap * heap_to_meld = dynamic_cast<BHeap*>(another);
	if (heap_to_meld == NULL || heap_to_meld->vertex.empty())
		return;
	if (heap_to_meld->get_heap_type() != HeapType::BINOMIAL)
		return;	
	BVertex extraBody;
	BVertex * extra = &extraBody;
	bool isExtra = false;
	while (heap_to_meld->size() < used.size())
		heap_to_meld->used.push_back(false);
	for (size_t i = 0; i < heap_to_meld->size(); i++)
	{
		if (isExtra)
		{
			if (i >= size())
			{
				if ((*heap_to_meld)[i])
				{
					merge_vertex(extra, heap_to_meld->vertex[i]);
					vertex.push_back(NULL);
					used.push_back(false);
				}
				else
				{
					vertex.push_back(extra);
					isExtra = false;
					used.push_back(true);
				}
			}
			else if (!(*heap_to_meld)[i] && !used[i])
			{
				vertex[i] = extra;
				isExtra = false;
				used[i] = true;
			}
			else if (!(*heap_to_meld)[i] && used[i])
			{
				merge_vertex(extra, vertex[i]);
				used[i] = false;
				vertex[i] = NULL;
			}
			else if ((*heap_to_meld)[i])
			{
				merge_vertex(extra, heap_to_meld->vertex[i]);
			}
		}
		else
		{
			if (i >= size())
			{
				if ((*heap_to_meld)[i])
				{
					used.push_back(true);
					vertex.push_back(heap_to_meld->vertex[i]);
				}
				else
				{
					used.push_back(false);
					vertex.push_back(NULL);;
				}
			}
			else if ((*heap_to_meld)[i] && !used[i])
			{
				vertex[i] = heap_to_meld->vertex[i];
				used[i] = true;
			}
			else if ((*heap_to_meld)[i] && used[i])
			{
				merge_vertex(vertex[i], heap_to_meld->vertex[i]);
				extra = vertex[i];
				isExtra = true;
				used[i] = false;
				vertex[i] = NULL;
			}
		}
	}
	if (isExtra)
	{
		used.push_back(true);
		vertex.push_back(extra);
	}
	heap_to_meld->clear();
}

int MHeap::BHeap::extract_min()
{
	if (vertex.empty())
		return 0;
	int min_key = INT_MAX, minKeyIndex = 0;
	for (size_t i = 0; i < vertex.size(); i++)
	{
		if (used[i] && vertex[i]->key < min_key)
		{
			min_key = vertex[i]->key;
			minKeyIndex = i;
		}
	}
	used[minKeyIndex] = false;
	if (minKeyIndex != 0)
	{
		BHeap new_heap = BHeap(vertex[minKeyIndex]);
		vertex[minKeyIndex] = NULL;
		meld(&new_heap);
	}
	if (min_key == INT_MAX)
		return 0;
	return min_key;
}
