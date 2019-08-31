mine_sweeper.out: playgame.o makeboard.o main.o
	gcc -g -Wall -Werror -o mine_sweeper.out playgame.o makeboard.o main.o
	
playgame.o: playgame.c playgame.h makeboard.h
	gcc -g -Wall -Werror -c playgame.c
	
makeboard.o: makeboard.c makeboard.h 
	gcc -g -Wall -Werror -c makeboard.c
	
main.o: main.c makeboard.h playgame.h
	gcc -g -Wall -Werror -c main.c 
	
clean:
	rm -fr *.o *.out