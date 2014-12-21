#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include "i_meldable_heap.h"

namespace MHeap
{
	class BVertex : public IVertex
	{
	public:
		int key;
		BVertex * left, *right;
		unsigned int rang;
		BVertex();
		~BVertex(){};
		explicit BVertex(int);
		BVertex(int, unsigned int, BVertex*, BVertex*, BVertex*);
		virtual bool operator<(const BVertex&);
	};

	class SVertex : public IVertex
	{
	public:
		int key;
		SVertex * left, *right;
		~SVertex(){};
		SVertex();
		explicit SVertex(int k);
		virtual bool operator<(SVertex *);
		void update_rang(){};
		virtual void swap_children();
		virtual void update();
		bool bad_leftist_vertex()
		{
			return false;
		}
		HeapType get_heap_type()
		{
			return HeapType::SKEW;
		}
	};

	class LVertex : public SVertex
	{
		unsigned int rang;
	public:
		LVertex * left, *right;
		LVertex();
		~LVertex(){};
		explicit LVertex(int);
		virtual void update_rang();
		virtual bool bad_leftist_vertex();
		virtual void swap_children();
		virtual void update();
		HeapType get_heap_type()
		{
			return HeapType::LEFTIST;
		}
	};
}
