#ifndef _AVL_TREE_H
#define _AVL_TREE_H

#include <memory>

template <typename Key, typename Value=Key>
class AVLTree {
public:
//==========================================================================================
  class Node {
  private:
    Key key_;
    Value value_;
    int height_;
    std::unique_ptr<Node> left_, right_;
    Node(const Key& key) : key_(key), height_(0) {}
    Node(const Key& key, const Value& value) : key_(key), value_(value), height_(0) {}

  public:
    Node *left() {
      return left_.get();
    }
    Node *right() {
      return right_.get();
    }
    const Key& key() const {
      return key_;
    }
    const Value& value() const {
      return value_;
    }
    const int height() const {
      if(this == NULL) return -1;
      return height_;
    }

    friend class AVLTree<Key, Value>;
  };
//==========================================================================================
private:
  std::unique_ptr<Node> root_;
  int size_;

public:
  AVLTree() : size_(0) {}

  // TODO: Add code to update node heights and do rebalancing...
  
  
  Value& operator[](const Key& key) {
    std::stack< std::unique_ptr<Node> > prev_parents;
    // Try to find the node with the value we want:
    std::unique_ptr<Node> *cur;
    for (cur = &root_;
         cur->get() != nullptr;
         cur = key < (*cur)->key_ ? &(*cur)->left_ : &(*cur)->right_) {
      prev_parents.push(cur);
      if (key == (*cur)->key_) {
        return (*cur)->value_;
      }
    }
    // If we didn't find it, insert a new node with that key:
    // (This is the same behaviour as an std::map.)
    cur->reset(new Node(key));
    ++size_;
    while( !prev_parents.empty() ){
      prev_parents->top()->get()->NAME_OF_RECALC_HEIGHT_FUNCTION(); // TODO $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
      prev_parents->pop();
    }
    return (*cur)->value_;
  }

  int size() {
    return size_;
  }

  Node *root() {
    return root_.get();
  }

private:
   void balance(std::unique_ptr<Node>* current) {
      Node* nodes[3];
      Node* branches[4];

      if( current->get()->left_->get()->value() - current->get()->right_->get()->value() == 2 )
      {

      }
      else if( current->get()->left_->get()->value() - current->get()->right_->get()->value() == -2 )
      {

      }
      else return; // doesn't need to be balanced

   }
};

#endif
