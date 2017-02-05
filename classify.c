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

// Computes the probability ratio p(S|D)/p(~S|D). if result > 1 it is likely that the email is Spam
// parameters:
// in: emailname is the name of the .txt file holding the email to classify
// in: spamdata is the name of the .txt file holding prior observations of words that appear in spam plus their quantities
// in: honestdata is the equivalent for honest mails
// in: probforspam is again a .txt filename in which a single number is saved that reflects the current estimate of p(S). this will change over time.
// out: the probability ratio

double probability(char* emailname, struct node* spam, struct node* honest, double pspam){ 
  double PS, PnS, PDgS, PDgnS; // P[Spam], P[~Spam], P[Email|Spam], P[Email|~Spam]
  PS = pspam;
  PnS = 1.-PS;

  struct node* mail = mktreefromFile(emailname);
  int totalwrds = totalwords(spam);
  PDgS = 1.;
  inline void compprob(struct node* a){
    struct node** dummy;
    int q = search(spam, a->value, dummy);
    if (q > 0) PDgnS *= ((double) q) / ((double) totalwrds);
  }
  mapinorder(mail, &compprob);
  PDgS = PDgnS;
  
  totalwrds = totalwords(honest);
  PDgnS = 1.;
  mapinorder(honest, &compprob);

  return (PS/PnS) * (PDgS/PDgnS);
}

void classify(char* emailname){
  char* spam = "spmdt.txt";
  char* honest = "hnstdt.txt";
  char* pforspam = "pfors.txt";
  struct node* sp = mktreefromFile(spam);
  struct node* hn = mktreefromFile(honest);
  FILE* fp = fopen(pforspam, "r
  
}

void training(char* emailname, int spam){
  char* spam = "spmdt.txt";
  char* honest = "hnstdt.txt";
  char* pforspam = "pfors.txt";
  
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
