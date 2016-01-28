#ifndef SEPARATE_CHAINING_HASHING_TABLE_H
#define SEPARATE_CHAINING_HASHING_TABLE_H

#include <vector>
#include <list>
#include <functional>
#include <cstddef>
#include <algorithm>

int nextPrime(int n);

template <typename HashedObj>
class HashTable
{
public:
    explicit HashTable(int size = 101) : currentSize{0}
    {
        theLists.resize(size);
    }

    bool contains(const HashedObj &x) const
    {
        auto &theList = theLists[myhash(x)];
        return std::find(theList.begin(), theList.end(), x) != theList.end();
    }

    void makeEmpty()
    {
        for (auto &theList : theLists)
            theList.clear();
    }

    bool insert(const HashedObj &x)
    {
        auto &theList = theLists[myhash(x)];
        if (std::find(theList.begin(), theList.end(), x) != theList.end())
            return false;
        theList.push_back(x);
        if (++currentSize > theLists.size())
            rehash();
        return true;
    }

    bool remove(const HashedObj &x)
    {
        auto &theList = theLists[myhash(x)];
        auto ite = std::find(theList.begin(), theList.end(), x);
        if (ite == theList.end())
            return false;
        theList.erase(ite);
        --currentSize;
        return true;
    }

private:
    std::vector<std::list<HashedObj>> theLists;
    int currentSize;

    size_t myhash(const HashedObj &x) const
    {
        static std::hash<HashedObj> hf;
        return hf(x) % theLists.size();
    }

    void rehash()
    {
        std::vector<std::list<HashedObj>> oldLists = theLists;

        for (auto &theList : theLists)
            theList.clear();

        theLists.resize(nextPrime(theLists.size() * 2));
        currentSize = 0;
        for (auto &theList : oldLists)
            for (auto &x : theList)
                insert(std::move(x));
    }
};

#endif
