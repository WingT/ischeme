#include"stype.h"
pair_t nilpair={(obj_t *)0,(obj_t *)0};
obj_t nil={PAIR,1,{(long)&nilpair}};
obj_t undef={UNDEF,1,{0}};
obj_t false_obj={BOOLEAN,1,{0}};
obj_t true_obj={BOOLEAN,1,{1}};
env_t global_environment_body={(obj_t *)0,(epair_t *)0};
obj_t global_environment={ENVIRONMENT,1,{(long)&global_environment_body}};
obj_t tail={UNDEF,1,{0}};
