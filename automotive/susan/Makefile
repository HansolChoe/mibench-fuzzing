CC=gcc
AFL_CC=afl-gcc

AFL_FLAG=-DAFL
GCOV_FLAG=-DGCOV

.PHONY: susan
susan: susan.c Makefile
	${CC} -static -O4 -o susan susan.c -lm 

gcov: susan.c Makefile
	${CC} -static -o susan driver.c susan.c -lm --coverage ${GCOV_FLAG}

afl: susan.c Makefile
	${AFL_CC} -static -o susan driver.c susan.c -lm ${AFL_FLAG}

clean:
	rm -rf susan output* *gcda *gcno *.gcov
