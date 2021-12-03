#include <config.h>
#if defined (HAVE_UNISTD_H)
#  include <unistd.h>
#endif
#include <stdio.h>
#include "builtins.h"
#include "shell.h"
#include "bashgetopt.h"
#include "common.h"

#define INIT_DYNAMIC_VAR(var, val, gfunc, afunc)   \
    do                                             \
    { SHELL_VAR *v = bind_variable(var, (val), 0); \
      if (v)                                       \
      {                                            \
          v->dynamic_value = gfunc;                \
          v->assign_func   = afunc;                \
      }                                            \
    }                                              \
    while (0)


static SHELL_VAR * assign_var (
     SHELL_VAR *self,
     char *value,
     arrayind_t unused,
     char *key ) {
  return (self);
}

char * get_ts_val(){
  char *p;
  struct timeval tv = { 0 };
  if (gettimeofday(&tv, NULL) < 0) {
    builtin_error("Failed to get time of day: %m");
    return NULL;
  }
  int TS = tv.tv_sec;
  if (asprintf(&p, "%d", TS) < 0) {
    builtin_error("Failed to get memory for time of day: %m");
    return NULL;
  }
  return p;
}

long currentTimeMillis() {
  struct timeval time;
  gettimeofday(&time, NULL);
  return time.tv_sec * 1000 + time.tv_usec / 1000;
}

char * get_ms_val(){
  char *p;
  struct timeval tv = { 0 };
  if (gettimeofday(&tv, NULL) < 0) {
    builtin_error("Failed to get time of day: %m");
    return NULL;
  }
  long TS = currentTimeMillis();
  if (asprintf(&p, "%ld", TS) < 0) {
    builtin_error("Failed to get memory for time of day: %m");
    return NULL;
  }
  return p;
}

static SHELL_VAR * get_ms (SHELL_VAR *var) {
  VSETATTR(var, att_integer);
  var_setvalue(var, get_ms_val());
  return var;
}

static SHELL_VAR * get_ts (SHELL_VAR *var) {
  VSETATTR(var, att_integer);
  var_setvalue(var, get_ts_val());
  return var;
}

int ts_builtin (list)     WORD_LIST *list;{
    int qty = 0;
    for (int i = 1; list != NULL; list = list->next, ++i){
      qty++;
    }
    if (qty == 0){
      fprintf(stdout, "%s\n", get_ts_val());
    }else{
      fprintf(stdout, "%s\n", get_ms_val());
    }
    fflush (stdout);
    return (EXECUTION_SUCCESS);
}


int ts_builtin_load (s) char *s; {
  unbind_variable("TS");
  unbind_variable("MS");
  INIT_DYNAMIC_VAR ("TS", (char *)NULL, get_ts, assign_var);
  INIT_DYNAMIC_VAR ("MS", (char *)NULL, get_ms, assign_var);
  fprintf (stderr, "ts builtin loaded- vars MS and TS are dynacally set\n");
  fflush (stdout);
  fflush (stderr);
  return (1);
}

void ts_builtin_unload (s) char *s; {
  kill_all_local_variables();
  unbind_variable("TS");
  unbind_variable("MS");
  printf ("ts builtin unloaded\n");
  fflush (stdout);
}

char *ts_doc[] = {
	"Time Bash Builtin.",
	"",
	"Create Dynamic Variable containing epoch. TS and MS",
	(char *)NULL
};

struct builtin ts_struct = {
	"ts",
	ts_builtin,	
	BUILTIN_ENABLED,
	ts_doc,
	"ts",
	0	
};
