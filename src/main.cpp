#include "avl_tree.h"


int main(const int argc, const char** argv) {
  using std::endl;
  using std::cout;

  srand(time(NULL));

  if(argc != 2 && argc != 3) { std::cerr << "./main TREE_SIZE NUMBER_RANGE" << std::endl; exit(1); }
  const int TREE_SIZE = std::stoi(*(argv + 1));
  const int NUMBER_RANGE = argc==3 ? std::stoi(*(argv + 2)) + 1 : 100 + 1;

  AVLTree<int> test;
  for( int k = 0; k < TREE_SIZE; k++ )
  {
    test[ rand() % NUMBER_RANGE ]; 
  }
  
  test.print();

  return 0;
}
