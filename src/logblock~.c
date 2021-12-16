/*
 * logblock~: Log a target block to a file.
 *
 * (c) 1999-2011 IOhannes m zmölnig, forum::für::umläute, institute of electronic music and acoustics (iem)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* ------------------------ logblock~ ----------------------------- */
/*
  a very useful function, which detects, whether a signal is zeroes-only this block or not
  this is really great together with the "switch~"-object
*/

#include "zexy.h"

static t_class *logblock_class=NULL;

typedef struct _logblock {
  t_object x_obj;
  t_float targetIndex;
  t_symbol *logname;
  int activate;
  int currentIndex; /* 0 == (signalblock == 0); 1==(signalblock != 0) */
} t_logblock;

static void logblock_banged(t_logblock *x)
{
  x->activate = x->activate ? 0 : 1;
  if (!x->activate) {
    x->currentIndex = 0;
  }
  printf("activate: %d\n", x->activate);
}

static t_int *logblock_perform(t_int *w)
{
  t_sample *in = (t_sample *)w[1];
  t_logblock *x = (t_logblock *)w[2];
  printf("targetIndex: %f, logname: %s\n", x->targetIndex, x->logname->s_name);
  int n = (int)w[3];

  if (x->activate) {
    x->currentIndex += 1;

    //int non_zero = 0;
    //while (n--) {
      //if (*in++ != 0.) {
        //non_zero = 1;
        //break;
      //}
    //}
    if (x->currentIndex == x->targetIndex) {
      printf("This is it: %d", x->currentIndex);
    }
    printf("Current index: %d |", x->currentIndex);
  }

  return (w+4);
}

static void logblock_dsp(t_logblock *x, t_signal **sp)
{
  dsp_add(logblock_perform, 3, sp[0]->s_vec, x, sp[0]->s_n);
}


static void logblock_tilde_helper(void)
{
  post("\n"HEARTSYMBOL
       " logblock~-object :: log a target block to a file");
  post("'bang'\t: turn the block counter on\n"
       "'off'\t: turn it off\n"
       "'targetIndex'\t: index of the block to log\n"
       "'logname'\t: the name of the log file\n"
       "'help'\t: view this\n"
       "signal~");
}

//static void logblock_set(t_logblock *x, t_float *f, t_symbol *s) {
static void logblock_set(t_logblock *x, t_symbol *s) {
  //x->targetIndex = *f;
  x->logname = s;
}

//static void *logblock_new(t_float *f, t_symbol *s)
static void *logblock_new(t_symbol *s)
{
  t_logblock *x = (t_logblock *)pd_new(logblock_class);
  //x->targetIndex = *f;
  x->targetIndex = 55555;
  x->logname = s;
  x->activate = 0;
  return (x);
}

ZEXY_SETUP void logblock_tilde_setup(void)
{
  // TODO: dtor
  logblock_class = zexy_new("logblock~",
                           logblock_new, 0, t_logblock, 0, "S");
  class_addbang(logblock_class, logblock_banged);

  zexy_addmethod(logblock_class, (t_method)nullfn, "signal", "");
  zexy_addmethod(logblock_class, (t_method)logblock_dsp, "dsp", "!");
  zexy_addmethod(logblock_class, (t_method)logblock_set, "set", "s");

  zexy_addmethod(logblock_class, (t_method)logblock_tilde_helper, "help", "");
  zexy_register("logblock~");
}
