tbucket: tbucket.o processInit.o packettransfer.o packet_generation.o token_generation.o server.o my402list.o const.o util.o submiterror.o siginthandler.o
	gcc -o tbucket -g tbucket.o processInit.o packettransfer.o packet_generation.o token_generation.o server.o my402list.o const.o util.o submiterror.o siginthandler.o -pthread -lrt -lm -D_POSIX_PTHREAD_SEMANTICS

tbucket.o: tbucket.c commonheader.h
	gcc -g -c -Wall tbucket.c
	
processInit.o: processInit.c commonheader.h
	gcc -g -c -Wall processInit.c
	
packettransfer.o: packettransfer.c commonheader.h
	gcc -g -c -Wall packettransfer.c
				
my402list.o: my402list.c my402list.h
	gcc -g -c -Wall my402list.c
	
const.o: const.c commonheader.h
	gcc -g -c -Wall const.c
	
util.o: util.c commonheader.h
	gcc -g -c -Wall util.c
	
packet_generation.o: packet_generation.c commonheader.h
	gcc -g -c -Wall packet_generation.c	
	
token_generation.o: token_generation.c commonheader.h
	gcc -g -c -Wall token_generation.c	
	
server.o: server.c commonheader.h
	gcc -g -c -Wall server.c	

submiterror.o: submiterror.c commonheader.h
	gcc -g -c -Wall submiterror.c

siginthandler.o: siginthandler.c commonheader.h
	gcc -g -c -Wall siginthandler.c -D_POSIX_PTHREAD_SEMANTICS
	

clean:
	rm -f *.o warmup2
