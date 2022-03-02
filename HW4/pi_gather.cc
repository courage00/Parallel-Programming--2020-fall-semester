#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    // --- DON'T TOUCH ---
    MPI_Init(&argc, &argv);
    double start_time = MPI_Wtime();
    double pi_result;
    long long int tosses = atoi(argv[1]);
    int world_rank, world_size;
    // ---

    // TODO: MPI init
    MPI_Comm_rank( MPI_COMM_WORLD , &world_rank);
    MPI_Comm_size( MPI_COMM_WORLD , &world_size );
    long long int numInCircleArray [world_size];
    long long int partTossing = tosses/world_size;
    long long int numInCircle = 0;
    long long int sum = 0;
    float x = 0.f, y = 0.f;
    double distanceSquared=0;
    unsigned int seed = time(NULL) + world_rank * world_rank * 6000; //隨機性

    for (int i = 0; i < partTossing; i++)
    {
        x = (float)rand_r(&seed) / RAND_MAX;
        y = (float)rand_r(&seed) / RAND_MAX;
        distanceSquared=x * x + y * y;
        if (distanceSquared <= 1)
        {
            numInCircle++;
        }
    }
    MPI_Gather(&numInCircle,1,MPI_LONG_LONG,&numInCircleArray,1,MPI_LONG_LONG,0,MPI_COMM_WORLD);
    if (world_rank == 0)
    { 
        // TODO: PI result
        for (int i = 0; i < world_size; i++)
        {
            sum += numInCircleArray[i];
        }
        pi_result = (4.0 * (double)sum / (double)tosses);
        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }
    
    MPI_Finalize();
    return 0;
}
