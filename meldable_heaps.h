#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include "i_meldable_heap.h"

typedef unsigned int ui32;

using namespace std;

const int INF = 10000000;

void make_binary(int n, vector <int> &pows)
{
    while (n)
    {
        pows.push_back(n % 2);
        n /= 2;
    }
}

///////////////
//BINOMIAL HEAP
///////////////

class Binomial_vertex
{
public:
    int key;
    ui32 degree;
    Binomial_vertex * parent, * child, * subling;
    Binomial_vertex()
    {
        key = 0;
        degree = 0u;
        parent = NULL;
        child = NULL;
        subling = NULL;
    };
    Binomial_vertex(int k)
    {
        key = k;
        degree = 0u;
        parent = NULL;
        child = NULL;
        subling = NULL;
    };
    Binomial_vertex(int k, ui32 d, Binomial_vertex * p, Binomial_vertex * c, Binomial_vertex * s)
    {
        key = k;
        degree = d;
        parent = p;
        child = c;
        subling = s;
    }
    bool operator<(Binomial_vertex a)
    {
        return key < a.key;
    }
};

vector <Binomial_vertex> global_binomial_vertex;

Binomial_vertex * create_binomial_vertex(int key)
{
    /*Binomial_vertex new_vertex;
    return &new_vertex;*/
    if (global_binomial_vertex.empty())
        global_binomial_vertex.reserve(INF);
    global_binomial_vertex.push_back(Binomial_vertex(key));
    return &global_binomial_vertex.back();
}

void merge_vertex(Binomial_vertex * base, Binomial_vertex * another)
{
    if (*another < *base)
        swap(*base, *another);
    another->subling = base->child;
    another->parent = base;
    base->child = another;
    base->degree++;
}

class Binomial_heap
{
public:
    vector <Binomial_vertex *> vertex;
    vector <int> used;

    Binomial_heap(){};
    Binomial_heap(int key)
    {
        //Binomial_vertex new_vertex = Binomial_vertex(key);
        //vertex.push_back(&new_vertex);
        vertex.push_back(create_binomial_vertex(key));
        used.push_back(1);
    }
    Binomial_heap(Binomial_vertex * main)
    {
        Binomial_vertex * curr = main->child, * curr_next;
        do
        {
            curr_next = curr->subling;
            used.push_back(1);
            curr->subling = NULL;
            curr->parent = NULL;
            vertex.push_back(curr);
            curr = curr_next;
        } while (curr != NULL);
        reverse(vertex.begin(), vertex.end());
    }
    int operator[](int index)
    {
        return used[index];
    }
    int size()
    {
        return used.size();
    }
    void insert(int key)
    {
        //Binomial_vertex new_vertex = Binomial_vertex(key);
        //Binomial_vertex * new_ver = &new_vertex;
        Binomial_vertex * new_ver = create_binomial_vertex(key);
        bool finish = false;
        for (int i = 0; i < used.size(); i++)
        {
            if (used[i] == 0)
            {
                vertex[i] = new_ver;
                used[i] = 1;
                finish = true;
                break;
            }
            else
            {
                merge_vertex(new_ver, vertex[i]);
                used[i] = 0;
            }
        }
        if (!finish)
        {
            used.push_back(1);
            vertex.push_back(new_ver);
        }
    }
    void meld(Binomial_heap &another)
    {
        Binomial_vertex extraBody;
        Binomial_vertex * extra = &extraBody;
        bool isExtra = false;
        while (another.size() < used.size())
            another.used.push_back(0);
        for (int i = 0; i < another.size(); i++)
        {
            if (isExtra)
            {
                if (i > size())
                {
                    if (another[i] == 1)
                    {
                        merge_vertex(extra, another.vertex[i]);
                        vertex.push_back(another.vertex[i]);
                        used.push_back(0);
                    }
                    else
                    {
                        vertex.push_back(extra);
                        isExtra = false;
                        used.push_back(1);
                    }
                }
                else if (another[i] == 0 && used[i] == 0)
                {
                    vertex[i] = extra;
                    isExtra = false;
                    used[i] = 1;
                }
                else if (another[i] == 0 && used[i] == 1)
                {
                    merge_vertex(extra, vertex[i]);
                    used[i] = 0;
                }
                else if (another[i] == 1)
                {
                    merge_vertex(extra, another.vertex[i]);
                }
            }
            else
            {
                if (i > size())
                {
                    if (another[i] == 1)
                    {
                        vertex.push_back(another.vertex[i]);
                        used.push_back(1);
                    }
                    else
                    {
                        vertex.push_back(another.vertex[i]);
                        used.push_back(0);
                    }
                }
                else if (another[i] == 1 && used[i] == 0)
                {
                    vertex[i] = another.vertex[i];
                    used[i] = 1;
                }
                else if (another[i] == 1 && used[i] == 1)
                {
                    merge_vertex(vertex[i], another.vertex[i]);
                    extra = vertex[i];
                    isExtra = true;
                    used[i] = 0;
                }
            }
        }
        if (isExtra)
        {
            used.push_back(1);
            vertex.push_back(extra);
        }
    }
    int extract_min()
    {
        int min_key = INT_MAX, minKeyIndex = 0;
        for (int i = 0; i < vertex.size(); i++)
        {
            if (used[i] == 1 && vertex[i]->key < min_key)
            {
                min_key = vertex[i]->key;
                minKeyIndex = i;
            }
        }
        used[minKeyIndex] = 0;
        if (minKeyIndex != 0)
        {
            Binomial_heap new_heap = Binomial_heap(vertex[minKeyIndex]);
            meld(new_heap);
        }
        if (min_key == INT_MAX)
            return 0;
        return min_key;
    }
    HeapType get_heap_type()
    {
        return HeapType::BINOMIAL;
    }
};

class Binomial_heap_array : public IMergeableHeap
{
private:
    vector <Binomial_heap> heaps;
public:
    void add_heap(int key)
    {
        heaps.push_back(Binomial_heap(key));
    }
    void insert(int index, int key)
    {
        heaps[index].insert(key);
    }
    int extract_min(int index)
    {
        return heaps[index].extract_min();
    }
    void meld(int first_index, int second_index)
    {
        if (first_index < heaps.size() && second_index < heaps.size() && first_index != second_index)
        {
            heaps[first_index].meld(heaps[second_index]);
            if (second_index + 1 != heaps.size())
            {
                heaps[second_index] = heaps.back();
                heaps.pop_back();
            }
        }
    }
};

//////////////
//LEFTIST HEAP
//////////////

class Leftist_vertex
{
public:
    Leftist_vertex *left, *right;
    int key, rang;
    Leftist_vertex()
    {
        left = NULL;
        right = NULL;
        rang = 0;
    }
    Leftist_vertex(int k)
    {
        left = NULL;
        right = NULL;
        rang = 1;
        key = k;
    }
    bool operator<(Leftist_vertex another)
    {
        return key < another.key;
    }
    void update_rang()
    {
        if (left == NULL || right == NULL)
            rang = 1;
        else
            rang = 1 + min(left->rang, right->rang);
    }
    void swap_children()
    {
        Leftist_vertex * s = left;
        left = right;
        right = s;
    }
    bool bad_leftist_vertex()
    {
        if (left == NULL && right == NULL)
            return false;
        if (left == NULL && right != NULL)
            return true;
        if (left != NULL && right == NULL)
            return false;
        return left->rang < right->rang;
    }
};

Leftist_vertex * merge_leftist_vertex(Leftist_vertex * base, Leftist_vertex * another)
{
    if (base == NULL)
        return another;
    if (another == NULL)
        return base;
    if (*another < *base)
        swap(*base, *another);
    base->right = merge_leftist_vertex(another, base->right);
    base->update_rang();
    if (base->bad_leftist_vertex())
        base->swap_children();
    return base;
}

vector <Leftist_vertex> global_leftist_vertex;

Leftist_vertex * create_leftist_vertex(int key)
{
    /*Binomial_vertex new_vertex;
    return &new_vertex;*/
    if (global_leftist_vertex.empty())
        global_leftist_vertex.reserve(INF);
    global_leftist_vertex.push_back(Leftist_vertex(key));
    return &global_leftist_vertex.back();
}

class Leftist_heap
{
public:
    Leftist_vertex *root;
    Leftist_heap(){};
    Leftist_heap(int key)
    {
        //Leftist_vertex r = Leftist_vertex(key);
        //root = &r;
        root = create_leftist_vertex(key);
    }
    void meld(Leftist_heap another)
    {
        root = merge_leftist_vertex(root, another.root);
    }
    void insert(int key)
    {
        Leftist_heap new_heap = Leftist_heap(key);
        meld(new_heap);
    }
    int extract_min()
    {
        if (root != NULL)
        {
            int result = root->key;
            root = merge_leftist_vertex(root->left, root->right);
            return result;
        }
        return 0;
    }
    HeapType get_heap_type()
    {
        return HeapType::LEFTIST;
    }
};

class Leftist_heap_array : public IMergeableHeap
{
private:
    vector <Leftist_heap> heaps;
public:
    void add_heap(int key)
    {
        heaps.push_back(Leftist_heap(key));
    }
    void insert(int index, int key)
    {
        heaps[index].insert(key);
    }
    int extract_min(int index)
    {
        return heaps[index].extract_min();
    }
    void meld(int first_index, int second_index)
    {
        if (first_index < heaps.size() && second_index < heaps.size() && first_index != second_index)
        {
            heaps[first_index].meld(heaps[second_index]);
            if (second_index + 1 != heaps.size())
            {
                heaps[second_index] = heaps.back();
                heaps.pop_back();
            }
        }
    }
};

//////
//SKEW
//////

class Skew_vertex
{
public:
    Skew_vertex *left, *right;
    int key;
    Skew_vertex()
    {
        left = NULL;
        right = NULL;
    }
    Skew_vertex(int k)
    {
        left = NULL;
        right = NULL;
        key = k;
    }
    bool operator<(Skew_vertex another)
    {
        return key < another.key;
    }
    void swap_children()
    {
        Skew_vertex * s = left;
        left = right;
        right = s;
    }
};

Skew_vertex * merge_skew_vertex(Skew_vertex * base, Skew_vertex * another)
{
    if (base == NULL)
        return another;
    if (another == NULL)
        return base;
    if (*another < *base)
        swap(*base, *another);
    base->right = merge_skew_vertex(another, base->right);
    base->swap_children();
    return base;
}

vector <Skew_vertex> global_skew_vertex;

Skew_vertex * create_skew_vertex(int key)
{
    /*Binomial_vertex new_vertex;
    return &new_vertex;*/
    if (global_skew_vertex.empty())
        global_skew_vertex.reserve(INF);
    global_skew_vertex.push_back(Skew_vertex(key));
    return &global_skew_vertex.back();
}

class Skew_heap
{
public:
    Skew_vertex *root;
    Skew_heap(){};
    Skew_heap(int key)
    {
        //Skew_vertex r = Skew_vertex(key);
        //root = &r;
        root = create_skew_vertex(key);
    }
    void meld(Skew_heap another)
    {
        root = merge_skew_vertex(root, another.root);
    }
    void insert(int key)
    {
        Skew_heap new_heap = Skew_heap(key);
        meld(new_heap);
    }
    int extract_min()
    {
        if (root != NULL)
        {
            int result = root->key;
            root = merge_skew_vertex(root->left, root->right);
            return result;
        }
        return 0;
    }
    HeapType get_heap_type()
    {
        return HeapType::SKEW;
    }
};

class Skew_heap_array : public IMergeableHeap
{
private:
    vector <Skew_heap> heaps;
public:
    void add_heap(int key)
    {
        heaps.push_back(Skew_heap(key));
    }
    void insert(int index, int key)
    {
        heaps[index].insert(key);
    }
    int extract_min(int index)
    {
        return heaps[index].extract_min();
    }
    void meld(int first_index, int second_index)
    {
        if (first_index < heaps.size() && second_index < heaps.size() && first_index != second_index)
        {
            heaps[first_index].meld(heaps[second_index]);
            if (second_index + 1 != heaps.size())
            {
                heaps[second_index] = heaps.back();
                heaps.pop_back();
            }
        }
    }
};

//////
//MELD
//////

template<class MHeap>
void meld(MHeap first, MHeap second)
{
    if (first.get_heap_type() == second.get_heap_type())
    {
        first.meld(second);
    }
}
