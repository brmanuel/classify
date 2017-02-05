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

// names of the files containing raw data:
// spam holds words and quantities of words that occurred in spam mail
// honest holds words and quantities of words that occurred in honest mail
// pforspam holds two doubles. the first one holds #total mails, the second one #spam mails -> P[S] = #spam mails / #total mails.
static char* spamdata = "spmdt.txt";
static char* honestdata = "hnstdt.txt";
static char* pforspamdata = "pfors.txt";


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

struct node* mktreefromMail(char* filename){
  int txtlength;
  char** words = parsetxt(filename, &txtlength);
  struct node* tree = mktree(words, txtlength);
  return tree;
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
// in: mail is a tree  holding info about the email to classify
// in: spamdata is a tree  holding info about the prior observations of words that appear in spam plus their quantities
// in: honestdata is the equivalent for honest mails
// in: pspam estimates p(S). this will change over time.
// out: the probability ratio

double probability(struct node* mail, struct node* spam, struct node* honest, double pspam){ 
  double PS, PnS, PDgS, PDgnS; // P[Spam], P[~Spam], P[Email|Spam], P[Email|~Spam]
  PS = pspam;
  PnS = 1.-PS;

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

void updateDatabase(double dec, int totalmail, int spammail, struct node* spam, struct node* honest, struct node* mail){
  if (dec > 1){
    spammail += 1;
    mergetrees(mail, spam);
    writetree2file(spamdata, mail);
  } else {
    mergetrees(mail, honest);
    writetree2file(honestdata, mail);
  }
  totalmail += 1;
  FILE* p = fopen(pforspamdata, "w");
  if (!p){
    printf("couldn't write back to file pspam.txt");
    return;
  }
  fprintf(p, "%d %d", totalmail, spammail);
  fclose(p);
}

int classify(char* emailname){
  struct node* mail = mktreefromMail(emailname);
  struct node* sp = mktreefromFile(spamdata);
  if (!sp) {
    printf("Spam training required. Spam datasheet empty");
    return -1;
  }
  struct node* hn = mktreefromFile(honestdata);
  if (!hn) {
    printf("Honest training required. Honest datasheet empty");
    return -1;
  }
  double ps;
  int totalmails, spammails;
  FILE* fp = fopen(pforspamdata, "r");
  if (!fp) ps = 0.5;
  else if (fscanf(fp, "%d %d", &totalmails, &spammails) < 2) ps = 0.5;
  else ps = ((double) spammails)/((double)totalmails);

  double decision = probability(mail, sp, hn, ps);

  updateDatabase(decision, totalmails, spammails, sp, hn, mail);

  if (decision > 1.) return 1;
  return 0;  
}

void training(char* emailname, int spam){
  FILE* pforspam = fopen(pforspamdata, "w+");
  int totalmail, spammail;
  if (!fscanf(pforspam, "%d %d", &totalmail, &spammail)){
    totalmail = 0;
    spammail = 0;
  }
  struct node* mail = mktreefromMail(emailname);
  if (spam) {
    struct node* sp = mktreefromFile(spamdata);
    if (sp) {
      mergetrees(mail, sp);
    }
    writetree2file(spamdata, mail);
    spammail += 1.;
  } else {
    struct node* hn = mktreefromFile(honestdata);
    if (hn) {
      mergetrees(mail, hn);
    }
    writetree2file(honestdata, mail);
  }
  totalmail += 1;
  fprintf(pforspam, "%d %d", totalmail, spammail);  
} 

int main(){
  /*int txtlength;
  char** words = parsetxt("exp.txt", &txtlength);
  struct node* tree = mktree(words, txtlength);
  writetree2file("exp2.txt", tree);
  struct node* tree2 = mktreefromFile("exp2.txt");
  mergetrees(tree, tree2);
  printtree(tree);*/
  training("honest1.txt", 0);
  return 0;
}
