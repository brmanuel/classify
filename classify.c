//
// uses naive bayes to classify email into spam/honest.
//

#include <stdio.h>

#ifndef _AVL_H_AND_PARSER_H_
#define _AVL_H_AND_PARSER_H_

#include "avl.h"
#include "parser.h"
#define MAXWRD 25
#define MAXMAIL 1000

#endif


// helpers for storing contents (words) in a tree (tree specified in avl.h)

struct node* mknode(char* val, int quant){
  struct node* nd = (struct node*) calloc(1, sizeof(struct node));
  strncpy(nd->value, val, MAXWRD);
  nd->quant = quant;
  return nd;
}

struct node* mktree(char* vals[], int length){
  struct node* root = mknode(vals[0], 1);
  for (int i = 1; i < length; i++){
    insert(root, vals[i], 1);
  }
  return root;
}

struct node* mktreefromFile(char* filename){
  FILE* fp = fopen(filename, "r");
  if (! fp){
    printf("couldn't read %s in function mktreefromFile", filename);
    return (struct node *) NULL;
  }
  int length;
  int err;
  char wrd[MAXWRD];
  int quant;
  
  fscanf(fp, "%d \n", &length);
  err = fscanf(fp, "%s \t %d \n", wrd, &quant);
  if (length < 1 || err < 2){
    printf("file %s seems to be empty", filename);
    return (struct node*) NULL;
  }
  struct node* root = mknode(wrd, quant);
  for (int i = 0; i < length; i++){
    err = fscanf(fp, "%s \t %d \n", wrd, &quant);
    insert(root, wrd, quant);
  }
  fclose(fp);
  return root;
}

void writetree2file(char* filename, struct node* root){
  FILE* fp = fopen(filename, "w");
  if (! fp){
    printf("couldn't read file in writetree2file");
    return;
  }
  int size = sizeoftree(root);
  fprintf(fp, "%d \n", size);
  inline void writenode(struct node *node){
    fprintf(fp, "%s \t %d \n", node->value, node->quant);
  }
  mappreorder(root, &writenode);
  fclose(fp);
}

void classify(char* emailname, char* spamdata, char* honestdata, char* probforspam){
  double PS, PnS, PDgS, PDgnS; // P[Spam], P[~Spam], P[Email|Spam], P[Email|~Spam]
  if (!probforspam) PS = 0.5;
  FILE* pforspam = fopen(probforspam, "r");
  if (!pforspam) PS = 0.5;
  else if (fscanf(pforspam, "%d", &PS) < 0) PS = 0.5;
  PnS = 1.-PS;
  
  
}

int main(){
  int txtlength;
  char** words = parsetxt("exp.txt", &txtlength);
  struct node* tree = mktree(words, txtlength);
  writetree2file("exp2.txt", tree);
  struct node* tree2 = mktreefromFile("exp2.txt");
  mergetrees(tree, tree2);
  printtree(tree);
  return 0;
}
