// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// User libraries:
#include "MatrixMath.h"
// Module-level variables:
float zero_matrix[3][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};
float matx[3][3] = {
    {1, 0, 0},
    {1, 0, 1},
    {1, 1, 1}
};
float maty[3][3] = {
    {2, 1, 1},
    {1, 1, 2},
    {2, 2, 2}
};
float mat1[3][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};
float mat2[3][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};
float mat3[3][3] = {
    {-1, 9, 8},
    {4, 7, 6},
    {7, 8, 10}
};
float mat4[3][3] = {
    {-1, 9, 8},
    {4, 7, 6},
    {7, 8, 10}
};
float mat[3][3] = {
    {1, 2, 6},
    {4, 5, 6},
    {7, 8, 9}
};
float matadd1[3][3] = {
    {2, 4, 6},
    {8, 10, 12},
    {14, 16, 18}
};
float matadd2[3][3] = {
    {2, 4, 9},
    {8, 10, 12},
    {14, 16, 18}
};
float matmult1[3][3] = {
    {30, 36, 42},
    {66, 81, 96},
    {102, 126, 150}
};
float matmult2[3][3] = {
    {30, 36, 45},
    {66, 81, 108},
    {102, 126, 171}
};
float matsadd1[3][3] = {
    {6, 7, 11},
    {9, 10, 11},
    {12, 13, 14}
};
float matsadd2[3][3] = {
    {11, 12, 16},
    {14, 15, 16},
    {17, 18, 19}
};
float matsmult1[3][3] = {
    {5, 10, 30},
    {20, 25, 30},
    {35, 40, 45}
};
float matsmult2[3][3] = {
    {10, 20, 30},
    {40, 50, 60},
    {70, 80, 90}
};
float mattran1[3][3] = {
    {1, 4, 7},
    {2, 5, 8},
    {6, 6, 9}
};
float mattran2[3][3] = {
    {1, 4, 7},
    {2, 5, 8},
    {3, 6, 9}
};
float matsub1[2][2] = {
    {4, 6},
    {7, 9}
};
float matsub2[2][2] = {
    {4, 6},
    {7, 9}
};
float matinv1[3][3] = {
    {1, 0, 0},
    {0, -1, 1},
    {-1, 1, 0}
};
float matinv2[3][3] = {
    {1, 0, -0.50000},
    {-1, -1, 1.5000},
    {0, 1, -0.50000}
};

// **** All the result matrices from called functions ****
float result[3][3];
float result1[3][3];
float result2[3][3];
float result3[3][3];
float result4[3][3];
float result5[3][3];
float result6[3][3];
float result7[3][3];
float result8[3][3];
float result9[3][3];
float result10[3][3];
float result11[2][2];
float result12[2][2];
float result13[3][3];
float result14[3][3];
float matadd[3][3];
float x;
int i;
int j;
int u;
int v;

int main() {
    // **** Variables for checking  ****
    int add_result1;
    int add_result2;
    add_result2 = 0;
    add_result1 = 0;

    // **** Variables used for scalar functions ****
    u = 5;
    v = 10;
    i = 1;
    j = 2;

    BOARD_Init();

    printf("Beginning slin41's mml test harness, compiled on %s %s\n", __DATE__, __TIME__);

    printf("-----------------\n");

    // **** Testing MatrixEquals ****
    // **** add_result will return 1 if true ****
    add_result1 = MatrixEquals(zero_matrix, mat1);
    add_result2 = MatrixEquals(mat3, mat4);

    if (add_result1 == 1) {
        if (add_result2 == 1) {
            printf("PASSED (2/2): MatrixEquals()\n");
            add_result1 = 0;
            add_result2 = 0;
        } else {
            printf("PASSED (1/2): MatrixEquals()\n");
            add_result1 = 0;
        }
    } else {
        printf("FAILED (0/2): MatrixEquals()\n");
        add_result1 = 0;
    }

    // **** Testing MatrixAdd ****
    // **** add_result will return 1 if true ****
    MatrixAdd(mat1, mat2, result);
    MatrixAdd(mat1, mat, result1);
    add_result1 = MatrixEquals(result, matadd1);
    add_result2 = MatrixEquals(result1, matadd2);

    if (add_result1 == 1) {
        if (add_result2 == 1) {
            printf("PASSED (2/2): MatrixAdd()\n");
            add_result1 = 0;
            add_result2 = 0;
        } else {
            printf("PASSED (1/2): MatrixAdd()\n");
            add_result1 = 0;
        }
    } else {
        printf("FAILED (0/2): MatrixAdd()\n");
        add_result1 = 0;
    }

    // **** Testing MatrixMultiply ****
    // **** add_result will return 1 if true ****
    MatrixMultiply(mat1, mat2, result3);
    MatrixMultiply(mat1, mat, result4);
    add_result1 = MatrixEquals(result3, matmult1);
    add_result2 = MatrixEquals(result4, matmult2);

    if (add_result1 == 1) {
        if (add_result2 == 1) {
            printf("PASSED (2/2): MatrixMultiply()\n");
            add_result1 = 0;
            add_result2 = 0;
        } else {
            printf("PASSED (1/2): MatrixMultiply()\n");
            add_result1 = 0;
        }
    } else {
        printf("FAILED (0/2): MatrixMultiply()\n");
        add_result1 = 0;
    }

    // **** Testing MatrixScalarAdd ****
    // **** add_result will return 1 if true ****
    MatrixScalarAdd(u, mat, result5);
    MatrixScalarAdd(v, mat, result6);
    add_result1 = MatrixEquals(result5, matsadd1);
    add_result2 = MatrixEquals(result6, matsadd2);

    if (add_result1 == 1) {
        if (add_result2 == 1) {
            printf("PASSED (2/2): MatrixScalarAdd()\n");
            add_result1 = 0;
            add_result2 = 0;
        } else {
            printf("PASSED (1/2): MatrixScalarAdd()\n");
            add_result1 = 0;
        }
    } else {
        printf("FAILED (0/2): MatrixScalarAdd()\n");
        add_result1 = 0;
    }

    // **** Testing MatrixScalarMultiply ****
    // **** add_result will return 1 if true ****
    MatrixScalarMultiply(u, mat, result7);
    MatrixScalarMultiply(v, mat1, result8);
    add_result1 = MatrixEquals(result7, matsmult1);
    add_result2 = MatrixEquals(result8, matsmult2);
    if (add_result1 == 1) {
        if (add_result2 == 1) {
            printf("PASSED (2/2): MatrixScalarMultiply()\n");
            add_result1 = 0;
            add_result2 = 0;
        } else {
            printf("PASSED (1/2): MatrixScalarMultiply()\n");
            add_result1 = 0;
        }
    } else {
        printf("FAILED (0/2): MatrixScalarMultiply()\n");
        add_result1 = 0;
    }

    // **** Testing MatrixTrace ****
    // **** add_result will return 15 if true ****
    add_result1 = MatrixTrace(mat);
    add_result2 = MatrixTrace(mat1);
    if (add_result1 == 15) {
        if (add_result2 == 15) {
            printf("PASSED (2/2): MatrixTrace()\n");
            add_result1 = 0;
            add_result2 = 0;
        } else {
            printf("PASSED (1/2): MatrixTrace()\n");
            add_result1 = 0;
        }
    } else {
        printf("FAILED (0/2): MatrixTrace()\n");
        add_result1 = 0;
    }

    // **** Testing MatrixTranspose ****
    // **** add_result will return 1 if true ****
    MatrixTranspose(mat, result9);
    MatrixTranspose(mat1, result10);
    add_result1 = MatrixEquals(result9, mattran1);
    add_result2 = MatrixEquals(result10, mattran2);

    if (add_result1 == 1) {
        if (add_result2 == 1) {
            printf("PASSED (2/2): MatrixTranspose()\n");
            add_result1 = 0;
            add_result2 = 0;
        } else {
            printf("PASSED (1/2): MatrixTranspose()\n");
            add_result1 = 0;
        }
    } else {
        printf("FAILED (0/2): MatrixTranspose()\n");
        add_result1 = 0;
    }

    // **** Testing MatrixSubmatrix ****
    // **** condition will check if cells are equal, if so will pass ****
    MatrixSubmatrix(i, j, mat, result11);
    MatrixSubmatrix(i, j, mat1, result12);

    if ((result11[0][0] && matsub1[0][0]) &&
            (result11[0][1] && matsub1[0][1]) &&
            (result11[1][0] && matsub1[1][0]) &&
            (result11[1][1] && matsub1[1][1])) {
        if ((result11[0][0] && matsub2[0][0]) &&
                (result11[0][1] && matsub2[0][1]) &&
                (result11[1][0] && matsub2[1][0]) &&
                (result11[1][1] && matsub2[1][1])) {
            printf("PASSED (2/2): MatrixSubmatrix()\n");
            add_result1 = 0;
            add_result2 = 0;
        } else {
            printf("PASSED (1/2): MatrixSubmatrix()\n");
            add_result1 = 0;
        }
    } else {
        printf("FAILED (0/2): MatrixSubmatrix()\n");
        add_result1 = 0;
    }

    // **** Testing MatrixDeterminant ****
    // **** add_result will return 1 if true ****
    add_result1 = MatrixDeterminant(mat);
    add_result2 = MatrixDeterminant(mat3);
    if (add_result1 == -9) {
        if (add_result2 == -140) {
            printf("PASSED (2/2): MatrixDeterminant()\n");
            add_result1 = 0;
            add_result2 = 0;
        } else {
            printf("PASSED (1/2): MatrixDeterminant()\n");
            add_result1 = 0;
        }
    } else {
        printf("FAILED (0/2): MatrixDeterminant()\n");
        add_result1 = 0;
    }

    // **** Testing MatrixInverse ****
    // **** add_result will return 1 if true ****
    MatrixInverse(matx, result13);
    MatrixInverse(maty, result14);
    add_result1 = MatrixEquals(result13, matinv1);
    add_result2 = MatrixEquals(result14, matinv2);

    if (add_result1 == 1) {
        if (add_result2 == 1) {
            printf("PASSED (2/2): MatrixInverse()\n");
            add_result1 = 0;
            add_result2 = 0;
        } else {
            printf("PASSED (1/2): MatrixInverse()\n");
            add_result1 = 0;
        }
    } else {
        printf("FAILED (0/2): MatrixInverse()\n");
        add_result1 = 0;
    }

    MatrixPrint(mat);
    while (1);
}

