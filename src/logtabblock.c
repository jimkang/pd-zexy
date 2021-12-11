/*
 * logtabblock: Log the last block in the signal to a text file.
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
#include "zexy.h"


/* =================== logtabblock ====================== */

static t_class *logtabblock_class=NULL;

typedef struct _logtabblock {
  t_object x_obj;
  t_symbol *x_arrayname;
  t_int startindex, stopindex;
} t_logtabblock;

static void logtabblock_bang(t_logtabblock *x)
{
  t_garray *A;
  int npoints;
  t_word *vec;

  if (!(A = (t_garray *)pd_findbyclass(x->x_arrayname, garray_class))) {
    error("%s: no such array", x->x_arrayname->s_name);
  } else if (!garray_getfloatwords(A, &npoints, &vec)) {
    error("%s: bad template for logtabblock", x->x_arrayname->s_name);
  } else {
    int n;
    t_atom *atombuf;

    int start=x->startindex;
    int stop =x->stopindex;
    if(start<0||start>stop) {
      start=0;
    }
    if(stop<start||stop>npoints) {
      stop=npoints;
    }
    npoints=stop-start;

    char logPath[MAXPDSTRING];
    sprintf(logPath, "/home/jimkang/gcw/vocode/pd-logs/%s.txt", x->x_arrayname->s_name);
    FILE *f = fopen(logPath, "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    atombuf = (t_atom *)getbytes(sizeof(t_atom)*npoints);
    for (n = 0; n < npoints; n++) {
      float sample = vec[start+n].w_float;
      SETFLOAT(&atombuf[n], sample);
      fprintf(f, "%f\n", sample);
    }
    fclose(f);

    outlet_list(x->x_obj.ob_outlet, gensym("list"), npoints, atombuf);
    freebytes(atombuf,sizeof(t_atom)*npoints);
  }
}

static void logtabblock_list(t_logtabblock *x, t_symbol* UNUSED(s),int argc,
                         t_atom*argv)
{
  int a,b;
  switch(argc) {
  case 2:
    a=atom_getint(argv);
    b=atom_getint(argv+1);
    x->startindex=(a<b)?a:b;
    x->stopindex =(a>b)?a:b;
    logtabblock_bang(x);
    break;
  default:
    error("logtabblock: list must be 2 floats (is %d atoms)", argc);
  }
}

static void logtabblock_set(t_logtabblock *x, t_symbol *s)
{
  x->x_arrayname = s;
}

static void *logtabblock_new(t_symbol *s)
{
  t_logtabblock *x = (t_logtabblock *)pd_new(logtabblock_class);
  x->x_arrayname = s;
  x->startindex=0;
  x->stopindex=-1;
  outlet_new(&x->x_obj, gensym("list"));

  return (x);
}

static void logtabblock_helper(void)
{
  post("\n"HEARTSYMBOL
       " logtabblock - object : dumps a table as a package of floats");
  post("'set <table>'\t: read out another table\n"
       "'bang'\t\t: dump the table\n"
       "outlet\t\t: table-data as package of floats");
  post("creation\t: \"logtabblock <table>\"");

}

ZEXY_SETUP void logtabblock_setup(void)
{
  logtabblock_class = zexy_new("logtabblock",
                           logtabblock_new, 0, t_logtabblock, 0, "S");
  class_addbang(logtabblock_class, (t_method)logtabblock_bang);
  class_addlist(logtabblock_class, (t_method)logtabblock_list);

  zexy_addmethod(logtabblock_class, (t_method)logtabblock_set, "set", "s");

  zexy_addmethod(logtabblock_class, (t_method)logtabblock_helper, "help", "");
  zexy_register("logtabblock");
}
