#include <iostream>

#define NUM_TESTS 3 //number of tests
#define N 1024 //elements of one row and one column of A and B
#define Nm 32 //number of elements of one block (square block)
#define Nn 32 //number of blocks in one row and one column

//initialize A and B
static void init_arrays(float *A,  float *B, float *D, float *D_sw)
{
     for (int i = 0; i < N; i++) {
          for (int j = 0; j < N; j++) {
               A[i * N + j] = 1+i*N+j;
               B[i * N + j] = rand() % (N * N);
               D[i * N + j] = 0.0;
               D_sw[i * N + j] = 0.0;
          }
     }
}


void mmult_golden(float *A,  float *B, float *C)
{
     for (int row = 0; row < N; row++) {
          for (int col = 0; col < N; col++) {
               float result = 0.0;
               for (int k = 0; k < N; k++) {
                    result += A[row*N+k] * B[k*N+col];
               }
               C[row*N+col] = result;
          }
     }
}

//addition of two matrices
void madd(float A[Nm*Nm], float B[Nm*Nm])
{
    for (int i = 0; i < Nm; i++){
        for (int j = 0; j < Nm; j++){
            B[i*Nm+j] = A[i*Nm+j] + B[i*Nm+j];
        }
    }
}

//multiplication of two matrices
void mmult (float A[Nm*Nm], float B[Nm*Nm], float C[Nm*Nm])
{
    float Abuf[Nm][Nm], Bbuf[Nm][Nm];
//#pragma HLS array_partition variable=Abuf block factor=32 dim=2
//#pragma HLS array_partition variable=Bbuf block factor=32 dim=1
    
    for(int i=0; i<Nm; i++) {
        for(int j=0; j<Nm; j++) {
            Abuf[i][j] = A[i * Nm + j];
            Bbuf[i][j] = B[i * Nm + j];
        }
    }
    
    for (int i = 0; i < Nm; i++) {
        for (int j = 0; j < Nm; j++) {
            float result = 0;
            for (int k = 0; k < Nm; k++) {
                float term = Abuf[i][k] * Bbuf[k][j];
                result += term;
            }
            C[i*Nm+j] = result;
        }
    }
}

//testing matrix multiplication methods
//method one is a traditional way to do matrix multip
//we consider that the result of method one is absolute correct
//then we compare the result of method two with the result of method one
//method two breaks large matrix into small blocks and do multip respectively
int mmult_test(float *A,  float *B, float *D, float *D_sw)
{
     std::cout << "Testing " << NUM_TESTS << " iterations of " << N << "x" << N 
               << " floating point mmultadd..." << std::endl;
     

     for (int i = 0; i < NUM_TESTS; i++) 
     {

          init_arrays(A, B, D, D_sw);

          float A_block[Nm*Nm] = {0};
          float B_block[Nm*Nm] = {0};
          float M_block[Nm*Nm] = {0};
		  float tmp2[Nm*Nm] = {0};

         //Method one: trdistional way to calculate AxB (no breaking)
          mmult_golden(A, B, D_sw);


         //Method two: breaking A,B into blocks and do multiplications
         for(int m = 0; m < Nn; m++){ //block location in D
             for(int n = 0; n < Nn; n++){ //block location in D
                 
                 for(int h = 0; h < Nn; h++){
                     
                     for(int k = 0; k < Nm; k++){ //row of a block
                         for(int p = 0; p < Nm; p++){ //colomn of a block
                             A_block[k*Nm + p] = A[m*N*Nm + k*N + h*Nm + p];//blocks of A
                             B_block[k*Nm + p] = B[h*N*Nm+ k*N + n*Nm + p];//blocks of B
                         }
                     }
                     mmult(A_block, B_block, M_block); //do multiplication
                     madd(M_block, tmp2); //store block multiplication results in tmp2
                 }
                 
                 for(int k = 0; k < Nm; k++){
                     for(int p = 0; p < Nm; p++){
                         D[m*N*Nm + k*N + n*Nm + p] = tmp2[k*Nm + p]; //add tmp2 to D
                     }
                 }
                 
                 //restore tmp2 to 0
                 for(int k = 0; k < Nm; k++){
                     for(int p = 0; p < Nm; p++){
                         tmp2[k*Nm + p] = 0;
                     }
                 }
             }
         }


          if (result_check(D, D_sw)) //check results between two methods
               return 1;
     }

     return 0;
}

//check the results between traditional way and breaking way
static int result_check(float *D, float *D_sw)
{
    for (int i = 0; i < N * N; i++) {
        //due to the floating point error, the error between D and D_sw is set to D_sw*0.01
        if ((D_sw[i] - D[i]) > (D_sw[i]*0.01)) {
            std::cout << "Mismatch: data index=" << i << " d=" << D_sw[i]
            << ", dout=" << D[i] << std::endl;
            return 1;
        }
    }
    return 0;
}

int main(int argc, char* argv[]){
     int test_passed = 0;
     float *A, *B, *D, *D_sw;
     
     A = (float *)malloc(N * N * sizeof(float));
     B = (float *)malloc(N * N * sizeof(float));
     D = (float *)malloc(N * N * sizeof(float));
     D_sw = (float *)malloc(N * N * sizeof(float));

     if (!A || !B || !C || !D || !D_sw) {
         if (A) free(A);
         if (B) free(B);
         if (D) free(D);
         if (D_sw) free(D_sw);
         return 2;
     }

     test_passed = mmult_test(A, B, D, D_sw);
     
     std::cout << "TEST " << (test_passed ? "FAILED" : "PASSED") << std::endl;

     free(A);
     free(B);
     free(D);
     free(D_sw);

     return (test_passed ? -1 : 0);
}

