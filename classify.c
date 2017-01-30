#include <stdio.h>

#ifndef _AVL_H_AND_PARSER_H_
#define _AVL_H_AND_PARSER_H_

#include "avl.h"
#include "parser.h"
#define MAXWRD 25
#define MAXMAIL 1000

#endif

struct node* mknode(char* val){
  struct node* nd = (struct node*) calloc(1, sizeof(struct node));
  strncpy(nd->value, val, MAXWRD);
  return nd;
}

struct node* mktree(char* vals[], int length){
  struct node* root = mknode(vals[0]);
  for (int i = 1; i < length; i++){
    insert(root, vals[i]);
  }
  return root;
}

int main(){
  int txtlength;
  char** words = parsetxt("exp.txt", &txtlength);
  for (int i = 0; i < txtlength; i++){
    printf("%s ", words[i]);
  }
}
