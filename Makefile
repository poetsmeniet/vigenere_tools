NAME = vtools 

CODE_FILES = main.c tableaugen.c generic.c kasinskiexam.c

DEBUG = YES
ifeq ($(DEBUG),YES)
		D = -g
	else
		D =
	endif

.PHONY: all clean docs

all: $(CODE_FILES)
		gcc -g -Wall -Warray-bounds=2 -O3 -o $(NAME) $(CODE_FILES)
		#gcc -g -Wall -Warray-bounds=2 -O3 -o a.out kasinskiexam.c generic.c

#	clean:
#	rm -rf $(NAME) $(DOC_FILES) *.o
