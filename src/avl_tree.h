#ifndef _AVL_TREE_H
#define _AVL_TREE_H

#include <iostream>
#include <memory>
#include <stack>

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
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    Node(const Key& key) : key_(key), height_(0) {}
    Node(const Key& key, const Value& value) : key_(key), value_(value), height_(0) {}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
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
      return this->height_;
    }
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  private:
    void recalc_height() {
      this->height_ = 1 + std::max( this->left()->height() , this->right()->height() );
    }
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    static bool balance( std::unique_ptr<Node>* current ) {
      /* arrays for node and branches go from least to greatest. eg: *nodes[0] < *nodes[1] < *nodes[2]
          Values are assigned in one of the 4 blocks of code or cases below.  They are then reattached
          and the height_ values are updated. */

      Node* nodes[3];
      Node* branches[4];
      if( current->get()->left()->height() - current->get()->right()->height() == 2 )   //Left
      {
        if( current->get()->left()->left()->height() - current->get()->left()->right()->height() == 1 ) {   //Left-Left
          branches[0] = current->get()->left()->left()->left_.release();
          branches[1] = current->get()->left()->left()->right_.release();
          branches[2] = current->get()->left()->right_.release();
          branches[3] = current->get()->right_.release();
          nodes[0] = current->get()->left()->left_.release();
          nodes[1] = current->get()->left_.release();
          nodes[2] = current->release();
        }
        else if( current->get()->left()->left()->height() - current->get()->left()->right()->height() == -1 ) { //Left-Right
          branches[0] = current->get()->left()->left_.release();
          branches[1] = current->get()->left()->right()->left_.release();
          branches[2] = current->get()->left()->right()->right_.release();
          branches[3] = current->get()->right_.release();
          nodes[1] = current->get()->left()->right_.release();
          nodes[0] = current->get()->left_.release();
          nodes[2] = current->release();
        }
        else { std::cerr << "WTF!?: Left case but not left-left or left-right." << std::endl; exit(10); }
      }
      else if( current->get()->left()->height() - current->get()->right()->height() == -2 )  //Right
      {
        if( current->get()->right()->left()->height() - current->get()->right()->right()->height() == -1 ) {  //Right-Right
          branches[0] = current->get()->left_.release();
          branches[1] = current->get()->right()->left_.release();
          branches[2] = current->get()->right()->right()->left_.release();
          branches[3] = current->get()->right()->right()->right_.release();
          nodes[2] = current->get()->right()->right_.release();
          nodes[1] = current->get()->right_.release();
          nodes[0] = current->release();
        }
        else if( current->get()->right()->left()->height() - current->get()->right()->right()->height() == 1 ) {  //Right-Left
          branches[0] = current->get()->left_.release();
          branches[1] = current->get()->right()->left()->left_.release();
          branches[2] = current->get()->right()->left()->right_.release();
          branches[3] = current->get()->right()->right_.release();
          nodes[1] = current->get()->right()->left_.release();
          nodes[2] = current->get()->right_.release();
          nodes[0] = current->release();
        }
        else { std::cerr << "WTF!?: Right case but not right-right or right-left." << std::endl; exit(11); }
      }
      else return false; // doesn't need to be balanced or it's child should be balanced first
  
      // Reconnect
      nodes[0]->left_.reset( branches[0] );
      nodes[0]->right_.reset( branches[1] );
      nodes[2]->left_.reset( branches[2] );
      nodes[2]->right_.reset( branches[3] );
      nodes[1]->left_.reset( nodes[0] );
      nodes[1]->right_.reset( nodes[2] );
      current->reset( nodes[1] );
      //Recalculate from bottom to top
      nodes[0]->recalc_height();
      nodes[2]->recalc_height();
      nodes[1]->recalc_height();
      return true; 
    }
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    friend class AVLTree<Key, Value>;
  };
//==========================================================================================
private:
  std::unique_ptr<Node> root_;
  int size_;

public:
  AVLTree() : size_(0) {}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  Value& operator[](const Key& key) {
    std::stack< std::unique_ptr<Node>* > prev_parents;
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

    // Now recalculate heights and balance
    while( !prev_parents.empty() ){
      prev_parents.top()->get()->recalc_height();
      Node::balance( prev_parents.top() );
      prev_parents.pop();
    }

    return (*this)[key];
  }
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  int size() {
    return size_;
  }
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  Node *root() {
    return root_.get();
  }
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  void print() { print( this->root() , 0 ); }
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
private:
  void print( Node* current, int indent_level ) {
    if( current == NULL ) return;

    print( current->right(), indent_level + 1 );
    for(int k = 0; k < indent_level; k++)
      std::cout << "    ";
    std::cout << "{ " << current->key_ << "," << current->height() << " }" << std::endl;

    print( current->left() , indent_level + 1 ); 
  }
  
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
private:  // testing methods
  void force_balance() {
    force_balance(&this->root_);
    force_calc_height( this->root() );
  }
  void force_balance( std::unique_ptr<Node>* head ) {
    if( !head->get() ) return;
    force_balance( &head->get()->left_ );
    force_balance( &head->get()->right_ );
    Node::balance( head );
  }
  void force_calc_height( Node* head ) {
    if(!head) return;
    force_calc_height( head->left() ); 
    force_calc_height( head->right() );
    head->recalc_height(); 
  }
};

#endif

