#include <stdio.h>
#include <stdlib.h>

// Structure for matrix
typedef struct matrix {
    int nrows;  // Number of rows
    int ncols;  // Number of columns
    int **arr;  // 2D array for matrix data
    int totalsum; // Total sum of elements (mod 10007)
} matrix;

// Structure for matrix array
typedef struct matrixarray {
    int cp;  // Current capacity of the array
    int dim; // Current number of matrices in the array
    matrix **mats; // Array of pointers to matrices
} matrixarray;

// Function to allocate memory for a matrix
int **memory_matrix(int m, int n) {
    int **a;
    int i;
    a = (int **)malloc(m * sizeof(int *)); // Allocate memory for rows
    if (!a) {
        fprintf(stderr, "malloc() failed\n");
        return NULL;
    }
    for (i = 0; i < m; i++) {
        a[i] = (int *)malloc(n * sizeof(int)); // Allocate memory for columns
        if (!a[i]) {
            fprintf(stderr, "malloc() failed\n");
            while (--i >= 0) // Free previously allocated rows
                free(a[i]);
            free(a);
            return NULL;
        }
    }
    return a;
}

// Function to load a matrix from input
void loadmat(matrix *mat) {
    scanf("%d ", &mat->nrows); // Input number of rows
    scanf("%d ", &mat->ncols); // Input number of columns
    mat->totalsum = 0; // Initialize the sum
    mat->arr = memory_matrix(mat->nrows, mat->ncols); // Allocate memory for the matrix
    for (int i = 0; i < mat->nrows; i++) {
        for (int j = 0; j < mat->ncols; j++) {
            scanf("%d", &mat->arr[i][j]); // Input matrix element
            mat->totalsum += mat->arr[i][j]; // Update the total sum
            mat->totalsum %= 10007; // Apply modulo to avoid overflow
            if (mat->totalsum < 0)
                mat->totalsum += 10007; // Ensure non-negative sum
        }
    }
    getchar(); // Consume newline character
}

// Function to get the dimensions of a matrix
void getdimensions(matrix *mat, int *rows, int *cols) {
    *rows = mat->nrows;
    *cols = mat->ncols;
}

// Function to print a matrix
void printmatrix(matrix *mat) {
    for (int i = 0; i < mat->nrows; i++) {
        for (int j = 0; j < mat->ncols; j++)
            printf("%d ", mat->arr[i][j]); // Print each element
        printf("\n");
    }
}

// Function to resize a matrix
matrix *resizematrix(matrix *mat, int *rows, int newr, int *cols, int newc) {
    matrix *a; // Auxiliary matrix
    a = (matrix *)malloc(sizeof(matrix));
    a->nrows = newr;
    a->ncols = newc;
    a->arr = memory_matrix(a->nrows, a->ncols);
    a->totalsum = 0;
    for (int i = 0; i < newr; i++) {
        for (int j = 0; j < newc; j++) {
            a->arr[i][j] = mat->arr[rows[i]][cols[j]]; // Copy selected elements to new matrix
            a->totalsum += a->arr[i][j]; // Update the total sum
            a->totalsum %= 10007; // Apply modulo to avoid overflow
            if (a->totalsum < 0)
                a->totalsum += 10007; // Ensure non-negative sum
        }
    }
    return a;
}

// Function to multiply two matrices
matrix *multiplymatrix(matrix *mat1, matrix *mat2) {
    if (mat1->ncols != mat2->nrows) {
        printf("Cannot perform matrix multiplication\n");
        return NULL; // Matrices must be compatible for multiplication
    }
    matrix *a;
    a = (matrix *)malloc(sizeof(matrix));
    a->nrows = mat1->nrows;
    a->ncols = mat2->ncols;
    a->arr = memory_matrix(a->nrows, a->ncols);
    a->totalsum = 0;
    for (int i = 0; i < a->nrows; i++) {
        for (int j = 0; j < a->ncols; j++) {
            a->arr[i][j] = 0;
            for (int s = 0; s < mat2->nrows; s++) {
                int aux = (mat1->arr[i][s] * mat2->arr[s][j]) % 10007; // Perform multiplication
                if (aux < 0)
                    aux = aux + 10007;
                a->arr[i][j] += aux; // Accumulate result
            }
            a->arr[i][j] %= 10007; // Apply modulo
            a->totalsum += a->arr[i][j]; // Update the total sum
            a->totalsum %= 10007; // Apply modulo
            if (a->totalsum < 0)
                a->totalsum += 10007; // Ensure non-negative sum
        }
    }
    return a;
}

// Function to sort matrices by their total sum
void sortarraybysum(matrixarray *v) {
    matrix *aux;
    for (int i = 0; i < v->dim - 1; i++) {
        for (int j = i + 1; j < v->dim; j++)
            if (v->mats[i]->totalsum > v->mats[j]->totalsum) {
                aux = v->mats[i];
                v->mats[i] = v->mats[j];
                v->mats[j] = aux;
            }
    }
}

// Function to transpose a matrix
matrix *transmat(matrix *mat) {
    matrix *a;
    a = (matrix *)malloc(sizeof(matrix));
    a->nrows = mat->ncols;
    a->ncols = mat->nrows;
    a->arr = memory_matrix(a->nrows, a->ncols);
    for (int i = 0; i < a->nrows; i++) {
        for (int j = 0; j < a->ncols; j++)
            a->arr[i][j] = mat->arr[j][i]; // Swap rows and columns
    }
    a->totalsum = mat->totalsum; // Total sum remains the same
    return a;
}

// Function to free memory allocated for a matrix
void clearmat(matrix *mat) {
    for (int i = 0; i < mat->nrows; i++)
        free(mat->arr[i]); // Free each row
    free(mat->arr); // Free the array of rows
}

// Function to add a matrix to the matrix array
void addmatrix(matrixarray *v, matrix *mat) {
    if (v->dim >= v->cp) {
        v->cp += 10; // Increase the capacity if needed
        v->mats = (matrix **)realloc(v->mats, sizeof(matrix *) * v->cp); // Reallocate memory
    }
    v->mats[v->dim] = mat;
    (v->dim)++; // Increment the number of matrices
}

// Function to free memory for the matrix array
void clearmatarray(matrixarray *v) {
    for (int i = 0; i < v->dim; i++) {
        clearmat(v->mats[i]); // Free each matrix
        free(v->mats[i]); // Free the pointer to the matrix
        v->mats[i] = NULL;
    }
    free(v->mats); // Free the array of matrix pointers
}

// Function to clear memory for a specific matrix in the array
void clearonematrix(matrixarray *v, int ind) {
    clearmat(v->mats[ind]); // Free the matrix
    free(v->mats[ind]); // Free the pointer to the matrix
    v->mats[ind] = NULL;
    for (int i = ind; i < v->dim - 1; i++)
        v->mats[i] = v->mats[i + 1]; // Shift the matrices in the array
    v->dim--; // Decrement the number of matrices
}

// Additional functions (Bonus): Matrix addition, subtraction, and submatrix extraction
// These operations follow the same principles as the above ones and involve manipulating matrices, updating totals, and handling memory management.

// The main function handles user input and matrix operations.
int main(void) {
    matrixarray *v; matrix *a; int ind; char command = '!';
    v = (matrixarray *)malloc(sizeof(matrixarray));
    v->cp = 0; v->dim = 0;
    v->mats = (matrix **)malloc(v->cp * sizeof(matrix *));
    while (command != 'Q') { // Loop until the user exits
        scanf("%c", &command);
        if (command == 'L') {
            a = (matrix *)malloc(sizeof(matrix));
            loadmat(a); // Load a matrix from input
            addmatrix(v, a); // Add it to the array
        }
        else if (command == 'P') {
            scanf("%d", &ind);
            if (ind < 0 || ind >= v->dim)
                printf("No matrix with the given index\n");
            else
                printmatrix(v->mats[ind]); // Print the matrix
        }
        else if (command == 'C') { // Resize matrix
            int m, n, *line, *col;
            scanf("%d", &ind);
            scanf("%d", &m);  // Number of rows for submatrix
            line = (int *)malloc(m * sizeof(int));
            for (int i = 0; i < m; i++)
                scanf("%d", &line[i]);  // Rows to be copied
            scanf("%d", &n);  // Number of columns for submatrix
            col = (int *)malloc(n * sizeof(int));
            for (int i = 0; i < n; i++)
                scanf("%d", &col[i]);  // Columns to be copied
            getchar();  // Consume the newline character

            if (ind < 0 || ind >= v->dim) {
                printf("No matrix with the given index\n");
            } else {
                // Resize the matrix by selecting specific rows and columns
                matrix *aux = resizematrix(v->mats[ind], line, m, col, n);
                clearmat(v->mats[ind]);  // Free the old matrix memory
                free(v->mats[ind]);
                v->mats[ind] = aux;  // Assign the resized matrix
            }
        }
        // Additional matrix operations can be added here following the same structure
    }
    clearmatarray(v); // Clean up all allocated memory
    free(v); // Free the matrix array structure
    return 0;
}
