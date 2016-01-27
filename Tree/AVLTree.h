#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <algorithm>
#include <iostream>
#include <stdexcept>

template <typename Comparable>
class AVLTree
{
public:
    AVLTree() : root{nullptr} {}

    AVLTree(const AVLTree &rhs)
    {
        root = clone(rhs.root);
    }

    AVLTree(AVLTree &&rhs) : root(rhs.root)
    {
        rhs.root = nullptr;
    }

    ~AVLTree()
    {
        makeEmpty();
    }

    AVLTree &operator=(const AVLTree &rhs)
    {
        AVLTree copy{rhs};
        std::swap(*this, copy);
        return *this;
    }

    AVLTree &operator=(AVLTree &&rhs)
    {
        std::swap(root, rhs->root);
        return *this;
    }

    const Comparable &findMin() const
    {
        if (empty())
            throw std::runtime_error{"find empty tree"};
        return findMin(root)->element;
    }

    const Comparable &findMax() const
    {
        if (empty())
            throw std::runtime_error{"find empty tree"};
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
    struct AVLNode
    {
        Comparable element;
        AVLNode *left;
        AVLNode *right;
        int height;

        AVLNode(const Comparable &ele, AVLNode *lt = nullptr, AVLNode *rt = nullptr, int h = 0)
            : element{ele}, left{lt}, right{rt}, height{h} {}

        AVLNode(Comparable &&ele, AVLNode *lt = nullptr, AVLNode *rt = nullptr, int h = 0)
            : element{std::move(ele)}, left{lt}, right{rt}, height{h} {}
    };

    AVLNode *root;

    void insert(const Comparable &x, AVLNode *&t)
    {
        if (t == nullptr)
            t = new AVLNode{x};
        else if (x < t->element)
            insert(x, t->left);
        else if (t->element < x)
            insert(x, t->right);
        balance(t);
    }

    void insert(Comparable &&x, AVLNode *&t)
    {
        if (t == nullptr)
            t = new AVLNode{std::move(x)};
        else if (x < t->element)
            insert(std::move(x), t->left);
        else if (t->element < x)
            insert(std::move(x), t->right);
        balance(t);
    }

    void remove(const Comparable &x, AVLNode *&t)
    {
        if (t == nullptr)
            return;
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
            AVLNode *old = t;
            t = t->left ? t->left : t->right;
            delete old;
        }
        balance(t);
    }

    static const int ALLOWED_IMBALANCE = 1;

    void balance(AVLNode *&t)
    {
        if (t == nullptr)
            return;
        if (ALLOWED_IMBALANCE < height(t->left) - height(t->right))
        {
            if (height(t->left->right) <= height(t->left->left)) //when delete node, maybe equal
                rotateWithLeftChild(t);
            else
                doubleWithLeftChild(t);
        }
        else if (ALLOWED_IMBALANCE < height(t->right) - height(t->left))
        {
            if (height(t->right->left) <= height(t->right->right))
                rotateWithRightChild(t);
            else
                doubleWithRightChild(t);
        }
        t->height = std::max(height(t->left), height(t->right)) + 1;
    }

    int height(AVLNode *t) const
    {
        return t ? t->height : -1;
    }

    void rotateWithLeftChild(AVLNode *&k2)
    {
        AVLNode *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = std::max(height(k2->left), height(k2->right)) + 1;
        k1->height = std::max(height(k1->left), height(k1->right)) + 1;
        k2 = k1;
    }

    void rotateWithRightChild(AVLNode *&k1)
    {
        AVLNode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = std::max(height(k1->left), height(k1->right)) + 1;
        k2->height = std::max(height(k2->left), height(k2->right)) + 1;
        k1 = k2;
    }

    void doubleWithLeftChild(AVLNode *&k3)
    {
        rotateWithRightChild(k3->left);
        rotateWithLeftChild(k3);
    }

    void doubleWithRightChild(AVLNode *&k1)
    {
        rotateWithLeftChild(k1->right);
        rotateWithRightChild(k1);
    }

    AVLNode *findMin(AVLNode *t) const
    {
        if (t == nullptr)
            return nullptr;
        else if (t->left == nullptr)
            return t;
        else
            return findMin(t->left);
    }

    AVLNode *findMax(AVLNode *t) const
    {
        if (t)
            while (t->right)
                t = t->right;
        return t;
    }

    bool contains(const Comparable &x, AVLNode *t) const
    {
        while (t)
        {
            if (x < t->element)
                t = t->left;
            else if (t->element < x)
                t = t->right;
            else
                break;
        }
        return t;
    }

    void makeEmpty(AVLNode *&t)
    {
        if (t)
        {
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
            t = nullptr;
        }
    }

    void printTree(AVLNode *t, std::ostream &out) const
    {
        if (t)
        {
            printTree(t->left, out);
            out << t->element << std::endl;
            printTree(t->right, out);
        }
    }

    AVLNode *clone(AVLNode *t) const
    {
        if (t == nullptr)
            return nullptr;
        else
            return new AVLNode{t->element, clone(t->left), clone(t->right), t->height};
    }
};

#endif
