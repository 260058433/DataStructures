#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <stdexcept>

template <typename Object>
class Vector
{
public:
    explicit Vector(int initSize = 0)
        : theSize{initSize}, theCapacity{initSize + SPARE_CAPACITY}
    {
        objects = new Object[theCapacity]();
    }

    Vector(const Vector &rhs)
        : theSize{rhs.theSize}, theCapacity{rhs.theCapacity}
    {
        objects = new Object[theCapacity];
        for (int i = 0; i < theSize; ++i)
            objects[i] = rhs.objects[i];
    }

    Vector &operator=(const Vector &rhs)
    {
        Vector copy(rhs);
        std::swap(*this, copy);
        return *this;
    }

    ~Vector()
    {
        delete[] objects;
    }

    Vector(Vector&& rhs)
        : theSize{rhs.theSize}, theCapacity{rhs.theCapacity}, objects{rhs.objects}
    {
        rhs.theSize = 0;
        rhs.theCapacity = 0;
        rhs.objects = nullptr;
    }

    Vector &operator=(Vector&& rhs)
    {
        std::swap(theSize, rhs.theSize);
        std::swap(theCapacity, rhs.theCapacity);
        std::swap(objects, rhs.objects);
        return *this;
    }

    bool empty() const
    {
        return size() == 0;
    }

    int size() const
    {
        return theSize;
    }

    int capacity() const
    {
        return theCapacity;
    }

    Object &operator[](int index)
    {
        if (index < 0 || index >= size())
            throw std::out_of_range{"index less than 0 or greater than the size"};
        return objects[index];
    }

    const Object &operator[](int index) const
    {
        if (index < 0 || index >= size())
            throw std::out_of_range{"index less than 0 or greater than the size"};
        return objects[index];
    }

    void resize(int newSize)
    {
        if (newSize > theCapacity)
            reserve(newSize * 2);
        theSize = newSize;
    }

    void reserve(int newCapacity)
    {
        if (newCapacity <= theCapacity)
            return;

        Object *newObjects = new Object[newCapacity]();
        for (int i = 0; i < theSize; ++i)
            newObjects[i] = std::move(objects[i]);

        theCapacity = newCapacity;
        std::swap(objects, newObjects);
        delete[] newObjects;
    }

    void push_back(const Object &x)
    {
        if (theSize == theCapacity)
            reserve(2 * theCapacity + 1);
        objects[theSize++] = x;
    }

    void push_back(const Object &&x)
    {
        if (theSize == theCapacity)
            reserve(2 * theCapacity + 1);
        objects[theSize++] = std::move(x);
    }

    void pop_back()
    {
        if (empty())
            throw std::runtime_error{"pop empty vector"};
        --theSize;
    }

    const Object &back() const
    {
        if (empty())
            throw std::runtime_error{"empty vector"};
        return objects[theSize - 1];
    }

    typedef Object * iterator;
    typedef const Object * const_iterator;

    iterator begin()
    {
        return objects;
    }

    const_iterator begin() const
    {
        return objects;
    }

    iterator end()
    {
        return objects + theSize;
    }

    const_iterator end() const
    {
        return objects + theSize;
    }

    static const int SPARE_CAPACITY = 2;

private:
    int theSize;
    int theCapacity;
    Object *objects;
};

#endif
