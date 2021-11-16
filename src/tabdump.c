/*
 * tabdump: get the content of a table as a list of floats
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


/* =================== tabdump ====================== */

static t_class *tabdump_class=NULL;

typedef struct _tabdump {
  t_object x_obj;
  t_symbol *x_arrayname;
  t_int startindex, stopindex;
} t_tabdump;

static void tabdump_bang(t_tabdump *x)
{
  t_garray *A;
  int npoints;
  t_word *vec;

  printf("Hey0");

  if (!(A = (t_garray *)pd_findbyclass(x->x_arrayname, garray_class))) {
    error("%s: no such array", x->x_arrayname->s_name);
  } else if (!garray_getfloatwords(A, &npoints, &vec)) {
    error("%s: bad template for tabdump", x->x_arrayname->s_name);
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

    FILE *f = fopen("/home/jimkang/gcw/vocode/pd-logs/tmp.txt", "w");
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

static void tabdump_list(t_tabdump *x, t_symbol* UNUSED(s),int argc,
                         t_atom*argv)
{
  int a,b;
  switch(argc) {
  case 2:
    a=atom_getint(argv);
    b=atom_getint(argv+1);
    x->startindex=(a<b)?a:b;
    x->stopindex =(a>b)?a:b;
    tabdump_bang(x);
    break;
  default:
    error("tabdump: list must be 2 floats (is %d atoms)", argc);
  }
}

static void tabdump_set(t_tabdump *x, t_symbol *s)
{
  x->x_arrayname = s;
}

static void *tabdump_new(t_symbol *s)
{
  t_tabdump *x = (t_tabdump *)pd_new(tabdump_class);
  x->x_arrayname = s;
  x->startindex=0;
  x->stopindex=-1;
  outlet_new(&x->x_obj, gensym("list"));

  return (x);
}

static void tabdump_helper(void)
{
  post("\n"HEARTSYMBOL
       " tabdump - object : dumps a table as a package of floats");
  post("'set <table>'\t: read out another table\n"
       "'bang'\t\t: dump the table\n"
       "outlet\t\t: table-data as package of floats");
  post("creation\t: \"tabdump <table>\"");

}

ZEXY_SETUP void tabdump_setup(void)
{
  tabdump_class = zexy_new("tabdump",
                           tabdump_new, 0, t_tabdump, 0, "S");
  class_addbang(tabdump_class, (t_method)tabdump_bang);
  class_addlist(tabdump_class, (t_method)tabdump_list);

  zexy_addmethod(tabdump_class, (t_method)tabdump_set, "set", "s");

  zexy_addmethod(tabdump_class, (t_method)tabdump_helper, "help", "");
  zexy_register("tabdump");
}
