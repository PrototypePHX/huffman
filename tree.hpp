#ifndef TREE_HPP
#define TREE_HPP

#include <queue>
#include <map>

#include "node.hpp"

class tree
{
   public:
      tree(const std::map<char, int> histo);
      ~tree();
      node* rootnode() const;

   private:
      node *root;
      std::priority_queue<node*, std::vector<node*>, node_comp> leaves;
      void create_tree();

      tree(const tree &t);
      tree& operator=(const tree &rhs);
};

#endif // TREE_HPP
