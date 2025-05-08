#include <iostream>
#include <pthread.h>
#include <cmath>
#include <sys/time.h>

using namespace std;

// Matrix dimensions and data
int **A, **B, **result_seq, **result_row, **result_col, **result_block, **result_cyclic;
int rowsA, colsA, rowsB, colsB;
const int NUM_THREADS = 512;  // Must be a perfect square for block partitioning

// Timing utility
double get_time() {
    timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

// Initialize matrices
void init_matrices() {
    A = new int*[rowsA];
    B = new int*[rowsB];
    
    for(int i=0; i<rowsA; i++) {
        A[i] = new int[colsA];
        for(int j=0; j<colsA; j++) 
            A[i][j] = rand() % 10;
    }
    
    for(int i=0; i<rowsB; i++) {
        B[i] = new int[colsB];
        for(int j=0; j<colsB; j++)
            B[i][j] = rand() % 10;
    }
}

// Reset result matrix
void reset_matrix(int** matrix) {
    for(int i=0; i<rowsA; i++)
        for(int j=0; j<colsB; j++)
            matrix[i][j] = 0;
}

// Sequential multiplication (baseline)
void seq_multiply() {
    for(int i=0; i<rowsA; i++) {
        for(int k=0; k<colsA; k++) {
            for(int j=0; j<colsB; j++) {
                result_seq[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// 1. Row-wise Partitioning
struct RowArgs { int start_row, end_row; };

void* row_multiply(void* arg) {
    RowArgs* args = (RowArgs*)arg;
    for(int i=args->start_row; i<args->end_row; i++) {
        for(int k=0; k<colsA; k++) {
            for(int j=0; j<colsB; j++) {
                result_row[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return nullptr;
}

// 2. Column-wise Partitioning
struct ColArgs { int start_col, end_col; };

void* col_multiply(void* arg) {
    ColArgs* args = (ColArgs*)arg;
    for(int j=args->start_col; j<args->end_col; j++) {
        for(int i=0; i<rowsA; i++) {
            int sum = 0;
            for(int k=0; k<colsA; k++) {
                sum += A[i][k] * B[k][j];
            }
            result_col[i][j] = sum;
        }
    }
    return nullptr;
}

// 3. Block Partitioning (2D)
struct BlockArgs { int start_row, end_row, start_col, end_col; };

void* block_multiply(void* arg) {
    BlockArgs* args = (BlockArgs*)arg;
    for(int i=args->start_row; i<args->end_row; i++) {
        for(int j=args->start_col; j<args->end_col; j++) {
            int sum = 0;
            for(int k=0; k<colsA; k++) {
                sum += A[i][k] * B[k][j];
            }
            result_block[i][j] = sum;
        }
    }
    return nullptr;
}

// 4. Cyclic Partitioning
struct CyclicArgs { int thread_id; };

void* cyclic_multiply(void* arg) {
    CyclicArgs* args = (CyclicArgs*)arg;
    for(int i=args->thread_id; i<rowsA; i += NUM_THREADS) {
        for(int k=0; k<colsA; k++) {
            for(int j=0; j<colsB; j++) {
                result_cyclic[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return nullptr;
}

// Verify results
bool verify(int** result) {
    for(int i=0; i<rowsA; i++) {
        for(int j=0; j<colsB; j++) {
            if(result_seq[i][j] != result[i][j]) 
                return false;
        }
    }
    return true;
}

int main() {
    cout << "Enter matrix A dimensions (rows cols): ";
    cin >> rowsA >> colsA;
    cout << "Enter matrix B dimensions (rows cols): ";
    cin >> rowsB >> colsB;

    if(colsA != rowsB) {
        cerr << "Dimension mismatch!\n";
        return 1;
    }

    // Allocate memory
    result_seq = new int*[rowsA];
    result_row = new int*[rowsA];
    result_col = new int*[rowsA];
    result_block = new int*[rowsA];
    result_cyclic = new int*[rowsA];
    
    for(int i=0; i<rowsA; i++) {
        result_seq[i] = new int[colsB]();
        result_row[i] = new int[colsB]();
        result_col[i] = new int[colsB]();
        result_block[i] = new int[colsB]();
        result_cyclic[i] = new int[colsB]();
    }

    init_matrices();

    // Sequential baseline
    double start = get_time();
    seq_multiply();
    double seq_time = get_time() - start;
    cout << "\nSequential time: " << seq_time << "s\n";

    // Parallel strategies
    const char* strategies[] = {"Row-wise", "Column-wise", "Block", "Cyclic"};
    double times[4];

    // 1. Row-wise
    reset_matrix(result_row);
    pthread_t row_threads[NUM_THREADS];
    RowArgs row_args[NUM_THREADS];
    int chunk = rowsA / NUM_THREADS;
    int rem = rowsA % NUM_THREADS;
    int start_row = 0;

    start = get_time();
    for(int i=0; i<NUM_THREADS; i++) {
        row_args[i] = {start_row, start_row + chunk + (i < rem)};
        pthread_create(&row_threads[i], nullptr, row_multiply, &row_args[i]);
        start_row = row_args[i].end_row;
    }
    for(auto& t : row_threads) pthread_join(t, nullptr);
    times[0] = get_time() - start;

    // 2. Column-wise
    reset_matrix(result_col);
    pthread_t col_threads[NUM_THREADS];
    ColArgs col_args[NUM_THREADS];
    chunk = colsB / NUM_THREADS;
    rem = colsB % NUM_THREADS;
    int start_col = 0;

    start = get_time();
    for(int i=0; i<NUM_THREADS; i++) {
        col_args[i] = {start_col, start_col + chunk + (i < rem)};
        pthread_create(&col_threads[i], nullptr, col_multiply, &col_args[i]);
        start_col = col_args[i].end_col;
    }
    for(auto& t : col_threads) pthread_join(t, nullptr);
    times[1] = get_time() - start;

    // 3. Block (2D)
    reset_matrix(result_block);
    pthread_t block_threads[NUM_THREADS];
    BlockArgs block_args[NUM_THREADS];
    int dim = sqrt(NUM_THREADS);
    int row_chunk = rowsA / dim;
    int col_chunk = colsB / dim;

    start = get_time();
    for(int i=0; i<dim; i++) {
        for(int j=0; j<dim; j++) {
            int idx = i*dim + j;
            block_args[idx] = {
                i*row_chunk, (i+1)*row_chunk,
                j*col_chunk, (j+1)*col_chunk
            };
            pthread_create(&block_threads[idx], nullptr, block_multiply, &block_args[idx]);
        }
    }
    for(auto& t : block_threads) pthread_join(t, nullptr);
    times[2] = get_time() - start;

    // 4. Cyclic
    reset_matrix(result_cyclic);
    pthread_t cyclic_threads[NUM_THREADS];
    CyclicArgs cyclic_args[NUM_THREADS];

    start = get_time();
    for(int i=0; i<NUM_THREADS; i++) {
        cyclic_args[i] = {i};
        pthread_create(&cyclic_threads[i], nullptr, cyclic_multiply, &cyclic_args[i]);
    }
    for(auto& t : cyclic_threads) pthread_join(t, nullptr);
    times[3] = get_time() - start;

    // Results
    cout << "\nVerification:\n";
    cout << "Row-wise: " << (verify(result_row) ? "Pass" : "Fail") << endl;
    cout << "Column-wise: " << (verify(result_col) ? "Pass" : "Fail") << endl;
    cout << "Block: " << (verify(result_block) ? "Pass" : "Fail") << endl;
    cout << "Cyclic: " << (verify(result_cyclic) ? "Pass" : "Fail") << endl;

    cout << "\nPerformance (Speedup):\n";
    for(int i=0; i<4; i++) {
        cout << strategies[i] << ": " << seq_time/times[i] << "x (" 
             << times[i] << "s)\n";
    }

    // Cleanup
    for(int i=0; i<rowsA; i++) {
        delete[] A[i];
        delete[] result_seq[i];
        delete[] result_row[i];
        delete[] result_col[i];
        delete[] result_block[i];
        delete[] result_cyclic[i];
    }
    for(int i=0; i<rowsB; i++) delete[] B[i];
    delete[] A; delete[] B;
    delete[] result_seq; delete[] result_row;
    delete[] result_col; delete[] result_block;
    delete[] result_cyclic;

    return 0;
}