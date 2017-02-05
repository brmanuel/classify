#include <stdlib.h>
#include <string.h>

#define MAXWRD 25

struct node {
  char value[MAXWRD];
  int quant;
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
    return root->quant;
  }
}

void insert(struct node* root, char* value, int quant){
  struct node* par = (struct node*) calloc(1, sizeof(struct node));
  if (!search(root, value, &par)){
    struct node* val = (struct node*) calloc(1, sizeof(struct node));
    strncpy(val->value, value, MAXWRD);
    val->quant = quant;
    if (gt(par->value, val->value) == 1) par->left = val;
    else par->right = val;
    val->parent = par;
  } else par->quant += quant;
}

void mapinorder(struct node* root, void (*func)(struct node*)){
  if (root->left) mapinorder(root->left, func);
  func(root);
  if (root->right) mapinorder(root->right, func);
}

void mappreorder(struct node* root, void (*func)(struct node*)){
  func(root);
  if (root->left) mapinorder(root->left, func);
  if (root->right) mapinorder(root->right, func);
}

void mergetrees(struct node* frst, struct node* scnd){ // resulting tree will be stored in frst
  inline void insertinother(struct node* a){
    insert(frst, a->value, a->quant);
  }
  mapinorder(scnd, &insertinother);
}

int sizeoftree(struct node* root){
  int counter = 0;
  inline void countnode(struct node* a){
    counter++;
  }
  mapinorder(root, &countnode);
  return counter;
}

int totalwords(struct node* root){
  int counter = 0;
  inline void countwords(struct node* a){
    counter += a->quant;
  }
  mapinorder(root, &countwords);
  return counter;
}
  

void printnode(struct node* nd){
  printf("%s \t %d \n", nd->value, nd->quant);
}

void printtree(struct node* root){
  mapinorder(root, &printnode);
}
