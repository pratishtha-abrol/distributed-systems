#include <mpi.h>
#include "bits/stdc++.h"

using namespace std;

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

    // code here
    if(argc != 3) {
        cout << "Only 2 arguments expected." << endl;
        return 0;
    }
    fstream infile;
    infile.open(argv[1], ios::in);

    fstream outfile;
    outfile.open(argv[2], ios::out);

    int n;
    double X[102][102], Y[102], trans[100010];

    if (!rank) {
        infile >> n;
    
        for (int i=0; i<n; i++) {
            for (int j=0; j<n+1; j++) {
                if (j == n) {
                    infile >> Y[i];
                    // cout << Y[i];
                } else {
                    infile >> X[i][j];
                }
            }
        }

        set<pair<int, int>> heaps;
        int nz[n];
        for (int i=0; i<n; i++) {
            int c=0;
            for (int j=0; j<n; j++) {
                c += X[j][i] !=0;
            }
            heaps.insert(make_pair(c,i));
            nz[i] = c;
        }

        while(!heaps.empty()) {
            pair<int,int> cur = *heaps.begin();
            heaps.erase(heaps.begin());
            int i = cur.second;
            double maxi =-1e5;
            int row = -1;
            for (int j=0; j<n; j++) {
                if (nz[j] <= 0) continue;
                if (X[j][i] > maxi) {
                    maxi = X[j][i];
                    row = j;
                }
            }
            for (int j=0; j<n; j++) {
                nz[j] -= X[row][j]!=0;
            }
            nz[i] = 0;
            heaps.clear();
            for (int i=0; i<n; i++) {
                if(nz[i] > 0) {
                    heaps.insert(make_pair(nz[i], i));
                }
            }
            if (row == 1) continue;
            for (int j=0; j<n; j++) {
                swap(X[i][j], X[row][j]);
            }
            swap(Y[i], Y[row]);
        }
        for (int i = 0; i<n; i++) {
            for (int j=0; j<n; j++) {
                trans[i*n+j] = X[i][j];
            }
        }

    }
    
    MPI_Bcast (&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast (trans, n*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast (Y, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    for (int i=0; i<n*n; i++) {
        X[i/n][i%n] = trans[i];
    }
    for (int i=0; i<n; i++) {
        MPI_Bcast(&X[i][i], n-i, MPI_DOUBLE, i%num_procs, MPI_COMM_WORLD);
        MPI_Bcast(&Y[i], 1, MPI_DOUBLE, i%num_procs, MPI_COMM_WORLD);
        for (int j = rank; j<n; j+=num_procs) {
            if (j<=i) continue;
            double r = 0;
            if (X[i][i])
                r = X[j][i]/X[i][i];
            for (int k=0; k<n; k++) {
                X[j][k] -= r*X[i][k];
            }
            Y[j] -= r*Y[i];
        }
    }
    
    if (!rank) {
        for (int i=n-1; i>=0; i--) {
            for (int j=i+1; j<n; j++) {
                Y[i] -=X[i][j]*Y[j];
            }
            Y[i] /= X[i][i];
        }

        outfile << fixed << setprecision(2);
        for (int i=0; i<n; i++) {
            outfile << Y[i] << " ";
        }
        outfile << "\n";
    }
    
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