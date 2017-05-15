#include"sconstants.h"
#include"sapi.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 1000
obj_t *parse_exp(char *s,char **endptr){
  obj_t *head_st[MAX];
  obj_t *tail_st[MAX];
  int top=-1;
  while (s[0]){
    switch (s[0]){
    case '(':
      {
        head_st[top+1]=NULL;
        tail_st[top+1]=NULL;
        top++;
        s++;
        break;
      }
    case ')':
      {
        do{
          if (tail_st[top]==NULL)
            head_st[top]=tail_st[top]=&nil;
          else
            tail_st[top]->data.pair->cdr=&nil;
          nil.ref++;
          if (top==0){
            *endptr=s+1;
            return head_st[top];
          }
          obj_t *newpair=new_pair();
          newpair->data.pair->car=head_st[top];
          head_st[top]->ref++;
          if (tail_st[top-1]==NULL){
            head_st[top-1]=tail_st[top-1]=newpair;
          }
          else{
            tail_st[top-1]->data.pair->cdr=newpair;
            newpair->ref++;
            tail_st[top-1]=newpair;
          }
          top--;
        }while (head_st[top]->data.pair->car->type==SYMBOL&&!strcmp(head_st[top]->data.pair->car->data.symbol,"quote"));
        s++;
        break;
      }
    case ' ':s++;break;
    case '\t':s++;break;
    case '\n':s++;break;
    case '\'':{
      obj_t *newpair=new_pair();
      obj_t *newsymbol=new_symbol();
      char *buf=(char *)malloc(6);
      strcpy(buf,"quote");
      newsymbol->data.symbol=buf;
      newpair->data.pair->car=newsymbol;
      newsymbol->ref++;
      top++;
      head_st[top]=tail_st[top]=newpair;
      s++;
      break;
    }
    default:{
      obj_t *newobj;
      if (s[0]=='"'){
        int leng=0;
        s++;
        while (s[0]!='"'){
          s++;
          leng++;
        }
        newobj=new_string();
        char *buf =(char *)malloc(leng+1),*p=s-leng;
        if (buf==NULL)
          error("no enough memory!\n");
        for (int i=0;i<leng;i++)
          buf[i]=p[i];
        buf[leng]='\0';
        newobj->data.string=buf;
        s++;
      }
      else{
        char *endptr;
        long longvar=strtol(s,&endptr,10);
        if (endptr[0]=='\''||endptr[0]=='('||endptr[0]==')'||endptr[0]==' '||endptr[0]=='\t'||endptr[0]=='\n'||endptr[0]=='\0'){
          newobj=new_integer();
          newobj->data.integer=longvar;
          s=endptr;
        }
        else{
          float floatvar=strtof(s,&endptr);
          if (endptr[0]=='\''||endptr[0]=='('||endptr[0]==')'||endptr[0]==' '||endptr[0]=='\t'||endptr[0]=='\n'||endptr[0]=='\0'){
            newobj=new_real();
            newobj->data.real=floatvar;
            s=endptr;
          }
          else{
            if (!strncmp(s,"#f",2)){
              newobj=&false_obj;
              s+=2;
            }
            else
              if (!strncmp(s,"#t",2)){
                newobj=&true_obj;
                s+=2;
              }
              else{
                newobj=new_symbol();
                int leng=0;
                while (s[0]!='\''&&s[0]!='('&&s[0]!=')'&&s[0]!=' '&&s[0]!='\t'&&s[0]!='\0'&&s[0]!='\n'){
                  s++;
                  leng++;
                }
                char *buf =(char *)malloc(leng+1),*p=s-leng;
                if (buf==NULL)
                  error("no enough memory!\n");
                for (int i=0;i<leng;i++)
                  buf[i]=p[i];
                buf[leng]='\0';
                newobj->data.symbol=buf;
              }
          }
        }
      }
      if (top==-1){
        *endptr=s;
        return newobj;
      }
      else{
        obj_t *newpair=new_pair();
        newpair->data.pair->car=newobj;
        newobj->ref++;
        if (head_st[top]==NULL)
          head_st[top]=tail_st[top]=newpair;
        else{
          tail_st[top]->data.pair->cdr=newpair;
          newpair->ref++;
          tail_st[top]=newpair;
        }
        while (head_st[top]->data.pair->car->type==SYMBOL&&!strcmp(head_st[top]->data.pair->car->data.symbol,"quote")){
          tail_st[top]->data.pair->cdr=&nil;
          nil.ref++;
          if (top==0){
            *endptr=s;
            return head_st[top];
          }
          obj_t *newpair=new_pair();
          newpair->data.pair->car=head_st[top];
          head_st[top]->ref++;
          tail_st[top-1]->data.pair->cdr=newpair;
          newpair->ref++;
          tail_st[top-1]=newpair;
          top--;
        }
      }
      break;
    }
    }
  }
  *endptr=s;
  return &undef;
}
obj_t *parse_source(char *s){
  obj_t *head,*tail;
  head=tail=new_pair();
  head->data.pair->car=new_symbol();
  char *buf=(char *)malloc(6);
  strcpy(buf,"begin");
  head->data.pair->car->data.symbol=buf;
  while (s[0]){
    obj_t *exp=parse_exp(s,&s);
    obj_t *newpair=new_pair();
    newpair->data.pair->car=exp;
    exp->ref++;
    tail->data.pair->cdr=newpair;
    newpair->ref++;
    tail=newpair;
    while((s[0]==' '||s[0]=='\t'||s[0]=='\n')&&s[0]!='\0')
      s++;
  }
  tail->data.pair->cdr=&nil;
  nil.ref++;
  return head;
}
