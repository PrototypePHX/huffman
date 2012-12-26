#include "tree.hpp"

tree::tree(const std::map<char, int> histo)
{
   for (auto it = histo.begin(); it != histo.end(); ++it)
      leaves.push(new leaf(it->first, it->second));
   create_tree();
}

tree::~tree()
{
   delete root;
}


void tree::create_tree()
{
   node *right = leaves.top();   leaves.pop();
   node *left = leaves.top();    leaves.pop();
   branch *parent = new branch(left, right);
   leaves.push(parent);

   if (leaves.size() < 2)
      root = parent;
   else
      create_tree();
}

node* tree::rootnode() const
{
   return root;
}
