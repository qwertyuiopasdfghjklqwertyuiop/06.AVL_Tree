#ifndef _AVL_TREE_H
#define _AVL_TREE_H

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
  private:
    void recalc_height() {
      this->height_ = 1 + std::max( this->left()->height() , this->right()->height() );
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
    while( !prev_parents.empty() ){
      prev_parents.top()->get()->recalc_height();
      prev_parents.pop();
    }
    return (*cur)->value_;
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
    std::cout << "{ " << current->key_ <<  "," << current->value_ << "," << current->height() << " }" << std::endl;
    print( current->left() , indent_level + 1 ); 
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  void balance(std::unique_ptr<Node>* current) {
    Node* nodes[3];
    Node* branches[4];
    if( current->get()->left()->height() - current->get()->right()->height() == 2 )   //Left
    {
      if( current->get()->left()->left()->height() - current->get()->left()->right()->height() == 1 ) {   //Left-Left
      }
      else if( current->get()->left()->left()->height() - current->get()->left()->right()->height() == -1 ) { //Left-Right
      }
      else { std::cerr << "WTF!?: Left case but not left-left or left-right." << std::endl; exit(1);
    }
    else if( current->get()->left()->height() - current->get()->right()->height() == -2 )  //Right
    {
      if( current->get()->right()->left()->height() - current->get()->right()->right()->height() == -1 ) {  //Right-Right
      }
      else if( current->get()->right()->left()->height() - current->get()->right()->right()->height() == 1 ) {  //Right-Left
      }
      else { std::cerr << "WTF!?: Right case but not right-right or right-left." << std::endl; exit(2);
    }
    else return; // doesn't need to be balanced

    // Reconnect
    nodes[0]->left_.reset( branches[0] );
    nodes[0]->left_.reset( branches[1] );
    nodes[2]->left_.reset( branches[2] );
    nodes[2]->left_.reset( branches[3] );
    nodes[1]->left_.reset( nodes[0] );
    nodes[1]->left_.reset( nodes[2] );
    current->reset( nodes[1] );
    //Recalculate from bottom to top
    nodes[0]->recalc_height();
    nodes[2]->recalc_height();
    nodes[1]->recalc_height();

    // TODO: Add Documentation for wtf is going on here
  }
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

#endif
