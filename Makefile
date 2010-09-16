##
## Makefile
##  
## Made by wingsit
## Login   <wingsitwingsit-desktop>
##
## Started on  Wed Sep 15 17:45:25 2010 wingsit
## Last update Wed Sep 15 17:45:25 2010 wingsit
## 
##############################
# Complete this to make it ! #
##############################
NAME 	= qp	# Name of executable file
SRC	= uQuadProg++.cc umain.cc EigenQP.cpp	# List of *.c
INCL  	= uQuadProg++.hh EigenQP.h		# List of *.h
################
# Optional add #
################
IPATH   = -I. -I./eigen/          # path of include file
OBJOPT  = -O4 -Wall -msse2 -fopenmp      # option for obj
EXEOPT  = -O4 -Wall -msse2 -fopenmp     # option for exe (-lefence ...)
LPATH   = -L.           # path for librairies ... 

#####################
# Macro Definitions #
#####################
CC 	= g++
MAKE 	= make
SHELL	= /bin/sh
OBJS 	= $(SRC:.c=.o) 
RM 	= /bin/rm -f 	
COMP	= gzip -9v
UNCOMP	= gzip -df
STRIP	= strip

CFLAGS  = $(OBJOPT) $(IPATH)
LDFLAGS = $(EXEOPT) $(LPATH)

.SUFFIXES: .h.Z .c.Z .h.gz .c.gz .c.z .h.z 

##############################
# Basic Compile Instructions #
##############################

all:	$(NAME)
$(NAME): $(OBJS) $(SRC) $(INCL)  
	$(CC) $(IPATH)  $(OBJS) $(LDFLAGS) -o $(NAME) 
#	$(STRIP) ./$(NAME) # if you debug ,don't strip ...

depend:
	g++ $(IPATH) -MM $(SRC) 
clean:
	-$(RM) $(NAME) $(OBJS) *~
fclean:
	-$(RM) $(NAME)
comp: clean
	$(COMP) $(INCL) $(SRC)
ucomp: 
	$(UNCOMP) $(SRC) $(INCL)

.c.Z.c .h.Z.h .c.gz.c .h.gz.h .c.z.c .h.z.h :
	 -$(UNCOMP) $<

.c.o:
	$(CC) $(INCLUDE) $(CFLAGS) -c $< 
################
# Dependencies #
################
