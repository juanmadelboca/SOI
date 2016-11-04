#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

int readCommand(char* argv[][100], char* command,int argc[]);

int main (void)
{
	char *argv[2][100]={NULL};
  int argc[2];
  char command[]= "date -a -b grep dia";

  readCommand(argv,command,argc);

  for (int i=0; i<2; i++)
    for (int j = 0; j < argc[i]; j++){
      printf("fila %i : %s\n", i,argv[i][j]);
    } 




  return 0;
}

int readCommand(char* argv[][100], char* command,int argc[]){
  int words=0;
  int row=0;
  argv[row][words] = strtok(command, " \n");
  if (argv[row][words]!=NULL)
  for(words = 1; words < 20; words++){
    argv[row][words] = strtok(NULL , " \n");
    if (argv[row][words] == NULL)
      break;
    else if (!strcmp(argv[row][words],"|")){
      if (row){
        printf("Error: No se acepta mas de 1 pipe");
        exit(1);
      }
      row=1;
      argc[0]= words;
      words=-1;
    } 
  }
  if (row)
    argc[1]=words;
  else 
    argc[0]=words;

  return row;
}