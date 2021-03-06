#ifndef QUADRATIC_PROBING_HASHING_TABLE_H
#define QUADRATIC_PROBING_HASHING_TABLE_H

#include <algorithm>
#include <vector>
#include <cstddef>
#include <functional>

int nextPrime(int n);

template <typename HashedObj>
class HashTable
{
public:
    explicit HashTable(int size = 101) : array(nextPrime(size)) {}

    bool contains(const HashedObj &x) const
    {
        return isActive(findPos(x));
    }

    void makeEmpty()
    {
        currentSize = 0;
        for (auto &entry : array)
            entry.info = EMPTY;
    }

    bool insert(const HashedObj &x)
    {
        int currentPos = findPos(x);
        if (isActive(currentPos))
            return false;

        array[currentPos].element = x;
        array[currentPos].info = ACTIVE;

        if (++currentSize > array.size() / 2)
            rehash();
        
        return true;
    }

    bool insert(HashedObj &&x)
    {
        int currentPos = findPos(x);
        if (isActive(currentPos))
            return false;

        array[currentPos].element = std::move(x);
        array[currentPos].info = ACTIVE;

        if (++currentSize > array.size() / 2)
            rehash();
        
        return true;
    }

    bool remove(const HashedObj &x)
    {
        int currentPos = findPos(x);
        if (!isActive(currentPos))
            return false;

        array[currentPos].info = DELETED;
        return true;
    }

    enum EntryType {ACTIVE, EMPTY, DELETED};
private:
    struct HashEntry
    {
        HashedObj element;
        EntryType info;

        HashEntry(const HashedObj &e = HashedObj{}, EntryType i = EMPTY)
            : element{e}, info{i} {}

        HashEntry(HashedObj &&e, EntryType i = EMPTY)
            : element{std::move(e)}, info{i} {}
    };

    std::vector<HashEntry> array;
    int currentSize;

    bool isActive(int currentPos) const
    {
        return array[currentPos].info == ACTIVE;
    }

    int findPos(const HashedObj &x) const
    {
        int offset = 1;
        int currentPos = myhash(x);

        while (array[currentPos].info != EMPTY && array[currentPos].element != x)
        {
            currentPos += offset;
            offset += 2;
            if (currentPos >= array.size())
                currentPos -= array.size();
        }
        return currentPos;
    }

    void rehash()
    {
        std::vector<HashEntry> oldArray = array;

        for (auto &entry : array)
            entry.info = EMPTY;

        array.resize(nextPrime(array.size() * 2));
        currentSize = 0;
        for (auto &entry : oldArray)
            if (entry.info == ACTIVE)
                insert(std::move(entry.element));
    }

    size_t myhash(const HashedObj &x) const
    {
        static std::hash<HashedObj> hf;
        return hf(x) % array.size();
    }
};

#endif
