CU_APPS=openacc_sieve_of_eratosthenes
C_APPS=pthread_sieve_of_eratosthenes

all: ${C_APPS} ${CU_APPS}

%: %.cu
	nvcc -g -G -o $@ $<

%: %.cpp
	g++ -pthread -g -o $@ $<

clean:
	rm -f ${CU_APPS} ${C_APPS}