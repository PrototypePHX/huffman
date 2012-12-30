#ifndef NODE_HPP
#define NODE_HPP

#include <functional>
#include <typeinfo>
#include <iostream>

enum struct nodetype: bool
{
   BRANCH,
   LEAF
};

struct node
{
      int weight;
      node *left;
      node *right;

      virtual nodetype type() const = 0;
      node(node *left_, node *right_, int weight_ = 0);
      virtual ~node() = 0;

   private:
      node(const node *n);
      node* operator=(const node *rhs);
};

struct branch: public node
{
      virtual nodetype type() const;

      branch(node *left_, node *right_): node(left_, right_) {}

};

struct leaf: public node
{
      char payload;

      virtual nodetype type() const;

      leaf(char payload_, int weight_): node(nullptr, nullptr, weight_), payload(payload_) { }
};



struct node_comp: public std::binary_function<node*, node*, bool>
{
      bool operator()(const node* N1, const node* N2);
};

#endif // NODE_HPP
