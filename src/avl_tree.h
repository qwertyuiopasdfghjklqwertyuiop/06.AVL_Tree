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
      return this->height_;
    }
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    static int height_helper(Node* node) {
// I had to make a helper for height because clang was whining about me checking for this==NULL
      if(!node) return -1;
      return node->height_;
    }
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  private:
    void recalc_height() {
      this->height_ = 1 + std::max( height_helper(this->left()) , height_helper(this->right()) );
    }
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    static bool balance( std::unique_ptr<Node>* current ) {
      /* arrays for node and branches go from least to greatest. eg: *nodes[0] < *nodes[1] < *nodes[2]
          Values are assigned in one of the 4 blocks of code or cases below.  They are then reattached
          and the height_ values are updated.  Parent height is updated outside this function.      */

      Node* nodes[3];
      Node* branches[4];
      if( height_helper(current->get()->left()) - height_helper(current->get()->right()) == 2 )   //Left
      {
        if( height_helper( current->get()->left()->left() ) - height_helper( current->get()->left()->right() ) == 1 ) {   //Left-Left
          branches[0] = current->get()->left()->left()->left_.release();
          branches[1] = current->get()->left()->left()->right_.release();
          branches[2] = current->get()->left()->right_.release();
          branches[3] = current->get()->right_.release();
          nodes[0] = current->get()->left()->left_.release();
          nodes[1] = current->get()->left_.release();
          nodes[2] = current->release();
        }
        else if( height_helper( current->get()->left()->left() ) - height_helper( current->get()->left()->right() ) == -1 ) { //Left-Right
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
      else if( height_helper( current->get()->left() ) - height_helper( current->get()->right() ) == -2 )  //Right
      {
        if( height_helper( current->get()->right()->left() ) - height_helper( current->get()->right()->right() ) == -1 ) {  //Right-Right
          branches[0] = current->get()->left_.release();
          branches[1] = current->get()->right()->left_.release();
          branches[2] = current->get()->right()->right()->left_.release();
          branches[3] = current->get()->right()->right()->right_.release();
          nodes[2] = current->get()->right()->right_.release();
          nodes[1] = current->get()->right_.release();
          nodes[0] = current->release();
        }
        else if( height_helper( current->get()->right()->left() ) - height_helper( current->get()->right()->right() ) == 1 ) {  //Right-Left
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
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
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
    Value &value_ref_holder = cur->get()->value_;
    while( !prev_parents.empty() ){
      prev_parents.top()->get()->recalc_height();
      Node::balance( prev_parents.top() );
      prev_parents.pop();
    }

    return value_ref_holder;
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
  void print() { print( this->root() ); }
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
private:
  void print( Node* current, int indent_level=0 ) {
    if( current == NULL ) return;

    print( current->right(), indent_level + 1 );

    for(int k = 0; k < indent_level; k++)
      std::cout << "    ";
    std::cout << "{ " << current->key_ << "," << current->value_ << "," << current->height() << " }" << std::endl;

    print( current->left() , indent_level + 1 ); 
  }  
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
};

#endif

