#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <algorithm>
#include <stdexcept>
#include <iostream>

template <typename Comparable>
class BinarySearchTree
{
public:
    BinarySearchTree()
        : root{nullptr} {}

    BinarySearchTree(const BinarySearchTree &rhs)
    {
        root = clone(rhs.root);
    }

    BinarySearchTree(BinarySearchTree &&rhs)
        : root{rhs.root}
    {
        rhs.root = nullptr;
    }
    
    ~BinarySearchTree()
    {
        makeEmpty();
    }

    BinarySearchTree &operator=(const BinarySearchTree &rhs)
    {
        BinaryNode copy{rhs};
        std::swap(*this, rhs);
        return *this;
    }

    BinarySearchTree &operator=(BinarySearchTree &&rhs)
    {
        std::swap(root, rhs.root);
        return *this;
    }

    const Comparable &findMin() const
    {
        if (empty())
            throw std::runtime_error{"find empty binary search tree"};
        return findMin(root)->element;
    }

    const Comparable &findMax() const
    {
        if (empty())
            throw std::runtime_error{"find empty binary search tree"};
        return findMax(root)->element;
    }

    bool contains(const Comparable &x) const
    {
        return contains(x, root);
    }

    bool empty() const
    {
        return root == nullptr;
    }

    void printTree(std::ostream &out = std::cout) const
    {
        if (empty())
            out << "Empty tree" << std::endl;
        else
            printTree(root, out);
    }

    void makeEmpty()
    {
        makeEmpty(root);
    }

    void insert(const Comparable &x)
    {
        insert(x, root);
    }

    void insert(Comparable &&x)
    {
        insert(std::move(x), root);
    }

    void remove(const Comparable &x)
    {
        remove(x, root);
    }

private:
    struct BinaryNode
    {
        Comparable element;
        BinaryNode *left;
        BinaryNode *right;

        BinaryNode(const Comparable &theElement, BinaryNode *lt, BinaryNode *rt)
            : element{theElement}, left{lt}, right{rt} {}

        BinaryNode(Comparable &&theElement, BinaryNode *lt, BinaryNode *rt)
            :element{std::move(theElement)}, left{lt}, right{rt} {}
    };

    BinaryNode *root;

    void insert(const Comparable &x, BinaryNode *&t)
    {
        if (t == nullptr)
            t = new BinaryNode{x, nullptr, nullptr};
        else if (x < t->element)
            insert(x, t->left);
        else if (t->element < x)
            insert(x, t->right);
        else
            ; //Duplicate
    }

    void insert(Comparable &&x, BinaryNode *&t)
    {
        if (t == nullptr)
            t = new BinaryNode{std::move(x), nullptr, nullptr};
        else if (x < t->element)
            insert(std::move(x), t->left);
        else if (t->element < x)
            insert(std::move(x), t->right);
        else
            ; //Duplicate
    }

    void remove(const Comparable &x, BinaryNode *&t)
    {
        if (t == nullptr)
            ; //Item not found
        else if (x < t->element)
            remove(x, t->left);
        else if (t->element < x)
            remove(x, t->right);
        else if (t->left && t->right)
        {
            t->element = findMin(t->right)->element;
            remove(t->element, t->right);
        }
        else
        {
            BinaryNode *old = t;
            t = t->left ? t->left : t->right;
            delete old;
        }
    }

    BinaryNode *findMin(BinaryNode *t) const
    {
        if (t)
            while (t->left)
                t = t->left;
        return t;
    }

    BinaryNode *findMax(BinaryNode *t) const
    {
        if (t)
            while (t->right)
                t = t->right;
        return t;
    }

    bool contains(const Comparable &x, BinaryNode *t) const
    {
        if (t == nullptr)
            return false;
        else if (x < t->element)
            return contains(x, t->left);
        else if (t->element < x)
            return contains(x, t->right);
        else
            return true;
    }

    void makeEmpty(BinaryNode *&t)
    {
        if (t)
        {
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
            t = nullptr;
        }
    }

    void printTree(BinaryNode *t, std::ostream &out) const
    {
        if (t)
        {
            printTree(t->left, out);
            out << t->element << std::endl;
            printTree(t->right, out);
        }
    }

    BinaryNode *clone(BinaryNode *t) const
    {
        if (t == nullptr)
            return nullptr;
        else
            return new BinaryNode{t->element, clone(t->left), clone(t->right)};
    }
};

#endif
