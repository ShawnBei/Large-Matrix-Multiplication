# Large-Matrix-Multiplication
Breaking large matrix into small blocks to achieve large matrix multiplication when computation resources are limited.

## Algorithm 

In this algorithm, A and B represent two large matrices, D represents the result of A multiply B.

Figure shows how one specific block of D is calculated using the algorithm in the assessment. The left matrix represents matrix A, the middle matrix represents matrix B and the right matrix represents matrix D. They are divided into many Nm by Nm blocks. Small blocks (1, 2, 3, 4) underneath are multiplication results of blocks with same colour in A and B. Their addition result (temporary matrix) will be then put to D in responding places, in this case, it is put to the D11 block in the first row and first column. For all blocks in D, the processes are the same. 

![alt text](https://github.com/ShawnBei/Large-Matrix-Multiplication/blob/master/algorithm.png)

Equation (1) shows the mathematical model of how blocks in D are calculated.

![alt text](https://github.com/ShawnBei/Large-Matrix-Multiplication/blob/master/equation.png)

where _m_ represents the row of _D_, n represents the column of _D_, _Nm_ represents the breaking block dimension.

## Code

See _main.cpp_ 
