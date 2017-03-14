#include<stdio.h>
#include"sconstants.h"
#include"stype.h"
#include"sapi.h"
static obj_t *io_display(obj_t *args){
  if (is_null(args)||!is_null(cdr(args))){
    error("display requires 1 argument!\n");
    return NULL;
  }
  //todo escape characters
  print(car(args));
  return &undef;
}
static obj_t *io_newline(obj_t *args){
  if (!is_null(args)){
    error("newline requires no argument!\n");
    return NULL;
  }
  printf("\n");
  return &undef;
}
static libfunc_t io_funcs[]={
  {"display",io_display},
  {"newline",io_newline},
  {0,0}
};
void schemeopen_io(void){
  add_libfunc(io_funcs);
}
