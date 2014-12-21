#pragma once

#include "vertex.h"
#include "heaps.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <set>

using namespace std;

ofstream out("heap_time.txt");

const size_t HEAP_SIZE_TO_MELD = 5000;

void commonInsert(MHeap::BHeap &BH, MHeap::SHeap &SH, MHeap::LHeap &LH, multiset <int> &PH, int x)
{
	BH.insert(x);
	SH.insert(x);
	LH.insert(x);
	PH.insert(x);
}

void testingDifferentHeaps(unsigned int numberOfTests)
{
	multiset <int> PH;
	MHeap::BHeap BH;
	MHeap::SHeap SH;
	MHeap::LHeap LH;
	bool workCorrect = true;
	for (unsigned int i = 0; i < numberOfTests; i++)
	{
		size_t type = rand() % 10;
		if (type > 1 || PH.size() < 2)
		{
			int x = rand();
			commonInsert(BH, SH, LH, PH, x);
		}
		else
		{
			int b = BH.extract_min();
			int s = SH.extract_min();
			int l = LH.extract_min();
			int p = *PH.begin();
			PH.erase(PH.begin());
			if (b != p || s != p || l != p)
				workCorrect = false;
		}
	}
	cerr << "Correct work of insert and extract_min: ";
	if (workCorrect)
		cerr << "+" << endl;
	else
		cerr << "-" << endl;
}

template <class RandHeap>
double randomInsertExtract(RandHeap &H, unsigned int numberOfTests)
{
	clock_t t = clock();
	for (unsigned int i = 0; i < numberOfTests; i++)
	{
		size_t type = rand() % 10;
		if (type > 1)
		{
			int x = rand();
			H.insert(x);
		}
		else
		{
			int b = H.extract_min();
		}
	}
	return (double)(clock() - t) / CLOCKS_PER_SEC;
}

void testingTimeBHeap(unsigned int numberOfTests)
{
	MHeap::BHeap BH;
	//cerr << "BHeap works on " << numberOfTests << " tests in " << randomInsertExtract(BH, numberOfTests) << "\n";
	out << randomInsertExtract(BH, numberOfTests) << ' ';
}

void testingTimeSHeap(unsigned int numberOfTests)
{
	MHeap::SHeap SH;
	//cerr << "SHeap works on " << numberOfTests << " tests in " << randomInsertExtract(SH, numberOfTests) << "\n";
	out << randomInsertExtract(SH, numberOfTests) << ' ';
}

void testingTimeLHeap(unsigned int numberOfTests)
{
	MHeap::LHeap LH;
	//cerr << "LHeap works on " << numberOfTests << " tests in " << randomInsertExtract(LH, numberOfTests) << "\n";
	out << randomInsertExtract(LH, numberOfTests) << "\n";
}

void testingTime(unsigned int numberOfTests)
{
	out << numberOfTests << ": ";
	testingTimeBHeap(numberOfTests);
	testingTimeSHeap(numberOfTests);
	testingTimeLHeap(numberOfTests);
}

void generateHeaps(MHeap::BHeap &BH, MHeap::SHeap &SH, MHeap::LHeap &LH, multiset <int> &PH, size_t heapSize)
{
	for (size_t i = 0; i < heapSize; i++)
	{
		int x = rand();
		commonInsert(BH, SH, LH, PH, x);
	}
}

void meldSet(multiset <int> &first, multiset <int> &second)
{
	if (second.empty())
		return;
	for (multiset <int>::iterator it = second.begin(); it != second.end(); it++)
		first.insert(*it);
	second.clear();
}

bool minsNotEqual(MHeap::BHeap &BH, MHeap::SHeap &SH, MHeap::LHeap &LH, multiset <int> &PH)
{
	int b = BH.extract_min();
	int s = SH.extract_min();
	int l = LH.extract_min();
	int p = *PH.begin();
	PH.erase(PH.begin());
	return (b != p || s != p || l != p);
}

void testingMeld(unsigned int numberOfTests)
{
	bool workCorrect = true;
	for (unsigned int i = 0; i < numberOfTests; i++)
	{
		size_t heapSizeFirst = rand() % HEAP_SIZE_TO_MELD + 1, heapSizeSecond = rand() % HEAP_SIZE_TO_MELD + 1;
		MHeap::BHeap BFirst, BSecond;
		MHeap::SHeap SFirst, SSecond;
		MHeap::LHeap LFirst, LSecond;
		multiset <int> PFirst, PSecond;
		generateHeaps(BFirst, SFirst, LFirst, PFirst, heapSizeFirst);
		generateHeaps(BSecond, SSecond, LSecond, PSecond, heapSizeSecond);
		MHeap::meld(&BFirst, &BSecond);
		MHeap::meld(&SFirst, &SSecond);
		MHeap::meld(&LFirst, &LSecond);
		meldSet(PFirst, PSecond);
		for (size_t i = 0; i + 1 < heapSizeFirst + heapSizeSecond; i++)
			if (minsNotEqual(BFirst, SFirst, LFirst, PFirst))
				workCorrect = false;
	}
	cerr << "Correct work of meld: ";
	if (workCorrect)
		cerr << "+" << endl;
	else
		cerr << "-" << endl;
}

enum ArrayTestType
{
	NEW_HEAP = 0,
	INSERT = 1,
	EXTRACT_MIN = 2,
	MELD = 3
};

size_t getIndex(MHeap::HeapArray &HA)
{
	if (HA.empty())
		return 0;
	return rand() % HA.size();
}

void addSet(vector< pair < multiset <int>, MHeap::HeapType > > &PA, int key, int heap_type)
{
	multiset <int> ms;
	ms.insert(key);
	PA.push_back(make_pair(ms, (MHeap::HeapType)heap_type));
}

void meldSetsInArray(vector< pair < multiset <int>, MHeap::HeapType > > &PA, size_t first, size_t second)
{
	if (PA[first].second == PA[second].second)
	{
		meldSet(PA[first].first, PA[second].first);
		PA[second] = PA.back();
		PA.pop_back();
	}
}

void testingHeapArray(unsigned int numberOfTests)
{
	MHeap::HeapArray HA;
	vector< pair < multiset <int>, MHeap::HeapType > > PA;
	bool workCorrect = true;
	size_t index, indexFirst, indexSecond;
	int x, heap_type, key, h, p;
	for (unsigned int i = 0; i < numberOfTests; i++)
	{
		int test_type = rand() % 4;
		if (HA.empty())
			test_type = 0;
		switch (test_type)
		{
		case NEW_HEAP:
			heap_type = rand() % 3;
			key = rand();
			HA.add_heap(key, (MHeap::HeapType)heap_type);
			addSet(PA, key, heap_type);
			break;
		case INSERT:
			index = getIndex(HA);
			x = rand();
			HA.insert(index, x);
			PA[index].first.insert(x);
			break;
		case EXTRACT_MIN:
			index = getIndex(HA);
			if (PA[index].first.empty())
				break;
			h = HA.extract_min(index);
			p = *PA[index].first.begin();
			PA[index].first.erase(PA[index].first.begin());
			if (h != p)
				workCorrect = false;
			break;
		case MELD:
			if (PA.size() < 2)
				break;
			indexFirst = getIndex(HA);
			indexSecond = getIndex(HA);
			if (indexFirst == indexSecond)
				indexSecond = (indexSecond + 1) % HA.size();
			HA.meld(indexFirst, indexSecond);
			meldSetsInArray(PA, indexFirst, indexSecond);
			break;
		}
	}
	cerr << "Correct work of heap array: ";
	if (workCorrect)
		cerr << "+" << endl;
	else
		cerr << "-" << endl;
}

void testing(unsigned int numberOfTests)
{
	testingDifferentHeaps(numberOfTests);
	testingTime(numberOfTests);
	testingMeld(numberOfTests);
	testingHeapArray(numberOfTests);
}
