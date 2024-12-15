#include <stdio.h>
#include <stdlib.h>

/**
 * Return an array of arrays of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */

int** generate(int numRows, int* returnSize, int** returnColumnSizes);

int main() {
    int numRows;
    int returnSize;
    int* returnColumnSizes;
    
    printf( "Informe o numero de linhas do Triangulo Pascal = ");
    scanf("%d", &numRows);

    int** pascalTriangle = generate(numRows, &returnSize, &returnColumnSizes);

    printf("Triangulo de pascal em %d linhas:\n", numRows);
    for (int i = 0; i < returnSize; i++) {
        for (int j = 0; j < returnColumnSizes[i]; j++) {
            printf("%d ", pascalTriangle[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < returnSize; i++) {
        free(pascalTriangle[i]);
    }
    free(pascalTriangle);
    free(returnColumnSizes);

    return 0;
}

int** generate(int numRows, int* returnSize, int** returnColumnSizes) {
    *returnSize = numRows;

    int** pascalTriangle = (int**)malloc(numRows * sizeof(int*));
    *returnColumnSizes = (int*)malloc(numRows * sizeof(int));

    for (int i = 0; i < numRows; i++) {
        pascalTriangle[i] = (int*)malloc((i + 1) * sizeof(int));
        (*returnColumnSizes)[i] = i + 1;

        pascalTriangle[i][0] = 1;
        pascalTriangle[i][i] = 1;

        for (int j = 0; j <= i; j++) {
                 if (j == 0 || j == i) {
                pascalTriangle[i][j] = 1;
                } else {
                    pascalTriangle[i][j] = pascalTriangle[i - 1][j - 1] + pascalTriangle[i - 1][j]; 
                }
        }
    }
    
    return pascalTriangle;
}