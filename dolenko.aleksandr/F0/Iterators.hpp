#ifndef LITER_HPP
#define LITER_HPP

#include <cassert>
#include <cstddef>
#include "Node.hpp"

namespace dolenko
{
    template< class T > class BiList;
    template< class T > struct Node;

    template< class T >
    class LIter
    {
        friend class BiList< T >;
    public:
        LIter() noexcept = default;
        T& operator*() const
        {
            assert(node_ && "LIter: dereference of end()/null iterator");
            return node_->val;
        }
        LIter& operator++()
        {
            assert(node_ && "LIter: increment of end()/null iterator");
            node_ = node_->next;
            return *this;
        }
        LIter operator++(int)
        {
            LIter tmp(*this);
            ++(*this);
            return tmp;
        }
        LIter& operator--()
        {
            assert(node_ && "LIter: decrement of end()/null iterator");
            node_ = node_->prev;
            return *this;
        }
        LIter operator--(int)
        {
            LIter tmp(*this);
            --(*this);
            return tmp;
        }
        bool operator==(const LIter& other) const noexcept
        {
            return node_ == other.node_;
        }
        bool operator!=(const LIter& other) const noexcept
        {
            return !(*this == other);
        }
    private:
        Node< T >* node_ = nullptr;
        explicit LIter(Node< T >* n) noexcept:
            node_(n)
        {}
    };

    template< class T >
    class LCIter
    {
        friend class BiList< T >;
    public:
        LCIter() noexcept = default;
        const T& operator*() const
        {
            assert(node_ && "LCIter: dereference of end()/null iterator");
            return node_->val;
        }
        LCIter& operator++()
        {
            assert(node_ && "LCIter: increment of end()/null iterator");
            node_ = node_->next;
            return *this;
        }
        LCIter operator++(int)
        {
            LCIter tmp(*this);
            ++(*this);
            return tmp;
        }
        LCIter& operator--()
        {
            assert(node_ && "LCIter: decrement of end()/null iterator");
            node_ = node_->prev;
            return *this;
        }
        LCIter operator--(int)
        {
            LCIter tmp(*this);
            --(*this);
            return tmp;
        }
        bool operator==(const LCIter& other) const noexcept
        {
            return node_ == other.node_;
        }
        bool operator!=(const LCIter& other) const noexcept
        {
            return !(*this == other);
        }
    private:
        const Node< T >* node_ = nullptr;
        explicit LCIter(const Node< T >* n) noexcept:
            node_(n)
        {}
    };

}

#endif
