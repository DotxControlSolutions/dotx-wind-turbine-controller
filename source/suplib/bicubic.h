/* ---------------------------------------------------------------------------------
 *          file : bicubic.h (C-header file)                                      *
 *   description : Header file for all bicubic spline interpolation functions     *
 *       toolbox : DotX Control Solutions BV                                      *
 *        author : James Wattstraat 23-25, Alkmaar, The Netherlands               *
--------------------------------------------------------------------------------- */

#ifndef _BICUBIC_H_
#define _BICUBIC_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup suplib 
 *  @{*/

 /* ------------------------------------------------------------------------------ */
/** \addtogroup bicubic Bicubic interpolation
    
    To interpolate over two variables you can use this bicubic interpolation 
    algorithm. It is based on a quadratic function depending on two variables 
    and 16 coefficients:
    \f[
            z(X,Y) = \sum_{i=0}^{3} \sum_{j=0}^{3} c_{ij} x_l^i y_l^j
    \f]
    in which: \n
    \f$c_{ij}\f$    are the 16 coefficients, \n
    \f$x_l\f$       is the normalized x coordinate between two nodes, \n
    \f$y_l\f$       is the normalized y coordinate between two nodes. \n
    
    The coefficients are obtained from data already known. This data should be 
    on an evenly spread grid in the	x and y direction. The figure below depicts 
    the \f$x\f$ and \f$y\f$ axes. Within this axes system the grids is 
    positioned at some random location between \f$ x_{min} \rightarrow x_{max} 
    \f$ and \f$ y_{min} \rightarrow y_{max} \f$. It is not required that delta
    x is equal to delta y. 
    
    \image html bicu_mesh.png
    
    The grid is comprised out of nodes, indicated with a dot. A group of four 
    nodes is called an element. On the grid there are \f$N_x\f$ nodes along the 
    \f$x\f$ axis and \f$N_y\f$ nodes along the \f$y\f$ axis. This amounts to 
    \f$N_x N_y\f$ nodes and \f$(N_x -1) (N_y -1)\f$ elements. The nodes and 
    elements are numbered using an index number starting from zero. The figure 
    below depicts the manner of numbering.
    
    \image html bicu_mesh_indeces.png   
    
    To be able to interpolate in between the nodes it is required to have the 
    value of \f$z\f$ and its derivatives at the nodes. The coefficients at 
    each node are solved using the equation:
    \f[
        c = A^{-1} x
    \f]
    in which:
    \f$
        c = \left[ \begin{array}{cccccccccccccccc} 
        c_{00} & c_{10} & c_{20} & c_{30} &
        c_{01} & c_{11} & c_{21} & c_{31} &
        c_{02} & c_{12} & c_{22} & c_{32} &
        c_{03} & c_{13} & c_{23} & c_{33} 
        \end{array} \right]^T
    \f$
    , 
    \f$
        A^{-1} = \left[ \begin{array}{rrrrrrrrrrrrrrrr} 
            1 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0  \\
            0 & 0 & 0 & 0 & 1 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0  \\ 
            -3 & 3 & 0 & 0 &-2 &-1 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0  \\
            2 &-2 & 0 & 0 & 1 & 1 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0  \\
            0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 1 & 0 & 0 & 0 & 0 & 0 & 0 & 0  \\
            0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 1 & 0 & 0 & 0  \\
            0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 &-3 & 3 & 0 & 0 &-2 &-1 & 0 & 0  \\
            0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 2 &-2 & 0 & 0 & 1 & 1 & 0 & 0  \\
            -3 & 0 & 3 & 0 & 0 & 0 & 0 & 0 &-2 & 0 &-1 & 0 & 0 & 0 & 0 & 0  \\
            0 & 0 & 0 & 0 &-3 & 0 & 3 & 0 & 0 & 0 & 0 & 0 &-2 & 0 &-1 & 0  \\
            9 &-9 &-9 & 9 & 6 & 3 &-6 &-3 & 6 &-6 & 3 &-3 & 4 & 2 & 2 & 1  \\
            -6 & 6 & 6 &-6 &-3 &-3 & 3 & 3 &-4 & 4 &-2 & 2 &-2 &-2 &-1 &-1  \\
            2 & 0 &-2 & 0 & 0 & 0 & 0 & 0 & 1 & 0 & 1 & 0 & 0 & 0 & 0 & 0  \\
            0 & 0 & 0 & 0 & 2 & 0 &-2 & 0 & 0 & 0 & 0 & 0 & 1 & 0 & 1 & 0  \\
            -6 & 6 & 6 &-6 &-4 &-2 & 4 & 2 &-3 & 3 &-3 & 3 &-2 &-1 &-2 &-1  \\
            4 &-4 &-4 & 4 & 2 & 2 &-2 &-2 & 2 &-2 & 2 &-2 & 1 & 1 & 1 & 1 
        \end{array} \right]
    \f$	
    and
    \f$
        x = \left[ \begin{array}{cccccccccccccccc} 
        f(0,0) & f(1,0) & f(0,1) & f(1,1) &
        f_x(0,0) & f_x(1,0) & f_x(0,1) & f_x(1,1) &
        f_y(0,0) & f_y(1,0) & f_y(0,1) & f_y(1,1) & 
        f_{xy}(0,0) & f_{xy}(1,0) & f_{xy}(0,1) & f_{xy}(1,1) 
        \end{array} \right]^T
    \f$ 
    In the \f$x\f$ vector we see the data values (\f$f\f$) and derivatives 
    (\f$f_x\f$, \f$f_y\f$, \f$f_{xy}\f$) at the nodes appear. \f$x\f$ comprises
    out of the values at each of the four nodes in an element.
    
    The Matlab script \link bicu_calc_coef.m \endlink sovles the the coefficients from given 
    data. The script \link generate_even_spread_data.m \endlink generates input files from Cq
    and Ct data from Phatas. These input files can be read by the function 
    \link bicu_loaddata \endlink
    

 *  @{*/

/*! \enum LOADEECERR
    Different devices that can run the mcu. Continue _info.dat, n, e, n2e and coef
*/
enum  {

	OKDAT                                  ,   //!<
    INFODAT                                ,   //!<
    NDAT                                   ,   //!<
    EDAT                                   ,   //!<
    N2EDAT                                 ,   //!<
    COEFDAT                                    //!<

} ;

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES 
/*!
    \file bicubic.h
    \brief All bicubic spline interpolation functions.
*/
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES 

#define TYPE_CQ 1 //!< Type ID for CQ data
#define TYPE_CT 2 //!< Type ID for CT data

#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_STRUCTS
//! Structure holding mesh data of the bicubic interpolation functions.
typedef struct bicu_mesh
{
    
    int       Nx    ;   /*!< Number of elements along the x axis. */
    int       Ny    ;   /*!< Number of elements along the y axis. */
    REAL  * r     ;   /*!< Array of four (4) elements holding the minimum and maximum values along the x and y axes: [xmin xmax ymin ymax]^T.  */
    REAL  * n1    ;   /*!< X coordinates of the nodes, size: Nx*Ny.  */
    REAL  * n2    ;   /*!< Y coordinates of the nodes, size: Nx*Ny.  */
    int     * e1    ;   /*!< Array indicating the lower left nodes beloning to the elements, size: (Nx-1)*(Ny-1).  */
    int     * e2    ;   /*!< Array indicating the lower right nodes beloning to the elements, size: (Nx-1)*(Ny-1).  */
    int     * e3    ;   /*!< Array indicating the upper left nodes beloning to the elements, size: (Nx-1)*(Ny-1). */
    int     * e4    ;   /*!< Array indicating the upper right nodes beloning to the elements, size: (Nx-1)*(Ny-1). */
    int     * n2e   ;   /*!< Conversion array between a node index and an element index, size: Nx*Ny. */
    REAL  * coef  ;   /*!< Array with the coefficients, size: 16*Nx*Ny; */
    
} bicu_mesh;
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS

//! Allocate memory for a mesh struct of size Nx by Ny.
/*!

    The allocated memory is empty. The user can use the function \link bicu_loaddata \endlink 
    to load data from a file. 
    
    Do not forget to free the memory after after it is no longer needed using 
    \link bicu_freemesh \endlink.
    
    \param Nx   Number of elements along the x axis.
    \param Ny   Number of elements along the y axis.
        
    \return     A new bicu_mesh struct. 
    
    \sa bicu_loaddata bicu_freemesh
*/
bicu_mesh * bicu_initmesh( int Nx, int Ny );

//! Load mesh data from the different config files. 
/*!
    A new bicu_mesh struct is created and data found in the given directory by 
    the given type name is loaded.
    
    A number of files are required to completely load all data:  
    - *_info.dat
    - *_n.dat
    - *_e.dat
    - *_n2e.dat 
    - *_coef.dat
    
    The * is replaced by the combination of dir and type. For example if dir 
    equals to "C:\data_folder\" and type equals to "Ct", the file names would
    be "C:\data_folder\Ct_info.dat", etc.
    
    These files can be generated from unevely spread Phatas Cq and Ct data 
    using the Matlab script \link generate_even_spread_data.m \endlink.
    
    \param  dir     The directory to load files from.
    \param  type    The type file name.
    
*/
bicu_mesh * bicu_loaddata( char* dir, char* type, int* loaderr );

//! Free the memory of the given bicu_mesh struct.
int bicu_freemesh( bicu_mesh * mesh );

//! Returns the element index of the given coordinates.
/*!

    \param mesh     The bicu_mesh structure of which the index is to be found.
    \param x        Input the x coordinate to interpolate at.
    \param y        Input the y coordinate to interpolate at.
    \param ei       Used to return the index of the desired element.
    
    \return         A value larger than zero is returned in case of an error.

*/
int bicu_findelement(   

        const bicu_mesh * mesh,
        const REAL      x   ,
        const REAL      y   ,
              int       * ei 
              
);  

//! Interpolation function
/*!
    Given a x and y coordinate the function calculates the interpolated z value
    and the derivatives at this point.
    
    \param mesh     The bicu_mesh structure on which to interpolate.
    \param x        The input x coordinate to interpolate at.
    \param y        The input y coordinate to interpolate at.
    \param z        The output interpolated value.  
    \param dzdx     The output derivative in x direction \f$ \frac{dz}{dx}\f$.
    \param dzdy     The output derivative in y direction \f$ \frac{dz}{dy}\f$.
    \param dzdxy    The output derivative in x and y direction \f$ \frac{dz}{dxdy}\f$.  
    
    \return     A value larger than zero is returned in case of an error. This could mean x is equal the right most node or y is equal to the upper node.
*/
int bicubic_interp( 

        const bicu_mesh * mesh ,    
        const REAL      x    ,
        const REAL      y    ,
              REAL    * z    ,
              REAL    * dzdx ,
              REAL    * dzdy ,
              REAL    * dzdxy
                
);  

//! CpCt interpolation with derivatives
/*!
    \param Cq_mesh      The input mesh data of Cq
    \param Ct_mesh      The input mesh data of Ct
    \param lambda       The input value of lambda to interpolate at.
    \param theta        The input value of theta to interpolate at.
    \param dCq_dLam     The output derivatives of Cq along lambda, \f$ \frac{dC_q}{d\lambda} \f$. 
    \param dCq_dTh      The output derivatives of Cq along theta, \f$ \frac{dC_q}{d\theta} \f$. 
    \param dCq_dLam     The output derivatives of Ct along lambda, \f$ \frac{dC_t}{d\lambda} \f$.
    \param dCq_dTh      The output derivatives of Ct along theta, \f$ \frac{dC_t}{d\theta} \f$.
    \param Cq           The output value of Cq at the given lambda and theta.   
    \param Ct           The output value of Ct at the given lambda and theta.
    
    \return     A value larger than zero is returned in case of an error.
*/
int dotxderivs( 

        const bicu_mesh * Cq_mesh    ,  
        const bicu_mesh * Ct_mesh    ,  
              REAL      lambda     ,
              REAL      theta      ,
              REAL    * dCq_dLam   ,
              REAL    * dCq_dTh    ,
              REAL    * dCt_dLam   ,
              REAL    * dCt_dTh    ,
              REAL    * Cq         ,
              REAL    * Ct

);
#endif

/** @}*/
/** @}*/
/* ------------------------------------------------------------------------------ */

#endif
