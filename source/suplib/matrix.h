/* ---------------------------------------------------------------------------------
 *          file : matrix.h                                                       *
 *   description : C-header file, struct and function definition for matrices     *
 *       toolbox : DotX Wind Turbine Control Software (support library)           *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _MATRIX_H_
#define _MATRIX_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup suplib 
 *  @{*/
 
/* ------------------------------------------------------------------------------ */
/** \addtogroup matrices Matrices
 
    An empty matrix of size \f$A \in \mathbb{R}^{ \mathrm{3} \times \mathrm{2} }\f$ is defined in by using: 
    \code
        matrix * A = mat_initEmpty( 3, 4 );
    \endcode
    
    To initialize a matrix or vector at a given value, an array must be given holding the initial values:
    \code
        REAL init_B[4] = {4.4, 5.5, 6.6, 7.7};
        matrix * B = mat_init( init_B, 4, 1 );
    \endcode
    
    To for instance multiply two matrices:
    \code
        // First define the three required matrices.
        matrix * A = = mat_initEmpty( 2, 2 );
        matrix * B = = mat_initEmpty( 2, 1 );
        matrix * C = = mat_initEmpty( 2, 1 );
        
        // Set the values of A and B.
        REAL init_A[4] = { 1.2, 2.3, 3.4, 4.5 };
        REAL init_B[2] = { 5.6, 6.7 };
        
        // Multiply the matrices AB = C.
        int Error = mat_mult( A, B, C );
        
        // The result is now stored in C which can be printed to the screen.
        printf("Matrix C:\n");
        mat_printf(E);
    \endcode 
    
 *  @{*/
 
/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES
/*! \file matrix.h 
    \brief This header file defines a matrix struct and related matrix operations.
 */
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS
/*! \struct matrix
    \brief A struct defining the properties of a matrix or vector.
    The matrix defined in this struct is has the dimension \f$\mathbb{R}^{ \mathrm{M} \times \mathrm{N} }\f$.
    \sa matrix.h
 */
typedef struct matrix{

    //! Number of rows. 
    int M;
    //! Number of columns. 
    int N;
    //! The actual matrix data, stored in a column major array.
    REAL * Mat;
    
} matrix;
#endif


/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS

//! \name Memory operations 
//!@{

//! Initialize an empty matrix A of size MxN 
matrix * mat_initEmpty( const int M, const int N ); 
matrix  mat_stackinit( const int M, const int N, REAL* realarrayptr);

//! Initialize a matrix with given values at the size of MxN. 
/*! 
    The initial value array A is ROW MAJOR!! This to allow for easy human readable input.
    
    \param A Row major array of MxN elements with the matrix values.
    \param M Number of rows.
    \param N Number of columns.
    
    \return A new matrix struct instance is returned. To not forget to free the allocated 
            memory with mat_free() after use of the matrix.

*/
matrix * mat_init( const REAL A[], const int M, const int N ); 

//! Initialize new matrix which is a copy of matrix A.
/*!
    \param A The matrix to copy.
    \return A new matrix struct instance is returned.
*/
matrix * mat_initCopy( const matrix * A );

//! Initialize a MxM identity matrix.
/*!
    \param M The size of the matrix to create.
    
    \return A new matrix struct instance is returned with size MxM and 1.0 on its diagonal.
*/
matrix * mat_initEye( const int M );

//! Initialize a MxM diagonal matrix with diagonal elements as defined in array diagonal.
/*!
    \param diag An array of length M with all the elements to be placed on the diagonal of the matrix.
    \param M The size of the matrix to create.
    
    \return A new matrix struct instance is created and returned with size MxM.
*/
matrix * mat_initDiag( const REAL diag[], const int M );

//! Initialize a block diagonal matrix from the given array of matrices. 
/*!

    The size of the output matrix is calculated based on the size of the given matrices. It is not required for the given matrices to have 
    the same dimensions. The matrices are added in the order of their location in the array diag.
    
    \param diag An array with pointers to matrix instances.
    \param nMatrices gives the number of matrices in the array. 
    
    \return A new matrix struct instances is returned with the given matrices as the diagonals.
*/
matrix * mat_initBlockDiag( const matrix diag[], const int nMatrices );

//! Free memory allocated to matrix A.
/*!
    \param A    A pointer to a matrix struct of which the memory should be released.
    \return     A non zero int will be returned in case of an failure.
*/
int mat_free( matrix * A);

//!@}   


//! \name Get and Set operations 
//!@{

//! Get the value of element (m,n) in matrix A.
int mat_getValue(const matrix * A, const int m, const int n, REAL * value);

//! Set element (m,n) in matrix A to a new value
int mat_setValue(const matrix * A, const int m, const int n, const REAL value);

//! Set the full matrix array in matrix struct A to new values. 
/*! 
    The values are copied from the ROW MAJOR array. This to allow for easy human readable.
    
    \param A        The matrix struct to operate on.
    \param Aarray   The row major array to copy in to matrix A.
    \param M        The number of columns Aarray is based on. This should match with matrix A.
    \param N        The number of rows Aarray is based on. This should match with matrix A.
    
    \return A non zero value is returned in case of an error.
    
*/
int mat_setArray(const matrix *A, const REAL Aarray[], const int M, const int N);

//! Copy the values of matrix A to matrix B 
/*!
    \param A The source matrix.
    \param B The destination matrix.
    \return A non zero value is returned in case of an error.
*/
int mat_setMatrix(const matrix * A, const matrix * B);

//!@}   


//! \name Algebraic operations 
//!@{

//! Sum two matrices    \f$A + B = C\f$.
/*!

    The matrices \f$A\f$, \f$B\f$ and \f$C\f$ need to have the same dimensions.
    
    \param A First matrix to sum.
    \param B Second matrix to sum.
    \param C Pointer to an empty matrix in which the result is returned.
    \return A non zero value is returned in case of an error.   
*/
int mat_add( const matrix * A, const matrix * B, matrix * C );

//! Subtract two matrices 	\f$A - B = C\f$.
/*!

    The matrices \f$A\f$, \f$B\f$ and \f$C\f$ need to have the same dimensions.
    
    \param A Matrix to subtract from.
    \param B Matrix to subtract.
    \param C Pointer to an empty matrix in which the result is returned.
    \return A non zero value is returned in case of an error.   
*/
int mat_subtract( const matrix * A, const matrix * B, matrix * C );

//! Multiply two matrices  \f$A B = C\f$
/*!

    The matrices \f$A\f$, \f$B\f$ and \f$C\f$ need to have correct dimensions.
    
    \param A The matrix to multiply.
    \param B The vector to multiply with.
    \param C Pointer to an empty matrix in which the result is returned.
    \return A non zero value is returned in case of an error.   
*/
int mat_mult( const matrix * A, const matrix * B, matrix * C );

//! Multiply a scalar and a matrix matrices	\f$a B = C\f$
/*!

    The matrices \f$B\f$ and \f$C\f$ need to have the same dimensions.
    
    \param a The scalar value.
    \param B The matrix to multiply with.
    \param C Pointer to an empty matrix in which the result is returned.
    \return A non zero value is returned in case of an error.   
*/
int mat_multScalar( const REAL a, const matrix * B, matrix * C );

//! Multiply two matrices element wise \f$A \mathrm{.*} B = C\f$.
/*!

    The matrix \f$A\f$ is multiplied element wise with vector \f$B\f$. The output matrix \f$C\f$ must have the same dimensions as matrix \f$A\f$.
    
    \param A    Matrix to multiply with.
    \param B    Vector to multiply with.
    \param C    An empty matrix in which the result is returned.
        
    \return     A non zero value is returned in case of an error.   
*/
int mat_multElement( const matrix * A, const matrix * B, matrix * C );

//! Solve the set of linear equations \f$A X = B\f$.
/*!

    The method used to solve the equation is Guassian Elimination, also known as LU decomposition.
    
    \param A    Matrix input.
    \param B    The solution of the equation, must have the same number of rows as \f$A\f$.
    \param X    An empty matrix in which the result is returned. \f$X\f$ must have the same number of rows as \f$A\f$.
        
    \return     A non zero value is returned in case of an error.   
*/
int mat_solve( const matrix * A, const matrix * B, matrix * X );

//! Compute the dot product between to row or column vectors.
/*!

    \param A    The first input vector to use.
    \param B    The second input vector to use.
    \param dot  REAL variable in which the result is returned.

    \return     A non zero value is returned in case of an error.
*/
int mat_dot( const matrix * A, const matrix * B, REAL * dot ) ;

//! Compute the 2-norm of a row or column vector. 
/*!
    
    \param A    The input vector to use.
    \param nrm2 REAL variable in which the result is returned.
    
    \return     A non zero value is returned in case of an error.
*/
int mat_nrm2( const matrix * A, REAL * nrm2 ) ;

//! Compute the 1-norm of a row or column vector.
/*!
    
    \param A    The input vector to use.
    \param nrm1 REAL variable in which the result is returned.
    
    \return     A non zero value is returned in case of an error.
*/
int mat_nrm1( const matrix * A, REAL * nrm1 );

//!@}   


//! \name Logic operations  
//!@{

//! Check to see if the matrices \f$A\f$ and \f$B\f$ have the same dimensions or not.
/*!
   \return      0   If \f$A\f$ and \f$B\f$ do not have the same dimensions.\n
                1   If \f$A\f$ and \f$B\f$ have the same dimensions.
*/  
int mat_sameDim( const matrix * A, const matrix * B );

//! Check to see if the matrices A and B have the same values 
/*!
    \return     0   Not the same.   \n
                1   The same values.
*/
int mat_equal(const matrix * A, const matrix * B );

//!@}   


//! \name Sub matrix operations 
//!@{

//! Insert matrix A into matrix B at coordinates (row, col).
/*!
    
    In the case \f$ A \in \mathbb{R}^{ \mathrm{M} \times \mathrm{N} }\f$ than matrix 
    \f$B\f$ must at least be \f$ B \in \mathbb{R}^{ \mathrm{(M+\mathrm{row})} \times \mathrm{(N+\mathrm{col})} }\f$.
    
    The indices row and col start from zero. 
        
    \param A    The matrix to insert into matrix \f$B\f$.
    \param B    The matrix to alter by inserting matrix \f$A\f$.
    \param row  The row index at which the first element of \f$A\f$ will be inserted in to \f$B\f$. The indexing starts from zero.	
    \param col  The column index at which the first element of \f$A\f$ will be inserted in to \f$B\f$. The indexing starts from zero.
    
    \return     A non zero value is returned in case of an error.   
*/
int mat_insert( const matrix * A, matrix * B, const int row, const int col );

//! Extract a sub matrix B from matrix A starting from coordinates (row, col).
/*! 
    In the case \f$ B \in \mathbb{R}^{ \mathrm{M} \times \mathrm{N} }\f$ than matrix 
    \f$A\f$ must at least be \f$ A \in \mathbb{R}^{ \mathrm{(M+\mathrm{row})} \times \mathrm{(N+\mathrm{col})} }\f$.
    
    \param A    The matrix to extract matrix \f$B\f$ from. \f$A\f$ is not altered.
    \param B    The matrix to copy the values to.
    \param row  The row index of the first element to extract from \f$A\f$. The indexing starts from zero.	
    \param col  The column index of the first element to extract from \f$A\f$. The indexing starts from zero.   

*/
int mat_extract( const matrix * A, matrix * B, const int row, const int col );

//!@}   


//! \name Auxiliary operations 
//!@{

//! Do a linear 1d interpolation on data matrices
/*!
    This function is especially important when using schedules, in which
    point-wise interpolation of given tables is essential.
    
    Note that the coupled vectors \f$(\mathbf{x},\mathbf{y})\f$ should be such that \f$\mathbf{x}\f$ is assending
    
    \param x_mat    Matrix of elements for \f$\mathbf{x}\f$.
    \param y_mat    Matrix of elements for \f$\mathbf{y}\f$.
    \param N        The number of elements in \f$\mathbf{x}\f$ and \f$\mathbf{y}\f$ (must be equal). 
    \param x_in     The point \f$x\f$ at which to interpolate.
    
    \return The calculated value is returned.
*/
REAL mat_interp1( const matrix * x_mat, const matrix * y_mat, const int N, const REAL x_in );

//! A linear 1d interpolation on an array.
/*!
    This function is especially important when using schedules, in which
    point-wise interpolation of given tables is essential.
    
    Note that the coupled vectors \f$(\mathbf{x},\mathbf{y})\f$ should be such that \f$\mathbf{x}\f$ is assending
    
    \param x    Array of elements for \f$\mathbf{x}\f$.
    \param y    Array of elements for \f$\mathbf{y}\f$.
    \param N    The number of elements in \f$\mathbf{x}\f$ and \f$\mathbf{y}\f$ (must be equal). 
    \param x_in The point \f$x\f$ at which to interpolate.
    
    \return The calculated value is returned.
*/
REAL interp1( const REAL *x, const REAL *y, const int N, const REAL x_in );

//!@}   


//! \name User output operations  
//!@{

//! Print a human readable representation of the matrix A in char buf outString.
#ifndef _MITA_EXPORT_
int mat_printf(const matrix * A);
#endif

//!@}   

#endif

/** @}*/
/** @}*/
/* ------------------------------------------------------------------------------ */

#endif

