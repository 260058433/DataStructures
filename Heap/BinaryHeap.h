#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include <vector>
#include <algorithm>
#include <stdexcept>

template <typename Comparable>
class BinaryHeap
{
public:
    explicit BinaryHeap(int capacity = 100) : heapArray(capacity + 1), currentSize{0} {}

    explicit BinaryHeap(const std::vector<Comparable> &items)
        : heapArray{items.size() + 10}, currentSize{items.size()}
    {
        for (int i = 0; i < items.size(); ++i)
            heapArray[i + 1] = items[i];
        buildHeap();
    }

    bool empty() const
    {
        return currentSize == 0;
    }

    const Comparable &top() const
    {
        if (empty())
            throw std::runtime_error{"access empty heap top element"};
        return heapArray[1];
    }

    void insert(const Comparable &x)
    {
        if (currentSize == heapArray.size() - 1)
            heapArray.resize(heapArray.size() * 2);


        int hole = ++currentSize;
        for (; hole > 1 && x < heapArray[hole / 2]; hole /= 2)
            heapArray[hole] = std::move(heapArray[hole / 2]);
        heapArray[hole] = x;
    }

    void insert(Comparable &&x)
    {
        if (currentSize == heapArray.size() - 1)
            heapArray.resize(heapArray.size() * 2);


        int hole = ++currentSize;
        for (; hole > 1 && x < heapArray[hole / 2]; hole /= 2)
            heapArray[hole] = std::move(heapArray[hole / 2]);
        heapArray[hole] = std::move(x);
    }

    void pop()
    {
        if (empty())
            throw std::runtime_error{"pop empty heap"};

        heapArray[1] = std::move(heapArray[currentSize--]);
        percolateDown(1);
    }

    void pop(Comparable &minItem)
    {
        if (empty())
            throw std::runtime_error{"pop empty heap"};

        minItem = std::move(heapArray[1]);
        heapArray[1] = std::move(heapArray[currentSize--]);
        percolateDown(1);
    }

    void makeEmpty()
    {
        currentSize = 0;
    }

private:
    int currentSize;
    std::vector<Comparable> heapArray;

    void buildHeap()
    {
        for (int i = currentSize / 2; i > 0; --i)
            percolateDown(i);
    }

    void percolateDown(int hole)
    {
        Comparable tmp = std::move(heapArray[hole]);
        for (int child = hole * 2; child <= currentSize; hole = child, child = hole * 2)
        {
            if (child != currentSize && heapArray[child + 1] < heapArray[child])
                ++child;
            if (heapArray[child] < tmp)
                heapArray[hole] = std::move(heapArray[child]);
            else
                break;
        }
        heapArray[hole] = std::move(tmp);
    }
};

#endif
