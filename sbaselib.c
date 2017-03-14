#include<dlfcn.h>
#include<string.h>
#include"stype.h"
#include"sapi.h"
#include"sconstants.h"
#define MAXLIBNAMELEN 100
static obj_t *schemeB_add(obj_t *arguments){
  if (is_null(arguments)){
    obj_t *newinteger=new_integer();
    newinteger->data.integer=0;
    return newinteger;
  }
  obj_t *sublist=arguments;
  int flag=0;
  while (!is_null(sublist)){
    obj_t *arg=car(sublist);
    if (arg->type==REAL)
      flag=1;
    if (arg->type!=REAL&&arg->type!=INTEGER)
      error("wrong argument type!\n");
    sublist=cdr(sublist);
  }
  if (flag){
    obj_t *sublist=arguments;
    float sum=0;
    while (!is_null(sublist)){
      obj_t *arg=car(sublist);
      if (arg->type==REAL)
        sum+=arg->data.real;
      else
        sum+=(float)arg->data.integer;
      sublist=cdr(sublist);
    }
    obj_t *ans=new_real();
    ans->data.real=sum;
    return ans;
  }
  else{
    obj_t *sublist=arguments;
    long sum=0;
    while (!is_null(sublist)){
      obj_t *arg=car(sublist);
      sum+=arg->data.integer;
      sublist=cdr(sublist);
    }
    obj_t *ans=new_integer();
    ans->data.integer=sum;
    return ans;
  }
}
static obj_t * schemeB_sub(obj_t *arguments){
  if (is_null(arguments)){
    error("- requires at least 1 argument!\n");
    return (obj_t *)0;
  }
  obj_t *sublist=arguments;
  int flag=0;
  while (!is_null(sublist)){
    obj_t *arg=car(sublist);
    if (arg->type==REAL)
      flag=1;
    if (arg->type!=REAL&&arg->type!=INTEGER)
      error("wrong argument type!\n");
    sublist=cdr(sublist);
  }
  if (flag){
    float sum;
    obj_t *sublist=arguments;
    obj_t *arg=car(sublist);
    if (is_null(cdr(arguments)))
      sum=-arg->data.real;
    else{
      if (arg->type==REAL)
        sum=arg->data.real;
      else
        sum=(float)arg->data.integer;
      sublist=cdr(sublist);
      while (!is_null(sublist)){
        arg=car(sublist);
        if (arg->type==REAL)
          sum-=arg->data.real;
        else
          sum-=(float)arg->data.integer;
        sublist=cdr(sublist);
      }
    }
    obj_t *ans=new_real();
    ans->data.real=sum;
    return ans;
  }
  else{
    long sum;
    obj_t *sublist=arguments;
    obj_t *arg=car(sublist);
    if (is_null(cdr(sublist)))
      sum=-arg->data.integer;
    else{
      sum=arg->data.integer;
      sublist=cdr(sublist);
      while (!is_null(sublist)){
        arg=car(sublist);
        sum-=arg->data.integer;
        sublist=cdr(sublist);
      }
      obj_t *ans=new_integer();
      ans->data.integer=sum;
      return ans;
    }
  }
  return (obj_t *)0;
}
static obj_t *schemeB_require(obj_t *args){
  char *lib=car(args)->data.string;
  char libname[MAXLIBNAMELEN];
  char libopenfunname[MAXLIBNAMELEN];
  strcpy(libname,"s");
  strcat(libname,lib);
  strcat(libname,"lib.so");
  strcpy(libopenfunname,"schemeopen_");
  strcat(libopenfunname,lib);
  void *dl=dlopen(libname,RTLD_LAZY);
  if (dl==NULL){
    error("require failed!\n");
    return NULL;
  }
  void (*libopenfun)(void);
  libopenfun=dlsym(dl,libopenfunname);
  libopenfun();
  return &undef;
}
static obj_t *schemeB_numeq(obj_t *args){
  obj_t *sublist=args;
  int flag=0;
  if (is_null(args)||is_null(cdr(args)))
    return &true_obj;
  while (!is_null(sublist)){
    obj_t *arg=car(sublist);
    if (arg->type==REAL)
      flag=1;
    if (arg->type!=REAL&&arg->type!=INTEGER)
      error("wrong argument type!\n");
    sublist=cdr(sublist);
  }
  sublist=args;
  if (flag){
    float pre;
    obj_t *arg=car(sublist);
    if (arg->type==REAL)
      pre=arg->data.real;
    else
      pre=(float)arg->data.integer;
    sublist=cdr(sublist);
    while (!is_null(sublist)){
      float now;
      arg=car(sublist);
      if (arg->type==REAL)
        now=arg->data.real;
      else
        now=(float)arg->data.integer;
      if (now!=pre)
        return &false_obj;
      sublist=cdr(sublist);
    }
    return &true_obj;
  }
  else{
    long pre=car(sublist)->data.integer;
    sublist=cdr(sublist);
    while(!is_null(sublist)){
      long now=car(sublist)->data.integer;
      if (now!=pre)
        return &false_obj;
      sublist=cdr(sublist);
    }
    return &true_obj;
  }
}
static obj_t *schemeB_numgt(obj_t *args){
  obj_t *sublist=args;
  int flag=0;
  if (is_null(args)||is_null(cdr(args)))
    return &true_obj;
  while (!is_null(sublist)){
    obj_t *arg=car(sublist);
    if (arg->type==REAL)
      flag=1;
    if (arg->type!=REAL&&arg->type!=INTEGER)
      error("wrong argument type!\n");
    sublist=cdr(sublist);
  }
  sublist=args;
  if (flag){
    float pre;
    obj_t *arg=car(sublist);
    if (arg->type==REAL)
      pre=arg->data.real;
    else
      pre=(float)arg->data.integer;
    sublist=cdr(sublist);
    while (!is_null(sublist)){
      float now;
      arg=car(sublist);
      if (arg->type==REAL)
        now=arg->data.real;
      else
        now=(float)arg->data.integer;
      if (pre<=now)
        return &false_obj;
      sublist=cdr(sublist);
    }
    return &true_obj;
  }
  else{
    long pre=car(sublist)->data.integer;
    sublist=cdr(sublist);
    while(!is_null(sublist)){
      long now=car(sublist)->data.integer;
      if (pre<=now)
        return &false_obj;
      sublist=cdr(sublist);
    }
    return &true_obj;
  }
}
static obj_t *schemeB_numlt(obj_t *args){
  obj_t *sublist=args;
  int flag=0;
  if (is_null(args)||is_null(cdr(args)))
    return &true_obj;
  while (!is_null(sublist)){
    obj_t *arg=car(sublist);
    if (arg->type==REAL)
      flag=1;
    if (arg->type!=REAL&&arg->type!=INTEGER)
      error("wrong argument type!\n");
    sublist=cdr(sublist);
  }
  sublist=args;
  if (flag){
    float pre;
    obj_t *arg=car(sublist);
    if (arg->type==REAL)
      pre=arg->data.real;
    else
      pre=(float)arg->data.integer;
    sublist=cdr(sublist);
    while (!is_null(sublist)){
      float now;
      arg=car(sublist);
      if (arg->type==REAL)
        now=arg->data.real;
      else
        now=(float)arg->data.integer;
      if (pre>=now)
        return &false_obj;
      sublist=cdr(sublist);
    }
    return &true_obj;
  }
  else{
    long pre=car(sublist)->data.integer;
    sublist=cdr(sublist);
    while(!is_null(sublist)){
      long now=car(sublist)->data.integer;
      if (pre>=now)
        return &false_obj;
      sublist=cdr(sublist);
    }
    return &true_obj;
  }
}
static obj_t *schemeB_numge(obj_t *args){
  obj_t *sublist=args;
  int flag=0;
  if (is_null(args)||is_null(cdr(args)))
    return &true_obj;
  while (!is_null(sublist)){
    obj_t *arg=car(sublist);
    if (arg->type==REAL)
      flag=1;
    if (arg->type!=REAL&&arg->type!=INTEGER)
      error("wrong argument type!\n");
    sublist=cdr(sublist);
  }
  sublist=args;
  if (flag){
    float pre;
    obj_t *arg=car(sublist);
    if (arg->type==REAL)
      pre=arg->data.real;
    else
      pre=(float)arg->data.integer;
    sublist=cdr(sublist);
    while (!is_null(sublist)){
      float now;
      arg=car(sublist);
      if (arg->type==REAL)
        now=arg->data.real;
      else
        now=(float)arg->data.integer;
      if (pre<now)
        return &false_obj;
      sublist=cdr(sublist);
    }
    return &true_obj;
  }
  else{
    long pre=car(sublist)->data.integer;
    sublist=cdr(sublist);
    while(!is_null(sublist)){
      long now=car(sublist)->data.integer;
      if (pre<now)
        return &false_obj;
      sublist=cdr(sublist);
    }
    return &true_obj;
  }
}
static obj_t *schemeB_numle(obj_t *args){
  obj_t *sublist=args;
  int flag=0;
  if (is_null(args)||is_null(cdr(args)))
    return &true_obj;
  while (!is_null(sublist)){
    obj_t *arg=car(sublist);
    if (arg->type==REAL)
      flag=1;
    if (arg->type!=REAL&&arg->type!=INTEGER)
      error("wrong argument type!\n");
    sublist=cdr(sublist);
  }
  sublist=args;
  if (flag){
    float pre;
    obj_t *arg=car(sublist);
    if (arg->type==REAL)
      pre=arg->data.real;
    else
      pre=(float)arg->data.integer;
    sublist=cdr(sublist);
    while (!is_null(sublist)){
      float now;
      arg=car(sublist);
      if (arg->type==REAL)
        now=arg->data.real;
      else
        now=(float)arg->data.integer;
      if (pre>now)
        return &false_obj;
      sublist=cdr(sublist);
    }
    return &true_obj;
  }
  else{
    long pre=car(sublist)->data.integer;
    sublist=cdr(sublist);
    while(!is_null(sublist)){
      long now=car(sublist)->data.integer;
      if (pre>now)
        return &false_obj;
      sublist=cdr(sublist);
    }
    return &true_obj;
  }
}
static obj_t *schemeB_multiply(obj_t *args){
  obj_t *sublist=args;
  int flag=0;
  if (is_null(args)){
    obj_t *newinteger=new_integer();
    newinteger->data.integer=1;
    return newinteger;
  }
  while (!is_null(sublist)){
    obj_t *arg=car(sublist);
    if (arg->type==REAL)
      flag=1;
    if (arg->type!=REAL&&arg->type!=INTEGER)
      error("wrong argument type!\n");
    sublist=cdr(sublist);
  }
  sublist=args;
  if (flag){
    float ans;
    obj_t *arg=car(sublist);
    if (arg->type==REAL)
      ans=arg->data.real;
    else
      ans=(float)arg->data.integer;
    sublist=cdr(sublist);
    while (!is_null(sublist)){
      float now;
      arg=car(sublist);
      if (arg->type==REAL)
        now=arg->data.real;
      else
        now=(float)arg->data.integer;
      ans*=now;
      sublist=cdr(sublist);
    }
    obj_t *newreal=new_real();
    newreal->data.real=ans;
    return newreal;
  }
  else{
    long ans=car(sublist)->data.integer;
    sublist=cdr(sublist);
    while(!is_null(sublist)){
      long now=car(sublist)->data.integer;
      ans*=now;
      sublist=cdr(sublist);
    }
    obj_t *newinteger=new_integer();
    newinteger->data.integer=ans;
    return newinteger;
  }
}
static obj_t *schemeB_divide(obj_t *args){
  if (is_null(args)){
    error("/ requires at least 1 argument!\n");
    return NULL;
  }
  obj_t *sublist=cdr(args);
  while (!is_null(sublist)){
    obj_t *arg=car(sublist);
    switch (arg->type){
    case REAL:
      {
        if (arg->data.real==0){
          error("divided by zero!\n");
          return NULL;
        }
      }break;
    case INTEGER:{
      if (arg->data.integer==0){
        error("divided by zero!\n");
        return NULL;
      }
      }break;
    default:error("wrong argument type!\n");return NULL;break;
    }
    sublist=cdr(sublist);
  }
  sublist=args;
  float ans;
  obj_t *arg=car(sublist);
  if (is_null(cdr(args))){
    if (arg->type==REAL){
      if (arg->data.real==0){
        error("divided by zero!\n");
        return NULL;
      }
      ans=1.0/arg->data.real;
    }
    else{
      if (arg->data.integer==0){
        error("divided by zero!\n");
        return NULL;
      }
      ans=1.0/(float)arg->data.integer;
    }
  }
  else{
    if (arg->type==REAL)
        ans=arg->data.real;
      else
        ans=(float)arg->data.integer;
    sublist=cdr(sublist);
    while (!is_null(sublist)){
      float now;
      arg=car(sublist);
      if (arg->type==REAL)
        now=arg->data.real;
      else
        now=(float)arg->data.integer;
      ans/=now;
      sublist=cdr(sublist);
    }
  }
  obj_t *newreal=new_real();
  newreal->data.real=ans;
  return newreal;
}
static obj_t *schemeB_car(obj_t *args){
  obj_t *pair=car(args);
  if (pair->type!=PAIR || is_null(pair)){
    print(pair);
    error("is not a pair!\n");
    return NULL;
  }
  return car(pair);
}
static obj_t *schemeB_cdr(obj_t *args){
  obj_t *pair=car(args);
  if (pair->type!=PAIR || is_null(pair)){
    print(pair);
    error("is not a pair!\n");
    return NULL;
  }
  return cdr(pair);
}
static obj_t *schemeB_cons(obj_t *args){
  if (is_null(args) || is_null(cdr(args))){
    error("cons requires exactly 2 arguments!\n");
    return NULL;
  }
  return cons(car(args),cadr(args));
}
static obj_t *schemeB_list(obj_t *args){
  return args;
}
static obj_t *schemeB_null(obj_t *args){
  if (car(args)==&nil)
    return &true_obj;
  else
    return &false_obj;
}
//todo add short cut support
static obj_t *schemeB_and(obj_t *args){
  obj_t *sublist=args;
  while (!is_null(sublist)){
    obj_t *arg=car(sublist);
    if (!is_true(arg)||is_null(cdr(sublist)))
      return arg;
    sublist=cdr(sublist);
  }
  return &true_obj;
}
static obj_t *schemeB_or(obj_t *args){
  obj_t *sublist=args;
  while (!is_null(sublist)){
    obj_t *arg=car(sublist);
    if (is_true(arg))
      return arg;
    sublist=cdr(sublist);
  }
  return &false_obj;
}
static obj_t *schemeB_quotient(obj_t *args){
  if (is_null(args)||is_null(cdr(args))){
    error("quotient requires 2 args!\n");
    return NULL;
  }
  if (car(args)->type!=INTEGER||cadr(args)->type!=INTEGER){
    error("quotient requires 2 integers!\n");
    return NULL;
  }
  obj_t *newinteger=new_integer();
  newinteger->data.integer=car(args)->data.integer/cadr(args)->data.integer;
  return newinteger;
}
static obj_t *schemeB_remainder(obj_t *args){
  if (is_null(args)||is_null(cdr(args))){
    error("reamainder requires 2 args!\n");
    return NULL;
  }
  if (car(args)->type!=INTEGER||cadr(args)->type!=INTEGER){
    error("reamainder requires 2 integers!\n");
    return NULL;
  }
  obj_t *newinteger=new_integer();
  newinteger->data.integer=car(args)->data.integer%cadr(args)->data.integer;
  return newinteger;
}
static obj_t *schemeB_eq(obj_t *args){
  if (is_null(args)||is_null(cdr(args))){
    error("eq? requires 2 args!\n");
    return NULL;
  }
  obj_t *arg1=car(args),*arg2=cadr(args);
    if (arg1->type==SYMBOL&&arg2->type==SYMBOL&&!strcmp(arg1->data.symbol,arg2->data.symbol))
    return &true_obj;
  if (arg1->type==INTEGER&&arg2->type==INTEGER&&arg1->data.integer==arg2->data.integer)
    return &true_obj;
  if (arg1->type==REAL&&arg2->type==REAL&&arg1->data.real==arg2->data.real)
    return &true_obj;
  if (arg1->type==PAIR&&arg2->type==PAIR&&arg1->data.pair==arg2->data.pair)
    return &true_obj;
  return &false_obj;
}
static libfunc_t schemeBasefuncs[]={
  {"+",schemeB_add},
  {"-",schemeB_sub},
  {"=",schemeB_numeq},
  {">",schemeB_numgt},
  {"<",schemeB_numlt},
  {">=",schemeB_numge},
  {"<=",schemeB_numle},
  {"*",schemeB_multiply},
  {"/",schemeB_divide},
  {"car",schemeB_car},
  {"cdr",schemeB_cdr},
  {"cons",schemeB_cons},
  {"list",schemeB_list},
  {"null?",schemeB_null},
  {"and",schemeB_and},
  {"or",schemeB_or},
  {"quotient",schemeB_quotient},
  {"remainder",schemeB_remainder},
  {"eq?",schemeB_eq},
  {"require",schemeB_require},
  {0,0}
};
void schemeopen_base(void){
  add_libfunc(schemeBasefuncs);
}
