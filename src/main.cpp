#include <iostream>
#include "avl_tree.h"


int main() {
  using std::endl;
  using std::cout;


  AVLTree<int,std::string> test;
  test[1] = "one"; 
  test[0] = "zero";
  test[3] = "three";
  test[-1] = "neg one";
  test.print();







}
