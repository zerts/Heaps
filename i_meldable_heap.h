#pragma once

#include <vector>
#include <typeinfo>


namespace MHeap
{
	enum HeapType
	{
		BINOMIAL = 0,
		SKEW = 1,
		LEFTIST = 2
	};

	class IVertex
	{
	public:
		virtual ~IVertex(){};
	};

	class IHeap
	{
	public:
		virtual void meld(IHeap*) = 0;
		virtual void insert(int) = 0;
		virtual int extract_min() = 0;
		virtual HeapType get_heap_type() = 0;
		virtual void clear() = 0;
		virtual ~IHeap(){};
	};

	class HeapArray
	{
		std::vector <IHeap *> heaps;
	public:
		virtual void add_heap(int, HeapType);
		virtual void insert(size_t, int);
		virtual int extract_min(size_t);
		virtual void meld(size_t, size_t);
		virtual size_t size();
		virtual bool empty();
		virtual ~HeapArray(){};
	};
}
