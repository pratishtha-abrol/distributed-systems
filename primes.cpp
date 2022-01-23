#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <fstream>

using namespace std;

int isprime(int N, int rank, int size) {
    int h = N / 2;
    int k = h / size + 1;
    int upper = (rank + 1) * k + 1;
    int lower = rank * k + 2;

    upper  = (upper > h) ? h : upper; 

    lower = (rank == 0) ? 2 : lower;

    int i = lower;

    while(i <= upper){
        if (N % i == 0)
            return 1;
        i++;
    }
    return 0;
}

int main(int argc, char **argv) {
    
    int rank, num_procs;

    // initiate MPI
    MPI_Init(&argc, &argv);

    // get size of the current communicator
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    
    // get current process rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // synchronize all processes
    MPI_Barrier( MPI_COMM_WORLD );
    double start_time = MPI_Wtime();

    // cout << "Hello World! I'm rank " << rank << endl;
    // cout << "Total no.of ranks = " << n_ranks << endl;

    int a, i, result = 0;

    if(argc != 3) {
        cout << "Only 2 arguments expected." << endl;
        return 0;
    }
    ifstream infile (argv[1]);
    if (!infile) {
        cout << "Unable to find file" << endl;
        return 0;
    }
    ofstream outfile (argv[2]);
    while(infile >> a) {
        // cout << a << endl;
        if (!rank) {
            i = 1;
            while (i<num_procs) {
                MPI_Send(&a, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                i++;
            }
            result = isprime(a, rank, num_procs);

            i=1;
            int result_slave = 0;
            while (i<num_procs) {
                MPI_Recv(&result_slave, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                result += result_slave;
                i++;
            }

            if (result == 0) {
                outfile << "YES" << endl;
            } else {
                outfile << "NO" << endl;
            }
        } else {
            MPI_Recv(&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            result = isprime(a, rank, num_procs);
            MPI_Send(&result, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }
    }
    outfile.close();
    infile.close();
    

    // get max program run time for all processes
    MPI_Barrier( MPI_COMM_WORLD );
    double end_time = MPI_Wtime() - start_time;
    double maxTime;
    MPI_Reduce( &end_time, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );
    if ( rank == 0 ) {
        cout<<"Total time (s): "<<maxTime<<"\n";
    }

    // shut down MPI and close
    MPI_Finalize();
    return 0;
}