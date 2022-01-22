#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>

using namespace std;

int main(int argc, char **argv) {
    
    int rank, n_ranks;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_ranks);

    cout << "Hello World! I'm rank " << rank << endl;
    cout << "Total no.of ranks = " << n_ranks << endl;

    MPI_Finalize();
    return 0;
}