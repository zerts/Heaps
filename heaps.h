#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include "vertex.h"
#include "meld.h"

namespace MHeap
{
	class BHeap : public IHeap
	{
		std::vector <BVertex *> vertex;
		std::vector <bool> used;
	public:
		BHeap(){};
		~BHeap();
		virtual void bdestructor(BVertex*);
		explicit BHeap(const int);
		explicit BHeap(const BVertex*);
		virtual int operator[](int);
		virtual size_t size();
		virtual void insert(int);
		virtual void meld(IHeap*);
		virtual int extract_min();
		HeapType get_heap_type()
		{
			return HeapType::BINOMIAL;
		}
		virtual void clear();
	};

	template <class RandVertex>
	class LSHeap : public IHeap
	{
	protected:
		RandVertex * root;
	public:
		LSHeap(){};
		~LSHeap(){};
		LSHeap(int key)
		{
			root = new RandVertex(key);
		}
		void clear()
		{
			root = NULL;
		}
		void meld(IHeap* another)
		{
			LSHeap * heap_to_meld = dynamic_cast<LSHeap*>(another);
			if (heap_to_meld == NULL || heap_to_meld->root == NULL)
				return;
			if (root == NULL)
			{
				root = heap_to_meld->root;
				heap_to_meld->clear();
				return;
			}
			root = merge_ls_heaps(root, heap_to_meld->root);
			heap_to_meld->clear();
		}
		HeapType get_heap_type()
		{
			return root->get_heap_type();
		}
		void insert(int key)
		{
			LSHeap new_heap = LSHeap(key);
			meld(&new_heap);
		}
		int extract_min()
		{
			if (root != NULL)
			{
				int result = root->key;
				RandVertex * copy_root = root;
				root = merge_ls_heaps(root->left, root->right);
				delete copy_root;
				return result;
			}
			return 0;
		}
		int get_high()
		{
			int e = 1;
			RandVertex * curr = root->right;
			while (curr != NULL)
			{
				e++;
				curr = curr->right;
			}
			return e;
		}
	};

	class LHeap : public LSHeap < LVertex >
	{
	public:
		LHeap(){};
		explicit LHeap(int key)
		{
			root = new LVertex(key);
		}
	};

	class SHeap : public LSHeap < SVertex >
	{
	public:
		SHeap(){};
		explicit SHeap(int key)
		{
			root = new SVertex(key);
		}
	};
}
