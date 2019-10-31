ping_pong:
	mpicc ping_pong.c -o out cronometer.c cronometer.h
	mpirun -np 2 ./out

ring:
	mpicc ring.c -o out cronometer.c cronometer.h
	mpirun -np 4 ./out

clean:
	rm -f *.o *out *.gch
