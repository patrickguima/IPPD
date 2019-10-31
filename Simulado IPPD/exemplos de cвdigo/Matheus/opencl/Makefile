hello:
	#g++ -o out hello.cpp -lOpenCL
	#g++ hello.cpp -o out -lOpenCL
	g++ -I/usr/include/CL/ hello.cpp -o out -lOpenCL
	#g++ -I/usr/include/CL/ hello.cpp -lOpenCL
	./out

heldalo:
	gcc hello.c -o out -fopenmp -Wall
	./out

somavet:
	g++ -I/usr/include/CL/ soma2vetores.cpp -o out -lOpenCL
	./out

somafunfa:
	g++ -I/usr/include/CL/ somafunfa.cpp -o out -lOpenCL
	#./out

findpath:
	find / -iname cl.h 2>/dev/null

hellopath:
	#g++ -I/usr/local/cuda-5.5/include main.c -lOpenCL
	g++ -I/usr/include/CL/ hello.cpp -lOpenCL
clean:
	rm -f *.o *out *.gch
