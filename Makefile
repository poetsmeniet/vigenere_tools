NAME = vtools 

CODE_FILES = main.c tableaugen.c

DEBUG = YES
ifeq ($(DEBUG),YES)
		D = -g
	else
		D =
	endif

.PHONY: all clean docs

all: $(CODE_FILES)
		gcc -g -Wall -o $(NAME) $(CODE_FILES)

#	clean:
#	rm -rf $(NAME) $(DOC_FILES) *.o
