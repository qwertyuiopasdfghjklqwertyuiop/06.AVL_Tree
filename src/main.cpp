#include <iostream>
#include "avl_tree.h"


int main() {
  using std::endl;
  using std::cout;

  cout << "======================== TESTING LEFT-LEFT BALANCE ========================\n\n\n";
  AVLTree<double,std::string> left_left;
  left_left[5] = "5";
  left_left[4] = "4";
  left_left[3] = "3";

  left_left[2.5] = "A";
  left_left[3.5] = "B";
  left_left[4.5] = "C";
  left_left[5.5] = "D";
  left_left.print();

//  AVLTree<double,std::string>::Node::balance(left_left.rootTest());
//  cout << endl << endl << endl << endl << endl; 
//  left_left.print();
  cout << endl << endl << endl << endl << endl; 

  cout << "======================== TESTING LEFT-RIGHT BALANCE ========================\n\n\n";
  AVLTree<double,std::string> left_right;
  left_right[5] = "5";
  left_right[3] = "3";
  left_right[4] = "4";

  left_right[2.5] = "A";
  left_right[3.5] = "B";
  left_right[4.5] = "C";
  left_right[5.5] = "D";
  left_right.print();

//  AVLTree<double,std::string>::Node::balance(left_right.rootTest());
//  cout << endl << endl << endl << endl << endl; 
//  left_right.print();
  cout << endl << endl << endl << endl << endl; 

  cout << "======================== TESTING RIGHT-RIGHT BALANCE ========================\n\n\n";
  AVLTree<double,std::string> right_right;
  right_right[3] = "3";
  right_right[4] = "4";
  right_right[5] = "5";

  right_right[2.5] = "A";
  right_right[3.5] = "B";
  right_right[4.5] = "C";
  right_right[5.5] = "D";
  right_right.print();

//  AVLTree<double,std::string>::Node::balance(right_right.rootTest());
//  cout << endl << endl << endl << endl << endl; 
//  right_right.print();
  cout << endl << endl << endl << endl << endl; 

  cout << "======================== TESTING RIGHT-LEFT BALANCE ========================\n\n\n";
  AVLTree<double,std::string> right_left;
  right_left[3] = "3";
  right_left[5] = "5";
  right_left[4] = "4";

  right_left[2.5] = "A";
  right_left[3.5] = "B";
  right_left[4.5] = "C";
  right_left[5.5] = "D";
  right_left.print();

//  AVLTree<double,std::string>::Node::balance(right_left.rootTest());
//  cout << endl << endl << endl << endl << endl; 
//  right_left.print(); 
  cout << endl << endl << endl << endl << endl; 




}
