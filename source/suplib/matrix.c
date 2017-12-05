/* ---------------------------------------------------------------------------------
 *          file : matrix.c                                                       *
 *   description : C-source file, functions for the matrix struct                 *
 *       toolbox : DotX Wind Turbine Control Software (support library)           *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "./../signals/signal_definitions_internal.h"

#include "./matrix.h"

/*----------------------------------------------------------------------------------
 * Memory operations 
--------------------------------------------------------------------------------- */

/* Initialize an empty matrix A of size MxN */
matrix * mat_initEmpty( const int M, const int N )
{
    matrix * new_matrix = (matrix*) calloc(1, sizeof(matrix));
    new_matrix->Mat = (REAL*) calloc(M*N, sizeof(REAL));
    new_matrix->M = M;
    new_matrix->N = N;
    return new_matrix;
}


/* Initialize an empty matrix A on the stack (REAL* is first memory address of the REAL[][] where the matrix data will be hold)*/
matrix  mat_stackinit( const int M, const int N, REAL * realarrayptr)
{
    matrix new_matrix;
    new_matrix.Mat = realarrayptr;
    new_matrix.M = M;
    new_matrix.N = N;
    return new_matrix;
}

/* Initialize a matrix with given values at the size of MxN     
   The initial value matrix A is ROW MAJOR!! This to allow for easy human readable 
   input. */
matrix * mat_init( const REAL A[], const int M, const int N )
{

    /* Create an empty matrix */
    matrix * new_matrix = mat_initEmpty(M,N);
    
    /* Copy the row major array to the matrix colomn major array */
    mat_setArray( new_matrix, A, M, N );
        
    return new_matrix;
        
}

/* Initialize a copy of matrix A */
matrix * mat_initCopy( const matrix * A )
{

    /* Create an empty matrix */
    matrix * new_matrix = mat_initEmpty(A->M, A->N);
    
    /* Copy the values of matrix A to the new matrix */
    mat_setMatrix(A, new_matrix);
    
    return new_matrix;
}

/* Initialize a MxM identity matrix */
matrix * mat_initEye( const int M )
{
    REAL diag[M];
    int i;
    for( i=0; i<M; i++ ){ diag[i] = R_(1.0); };
    return mat_initDiag( diag, M );
}


/* Initialize a MxM diagonal matrix with diagonal elements as defined in array diag */
matrix * mat_initDiag( const REAL diag[], const int M )
{
    /* Create an empty matrix */
    matrix * new_matrix = mat_initEmpty(M,M);
    
    int i;
    for( i=0; i<M; i++ )
    {
        mat_setValue( new_matrix, i, i, diag[i] );
    };
    
    return new_matrix;
}

/* Initialize a block diagonal matrix from the given array of matrices. nMatrices gives
   the number of matrices in the array. The size of the output array is calculated based 
   on the size of the given matrices. It is not required of the given matrices to have 
   the same dimensions. */
matrix * mat_initBlockDiag( const matrix diag[], const int nMatrices )
{
    int M = 0;
    int N = 0;
    int i;
    
    /* Loop over all given matrices to calculate the size of the output matrix */
    for( i=0; i<nMatrices; i++ )
    {
        M += diag[i].M;
        N += diag[i].N;
    };
    
    /* Initialize an empty matrix */
    matrix * new_matrix = mat_initEmpty(M,N);
    
    /* Insert the submatrices */
    int row = 0;
    int col = 0;
    for( i=0; i<nMatrices; i++ )
    {
        mat_insert( &(diag[i]), new_matrix, row, col );
        row += diag[i].M;
        col += diag[i].N;
    };  
    
    
    return new_matrix;
    
}

/* Free memory allocated to matrix A */ 
int mat_free( matrix * A )
{

    free(A->Mat);
    free(A);
    
    /* Always retruning no error !!! Maybe implement a check of some sort */
    return MCU_OK;
}


/*----------------------------------------------------------------------------------
 * Get and Set operations 
--------------------------------------------------------------------------------- */

/* Get the value of element (m,n) of matrix A */
int mat_getValue(const matrix * A, const int m, const int n, REAL * value)
{
    if( m > A->M - 1){ return MCU_ERR; };
    if( n > A->N - 1){ return MCU_ERR; };
    
    
    (*value) = A->Mat[m+n*A->M];
    
    return MCU_OK;
}


/* Set a specified matrix value */
int mat_setValue(const matrix * A, const int m, const int n, const REAL value)
{
    if( m > A->M - 1){ return MCU_ERR; };
    if( n > A->N - 1){ return MCU_ERR; };
    
    A->Mat[m+n*A->M] = value;
    return MCU_OK;
}


/* Set the full matrix array in matrix struct A. The values are copied.
   The initial value array A is ROW MAJOR!! This to allow for easy human readable 
   input. */
int mat_setArray(const matrix *A, const REAL Aarray[], const int M, const int N)
{

    if( A->M != M ){ return MCU_ERR; }
    if( A->N != N ){ return MCU_ERR; }
    
    /* Convert the row major input array to column major array in the matrix struct */
    /* Loop over all element of array A */
    int m, n;
    for( m = 0; m<M; m++)
    {
        for( n = 0; n<N; n++)
        {
            mat_setValue(A, m, n, Aarray[n+m*N]);
        };
    };  
    
    return MCU_OK;
}


/* Copy the values of matrix A to matrix B */
int mat_setMatrix(const matrix * A, const matrix * B)
{

    if( !mat_sameDim(A, B) ){ return MCU_ERR; };
    
    memcpy(B->Mat, A->Mat, A->M * A->N * sizeof(REAL) );
    
    return MCU_OK;
}


/*----------------------------------------------------------------------------------
 * Algebraic operations 
--------------------------------------------------------------------------------- */

/* Summate two matrices A + B = C   */
int mat_add( const matrix * A, const matrix * B, matrix * C )
{
    /* Check the dimensions */
    if( !mat_sameDim(A,B) ){ return MCU_ERR; };
    if( !mat_sameDim(B,C) ){ return MCU_ERR; };
        
    int i;
    for(i=0; i < A->M * A->N; i++)
    {
        C->Mat[i] = A->Mat[i] + B->Mat[i];
    }
    
    return MCU_OK;
}

/* Subtract two matrices    A - B = C   */
int mat_subtract( const matrix * A, const matrix * B, matrix * C )
{
    /* Check the dimensions */
    if( !mat_sameDim(A,B) ){ return MCU_ERR; };
    if( !mat_sameDim(B,C) ){ return MCU_ERR; };
        
    int i;
    for(i=0; i < A->M * A->N; i++)
    {
        C->Mat[i] = A->Mat[i] - B->Mat[i];
    }   
    
    return MCU_OK;
}

/* Multiply two matrices    A B = C */
int mat_mult( const matrix * A, const matrix * B, matrix * C )
{
    /* Check the matrices to see if they have the correct dimensions */
    if( A->M != C->M ){ return MCU_ERR; };
    if( B->N != C->N ){ return MCU_ERR; };
    if( A->N != B->M ){ return MCU_ERR; };
    
    int j;
    int i;
    int l;

	for (j = 0; j < A->M; j++) //m
	{
		for (i = 0; i < C->N; i++)
		{
			C->Mat[(i * C->M) + j] = 0.0; // Clean column first
		}
		for (l = 0; l < A->N; l++) //k
		{
			if (A->Mat[(l * A->M) + j] != 0.0)
			{
				for (i = 0; i < B->N; i++) //m
				{
					C->Mat[(i * C->M) + j] += B->Mat[(i * B->M) + l] * A->Mat[(l * A->M) + j];
				}
			}
		}
	}
    
    return MCU_OK;
}

/* Multiply a scalar and a matrix matrices  a B = C */
int mat_multScalar( const REAL a, const matrix * B, matrix * C )
{
    /* Check the matrices to see if they have the correct dimensions */
    if( !mat_sameDim(B,C) ){ return MCU_ERR; };
    
    int i;
    for( i = 0; i < B->M * B->N; i++ )
    {
        C->Mat[i] = a * B->Mat[i];
    };
    
    return MCU_OK;
    
}


/* Multiply two matrices element whise A .* B = C   */
int mat_multElement( const matrix * A, const matrix * B, matrix * C )
{
    /* Check the dimensions */
    if( !mat_sameDim(A,B) ){ return MCU_ERR; };
    if( !mat_sameDim(B,C) ){ return MCU_ERR; };
    
    int i;
    for( i=0; i < A->M * A->N; i++ )
    {
        C->Mat[i] = A->Mat[i] * B->Mat[i];
    };
    
    return MCU_OK;
}

/* Solve the equation A * X = B */
int mat_solve( const matrix * A, const matrix * B, matrix * X ) 
{
    /* Check the dimensions */
    if ( A->M != A->N ) return MCU_ERR;
    if ( A->M != B->M ) return MCU_ERR;
    if ( X->M != A->M ) return MCU_ERR;
    
    /* Precomputed Solution */
    X->Mat[1] = ( A->Mat[0]*B->Mat[1] - A->Mat[1]*B->Mat[0] )/( A->Mat[0]*A->Mat[3] - A->Mat[1]*A->Mat[2] );
    X->Mat[0] = ( B->Mat[0] - A->Mat[2]*X->Mat[1] )/( A->Mat[0] );
    
    return MCU_OK;
}

/* Compute the dot product between to row or column vectors */
int mat_dot( const matrix * A, const matrix * B, REAL * dot ) 
{
    /* Check the dimensions */
    if ( MIN( A->M, A->N ) != 1 ) return MCU_ERR;
    if ( MIN( B->M, B->N ) != 1 ) return MCU_ERR;
    if ( MAX( A->M, A->N ) != MAX( B->M, B->N ) ) return MCU_ERR;
    
    /* Compute the dot (inner) product */
    int i;
    *dot = 0;
    for (i = 0; i < MAX( A->M, A->N ); i++)
    {
    	*dot += A->Mat[i] * B->Mat[i];
    }
    
    return MCU_OK;
}

/* Compute the 2-norm of a row or column vector  */
int mat_nrm2( const matrix * A, REAL * nrm2 ) 
{
    /* Check the dimensions */
    if ( MIN( A->M, A->N ) != 1 ) return MCU_ERR;
    
    /* Compute the 2-norm */
    int i;
    *nrm2 = 0;
    for (i = 0; i < A->M * A->N; i++)
	{
		*nrm2 += A->Mat[i]*A->Mat[i];
	}
    *nrm2 = sqrt(*nrm2);

    return MCU_OK;
}

/* Compute the 1-norm of a row or column vector */
int mat_nrm1( const matrix * A, REAL * nrm1 ) 
{
    /* Check the dimensions */
    if ( MIN( A->M, A->N ) != 1 ) return MCU_ERR;
    
    /* Compute the 2-norm */
    int i;
	*nrm1 = 0;
	for (i = 0; i < A->M * A->N; i++)
	{
		*nrm1 += A->Mat[i];
	}
	*nrm1 = abs(*nrm1);
    
    return MCU_OK;
}

/*----------------------------------------------------------------------------------
 * Logic operations 
--------------------------------------------------------------------------------- */

/* Check to see if the matrices A and B have the same dimensions or not
   Returns:
    0   If A and B do not have the same dimensions.
    1   If A and B have the same dimensions.
*/  
int mat_sameDim( const matrix * A, const matrix * B )
{
    if( A->M != B->M ){ return 0; };
    if( A->N != B->N ){ return 0; };    
    return 1;
}

/* Check to see if the matrices A and B have the samve valuse 
   Returns:
    0   Not the same
    1   The same values
*/
int mat_equal(const matrix * A, const matrix * B )
{
    /* Check the dimensions */
    if( !mat_sameDim(A,B) ){ return 0; };
    
    /* If the pointer addresses correspont the values must be the same */
    if( A->Mat == B->Mat ){ return 1; };
    
    /* Compare the actual value */
    int i;
    for(i=0; i<(A->M * A->N); i++)
    {
        if( A->Mat[i] != B->Mat[i] ){ return 0; }
    }
    
    return 1; 
}


/*----------------------------------------------------------------------------------
 * Submatrix operations 
--------------------------------------------------------------------------------- */

/* Insert matrix A into matrix B at coordinates (row, col) */ 
int mat_insert( const matrix * A, matrix * B, const int row, const int col )
{

    int err = MCU_OK;
    
    /* Check to see if the size of B is large enough to insert the submatrix. */
    if( (A->M - row) > B->M ){ return MCU_ERR; }
    if( (A->N - col) > B->N ){ return MCU_ERR; }	
    
    int m, n;
    REAL value;
    for( n = 0; n < A->N; n++ )
    {
        for( m = 0; m < A->M; m++ )
        {
            err += mat_getValue(A, m, n, &value);
            err += mat_setValue(B, m+row, n+col, value);
        };
    };
    
    return err;
}

/* Extract a submatrix B from matrix A starting from coordinates (row, col) the 
   size of matrix B. */
int mat_extract( const matrix * A, matrix * B, const int row, const int col )
{

    int err = MCU_OK;
    
    /* Check to see if the size of A is large enough to extract the submatrix. */
    if( A->M - row < 0 ){ return MCU_ERR; }
    if( A->N - col < 0 ){ return MCU_ERR; }	
    
    
    /* Copy vector by vector from A to B */
    int m,n;
    REAL value;
    for( m = 0; m < B->M; m++ )
    {
        for( n = 0; n < B->N; n++ )
        {
            err += mat_getValue(A, m+row, n+col, &value);
            err += mat_setValue(B, m, n, value);
        };
    };
    return err;
}

/*----------------------------------------------------------------------------------
 * Auxiliary operations
--------------------------------------------------------------------------------- */

/* do linear 1d interpolation on data array */
REAL mat_interp1( const matrix * x_mat, const matrix * y_mat, const int N, const REAL x_in )
{
    int k;
    REAL * x = x_mat->Mat;
    REAL * y = y_mat->Mat;
    if ( x_in < x[0]   ) return y[0];
    for ( k = 1; k < N; ++k ) 
        if ( x_in <= x[k] ) 
            return y[k-1] + 
                    ( y[k] - y[k-1] ) * ( x_in - x[k-1] ) / ( x[k] - x[k-1] );
    return y[N-1];
}

/* Do linear 1d interpolation on data array. */
REAL interp1( const REAL *x, const REAL *y, const int N, const REAL x_in ) 
{
    int k;
    if ( x_in < x[0]   ) return y[0];
    for (k = 1; k < N; ++k ) 
        if ( x_in <= x[k] ) 
            return y[k-1] + 
                    ( y[k] - y[k-1] ) * ( x_in - x[k-1] ) / ( x[k] - x[k-1] );
    return y[N-1];
}

/*----------------------------------------------------------------------------------
 * Output operations 
--------------------------------------------------------------------------------- */

/* prints a human readable representation of the matrix A in char buf outString */
int mat_printf(const matrix * A)
{
    printf("M=%i\tN=%i\n", A->M, A->N);
    int m,n;
    REAL value;
    
    for(m=0; m<A->M; m++)
    {
        if( m==0 )
        {
            printf("[\t");
        }
        else
        {
            printf("\t");   
        }
        
        for(n=0; n<A->N; n++)
        {
            mat_getValue(A,m,n, &value);
            printf("%f\t", value);
        }
        
        if( m == A->M-1 )
        {
            printf(" ];\n");
        }
        else
        {
            printf(" ;...\n");
        }
    }
    
    return MCU_OK;
    
}

/*----------------------------------------------------------------------------------
  end matrix.c
--------------------------------------------------------------------------------- */
