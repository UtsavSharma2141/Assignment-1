/*
   File: help_client.c
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


void pipeSigHandler(int signum)
{
   fprintf(stderr,"Broken pipe, server not available\n");
   exit(1);
}

void ctrlCHandler(int signum)
{
   fprintf(stdout,"Program ended using CTRL-C\n");
   exit(2);
}

int main(int argc, char * argv[],char * envp[])
{
   char incomingMsg[1000];
   char *msg;
   int nBytes;

   signal(SIGPIPE,pipeSigHandler);
   signal(SIGINT,ctrlCHandler);

   int UtsavPipeIn=open("UtsavPipeIn",O_WRONLY);
   int UtsavPipeOut=open("UtsavPipeOut",O_RDONLY);

   read_history(".utsavassignment1.history");   
   while(true)
   {
      history_comment_char='#';

      msg=readline("Enter a msg for the server: ");
      if(!msg)
      {
         fprintf(stdout,"Client has ended the conversation using CTRL-D\n");
         close(UtsavPipeOut);
         close(UtsavPipeIn);
         break;
      }

      add_history(msg);
      write(UtsavPipeIn,msg,strlen(msg));

      memset(incomingMsg,0,sizeof incomingMsg);
      nBytes=read(UtsavPipeOut,incomingMsg,sizeof incomingMsg);
      if(!nBytes) {fprintf(stdout,"Server has closed the connection\n"); break;}
      fprintf(stdout,"Server said: %*s \n",nBytes,incomingMsg);
   }   
   write_history(".utsavassignment1.history");

   return 0;
}
