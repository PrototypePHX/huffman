#include "node.hpp"


node::node(node *left_, node *right_, int weight_): left(left_), right(right_)
{
   if (weight_ == 0)
      weight = left->weight + right->weight;
   else
      weight = weight_;
}

node::~node()
{
   delete left;
   delete right;
}


nodetype branch::type() const
{
   return nodetype::BRANCH;
}

nodetype leaf::type() const
{
   return nodetype::LEAF;
}

bool node_comp::operator()(const node* N1, const node* N2) {
   return N1->weight > N2->weight;
}
