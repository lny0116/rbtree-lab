#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

//algorithm book reference

void left_rotate(rbtree *t, node_t *x){
  // set y
  node_t *y = x->right;
  // y's left sub tree change x's right sub tree
  x->right = y->left;
  // y's left child point
  if (y->left != t->nil){
    y->left->parent = x;
  }
  // y change (x's parent)'s child [y point]
  y->parent = x->parent;
  // x's parent point, if x == root case
  if (x->parent == t->nil){
    t->root = y;
  // if x == left child case
  } else if(x==x->parent->left){
    x->parent->left = y;
  // if x == right child case
  } else{
    x->parent->right = y;
  }
  // x change y's child: x is y's left child
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x){
  node_t *y = x->left;
  x->left = y->right;
  if(y->right != t->nil){
    y->right->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil){
    t->root = y;
  } else if(x==x->parent->left){
    x->parent->left = y;
  } else{
    x->parent->right = y;
  }
  y->right=x;
  x->parent=y;
}

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *n = (node_t *)calloc(1, sizeof(node_t));
  n->color = RBTREE_BLACK;
  p->nil = n;
  p->root = n;
  p->root->parent = n;
  
  return p;
}

// == postorder
void delete_node(rbtree *t ,node_t *x){
  if (x == t->nil){
    return;
  }
  delete_node(t,x->left);
  delete_node(t,x->right);
  x->left = x->right = x->parent = NULL;
  free(x);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_node(t,t->root);
  free(t->nil);
  free(t);
}

void rbtree_insert_fixup(rbtree *t, node_t *z){
  node_t *uncle;

  while (z->parent->color == RBTREE_RED){
    if (z->parent == z->parent->parent->left){
      uncle = z->parent->parent->right;
      if (uncle->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->right){
          z = z->parent;
          left_rotate(t,z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t,z->parent->parent);
      }
    }
    
    else{ //66_line: right switching left
      uncle = z->parent->parent->left;
      if (uncle->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->left) {
          z = z->parent;
          right_rotate(t,z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t,z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  // new_node create
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  node_t *parent_node = t->nil;
  node_t *child_node = t->root;
  new_node->color = RBTREE_RED; //start red
  // new insert node always BST's leaf node. so, left and right child = null
  new_node->left = t->nil;
  new_node->right = t->nil;
  new_node->key=key;
  
  while (child_node != t->nil){
    parent_node = child_node;
    if (new_node->key<child_node->key){
      child_node=child_node->left;
    } else{
      child_node = child_node->right;
    }
  }

  new_node->parent = parent_node;

  if (parent_node == t->nil){
    t->root = new_node;
  } else if (new_node->key < parent_node->key){
    parent_node->left = new_node;
  } else {
    parent_node->right = new_node;
  }

  rbtree_insert_fixup(t, new_node);

  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  // x == node
  node_t *x = t->root;
  
  // while until found
  while (x != t->nil){
    if (x->key < key){
      x = x->right;
    } else if (key < x->key){
      x = x->left;
    } else {
      return x;
    }
  }
  return NULL;
}

node_t *minimum(rbtree *t, node_t *x){
  node_t *temp = x;

  while(temp->left != t->nil){
    temp = temp->left;
  }
  return temp;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  // check left
  node_t *x = t->root;

  // error reason => not consider null case 

  // if (x == NULL){
  //   return t->root;
  // }

  while (x->left != t->nil){
    x = x->left;
  }
  return x;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  // check right
  node_t *x = t->root;

  // if (x==NULL){
  //   return t->root;
  // }

  while(x->right != t->nil){
    x = x->right;
  }
  return x;
}

void transplant(rbtree* t, node_t* u, node_t* v) {
  if (u->parent == t->nil){
    t->root = v;
  } else if (u == u->parent->left){
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
  // if (v != NULL){ // not use NIL, so append 'v is not null case'
  v->parent = u->parent;
  // }
}

// if not use null, delete successor
// node_t *successor(rbtree *t, node_t *x){
//   node_t *y;

//   if (x->right != NULL){
//     while (x->left != NULL){
//       x = x->left;
//     }
//     return x;
//   }
//   y = x->parent;
//   while (y != NULL && x == y->right){
//     x = y;
//     y = y->parent;
//   }
//   return y;
// }

// node_t *temp(node_t *x){
//   node_t *n = (node_t *)malloc(sizeof(node_t));
//   n->color = RBTREE_BLACK;
//   n->left = NULL;
//   n->right = NULL;
//   n->parent = NULL;

//   if (x == NULL){
//     return n;
//   } else {
//     return NULL;
//   }
// }


void delete_fixup(rbtree *t, node_t *x) {
  node_t *w;
  while (x != t->root && x->color == RBTREE_BLACK){
    // first
    if (x == x->parent->left){
      w = x->parent->right;
      // case1 start
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
    //   w->left = temp(w->left); //여기 이 부분에서도 temp가 쓰여야하는데 일단 추가..
    //   w->right = temp(w->right);

      // case2 start
      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
        // case3 start
      } 
      else {
        if (w->right->color == RBTREE_BLACK){
        w->left->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        right_rotate(t,w);
        w = x->parent->right;
        }
        // case4 start
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t,x->parent);
        x=t->root;
      }
    }
    
    else { // second == switching left & right
      w = x->parent->left;
      if(w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t,x->parent);
        w = x->parent->left;
      }
      // w->left = temp(w->left);
      // w->right = temp(w->right);

      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if (w->left->color == RBTREE_BLACK){
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t,w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  node_t *y = z;
  color_t y_original_color = y->color;
  // node_t *y_original_color = y->color;
  node_t *x = t->nil;

  // y=z;
  // y_original_color = y->color;
  if (z->left == t->nil){
    x = z->right;
    transplant(t,z,z->right);
  } else if (z->right == t->nil){
    x = z->left;
    transplant(t,z,z->left);
  } else{
    y = minimum(t,z->right); // min error continue. so change successor // not use null
    y_original_color = y->color;
    x = y->right;
    if (y->parent == z){
      x->parent = y;
    } else{
      transplant(t,y,y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    transplant(t,z,y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if (y_original_color == RBTREE_BLACK){
    delete_fixup(t,x);
  }
  free(z);

  return 0;
}

int inorder(node_t *nil, node_t *x, key_t *arr, int i){
  if (x != nil){
    i = inorder(nil, x->left,arr,i);
    arr[i] = x->key;
    i++;
    i = inorder(nil, x->right,arr,i);
  }
  return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  inorder(t->nil,t->root,arr,0);
  return 0;
}

// void inorderP(rbtree *t, node_t *cur) {
//   if (cur == t->nil)
//     return;

//   inorderP(t, cur->left);
//   printf("%d (%d) / ", cur->key, cur->color);
//   inorderP(t, cur->right);
// }

// void main() {
//   const key_t arr[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
//   const size_t n = sizeof(arr) / sizeof(arr[0]);
//   rbtree *t = new_rbtree();
//   puts("asd");

//   for (int i = 0; i < n; i++) {
//     rbtree_insert(t, arr[i]);
//     printf("%d insert\n", arr[i]);
//   }

//   inorderP(t, t->root);
//   puts("");

// }


// Passed all tests! file~~ YEAH!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
