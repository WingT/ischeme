#ifndef _stype
#define _stype
#define INTEGER 0
#define REAL 1
#define STRING 2
#define SYMBOL 3
#define PAIR 4
#define PROCEDURE 5
#define PRIMITIVE 6
#define ENVIRONMENT 7
#define BOOLEAN 8
#define UNDEF 9
typedef struct obj_t{
  int type;
  int ref;
  union {
    long integer;
    float real;
    char *string;
    char *symbol;
    struct pair_t *pair;
    struct procedure_t *procedure;
    struct primitive_t *primitive;
    struct env_t *env;
  }data;
}obj_t;
typedef struct epair_t{
  char *symbol;
  obj_t *obj;
  struct epair_t *prev,*next;
}epair_t;
typedef struct env_t{
  obj_t *outside;
  epair_t *first;
}env_t;
typedef struct pair_t{
  obj_t *car,*cdr;
}pair_t;
typedef struct procedure_t{
  obj_t *parameters;
  obj_t *body;
  obj_t *env;
}procedure_t;
typedef struct primitive_t{
  obj_t *(*fun)(obj_t *args);
}primitive_t;
typedef struct libfunc_t{
  char *name;
  obj_t *(*fun)(obj_t *args);
}libfunc_t;

#endif
