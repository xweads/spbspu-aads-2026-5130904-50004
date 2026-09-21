#ifndef SCHEDULE_AVL_HPP
#define SCHEDULE_AVL_HPP

#include <memory>
#include <cassert>
#include <stack>
#include "AVLNode.hpp"
#include "Booking.hpp"

namespace dolenko
{
  template< class T >
  class AVLIterator
  {
  private:
    std::stack< AVLNode< T >* > stack_;

    void pushLeft(AVLNode< T >* node) noexcept
    {
      while (node)
      {
        stack_.push(node);
        node = node->left.get();
      }
    }

  public:
    explicit AVLIterator(AVLNode< T >* root) noexcept
    {
      pushLeft(root);
    }

    AVLIterator() noexcept = default;

    T& operator*()
    {
      assert(!stack_.empty() && "AVLIterator: dereference of end() iterator");
      return stack_.top()->data;
    }

    const T& operator*() const
    {
      assert(!stack_.empty() && "AVLIterator: dereference of end() iterator");
      return stack_.top()->data;
    }

    AVLIterator& operator++()
    {
      assert(!stack_.empty() && "AVLIterator: increment of end() iterator");
      AVLNode< T >* node = stack_.top();
      stack_.pop();

      if (node->right)
      {
        pushLeft(node->right.get());
      }
      return *this;
    }

    AVLIterator operator++(int)
    {
      AVLIterator tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const AVLIterator& other) const noexcept
    {
      if (stack_.empty() && other.stack_.empty())
      {
        return true;
      }
      if (stack_.empty() || other.stack_.empty())
      {
        return false;
      }
      return stack_.top() == other.stack_.top();
    }

    bool operator!=(const AVLIterator& other) const noexcept
    {
      return !(*this == other);
    }
  };

  template< class T >
  class AVLConstIterator
  {
  private:
    std::stack< const AVLNode< T >* > stack_;

    void pushLeft(const AVLNode< T >* node) noexcept
    {
      while (node)
      {
        stack_.push(node);
        node = node->left.get();
      }
    }

  public:
    explicit AVLConstIterator(const AVLNode< T >* root) noexcept
    {
      pushLeft(root);
    }

    AVLConstIterator() noexcept = default;

    const T& operator*() const
    {
      assert(!stack_.empty() && "AVLConstIterator: dereference of end() iterator");
      return stack_.top()->data;
    }

    AVLConstIterator& operator++()
    {
      assert(!stack_.empty() && "AVLConstIterator: increment of end() iterator");
      const AVLNode< T >* node = stack_.top();
      stack_.pop();

      if (node->right)
      {
        pushLeft(node->right.get());
      }
      return *this;
    }

    AVLConstIterator operator++(int)
    {
      AVLConstIterator tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const AVLConstIterator& other) const noexcept
    {
      if (stack_.empty() && other.stack_.empty())
      {
        return true;
      }
      if (stack_.empty() || other.stack_.empty())
      {
        return false;
      }
      return stack_.top() == other.stack_.top();
    }

    bool operator!=(const AVLConstIterator& other) const noexcept
    {
      return !(*this == other);
    }
  };

  template< class T >
  class ScheduleAVL
  {
  private:
    std::unique_ptr< AVLNode< T > > root_;
    std::size_t size_;

    int getHeight(const AVLNode< T >* node) const noexcept
    {
      return node ? node->height : 0;
    }

    int getBalance(const AVLNode< T >* node) const noexcept
    {
      return node ? (getHeight(node->left.get()) - getHeight(node->right.get())) : 0;
    }

    void updateHeight(AVLNode< T >* node) noexcept
    {
      if (node)
      {
        node->height = 1 + std::max(getHeight(node->left.get()), getHeight(node->right.get()));
      }
    }

    std::unique_ptr< AVLNode< T > > rotateRight(std::unique_ptr< AVLNode< T > > y)
    {
      std::unique_ptr< AVLNode< T > > x = std::move(y->left);
      y->left = std::move(x->right);
      updateHeight(y.get());
      x->right = std::move(y);
      updateHeight(x.get());
      return x;
    }

    std::unique_ptr< AVLNode< T > > rotateLeft(std::unique_ptr< AVLNode< T > > x)
    {
      std::unique_ptr< AVLNode< T > > y = std::move(x->right);
      x->right = std::move(y->left);
      updateHeight(x.get());
      y->left = std::move(x);
      updateHeight(y.get());
      return y;
    }

    std::unique_ptr< AVLNode< T > > insertNode(std::unique_ptr< AVLNode< T > > node, const T& val, bool& success)
    {
      if (!node)
      {
        success = true;
        return std::make_unique< AVLNode< T > >(val);
      }

      if (val < node->data)
      {
        node->left = insertNode(std::move(node->left), val, success);
      }
      else if (val > node->data)
      {
        node->right = insertNode(std::move(node->right), val, success);
      }
      else
      {
        success = false;
        return node;
      }

      if (!success) return node;

      updateHeight(node.get());
      int balance = getBalance(node.get());

      if (balance > 1 && val < node->left->data)
      {
        return rotateRight(std::move(node));
      }
      if (balance < -1 && val > node->right->data)
      {
        return rotateLeft(std::move(node));
      }
      if (balance > 1 && val > node->left->data)
      {
        node->left = rotateLeft(std::move(node->left));
        return rotateRight(std::move(node));
      }
      if (balance < -1 && val < node->right->data)
      {
        node->right = rotateRight(std::move(node->right));
        return rotateLeft(std::move(node));
      }

      return node;
    }

    std::unique_ptr< AVLNode< T > > findMin(const std::unique_ptr< AVLNode< T > >& node)
    {
      if (!node->left)
      {
        return std::move(node->right);
      }
      node->left = findMin(node->left);
      updateHeight(node.get());
      return node;
    }

    std::unique_ptr< AVLNode< T > > deleteNode(std::unique_ptr< AVLNode< T > > node, const T& val, bool& success)
    {
      if (!node)
      {
        success = false;
        return nullptr;
      }

      if (val < node->data)
      {
        node->left = deleteNode(std::move(node->left), val, success);
      }
      else if (val > node->data)
      {
        node->right = deleteNode(std::move(node->right), val, success);
      }
      else
      {
        success = true;
        if (!node->left)
        {
          return std::move(node->right);
        }
        if (!node->right)
        {
          return std::move(node->left);
        }

        std::unique_ptr< AVLNode< T > > minRight = std::move(node->right);
        T minVal = minRight->data;
        while (minRight->left)
        {
          minRight = std::move(minRight->left);
          minVal = minRight->data;
        }

        node->data = minVal;
        node->right = deleteNode(std::move(node->right), minVal, success);
      }

      if (!node) return nullptr;

      updateHeight(node.get());
      int balance = getBalance(node.get());

      if (balance > 1 && getBalance(node->left.get()) >= 0)
      {
        return rotateRight(std::move(node));
      }
      if (balance > 1 && getBalance(node->left.get()) < 0)
      {
        node->left = rotateLeft(std::move(node->left));
        return rotateRight(std::move(node));
      }
      if (balance < -1 && getBalance(node->right.get()) <= 0)
      {
        return rotateLeft(std::move(node));
      }
      if (balance < -1 && getBalance(node->right.get()) > 0)
      {
        node->right = rotateRight(std::move(node->right));
        return rotateLeft(std::move(node));
      }

      return node;
    }

    void clearNode(std::unique_ptr< AVLNode< T > >& node) noexcept
    {
      if (node)
      {
        clearNode(node->left);
        clearNode(node->right);
        node.reset();
      }
    }

  public:
    ScheduleAVL() noexcept :
      root_(nullptr),
      size_(0)
    {}

    ScheduleAVL(const ScheduleAVL& other)
    {
      ScheduleAVL tmp;
      for (auto it = other.begin(); it != other.end(); ++it)
      {
        tmp.insert(*it);
      }
      swap(tmp);
    }

    ScheduleAVL(ScheduleAVL&& other) noexcept :
      root_(std::move(other.root_)),
      size_(other.size_)
    {
      other.size_ = 0;
    }

    ScheduleAVL& operator=(const ScheduleAVL& other)
    {
      if (this == &other) return *this;
      ScheduleAVL tmp(other);
      swap(tmp);
      return *this;
    }

    ScheduleAVL& operator=(ScheduleAVL&& other) noexcept
    {
      if (this == &other) return *this;
      root_ = std::move(other.root_);
      size_ = other.size_;
      other.size_ = 0;
      return *this;
    }

    ~ScheduleAVL() noexcept
    {
      clearNode(root_);
    }

    void swap(ScheduleAVL& other) noexcept
    {
      std::swap(root_, other.root_);
      std::swap(size_, other.size_);
    }

    bool insert(const T& val)
    {
      bool success = false;
      root_ = insertNode(std::move(root_), val, success);
      if (success) ++size_;
      return success;
    }

    bool insert(T&& val)
    {
      bool success = false;
      root_ = insertNode(std::move(root_), val, success);
      if (success) ++size_;
      return success;
    }

    bool remove(const T& val)
    {
      bool success = false;
      root_ = deleteNode(std::move(root_), val, success);
      if (success) --size_;
      return success;
    }

    bool contains(const T& val) const noexcept
    {
      AVLNode< T >* cur = root_.get();
      while (cur)
      {
        if (val < cur->data)
        {
          cur = cur->left.get();
        }
        else if (val > cur->data)
        {
          cur = cur->right.get();
        }
        else
        {
          return true;
        }
      }
      return false;
    }

    std::size_t size() const noexcept
    {
      return size_;
    }

    bool empty() const noexcept
    {
      return size_ == 0;
    }

    void clear() noexcept
    {
      clearNode(root_);
      size_ = 0;
    }

    AVLIterator< T > begin() noexcept
    {
      return AVLIterator< T >(root_.get());
    }

    AVLIterator< T > end() noexcept
    {
      return AVLIterator< T >(nullptr);
    }

    AVLConstIterator< T > begin() const noexcept
    {
      return AVLConstIterator< T >(root_.get());
    }

    AVLConstIterator< T > end() const noexcept
    {
      return AVLConstIterator< T >(nullptr);
    }

    AVLConstIterator< T > cbegin() const noexcept
    {
      return begin();
    }

    AVLConstIterator< T > cend() const noexcept
    {
      return end();
    }
  };
}

#endif
