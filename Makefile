all:
	gcc lsCommand.c -o ls
	gcc dateCommand.c -o date
	gcc mkdirCommand.c -o mkdir
	gcc rmCommand.c -o rm
	gcc catCommand.c -o cat
	gcc main.c -o output.out
	./output.out