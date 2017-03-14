#include<math.h>
#include<stdlib.h>
#include"sconstants.h"
#include"stype.h"
#include"sapi.h"
static obj_t *math_sin(obj_t *args){
  obj_t *newreal=new_real();
  switch (car(args)->type){
  case REAL:newreal->data.real=sinf(car(args)->data.real);break;
  case INTEGER:newreal->data.real=sinf((float)car(args)->data.integer);break;
  }
  return newreal;
}
static obj_t *math_log(obj_t *args){
  obj_t *newreal=new_real();
  switch (car(args)->type){
  case REAL:newreal->data.real=logf(car(args)->data.real);break;
  case INTEGER:newreal->data.real=logf((float)car(args)->data.integer);break;
  }
  return newreal;
}
static obj_t *math_abs(obj_t *args){
  switch (car(args)->type){
  case REAL:
    {
      obj_t *newreal=new_real();
      newreal->data.real=fabsf(car(args)->data.real);
      return newreal;
    }break;
  case INTEGER:{
    obj_t *newinteger=new_integer();
    newinteger->data.integer=labs(car(args)->data.integer);
    return newinteger;
  }break;
  default:error("abs: wrong argument type!\n");return 0;break;
  }
}
static libfunc_t math_funcs[]={
  {"sin",math_sin},
  {"log",math_log},
  {"abs",math_abs},
  {0,0}
};
void schemeopen_math(void){
  add_libfunc(math_funcs);
}
