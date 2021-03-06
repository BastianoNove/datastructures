#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
const Q_SIZE = 100;

void test_one() {
  avl_node_t *node = create_node(10);
  avl_node_t **root = &node;
  int vals[9] = {9,8,7,6,5,4,3,2,1};
  build_tree(root, vals, 9);
  assert(check_balance(*root, 10));
  assert(check_bst(*root, 10));
}

void test_two() {
  avl_node_t *node = create_node(1);
  avl_node_t **root = &node;
  int vals[9] = {2,3,4,5,6,7,8,9,10};
  build_tree(root, vals, 9);
  assert(check_balance(*root, 10));
  assert(check_bst(*root, 10));
}

void test_three() {
  avl_node_t *node = create_node(13);
  avl_node_t **root = &node;
  int vals[7] = {5,2,3,11,19,1,9};
  build_tree(root, vals, 7);
  assert(check_balance(*root, 8));
  assert(check_bst(*root, 8));
}

void test_four() {
  avl_node_t *node = create_node(20);
  avl_node_t **root = &node;
  int vals[6] = {8,22,4,12,10,14};
  build_tree(root, vals, 6);
  delete(root, 10);
  delete(root, 20);
  delete(root, 12);
  assert(check_balance(*root, 4));
  assert(check_bst(*root, 4));
}

void build_tree(avl_node_t** root, int vals[], int vals_size) {
  avl_node_t* cur;
  int i = 0, k;
  while(i < vals_size) {
    cur = create_node(vals[i++]);
    insert(root, cur);
  }
}

bool check_balance(avl_node_t* root, int n) {
  avl_node_t* cur;
  avl_node_t* queue[n];
  int i = 1, j = 0, hl, hr;
  memset(queue, 0, n);
  queue[0] = root;
  while(j < n) {
    cur = queue[j];
    hl = cur->left ? cur->left->height : 0;
    hr = cur->right ? cur->right->height : 0;
    if (abs(hr-hl) > 2) {
      return false;
    }
    if(cur->left) { 
      queue[i++] = cur->left;
    }
    if(cur->right) {
      queue[i++] = cur->right;
    }
    j++;
  }
  return true;
}

bool check_bst(avl_node_t* root, int n) {
  avl_node_t* cur;
  avl_node_t* queue[n];
  int i = 1, j = 0;
  memset(queue, 0, n);
  queue[0] = root;
  while(j < n) {
    cur = queue[j];
    if(cur->left){
     queue[i++] = cur->left;
     if (cur->left->key > cur->key) {
      return false;
     }
    }

    if (cur->right){
      queue[i++] = cur->right;
      if (cur->right->key < cur->key) {
        return false;
      }
    }
    j++;
  }
  return true;
}

int main() {
  test_one();
  test_two();
  test_three();
  test_four();
  printf("tests pass\n");
  return 0;
}

avl_node_t* search(avl_node_t* root, int key) {
  avl_node_t* cur = root;
  while(cur && cur->key != key) {
    if (key < cur->key) {
      cur = cur->left;
    }
    else {
      cur = cur->right;
    }
  }
  return cur;
}

void insert(avl_node_t** root, avl_node_t* node) {
  avl_node_t* bp;
  avl_node_t* cur;
  int hl, hr; //height left, height right;
  int balance;
  bp = cur = *root;

  if (!cur) {
    *root = node;
    (*root)->parent = NULL;
    return;
  }

  while(cur) {
    bp = cur;
    hr = cur->right ? (cur->right)->height : 0;
    hl = cur->left ? (cur->left)->height : 0;

    if (node->key < cur->key) {
      cur = cur->left;
      if (hl >= hr) {
        bp->height += hl + 1; 
      }
    }
    else {
      cur = cur->right;
      if (hr >= hl) {
        bp->height += hr + 1;
      }
    }
  }
  node->parent = bp;
  if (node->key < bp->key) { 
    bp->left = node;
  }
  else {
    bp->right = node;
  }

  balance_tree(root, bp, node);
}

void balance_tree(avl_node_t** root, avl_node_t* bp, avl_node_t* node) {
  int balance;
  bp = bp->parent;
  node = node->parent;
  while(bp) {
    balance = bp->right ? (bp->right)->height+1 : 0;
    balance -= bp->left ? (bp->left)->height+1 : 0;

    if (balance == 2 ) {
      balance = node->right ? (node->right)->height+1 : 0;
      balance -= node->left ? (node->left)->height+1 : 0;
      if (balance < 0) {
        // LR or Double left rotation
        right_rotate(node);
        node = bp->right;
      }
      left_rotate(bp);
      bp = bp->parent;
      node = bp->left;
    }
    else if (balance == -2) {
      balance = node->right ? (node->right)->height+1 : 0;
      balance -= node->left ? (node->left)->height+1 : 0;
      
      if (balance > 0) {
        // RL or Double right rotation
        left_rotate(node);
        node = bp->left;
      }
      right_rotate(bp);
      bp = bp->parent;
      node = node->right;
    }
    node = node->parent;
    bp = bp->parent;
  }

  *root = node;
}

avl_node_t* delete(avl_node_t** root, int key) {
  avl_node_t* y;
  avl_node_t* z;
  int temp_height, temp_key;
  avl_node_t* x = search(*root, key);
  if (x) {
    if (x->height == 0) {
       y = x->parent;
       if (y == *root) {
         *root = NULL;
       }
       if (y && x == y->left) {
         y->left = NULL;
       }
       else if(y && x == y->right) {
         y->right = NULL;
       }
       balance_tree(root, y->parent, y);
       return x;
    }
    else if (x->height == 1 && ((x->left && !x->right) || (!x->left && x->right))) {
      y = x->left ? x->left : x->right;
      temp_height  = y->height;
      temp_key = y->key;
      y->key = x->key;
      y->height = x->height;
      y->left = y->right = NULL;
      x->key = temp_key;
      x->height = 0;
      if (y == x->left) {
        x->left = NULL;
      }
      else {
        x->right = NULL;
      }
      balance_tree(root, x->parent, x);
      return  y;
    }
    else {
      z =  successor(x);
      temp_height = x->height;
      temp_key = x->key;
      x->key = z->key;
      x->height = z->height;
      z->key = temp_key;
      z->height= temp_height;
      if (z->parent->left == z){
        z->parent->left = NULL;
      }
      else {
        z->parent->right = NULL;
      }
      balance_tree(root, z->parent->parent, z->parent);
      return z;
    }
  }
  return NULL;
}

void left_rotate(avl_node_t* x) {
  int hl, hr;
  avl_node_t* y = x->right;
  x->right = y->left;
  if (x->right) {
    x->right->parent = x; 
  }
  y->left = x;
  y->parent = x->parent;
  x->parent = y;
  if (y->parent) {
    if (y->parent->right == x) {
      y->parent->right = y;
    }
    else {
      y->parent->left = y;
    }
  }
  //adjust heights of rotated trees
  hl = x->left ? x->left->height + 1 : 0;
  hr = x->right ? x->right->height + 1 : 0;
  x->height = hr ;
  if (hl > hr) {
    x->height = hl;
  }
  y->height = y->right ? y->right->height + 1 : 0;
  if (x->height > y->height){
    y->height = x->height + 1;
  }
  //adjust heights from y to top
  y = y->parent;
  while(y) {
    hl = y->left ? y->left->height : 0;
    hr = y->right ? y->right->height : 0;
    if (hr > hl) {
      y->height = hr + 1;
    }
    else {
      y->height = hl + 1;
    }
    y = y->parent;
  }
}

void right_rotate(avl_node_t* y) {
  int hr, hl;
  avl_node_t* x = y->left;
  y->left = x->right;
  if (y->left) {
    y->left->parent = y;
  }
  x->right = y;
  x->parent = y->parent;
  y->parent = x;
  if (x->parent) {
    if (x->parent->right == y) {
      x->parent->right = x;
    }
    else {
      x->parent->left = x;
    }
  }
  //adjust heights of rotated trees
  hl = y->left ? y->left->height + 1 : 0;
  hr = y->right ? y->right->height + 1 : 0;
  y->height = hr;
  if (hl > hr) {
    y->height = hl;
  }
  x->height = x->left ? x->left->height + 1 : 0;
  if (y->height > x->height) {
    x->height = y->height + 1;
  }
  //adjust heights from x to top
  y = x;
  x = x->parent;
  while(x) {
    hl = x->left ? x->left->height : 0;
    hr = x->right ? x->right->height : 0;
    if (hr > hl) {
      x->height = hr +  1;
    }
    else {
      x->height = hl + 1;
    }
    x = x->parent;
  }
}

avl_node_t* predecessor(avl_node_t* x) {
 avl_node_t* y;
 if (x->left) {
   return tree_maximum(x->left);
 }
 y = x->parent;
 while(y && x == y->left) {
   x = y;
   y = y->parent;
 }
 return y;
}

avl_node_t* successor(avl_node_t* x) {
  avl_node_t* y;
  if (x->right) {
    return tree_minimum(x->right);
  }
  y = x->parent;
  while (y && x == y->right) {
    x = y;
    y = y->parent;
  }
  return y;
}

avl_node_t* tree_minimum(avl_node_t* x) {
  while (x->left) {
    x = x->left;
  }
  return x;
}

avl_node_t* tree_maximum(avl_node_t* x) {
  while (x->right) {
    x = x->right;
  }
  return x;
}

void print_tree(avl_node_t* root) { 
  avl_node_t* queue[Q_SIZE];
  memset(queue, 0, Q_SIZE);
  avl_node_t* cur = root;
  int i = 1, j = 0 ;
  int left, right;
  queue[0] = root;
  while(queue[j]) {
    cur = queue[j];
    printf("%d\n", cur->key);
    if (cur->left) {
      printf("left %d\n", cur->left->key);
      queue[i++] = cur->left;
    }
    else {
      printf("left empty\n");
    }
    if (cur->right) {
      printf("right %d\n", cur->right->key);
      queue[i++] = cur->right;
    }
    else {
      printf("right empty\n");
    }
    j++;
  }
}

avl_node_t* create_node(int key) {
  avl_node_t* node = malloc(sizeof(*node));
  node->key = key; 
  node->left = NULL;
  node->right = NULL;
  return node;
}
