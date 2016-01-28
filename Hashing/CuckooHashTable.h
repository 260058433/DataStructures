#ifndef CUCKOO_HASH_TABLE_H
#define CUCKOO_HASH_TABLE_H

#include <cstddef>
#include <algorithm>
#include <vector>
#include <random>
#include <string>

class UniformRandom
{
public:
    int next()
    {
        static std::uniform_int_distribution<unsigned int> distribution;
        return static_cast<int>(distribution(generator));
    }

    int next(int high)
    {
        return next(0, high - 1);
    }

    int next(int low, int high)
    {
        static std::uniform_int_distribution<int> distribution(low, high);
        return distribution(generator);
    }

private:
    std::default_random_engine generator;
};

template <typename AnyType>
class CuckooHashFamily
{
public:
    size_t hash(const AnyType &x, int which) const;
    int getNumberOfFunctions();
    void generateNewFunctions();
};

template <int count>
class StringHashFamily
{
public:
    StringHashFamily() : MULTIPLIERS(count)
    {
        generateNewFunctions();
    }

    int getNumberOfFunctions() const
    {
        return count;
    }

    void generateNewFunctions()
    {
        for (auto &mult : MULTIPLIERS)
            mult = r.next();
    }

    size_t hash(const std::string &x, int which) const
    {
        const int multiplier = MULTIPLIERS[which];
        size_t hashVal = 0;
        for (auto ch : x)
            hashVal = multiplier * hashVal + ch;
        return hashVal;
    }

private:
    std::vector<int> MULTIPLIERS;
    UniformRandom r;
};

int nextPrime(int n);

#define MAX_LOAD 0.40

template <typename AnyType, typename HashFamily>
class HashTable
{
public:
    explicit HashTable(int size = 101) : array(nextPrime(size))
    {
        numHashFunctions = hashFunctions.getNumberOfFunctions();
        rehashes = 0;
        makeEmpty();
    }

    bool contains(const AnyType &x) const
    {
        return findPos(x) != -1;
    }

    void makeEmpty()
    {
        currentSize = 0;
        for (auto &entry : array)
            entry.isActive = false;
    }

    bool insert(const AnyType &x)
    {
        if (contains(x))
            return false;
        
        if (currentSize >= array.size() * MAX_LOAD)
            expand();

        return insertHelper(x);
    }

    bool insert(AnyType &&x)
    {
        if (contains(x))
            return false;
        
        if (currentSize >= array.size() * MAX_LOAD)
            expand();

        return insertHelper(std::move(x));
    }

    int size() const
    {
        return currentSize;
    }

    int capacity() const
    {
        return array.size();
    }

    bool remove(const AnyType &x)
    {
        int currentPos = findPos(x);
        if (!isActive(currentPos))
            return false;

        array[currentPos].isActive = false;
        return true;
    }

private:
    struct HashEntry
    {
        AnyType element;
        bool isActive;

        HashEntry(const AnyType &e = AnyType(), bool a = false)
            : element{e}, isActive{a} {}

        HashEntry(AnyType &&e, bool a = false)
            : element{std::move(e)}, isActive{a} {}
    };

    std::vector<HashEntry> array;
    int currentSize;
    int numHashFunctions;
    int rehashes;
    UniformRandom r;
    HashFamily hashFunctions;

    static const int ALLOWED_REHASHES = 5;

    bool insertHelper(const AnyType &xx)
    {
        const int COUNT_LIMIT = 100;
        AnyType x = xx;

        while (true)
        {
            int lastPos = -1;
            int pos;
            
            for (int count = 0; count < COUNT_LIMIT; ++count)
            {
                for (int i = 0; i < numHashFunctions; ++i)
                {
                    pos = myhash(x, i);
                    if (!isActive(pos))
                    {
                        array[pos] = std::move(HashEntry{std::move(x), true});
                        ++currentSize;
                        return true;
                    }
                }

                int i = 0;
                do
                {
                    pos = myhash(x, r.next(numHashFunctions));
                } while(pos == lastPos && i++ < 5);

                lastPos = pos;
                std::swap(x, array[pos].element);
            }
        }

        if (++rehashes > ALLOWED_REHASHES)
        {
            expand();
            rehashes = 0;
        }
        else
            rehash();
    }

    bool insertHelper(AnyType &&x)
    {
        const int COUNT_LIMIT = 100;

        while (true)
        {
            int lastPos = -1;
            int pos;
            
            for (int count = 0; count < COUNT_LIMIT; ++count)
            {
                for (int i = 0; i < numHashFunctions; ++i)
                {
                    pos = myhash(x, i);
                    if (!isActive(pos))
                    {
                        array[pos] = std::move(HashEntry{std::move(x), true});
                        ++currentSize;
                        return true;
                    }
                }

                int i = 0;
                do
                {
                    pos = myhash(x, r.next(numHashFunctions));
                } while(pos == lastPos && i++ < 5);

                lastPos = pos;
                std::swap(x, array[pos].element);
            }
        }

        if (++rehashes > ALLOWED_REHASHES)
        {
            expand();
            rehashes = 0;
        }
        else
            rehash();
    }

    bool isActive(int currentPos) const
    {
        return currentPos != -1 && array[currentPos].isActive;
    }

    int findPos(const AnyType &x) const
    {
        for (int i = 0; i < numHashFunctions; ++i)
        {
            int pos = myhash(x, i);
            if (isActive(pos) && array[pos].element == x)
                return pos;
        }
        return -1;
    }

    void expand()
    {
        rehash(static_cast<int>(array.size() / MAX_LOAD));
    }

    void rehash()
    {
        hashFunctions.generateNewFunctions();
        rehash(array.size());
    }

    void rehash(int newSize)
    {
        std::vector<HashEntry> oldArray = array;

        for (auto &entry : array)
            entry.isActive = false;

        array.resize(nextPrime(newSize));
        currentSize = 0;
        for (auto &entry : oldArray)
            if (entry.isActive)
                insert(std::move(entry.element));
    }

    size_t myhash(const AnyType &x, int which) const
    {
        return hashFunctions.hash(x, which) % array.size();
    }
};

#endif
