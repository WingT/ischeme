#include"sapi.h"
#include"sconstants.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
obj_t *eval(obj_t *exp,obj_t *env);
obj_t *eval_assignment(obj_t *exp,obj_t *env);
obj_t *eval_definition(obj_t *exp,obj_t *env);
obj_t *eval_if(obj_t *exp,obj_t *env);
obj_t *eval_lambda(obj_t *exp,obj_t *env);
obj_t *eval_begin(obj_t *exp,obj_t *env);
obj_t *eval_sequence(obj_t *exp,obj_t *env);
obj_t *eval_cond(obj_t *exp,obj_t *env);
obj_t *list_of_values(obj_t *exp,obj_t *env);
obj_t *eval_apply(obj_t *exp,obj_t *env);
obj_t *apply(obj_t *procedure,obj_t *arguments);
obj_t *apply_primitive_procedure(obj_t *procedure,obj_t *arguments);
obj_t *extend_environment(obj_t *vars,obj_t *vals,obj_t *base_env);
obj_t *eval_let(obj_t *exp,obj_t *env);
obj_t *eval(obj_t *exp,obj_t *env){
  obj_t *ans;
  if (self_evaluating(exp))
    return exp;
  if (variable(exp))
    return lookup_variable_value(exp,env);
  if (quoted(exp))
    return text_of_quotation(exp);
  if (assignment(exp))
    return eval_assignment(exp,env);
  if (definition(exp))
    return eval_definition(exp,env);
  if (is_lambda(exp))
    return eval_lambda(exp,env);
  if (is_if(exp))
    ans=eval_if(exp,env);
  else
    if (is_begin(exp))
      ans=eval_begin(exp,env);
    else
      if (is_cond(exp))
        ans=eval_cond(exp,env);
      else
        if (is_let(exp))
          ans=eval_let(exp,env);
        else
          if (is_application(exp))//todo let* letrc
            ans=eval_apply(exp,env);
  while (ans->type==PAIR && car(ans)==&tail){
    obj_t *next_ans;
    exp=cadr(ans);
    env=cdr(cdr(ans));
    if (self_evaluating(exp))
      next_ans=exp;
    else
      if (variable(exp))
        next_ans=lookup_variable_value(exp,env);
      else
        if (quoted(exp))
          next_ans=text_of_quotation(exp);
        else
          if (assignment(exp))
            next_ans=eval_assignment(exp,env);
          else
            if (definition(exp))
              next_ans=eval_definition(exp,env);
            else
              if (is_lambda(exp))
                next_ans=eval_lambda(exp,env);
              else
                if (is_if(exp))
                  next_ans=eval_if(exp,env);
                else
                  if (is_begin(exp))
                    next_ans=eval_begin(exp,env);
                  else
                    if (is_cond(exp))
                      next_ans=eval_cond(exp,env);
                    else
                      if (is_let(exp))
                        next_ans=eval_let(exp,env);
                      else
                        if (is_application(exp))
                          next_ans=eval_apply(exp,env);
    next_ans->ref++;
    del_obj(ans);
    next_ans->ref--;
    ans=next_ans;
  }
  return ans;
}
obj_t *eval_assignment(obj_t *exp,obj_t *env){
  set_variable_value(assignment_variable(exp),eval(assignment_value(exp),env),env);
  return &undef;
}
obj_t *eval_definition(obj_t *exp,obj_t *env){
  obj_t *val_exp=definition_value(exp);
  define_variable(definition_variable(exp),eval(val_exp,env),env);
  del_obj(val_exp);
  return &undef;
}
obj_t *eval_if(obj_t *exp,obj_t *env){
  obj_t *test_result=eval(if_predicate(exp),env);
  int true_flag=is_true(test_result);
  del_obj(test_result);
  if (true_flag)
    return make_tail(if_consequent(exp),env);
  else
    return make_tail(if_alternative(exp),env);
}
obj_t *eval_lambda(obj_t *exp,obj_t *env){
  return make_procedure(lambda_parameters(exp),lambda_body(exp),env);
}
obj_t *eval_begin(obj_t *exp,obj_t *env){
  return eval_sequence(begin_actions(exp),env);
}
obj_t *eval_sequence(obj_t *exp,obj_t *env){
  if (exp==&nil)
    return &undef;
  while (cdr(exp)!=&nil){
    del_obj(eval(car(exp),env));
    exp=cdr(exp);
  }
  return make_tail(car(exp),env);
}
obj_t *eval_cond(obj_t *exp,obj_t *env){
  exp=cdr(exp);
  while (exp!=&nil){
    obj_t *cond_clause=car(exp);
    obj_t *cond_predicate=car(cond_clause);
    obj_t *cond_actions=cdr(cond_clause);
    if (!objSymbolEqString(cond_predicate,"else")){
      obj_t *test_result=eval(cond_predicate,env);
      int true_flag=is_true(test_result);
      del_obj(test_result);
      if (true_flag)
        return eval_sequence(cond_actions,env);
      exp=cdr(exp);
    }
    else
      return eval_sequence(cond_actions,env);
  }
  return &undef;
}
obj_t *eval_let(obj_t *exp,obj_t *env){
  obj_t *combination=let2combination(exp);
  obj_t *ans=eval_apply(combination,env);//todo
  del_obj(combination);
  return ans;
}
obj_t *list_of_values(obj_t *list,obj_t *env){
  obj_t *head=NULL,*tail;
  while (list!=&nil){
    obj_t *ans_element=eval(car(list),env);
    obj_t *newpair=new_pair();
    newpair->data.pair->car=ans_element;
    ans_element->ref++;
    if (head==NULL)
      head=tail=newpair;
    else{
      tail->data.pair->cdr=newpair;
      newpair->ref++;
      tail=newpair;
    }
    list=cdr(list);
  }
  if (head==NULL)
    return &nil;
  else{
    tail->data.pair->cdr=&nil;
    nil.ref++;
    return head;
  }
}
obj_t *eval_apply(obj_t *exp,obj_t *env){
  obj_t *values=list_of_values(operands(exp),env);
  obj_t *op=eval(operator(exp),env);
  obj_t *ans=apply(op,values);
  ans->ref++;
  del_obj(op);
  del_obj(values);
  ans->ref--;
  return ans;
}
obj_t *apply(obj_t *procedure,obj_t *arguments){
  if (procedure->type==PRIMITIVE)
    return apply_primitive_procedure(procedure,arguments);
  if (procedure->type==PROCEDURE){
    obj_t *extended_environment=extend_environment(procedure_parameters(procedure),arguments,procedure_environment(procedure));
    obj_t *ans=eval_sequence(procedure_body(procedure),extended_environment);
    ans->ref++;
    del_obj(extended_environment);
    ans->ref--;
    return ans;
  }
  error("unknown procedure type!\n");
  return NULL;
}
obj_t *apply_primitive_procedure(obj_t *procedure,obj_t *arguments){
  return procedure->data.primitive->fun(arguments);
}
obj_t *extend_environment(obj_t *vars,obj_t *vals,obj_t *base_env){
  obj_t *newenv=new_env();
  newenv->data.env->outside=base_env;
  base_env->ref++;
  while (vars!=&nil&&vals!=&nil){
    obj_t *var=car(vars);
    obj_t *val=car(vals);
    define_variable(var,val,newenv);
    vars=cdr(vars);
    vals=cdr(vals);
  }
  if (vars!=&nil){
    error("too few arguments supplied!\n");
    return NULL;
  }
  if (vals!=&nil){
    error("too many arguments supplied!\n");
    return NULL;
  }
  return newenv;
}
