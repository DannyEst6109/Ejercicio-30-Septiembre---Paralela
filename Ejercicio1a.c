/*----------------------------------------------------------------------
 * Universidad del Valle de Guatemala
 * Curso: CC3169 - Computacion Paralela y Distribuida
 * Ejercicio 30 de septiembre - Caso 1
 * Descripcion: recoleccion de dos temperaturas por sucursal usando MPI_Gather.
 *----------------------------------------------------------------------*/
#include <stdio.h>
#include <mpi.h>

#define NUM_PROCESOS 4
#define MEDICIONES_POR_PROCESO 2

int main(int argc, char *argv[]) {
    int rank, size;
    float mediciones[MEDICIONES_POR_PROCESO];
    float temperaturas[NUM_PROCESOS * MEDICIONES_POR_PROCESO];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != NUM_PROCESOS) {
        if (rank == 0) printf("Este programa requiere exactamente %d procesos.\n", NUM_PROCESOS);
        MPI_Finalize();
        return 0;
    }

    /* Cada sucursal toma dos mediciones locales. */
    if (rank == 0) { mediciones[0] = 24.5f; mediciones[1] = 25.0f; }
    else if (rank == 1) { mediciones[0] = 26.1f; mediciones[1] = 26.4f; }
    else if (rank == 2) { mediciones[0] = 23.8f; mediciones[1] = 24.1f; }
    else { mediciones[0] = 27.0f; mediciones[1] = 27.3f; }

    printf("Proceso %d: %.1f C, %.1f C\n", rank, mediciones[0], mediciones[1]);
    /* El proceso raiz recibe dos valores consecutivos de cada proceso. */
    MPI_Gather(mediciones, MEDICIONES_POR_PROCESO, MPI_FLOAT,
               temperaturas, MEDICIONES_POR_PROCESO, MPI_FLOAT,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("\nOficina Central: temperaturas recibidas\n");
        for (int i = 0; i < size; i++) {
            printf("Proceso %d: %.1f C, %.1f C\n", i,
                   temperaturas[i * MEDICIONES_POR_PROCESO],
                   temperaturas[i * MEDICIONES_POR_PROCESO + 1]);
        }
    }
    MPI_Finalize();
    return 0;
}
