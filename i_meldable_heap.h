enum HeapType
{
    BINOMIAL,
    LEFTIST,
    SKEW
};

class IMergeableHeap
{ 
public:
    virtual void add_heap(int) = 0;
    virtual void insert(int, int) = 0;
    virtual int extract_min(int) = 0;
    virtual void meld(int, int) = 0;
    virtual HeapType get_heap_type() = 0;
};
