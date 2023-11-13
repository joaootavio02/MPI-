#include <iostream>
#include <mpi.h>
#include <cmath>

// Variáveis globais para armazenar o rank do processo e o tamanho
// do comunicador
int rank, size;

// Número de pontos em um lado. O número total de pontos
// será p_count * p_count.
constexpr int p_count = 512;

// Outras variáveis globais. Elas são lidas da linha de comando
int cutoff;
double min_x, max_x, min_y, max_y, dx, dy;

// O módulo de um número complexo
double modulo(double x, double y) {
    return sqrt(x*x + y*y);
} 

// Multiplicando um número complexo por ele mesmo
void auto_mul(double &x, double &y) {
    double ox = x*x - y*y;
    double oy = 2*x*y;
    x = ox;
    y = oy;
}

// Cálculo do número de iterações em um conjunto de pontos
// O resultado é armazenado em mset.
void calcular_mandelbrot(double *pontos, int npts, int mset[]) {
    for (int i = 0; i < npts; ++i) {
        double px = pontos[i*2];
        double py = pontos[i*2 + 1];

        int iteracao = 0;
        double zx = 0;
        double zy = 0;

        while (iteracao < cutoff) {
            auto_mul(zx, zy);
            zx += px;
            zy += py;
            double mod = modulo(zx, zy);

            if (mod > 2.0f)
                break;

            iteracao++;
        }

        if (iteracao == cutoff)
            mset[i] = -1; // Ponto está dentro do conjunto de Mandelbrot
        else
            mset[i] = iteracao; // Ponto está fora do conjunto de Mandelbrot após "iteracao" iterações
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Lendo parâmetros da linha de comando
    min_x = std::stod(argv[1]);
    max_x = std::stod(argv[2]);
    min_y = std::stod(argv[3]);
    max_y = std::stod(argv[4]);
    dx = max_x - min_x;
    dy = max_y - min_y;
    cutoff = std::stoi(argv[5]);

    int total_points = p_count * p_count;
    int points_per_process = total_points / size;
    int remaining = total_points % size;

    double *pontos = NULL;
    double *pontos_locais = new double[points_per_process * 2];
    int *mset_local = new int[points_per_process];

    if (rank == 0) {
        // Gerando coordenadas dos pontos
        pontos = new double[p_count * p_count * 2];
        for (int yp = 0; yp < p_count; ++yp) {
            double py = min_y + dy * yp / p_count;
            for (int xp = 0; xp < p_count; ++xp) {
                int lid = yp * p_count * 2 + xp * 2;
                pontos[lid] = min_x + dx * xp / p_count;
                pontos[lid + 1] = py;
            }
        }
    }

    // Distribuindo os pontos para os processos
    MPI_Scatter(pontos, points_per_process * 2, MPI_DOUBLE,
                pontos_locais, points_per_process * 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Calculando o conjunto de Mandelbrot local
    calcular_mandelbrot(pontos_locais, points_per_process, mset_local);

    int *mset = NULL;
    if (rank == 0) {
        mset = new int[total_points];
    }

    // Coletando os resultados de cada processo
    MPI_Gather(mset_local, points_per_process, MPI_INT, mset, 
               points_per_process, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Imprimindo o resultado para o processo mestre
        for (int i = 0; i < total_points; ++i) {
            std::cout << mset[i] << (i % p_count == p_count - 1 ? "\n" : " ");
        }
        delete[] pontos;
        delete[] mset;
    }

    delete[] pontos_locais;
    delete[] mset_local;

    MPI_Finalize();
    return 0;
}
