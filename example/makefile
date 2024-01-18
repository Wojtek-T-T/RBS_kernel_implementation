$(CC) = gcc -O3
CFLAGS = -I -lpthread -lrt

rbs_app: main.o sequences.o workload.o ../otw/otw.o ../rbs_lib_kernel/rbs_lib_kernel.o
	$(CC) $(CFLAGS) main.o sequences.o workload.o ../otw/otw.o ../rbs_lib_kernel/rbs_lib_kernel.o -o rbs_app -lpthread -lrt

main.o: main.c sequences.h
	$(CC) $(CFLAGS) -c main.c

sequences.o: sequences.c workload.h ../rbs_lib_kernel/rbs_lib_kernel.h
	$(CC) $(CFLAGS) -c sequences.c

rbs_lib.o: ../rbs_lib_kernel/rbs_lib_kernel.c ../rbs_lib_kernel/rbs_lib_kernel.h
	$(CC) $(CFLAGS) -c ../rbs_lib_kernel/rbs_lib_kernel.c

workload.o: workload.c workload.h ../otw/otw.h
	$(CC) -c workload.c

otw.o: ../otw/otw.c ../otw/otw.h
	$(CC) -c ../otw/otw.c

clean:
	rm *.o rbs_app

#gcc -o rbs_app main.c rbs_lib.c workload.c sequences.c otw.c -lpthread -lrt