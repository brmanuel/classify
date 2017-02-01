#include <stdio.h>
#include <ctype.h>

void analysetxt(char *filename, int* a, int* b){
  FILE *fp = fopen(filename, "r");
  if (!fp){
    printf("can't read file in analysetxt");
    return;
  }
  *a = 0;
  *b = 0;
  char c;
  int wrdlngth;
  while (c = fgetc(fp)){
    if (c == EOF) break;
    else if (isspace(c)){
      if (wrdlngth > *b){
	*b = wrdlngth;
      }
      wrdlngth = 0;
      (*a)++;
    } else wrdlngth++;
  }
  fclose(fp);
}

char** parsetxt(char* filename, int *wordcount){
  int maxwrdlength;
  analysetxt(filename, wordcount, &maxwrdlength);
  char** words = (char **) malloc(*wordcount*sizeof(char *));
  for (int i = 0; i < *wordcount; i++) words[i] = (char *) malloc(maxwrdlength);

  FILE* file = fopen(filename, "r");
  if (! file){
    printf("can't read file in parsetxt");
    return words;
  }
  for (int i = 0; i < *wordcount; i++){
    int j = 0;
    char wrd[maxwrdlength];
    char a = fgetc(file);
    if (feof(file)) break;
    while(! isspace(a) && j < maxwrdlength){
      if (feof(file)) goto done;
      if (! ispunct(a)) {
	wrd[j] = a;
	j++;
      }
      a = fgetc(file);
    }
    if (wrd != "") strncpy(words[i], wrd, j);
  }
 done:
  fclose(file);
  return words;
}
