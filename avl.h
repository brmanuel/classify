#include <stdlib.h>
#include <string.h>

#define MAXWRD 25

struct node {
  char value[MAXWRD];
  struct node *left;
  struct node *right;
  struct node *parent;
};

int gt(char* a, char* b){
  if (a[0] == '\0' && b[0] == '\0') return 0;
  else if (a[0] < b[0]) return -1;
  else if (a[0] > b[0]) return 1;
  else return gt(a+1, b+1);  
}

int search(struct node* root, char* value, struct node** res){
  if (gt(root->value, value)== 1){
    if (root->left) return search(root->left, value, res);
    else {
      *res = root;
      return 0;
    }
  }
  else if (gt(root->value, value) == -1) {
    if (root->right) return search(root->right, value, res);
    else {
	*res = root;
	return 0;
    }
  }
  else if (gt(root->value, value) == 0){
    *res = root;
    return 1;
  }
}

void insert(struct node* root, char* value){
  struct node* par = (struct node*) calloc(1, sizeof(struct node));
  struct node* val = (struct node*) calloc(1, sizeof(struct node));
  strncpy(val->value, value, MAXWRD);
  if (!search(root, value, &par)){
    if (gt(par->value, val->value) == 1) par->left = val;
    else par->right = val;
    val->parent = par;
  }
}

void mapinorder(struct node* root, void (*func)(struct node*)){
  if (root->left) mapinorder(root->left, func);
  func(root);
  if (root->right) mapinorder(root->right, func);
}

void printnode(struct node* nd){
  printf("%s ", nd->value);
}

void printtree(struct node* root){
  mapinorder(root, &printnode);
}
