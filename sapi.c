#include"sconstants.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
obj_t *new_pair(void);
void del_obj(obj_t *obj);
void error(char *err_info){
  fprintf(stderr,"%s",err_info);
  exit(1);
}
int eq(obj_t *obj1,obj_t *obj2){
  if (obj1->type==SYMBOL&&obj2->type==SYMBOL&&!strcmp(obj1->data.symbol,obj2->data.symbol))
    return 1;
  if (obj1->type==INTEGER&&obj2->type==INTEGER&&obj1->data.integer==obj2->data.integer)
    return 1;
  if (obj1->type==REAL&&obj2->type==REAL&&obj1->data.real==obj2->data.real)
    return 1;
  if (obj1->type==PAIR&&obj2->type==PAIR&&obj1->data.pair==obj2->data.pair)
    return 1;
  return 0;
}
obj_t *car(obj_t *pair){
  if (pair->type==PAIR)
    return pair->data.pair->car;
  else{
    error("CAR:not a pair!\n");
    return NULL;
  }
}
obj_t *cdr(obj_t *pair){
  if (pair->type==PAIR)
    return pair->data.pair->cdr;
  else{
    error("CDR:not a pair!\n");
    return NULL;
  }
}
obj_t *cadr(obj_t *pair){
  return car(cdr(pair));
}
obj_t *cons(obj_t *car,obj_t *cdr){
  obj_t *newpair=new_pair();
  newpair->data.pair->car=car;
  car->ref++;
  newpair->data.pair->cdr=cdr;
  cdr->ref++;
  return newpair;
}
obj_t *new_obj(void){
  obj_t *newobj=(obj_t *)malloc(sizeof(obj_t));
  if (newobj==NULL){
    error("no enough memory!\n");
    return NULL;
  }
  else{
    newobj->ref=0;
    return newobj;
  }
}
obj_t *new_procedure(void){
  obj_t *newobj=new_obj();
  newobj->type=PROCEDURE;
  procedure_t *newprocedure=(procedure_t *)malloc(sizeof(procedure_t));
  if (newprocedure==NULL){
    free(newobj);
    error("no enough memory!\n");
    return NULL;
  }
  newobj->data.procedure=newprocedure;
  return newobj;
}
obj_t *new_primitive(void){
  obj_t *newobj=new_obj();
  newobj->type=PRIMITIVE;
  primitive_t *newprimitive=(primitive_t *)malloc(sizeof(primitive_t));
  if (newprimitive==NULL){
    free(newobj);
    error("no enough memory!\n");
    return NULL;
  }
  newobj->data.primitive=newprimitive;
  return newobj;
}
obj_t *new_pair(void){
  obj_t *newpairobj=new_obj();
  newpairobj->type=PAIR;
  pair_t *newpairdata=(pair_t *)malloc(sizeof(pair_t));
  if (newpairdata==NULL){
    free(newpairobj);
    error("no enough memory!\n");
    return NULL;
  }
  newpairdata->car=NULL;
  newpairdata->cdr=NULL;
  newpairobj->data.pair=newpairdata;
  return newpairobj;
}
obj_t *new_symbol(void){
  obj_t *newsymbol=new_obj();
  if (newsymbol==NULL){
    error("no enough memory!\n");
    return NULL;
  }
  newsymbol->type=SYMBOL;
  newsymbol->data.symbol=NULL;
  return newsymbol;
}
obj_t *new_string(void){
  obj_t *newstring=new_obj();
  if (newstring==NULL){
    error("no enough memory!\n");
    return NULL;
  }
  newstring->type=STRING;
  newstring->data.string=NULL;
  return newstring;
}
obj_t *new_integer(void){
  obj_t *newinteger=new_obj();
  if (newinteger==NULL){
    error("no enough memory!\n");
    return NULL;
  }
  newinteger->type=INTEGER;
  return newinteger;
}
obj_t *new_real(void){
  obj_t *newreal=new_obj();
  if (newreal==NULL){
    error("no enough memory!\n");
    return NULL;
  }
  newreal->type=REAL;
  return newreal;
}
epair_t *new_epair(void){
  epair_t *newepair=(epair_t *)malloc(sizeof(epair_t));
  if (newepair==NULL){
    error("no enough memory!\n");
    return NULL;
  }
  return newepair;
}
int objSymbolEqString(obj_t *objSymbol,char *string){
  return objSymbol->type==SYMBOL&&!strcmp(objSymbol->data.symbol,string);
}
int self_evaluating(obj_t *exp){
  return exp->type==INTEGER||exp->type==REAL||exp->type==STRING||exp->type==BOOLEAN||exp->type==PROCEDURE||exp->type==ENVIRONMENT||exp->type==UNDEF;
}
int variable(obj_t *exp){
  return exp->type==SYMBOL;
}
obj_t *lookup_variable_value(obj_t *exp,obj_t *env){
  while (env){
    epair_t *epair=env->data.env->first;
    while (epair){
      if (objSymbolEqString(exp,epair->symbol))
        return epair->obj;
      epair=epair->next;
    }
    env=env->data.env->outside;
  }
  if (!env)
    error("unbound variable!\n");
  return NULL;
}
int quoted(obj_t *exp){
  if (exp->type==PAIR){
    obj_t *carobj=car(exp);
    if (carobj->type==SYMBOL && !strcmp(carobj->data.symbol,"quote"))
      return 1;
  }
  return 0;
}
obj_t *text_of_quotation(obj_t *exp){
  return cadr(exp);
}
int assignment(obj_t *exp){
  if (exp->type==PAIR){
    obj_t *carobj=car(exp);
    if (carobj->type==SYMBOL && !strcmp(carobj->data.symbol,"set!"))
      return 1;
  }
  return 0;
}
obj_t *assignment_variable(obj_t *exp){
  return cadr(exp);
}
obj_t *assignment_value(obj_t *exp){
  return cadr(cdr(exp));
}
void set_variable_value(obj_t *var,obj_t *val,obj_t *env){
  while (env){
    epair_t *epair=env->data.env->first;
    while (epair){
      if (objSymbolEqString(var,epair->symbol)){
        val->ref++;
        epair->obj->ref--;
        del_obj(epair->obj);
        epair->obj=val;
        return;
      }
      epair=epair->next;
    }
    env=env->data.env->outside;
  }
  if (env==NULL)
    error("unbound variable! --Set!\n");
}
int definition(obj_t *exp){
  if (exp->type==PAIR){
    obj_t *carobj=car(exp);
    if (carobj->type==SYMBOL && !strcmp(carobj->data.symbol,"define")){
      return 1;
    }
  }
  return 0;
}
obj_t *definition_variable(obj_t *exp){
  obj_t *var=cadr(exp);
  if (var->type==SYMBOL)
    return var;
  else
    return car(var);
}
obj_t *make_lambda(obj_t *parameters,obj_t *body){
  obj_t *newsymbol=new_symbol();
  char *buf=(char *)malloc(7);
  strcpy(buf,"lambda");
  newsymbol->data.symbol=buf;
  return cons(newsymbol,cons(parameters,body));
}
obj_t *definition_value(obj_t *exp){
  obj_t *var=cadr(exp);
  if (var->type==SYMBOL)
    return cadr(cdr(exp));
  else
    return make_lambda(cdr(cadr(exp)),cdr(cdr(exp)));
}
void define_variable(obj_t *var,obj_t *val,obj_t *env){
  env_t *envbody=env->data.env;
  epair_t *epair=envbody->first;
  while (epair){
    if (!strcmp(var->data.symbol,epair->symbol)){
      val->ref++;
      epair->obj->ref--;
      del_obj(epair->obj);
      epair->obj=val;
      return;
    }
    epair=epair->next;
  }
  if (epair==NULL){
    epair_t *newepair=new_epair();
    newepair->symbol=(char *)malloc(strlen(var->data.symbol)+1);
    if (newepair->symbol==NULL)
      error("no enough memory!\n");
    else{
      strcpy(newepair->symbol,var->data.symbol);
      newepair->obj=val;
      val->ref++;
      newepair->prev=NULL;
      newepair->next=envbody->first;
      if (envbody->first)
        envbody->first->prev=newepair;
      envbody->first=newepair;
    }
  }
}
int is_if(obj_t *exp){
  if (exp->type==PAIR){
    obj_t *carobj=car(exp);
    if (carobj->type==SYMBOL && objSymbolEqString(carobj,"if"))
      return 1;
  }
  return 0;
}
int is_null(obj_t *exp){
  return exp==&nil;
}
int is_true(obj_t *exp){
  return !(exp==&false_obj);
}
obj_t *if_predicate(obj_t *exp){
  return cadr(exp);
}
obj_t *if_consequent(obj_t *exp){
  return cadr(cdr(exp));
}
obj_t *if_alternative(obj_t *exp){
  if (is_null(cdr(cdr(cdr(exp)))))
    return &undef;
  else
    return car(cdr(cdr(cdr(exp))));
}
int is_lambda(obj_t *exp){
  if (exp->type==PAIR){
    obj_t *carobj=car(exp);
    if (carobj->type==SYMBOL && objSymbolEqString(carobj,"lambda"))
      return 1;
  }
  return 0;
}
obj_t *lambda_parameters(obj_t *exp){
  return cadr(exp);
}
obj_t *lambda_body(obj_t *exp){
  return cdr(cdr(exp));
}
obj_t *make_procedure(obj_t *parameters,obj_t *body,obj_t *env){
  obj_t *newprocedure=new_procedure();
  newprocedure->data.procedure->parameters=parameters;
  parameters->ref++;
  //todo scan-out-definition
  newprocedure->data.procedure->body=body;
  body->ref++;
  newprocedure->data.procedure->env=env;
  env->ref++;
  return newprocedure;
}
int is_begin(obj_t *exp){
  if (exp->type==PAIR){
    obj_t *carobj=car(exp);
    if (carobj->type==SYMBOL && !strcmp(carobj->data.symbol,"begin"))
      return 1;
  }
  return 0;
}
obj_t *begin_actions(obj_t *exp){
  return cdr(exp);
}
int is_cond(obj_t *exp){
  if (exp->type==PAIR){
    obj_t *carobj=car(exp);
    if (carobj->type==SYMBOL && objSymbolEqString(carobj,"cond"))
      return 1;
  }
  return 0;
}
int is_let(obj_t *exp){
  return objSymbolEqString(car(exp),"let");
}
obj_t *map(obj_t *(*fun)(obj_t *element),obj_t *list){
  obj_t *head=NULL,*tail;
  while (list!=&nil){
    obj_t *ans_element=fun(car(list));
    obj_t *newpair=new_pair();
    newpair->data.pair->car=ans_element;
    ans_element->ref++;
    if (head==NULL)
      head=tail=newpair;
    else{
      newpair->ref++;
      tail->data.pair->cdr=newpair;
      tail=newpair;
    }
    list=cdr(list);
  }
  nil.ref++;
  if (head==NULL)
    return &nil;
  else{
    tail->data.pair->cdr=&nil;
    return head;
  }
}
void print(obj_t *exp);
obj_t *let2combination(obj_t *exp){
  return cons(make_lambda(map(car,cadr(exp)),cdr(cdr(exp))),map(cadr,cadr(exp)));
}
int is_application(obj_t *exp){
  return exp->type==PAIR;
}
obj_t *operator(obj_t *exp){
  return car(exp);
}
obj_t *operands(obj_t *exp){
  return cdr(exp);
}
obj_t *procedure_body(obj_t *procedure){
  return procedure->data.procedure->body;
}
obj_t *procedure_parameters(obj_t *procedure){
  return procedure->data.procedure->parameters;
}
obj_t *procedure_environment(obj_t *procedure){
  return procedure->data.procedure->env;
}
obj_t *new_env(void){
  env_t *newenvbody=(env_t *)malloc(sizeof(env_t));
  if (newenvbody==NULL){
    error("no enough memory!\n");
    return NULL;
  }
  else{
    newenvbody->first=NULL;
    newenvbody->outside=NULL;
    obj_t *newenv=new_obj();
    newenv->type=ENVIRONMENT;
    newenv->data.env=newenvbody;
    return newenv;
  }
}
void print(obj_t *exp){
  if (exp==NULL)
    return;
  switch (exp->type){
  case SYMBOL:printf("%s",exp->data.symbol);break;
  case STRING:printf("\"%s\"",exp->data.string);break;
  case INTEGER:printf("%ld",exp->data.integer);break;
  case REAL:printf("%f",exp->data.real);break;
  case UNDEF:printf("unspecified-value");break;
  case PROCEDURE:{
    printf("procedure:");
    print(procedure_parameters(exp));
    print(procedure_body(exp));
  }break;
  case PRIMITIVE:printf("primitive");break;
  case ENVIRONMENT:{
    printf("environment:");
    if (exp==&global_environment)
      break;
    printf("{");
    epair_t *epair=exp->data.env->first;
    while (epair){
      printf("%s:",epair->symbol);
      print(epair->obj);
      printf("\n");
      epair=epair->next;
    }
    printf("}");
  }break;
  case BOOLEAN:printf("%s",exp==&true_obj?"true":"false");break;
  case PAIR:{
    printf("(");
    if (exp->data.pair->car!=NULL){
      print(exp->data.pair->car);
      obj_t *tmp=exp->data.pair->cdr;
      while (tmp->type==PAIR&&tmp!=&nil){
        printf(" ");
        print(tmp->data.pair->car);
        tmp=tmp->data.pair->cdr;
      }
      if (tmp->type!=PAIR){
        printf(" . ");
        print(tmp);
      }
    }
    printf(")");
    break;
  }
  }
}
obj_t *make_tail(obj_t *exp,obj_t *env){
   return cons(&tail,cons(exp,env));
}
void add_libfunc(libfunc_t funcs[]){
  for (int i=0;funcs[i].name;i++){
    obj_t *newsymbol=new_symbol();
    newsymbol->data.symbol=funcs[i].name;
    obj_t *newprimitive=new_primitive();
    newprimitive->data.primitive->fun=funcs[i].fun;
    define_variable(newsymbol,newprimitive,&global_environment);
  }
}
void del_obj(obj_t *obj){
  if (obj->type==ENVIRONMENT){
    epair_t *i,*j;
    int count_sum=0;
    for (i=obj->data.env->first;i;i=i->next){
      if (i->obj->type==PROCEDURE&&procedure_environment(i->obj)==obj){
        obj_t *procedure=i->obj;
        int count=0;
        for (j=obj->data.env->first;j;j=j->next)
          if (j->obj==procedure)
            count++;
        count_sum++;
        if (count!=procedure->ref)
          return;
      }
    }
    if (count_sum!=obj->ref)
      return;
    for (i=obj->data.env->first;i;i=i->next){
      i->obj->ref--;
      if (i->obj->ref==0){
        if (i->obj->type==PROCEDURE&&procedure_environment(i->obj)==obj){
          obj_t *parameters=procedure_parameters(i->obj);
          parameters->ref--;
          del_obj(parameters);
          obj_t *body=procedure_body(i->obj);
          body->ref--;
          del_obj(body);
          free(i->obj->data.procedure);
          free(i->obj);
        }
        else
          del_obj(i->obj);
      }
    }
    for (i=obj->data.env->first;i;){
      epair_t *next=i->next;
      free(i->symbol);
      free(i);
      i=next;
    }
    obj_t *outside=obj->data.env->outside;
    free(obj->data.env);
    free(obj);
    outside->ref--;
    del_obj(outside);
  }
  if (obj->ref==0 && obj->type!=BOOLEAN && obj->type!=UNDEF &&obj!=&nil){
    switch (obj->type){
    case(INTEGER):free(obj);break;
    case(REAL):free(obj);break;
    case(STRING):free(obj->data.string);free(obj);break;
    case(SYMBOL):free(obj->data.symbol);free(obj);break;
    case(PAIR):{
      obj_t *carobj=car(obj);
      carobj->ref--;
      obj_t *cdrobj=cdr(obj);
      cdrobj->ref--;
      del_obj(carobj);
      del_obj(cdrobj);
      free(obj->data.pair);
      free(obj);
    }break;
    case(PROCEDURE):{
      obj_t *parameters=procedure_parameters(obj);
      parameters->ref--;
      del_obj(parameters);
      obj_t *body=procedure_body(obj);
      body->ref--;
      del_obj(body);
      obj_t *env=procedure_environment(obj);
      env->ref--;
      del_obj(env);
      free(obj->data.procedure);
      free(obj);
    }break;
    case(PRIMITIVE):{
      //todo unload shared library?
      free(obj->data.primitive);
      free(obj);
    }break;
    case(ENVIRONMENT):{
      env_t *envbody=obj->data.env;
      epair_t *envpair=envbody->first;
      while (envpair){
        epair_t *next=envpair->next;
        envpair->obj->ref--;
        del_obj(envpair->obj);
        free(envpair->symbol);
        free(envpair);
        envpair=next;
      }
      obj_t *outside=envbody->outside;
      free(envbody);
      free(obj);
      outside->ref--;
      del_obj(outside);
    }break;
    default:break;
    }
  }
}
