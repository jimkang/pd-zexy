#!/bin/sh

ZEXY_H=z_zexy.h
ZEXY_C=z_zexy.c
ZEXY_S=Make.source


GREP=grep
AWK=awk
SED=sed

#################################
## functions

function head_h() {
 echo "/* zexy-setup autogenerated header-file"
 echo " * generated by \"$0\""
 echo " * !! DO NOT MANUALLY EDIT  !!"
 echo " */"
 echo
 echo "#ifndef Z_ZEXY_H__"
 echo "#define Z_ZEXY_H__"
}

function foot_h() {
 echo "#endif /* Z_ZEXY_H__ */"
 echo ""
}

function head_c() {
 echo "/* zexy-setup autogenerated setup-file"
 echo " * generated by \"$0\""
 echo " * !! DO NOT MANUALLY EDIT  !!"
 echo " */"
 echo 
 echo "#include \"$ZEXY_H\""
 echo
 echo "void z_zexy_setup(void)"
 echo "{"
}

function foot_c() {
 echo "}"
 echo
}

function head_s() {
  echo "SOURCES = \\"
}

function foot_s() {
  echo "	zexy.c"
  echo
}


##################################
## body

head_h > $ZEXY_H
head_c > $ZEXY_C
head_s > $ZEXY_S

for i in `ls *.c | grep -v zexy.c`
do
## each c-file in zexy needs to have a z_<file>_setup()-function
## that calls all needed setup-functions
## any non-alpha-numeric-character is replaced by "_"
## e.g. "multiplex~.c" -> "z_multiplex__setup()"
  SETUPNAME=z_`echo ${i%.c} | sed -e 's/[^[:alnum:]]/_/g'`_setup
  echo "	$i \\" >> $ZEXY_S
  echo "void ${SETUPNAME}(void); /* $i */" >> $ZEXY_H
  echo "	${SETUPNAME}(); /* $i */" >> $ZEXY_C
done

foot_h >> $ZEXY_H
foot_c >> $ZEXY_C
foot_s >> $ZEXY_S

