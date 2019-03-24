# Large-Matrix-Multiplication
Breaking large matrix into small blocks to achieve large matrix multiplication when computation resources are limited.

## Algorithm Design
The algorithm for large matrix multiplication can be divided into three parts. The first part is to break matrix A and matrix B into many 128 by 128 square matrixes in the main function. The second part is using hardware function to multiply them respectively and adding them to a temporary matrix. In this part, HLS pipeline and array partition are used to achieve fast parallel computation. The last part is to put all temporary matrices to responding places of matrix D in the main function. 
Figure 2 shows how one specific block of D is calculated using the algorithm in the assessment. The left matrix represents matrix A, the middle matrix represents matrix B and the right matrix represents matrix D. They are divided into many Nm by Nm blocks. Small blocks (1, 2, 3, 4) underneath are multiplication results of blocks with same colour in A and B. Their addition result (temporary matrix) will be then put to D in responding places, in this case, it is put to the D11 block in the first row and first column. For all blocks in D, the processes are the same. Equation (1) shows the mathematical model of how blocks in D are calculated.
D_mn=A_m1×B_1n+A_m2×B_2n+⋯+A_m(N/N_m ) ×B_(N/N_m )n                                     (1)
where m represents the row of D, n represents the column of D, Nm represents the breaking block dimension.
