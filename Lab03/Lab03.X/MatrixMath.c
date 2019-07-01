/* 
 * File:   MatrixMath.c
 * Author: Stephanie Lin
 *
 * Created on April 19, 2019, 2:21 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "MatrixMath.h"

/*
 * 
 */
static float MatrixDeterminant2x2(float mat[2][2]) {
    float a;
    float b;
    float c;
    
    // Does cross multiplication of 2x2 matrix
    a = (mat[0][0] * mat[1][1]);
    b = (mat[0][1] * mat[1][0]);
    c = a - b;
    return c;
}

void MatrixPrint(float mat[3][3]) {

    printf("\nOutput of MatrixPrint():\n");
    printf("_________________\n");
    printf("\n%0.2f  %0.2f  %0.2f\n%0.2f  %0.2f  %0.2f\n%0.2f  %0.2f  %0.2f\n",
            mat[0][0], mat[0][1], mat[0][2],
            mat[1][0], mat[1][1], mat[1][2],
            mat[2][0], mat[2][1], mat[2][2]);
    printf("-----------------\n");
    printf("\nExpected output of MatrixPrint():\n");
    printf("_________________\n");
    printf("\n%0.2f  %0.2f  %0.2f\n%0.2f  %0.2f  %0.2f\n%0.2f  %0.2f  %0.2f\n",
            1.00,2.00,6.00,4.00,5.00,6.00,7.00,8.00,9.00);
    printf("-----------------\n");
}

int MatrixEquals(float mat1[3][3], float mat2[3][3]) {
    int not_equal = 1;
    int r;
    int c;
    float abs;

    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            if ((mat1[r][c] - mat2[r][c]) <= 0) { //checks if difference is negative
                abs = (mat1[r][c] - mat2[r][c]) * -1;
                if (FP_DELTA >= abs) { //checks if error is less that FP_DELTA
                    not_equal = 0; //sets flag for matrix equal so far
                } else {
                    not_equal = 1;
                    break;
                }
            } else if (((mat1[r][c] - mat2[r][c]) > 0)) { //checks if difference is positive
                not_equal = 1;
                break;

            } else if (not_equal == 1) {
                break;
            } else {
                not_equal = 1;
            }
            if (not_equal == 1) {
                break;
            }
        }
        if (not_equal == 1) {
            break;
        }
    }
    if (not_equal == 0) {
        return 1;
    } else {
        return 0;
    }
}

void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    int r;
    int c;
    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            result[r][c] = (mat1[r][c] + mat2[r][c]);
        }
    }
}

void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    int r;
    int c;
    int i;
    int j;
    int k;
    float cell = 0;
    for (r = 0; r < 3; r++) {
        for (j = 0; j < 3; j++) {
            for (c = 0, i = 0; c < 3, i < 3; c++, i++) {
                //multiplies row and column and add to itself each time
                cell += (mat1[r][c] * mat2[i][j]);
            }
            result[r][j] = cell;
            cell = 0;
        }
    }
}

void MatrixScalarAdd(float x, float mat[3][3], float result[3][3]) {
    int r;
    int c;
    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            result[r][c] = (mat[r][c] + x);
        }
    }
}

void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3]) {
    int r;
    int c;
    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            result[r][c] = (mat[r][c] * x);
        }
    }
}

float MatrixTrace(float mat[3][3]) {
    int r;
    int c;
    float result;
    result = 0;
    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            if (r == c) {
                //if at diagonal, then  will add to itself
                result += mat[r][c];
            }
        }
    }
    return result;
}

void MatrixTranspose(float mat[3][3], float result[3][3]) {
    int r;
    int c;
    int i;
    int j;
    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            if (r != c) {
                //swaps cells so information doesn't get lost
                result[c][r] = mat[r][c];
                result[r][c] = mat[c][r];
            } else {
                //if at diagonal information remains the same
                result[r][c] = mat[r][c];
            }
        }
    }
}

void MatrixSubmatrix(int i, int j, float mat[3][3], float result[2][2]) {
    int r;
    int c;
    int r2;
    int c2;
    r2 = 0;
    c2 = 0;

    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            if ((r != i - 1) && (c != j - 1)) {
                //checks if i and j is equal to r and c. if so, skips those cells
                result[r2][c2] = mat[r][c];
                c2++;
            } else {
            }
        }
    }
}

float MatrixDeterminant(float mat[3][3]) {
    float result[2][2];
    float det;
    float det1;
    float det2;
    float det3;
    float determinant1;
    float determinant2;
    float determinant3;

    //calculating determinant of each submatrix to calculate 3x3 determinant
    MatrixSubmatrix(1, 1, mat, result);
    det1 = MatrixDeterminant2x2(result);
    determinant1 = mat[0][0] * det1;

    MatrixSubmatrix(1, 2, mat, result);
    det2 = MatrixDeterminant2x2(result);
    determinant2 = mat[0][1] * det2;

    MatrixSubmatrix(1, 3, mat, result);
    det3 = MatrixDeterminant2x2(result);
    determinant3 = mat[0][2] * det3;

    det = determinant1 - determinant2 + determinant3;

    return det;
}

void MatrixInverse(float mat[3][3], float result[3][3]) {
    int r;
    int c;
    float det;
    float result1[3][3];
    float two[2][2];
    float divdet;

    det = MatrixDeterminant(mat);

    MatrixTranspose(mat, result1);

    MatrixSubmatrix(1, 1, result1, two);
    result[0][0] = MatrixDeterminant2x2(two);

    MatrixSubmatrix(1, 2, result1, two);
    result[0][1] = -(MatrixDeterminant2x2(two));

    MatrixSubmatrix(1, 3, result1, two);
    result[0][2] = MatrixDeterminant2x2(two);

    MatrixSubmatrix(2, 1, result1, two);
    result[1][0] = -(MatrixDeterminant2x2(two));

    MatrixSubmatrix(2, 2, result1, two);
    result[1][1] = MatrixDeterminant2x2(two);

    MatrixSubmatrix(2, 3, result1, two);
    result[1][2] = -(MatrixDeterminant2x2(two));

    MatrixSubmatrix(3, 1, result1, two);
    result[2][0] = MatrixDeterminant2x2(two);

    MatrixSubmatrix(3, 2, result1, two);
    result[2][1] = -(MatrixDeterminant2x2(two));

    MatrixSubmatrix(3, 3, result1, two);
    result[2][2] = MatrixDeterminant2x2(two);

    if (det == 0) {
    } else {
        divdet = (1 / det);
        MatrixScalarMultiply(divdet, result, result1);
    }

}