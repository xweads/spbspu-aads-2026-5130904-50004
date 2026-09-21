#ifndef BILIST_HPP
#define BILIST_HPP

#include <cassert>
#include <cstddef>
#include <utility>
#include "Node.hpp"
#include "Iterators.hpp"

namespace dolenko
{
  template< class T >
  class BiList
  {
  public:
    using Iterator = LIter< T >;
    using ConstIterator = LCIter< T >;

    BiList() noexcept = default;
    BiList(const BiList& other)
    {
      BiList tmp;
      for (Node< T >* cur = other.head_; cur; cur = cur->next)
      {
        tmp.push_back(cur->val);
      }
      swap(tmp);
    }
    BiList(BiList&& other) noexcept:
      head_(other.head_),
      tail_(other.tail_),
      size_(other.size_)
    {
      other.head_ = nullptr;
      other.tail_ = nullptr;
      other.size_ = 0;
    }
    ~BiList() noexcept
    {
      clear();
    }

    BiList& operator=(const BiList& other)
    {
      if (this == &other)
      {
        return *this;
      }
      BiList tmp(other);
      swap(tmp);
      return *this;
    }
    BiList& operator=(BiList&& other) noexcept
    {
      if (this == &other)
      {
        return *this;
      }
      clear();
      head_ = other.head_;
      tail_ = other.tail_;
      size_ = other.size_;
      other.head_ = nullptr;
      other.tail_ = nullptr;
      other.size_ = 0;
      return *this;
    }

    bool empty() const noexcept
    {
      return size_ == 0;
    }
    std::size_t size() const noexcept
    {
      return size_;
    }

    T& front() noexcept
    {
      assert(head_ && "BiList: front() on empty list");
      return head_->val;
    }
    const T& front() const noexcept
    {
      assert(head_ && "BiList: front() on empty list");
      return head_->val;
    }
    T& back() noexcept
    {
      assert(tail_ && "BiList: back() on empty list");
      return tail_->val;
    }
    const T& back() const noexcept
    {
      assert(tail_ && "BiList: back() on empty list");
      return tail_->val;
    }

    LIter< T > begin() noexcept
    {
      return LIter< T >(head_);
    }
    LIter< T > end() noexcept
    {
      return LIter< T >(nullptr);
    }
    LCIter< T > begin() const noexcept
    {
      return LCIter< T >(head_);
    }
    LCIter< T > end() const noexcept
    {
      return LCIter< T >(nullptr);
    }

    void push_front(const T& value)
    {
      Node< T >* node = create_node(value);
      link_front(node);
    }
    void push_front(T&& value)
    {
      Node< T >* node = create_node(std::move(value));
      link_front(node);
    }
    void push_back(const T& value)
    {
      Node< T >* node = create_node(value);
      link_back(node);
    }
    void push_back(T&& value)
    {
      Node< T >* node = create_node(std::move(value));
      link_back(node);
    }
    void pop_front() noexcept
    {
      if (!head_)
      {
        return;
      }
      Node< T >* old_head = head_;
      Node< T >* new_head = old_head->next;
      if (new_head)
      {
        new_head->prev = nullptr;
      }
      head_ = new_head;
      if (tail_ == old_head)
      {
        tail_ = new_head;
      }
      destroy_node(old_head);
      --size_;
    }
    void pop_back() noexcept
    {
      if (!tail_)
      {
        return;
      }
      Node< T >* old_tail = tail_;
      Node< T >* new_tail = old_tail->prev;
      if (new_tail)
      {
        new_tail->next = nullptr;
      }
      tail_ = new_tail;
      if (head_ == old_tail)
      {
        head_ = new_tail;
      }
      destroy_node(old_tail);
      --size_;
    }
    void swap(BiList& other) noexcept
    {
      Node< T >* h = head_;
      head_ = other.head_;
      other.head_ = h;

      Node< T >* t = tail_;
      tail_ = other.tail_;
      other.tail_ = t;

      std::size_t s = size_;
      size_ = other.size_;
      other.size_ = s;
    }
    void clear() noexcept
    {
      while (head_)
      {
        pop_front();
      }
    }

  private:
    Node< T >* head_ = nullptr;
    Node< T >* tail_ = nullptr;
    std::size_t size_ = 0;

    void link_front(Node< T >* node) noexcept
    {
      if (!head_)
      {
        head_ = node;
        tail_ = node;
        size_ = 1;
        return;
      }
      node->next = head_;
      head_->prev = node;
      head_ = node;
      ++size_;
    }
    void link_back(Node< T >* node) noexcept
    {
      if (!tail_)
      {
        head_ = node;
        tail_ = node;
        size_ = 1;
        return;
      }
      node->prev = tail_;
      tail_->next = node;
      tail_ = node;
      ++size_;
    }

    Node< T >* create_node(const T& value)
    {
      return new Node< T >(value, nullptr, nullptr);
    }
    Node< T >* create_node(T&& value)
    {
      return new Node< T >(std::move(value), nullptr, nullptr);
    }
    void destroy_node(Node< T >* node) noexcept
    {
      delete node;
    }
  };

  template< class T >
  using List = BiList< T >;

  template< class T >
  void swap(BiList< T >& a, BiList< T >& b) noexcept
  {
    a.swap(b);
  }

}

#endif
