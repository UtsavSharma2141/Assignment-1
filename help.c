/*
   File: help.c
   Author: Utsav Sharma	
   */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h> 
#include <readline/readline.h>
#include <readline/history.h> 
#include <stdbool.h> 
#include <signal.h> 
#include <string.h> 
#include <time.h> 
#include <sys/socket.h>
#include <linux/un.h> 

void ctrlCHandler(int signum)
{
  fprintf(stderr,"Command server terminated using <ctrl>C\n");  
  exit(1); 
  } 

//A routine to make sure strcat doesn't cause a buffer overflow by accident 
char *safecat(char * dest, char *src, int maxSize) 
{
  return strncat(dest,src,maxSize-strlen(dest)-1);  
  } 

char *helpCommand(char *cmd, char **tokens);

//ls command function
char *lsCommand(char *cmd, char **tokens)
{

}
  
//who command function
char *whoCommand()
{
   
}

//struct holding command and corresponding function names
struct CMDSTRUCT
{
   char *cmd;
   char *(*method)();
   char *help;
}
cmdStruct[]={{"help",helpCommand,"help: It gives more available information about the command\n"},
               {"ls",lsCommand,"ls: allows you to view a list of the files and folders in a given directory\n"},
                {"who",whoCommand,"who: show information about users who are currently logged in\n"},
               {NULL,NULL,NULL}};

//help command function
char *helpCommand(char *cmd, char **tokens)
{
int i = 0;
int j =0 ;

   static char r[1000];
   char t[100];
   r[0]=0;
   t[0]=0;
 
   if (tokens[0]==NULL)
   return ("\nYou can get more information about the following command:\n who\n ls \n help ");
   
   else
   while(tokens[i])
   {

      while(cmdStruct[j].cmd)
      {
         if(strcasecmp(tokens[i],cmdStruct[j].cmd)==0)
         {
            sprintf(t,"%s",cmdStruct[j].help); 
            strcat(r,t);
         }
	j++;
      }
	i++;
   }
   return r;
}   
  
char *interpret(char *cmdline)
{
  char **tokens;
  char *cmd;
  int  i; 
  char *result; 

  tokens=history_tokenize(cmdline);   //Split cmdline into individual words.  
  if(!tokens) return "no response needed"; 
  cmd=tokens[0]; 


  for(i=0;cmdStruct[i].cmd;i++)
    if(strcasecmp(cmd,cmdStruct[i].cmd)==0) return (cmdStruct[i].method)(cmd,&tokens[1],cmdline);    
  
 
  return "command not found"; 

 } 

int main(int argc, char * argv[],char * envp[])
{
  char cmd[100]; 
  char *cmdLine; 
  int  nBytes; //size of msg rec'd

  signal(SIGINT,ctrlCHandler); 
 

  fprintf(stdout,"Please type help command \n"); 
  while(true) {

          cmdLine=readline("Enter a command: ");   
          if(!cmdLine) break; 
          char *response=interpret(cmdLine); 
          fprintf(stdout,"%s\n",response);    
         
        }  

  fprintf(stdout,"Server is now terminated \n"); 
  return 0;
  }

