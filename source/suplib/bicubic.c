/* ---------------------------------------------------------------------------------
 *          file : bicubic.c (C-source file)                                      *
 *   description : Header file for all bicubic spline interpolation functions     *
 *       toolbox : DotX Control Solutions BV                                      *
 *        author : James Wattstraat 23-25, Alkmaar, The Netherlands               *
--------------------------------------------------------------------------------- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "./suplib.h"

bicu_mesh * bicu_initmesh( int Nx, int Ny )
{

    bicu_mesh * new_mesh = (bicu_mesh*) calloc(1, sizeof(bicu_mesh));
    
    new_mesh->Nx = Nx;
    new_mesh->Ny = Ny;
    
    new_mesh->r = (REAL*) calloc(4, sizeof(REAL));

    new_mesh->n1 = (REAL*) calloc(Nx*Ny, sizeof(REAL));
    new_mesh->n2 = (REAL*) calloc(Nx*Ny, sizeof(REAL));

    new_mesh->e1 = (int*) calloc((Nx-1)*(Ny-1), sizeof(int));   
    new_mesh->e2 = (int*) calloc((Nx-1)*(Ny-1), sizeof(int));   
    new_mesh->e3 = (int*) calloc((Nx-1)*(Ny-1), sizeof(int));   
    new_mesh->e4 = (int*) calloc((Nx-1)*(Ny-1), sizeof(int));

    new_mesh->n2e = (int*) calloc(Nx*Ny, sizeof(int));
    
    new_mesh->coef = (REAL*) calloc(16*Nx*Ny, sizeof(REAL));

    
    return new_mesh;
};


// Load mesh data from the different config files.
// dir is the directory in which they are located.
// type is eighter "Cq" or "Ct".
bicu_mesh * bicu_loaddata( char* dir, char* type, int* loaderr )
{
    
    // Read in Nx, Ny and r.
    int Nx, Ny;
    REAL Nxd, Nyd;
    REAL temp;
    int k, j;
    bicu_mesh * mesh;
    
    FILE * fid; 
    char name[200];
    
    
    strcpy(name, dir);
    strcat( name, type );
    strcat( name, "_info.dat" );
    fid = fopen( name, "r");   // open DIR/Cq(or Ct)_info.dat
    if (fid==NULL)
    {
    	*loaderr = INFODAT;
    	return NULL;
    }
    fscanf( fid, "%lf" , &Nxd );
    fscanf( fid, "%lf" , &Nyd );
    
    Nx = (int)Nxd;
    Ny = (int)Nyd;
    
    mesh = bicu_initmesh( Nx, Ny ); // Init mesh struct based on info
    
    fscanf( fid, "%lf" , mesh->r   );
    fscanf( fid, "%lf" , mesh->r+1 );
    fscanf( fid, "%lf" , mesh->r+2 );
    fscanf( fid, "%lf" , mesh->r+3 );
    
    fclose(fid);    
    
    // Continue with n, e, n2e and coef
    
    
    strcpy( name, dir );
    strcat( name, type );
    strcat( name, "_n.dat" );   
    fid = fopen( name, "r" );   // open DIR/Cq(or Ct)_n.dat
    if (fid==NULL)
    {
    	*loaderr = NDAT;
    	return mesh;
    }
    for ( k = 0 ; k < Nx*Ny; ++k ) 
        fscanf( fid, "%lf %lf\n", mesh->n1+k, mesh->n2+k );
    fclose(fid);    


    strcpy( name, dir );
    strcat( name, type );
    strcat( name, "_e.dat" );   
    fid = fopen(name, "r");    // open DIR/Cq(or Ct)_e.dat
    if (fid==NULL)
    {
    	*loaderr = EDAT;
    	return mesh;
    }
    for ( k = 0 ; k < (Nx-1)*(Ny-1); ++k ) 
        fscanf( fid, "%d %d %d %d\n", mesh->e1+k, mesh->e2+k, mesh->e3+k, mesh->e4+k ); 
    fclose(fid);    
    

    strcpy(name, dir);
    strcat( name, type );
    strcat( name, "_n2e.dat" );	
    fid = fopen(name, "r");   // open DIR/Cq(or Ct)_n2e.dat
    if (fid==NULL)
    {
    	*loaderr = N2EDAT;
    	return mesh;
    }
    for ( k = 0 ; k < Nx*Ny; ++k ) 
        fscanf( fid, "%d\n", mesh->n2e + k );
    fclose(fid);
    

    strcpy(name, dir);
    strcat( name, type );
    strcat( name, "_coef.dat" );    
    fid = fopen(name, "r");   // open DIR/Cq(or Ct)_coef.dat
    if (fid==NULL)
    {
    	*loaderr = COEFDAT;
    	return mesh;
    }
    for ( k = 0 ; k < Nx*Ny; ++k ) 
    {
        for (j = 0; j <  16; ++j ) 
        {
            fscanf( fid, "%lf ", &temp );
            mesh->coef[ j + 16*k ] = temp;  
        }
    }
    fclose(fid);
    

    *loaderr = OKDAT;
    
    return mesh;
};


// Releace memory allocated to the mesh.
int bicu_freemesh( bicu_mesh * mesh )
{

    free(mesh->r);
    free(mesh->n1);
    free(mesh->n2);
    free(mesh->e1);
    free(mesh->e2);
    free(mesh->e3); 
    free(mesh->e4);
    free(mesh->n2e);
    free(mesh->coef);
    free(mesh);
    
    return 0;
};

// Returns the element index of the given coordinates.
int bicu_findelement(   const bicu_mesh * mesh, // IN
                        const REAL x,         // IN   x coordinate to interpolate at.
                        const REAL y,         // IN   y coordinate to interpolate at.
                        int * ei )              // OUT  index of the desired element.
{

    int ix = (int) floor( ( x - mesh->r[0] ) / ( mesh->r[1] - mesh->r[0] ) * ( mesh->Nx-1.0 ) + 1 ); // Matlab index
    int iy = (int) floor( ( y - mesh->r[2] ) / ( mesh->r[3] - mesh->r[2] ) * ( mesh->Ny-1.0 ) + 1 ); // Matlab index
    
    int index_node = ix + mesh->Nx * (iy-1) -1; // The second -1 is for Matlab to C index
    
    *ei = mesh->n2e[ index_node ] -1; // -1 is for Matlab to C indexes.
    
    if( *ei < 0 )
    {
        return 1;
    }   
    else
    {
        return 0;
    }

};  

// Interpolation function
int bicubic_interp( const bicu_mesh * mesh, // IN
                    const REAL x,         // IN   x coordinate to interpolate at.
                    const REAL y,         // IN   y coordinate to interpolate at.
                    REAL * z,             // OUT  interpolated value.
                    REAL * dzdx,          // OUT  derivative in x direction.
                    REAL * dzdy,          // OUT  derivative in y direction.
                    REAL * dzdxy )        // OUT  derivative in xy direction.
{
    int ei, node1, node2, node3, node4, i;
    REAL delta_x, delta_y, xl, yl, x2, x3, y2, y3,
        x_vec[16], y_vec[16], dx_vec[16], dy_vec[16];
    
    
        
    // Find the element index.
    if( bicu_findelement( mesh, x, y, &ei ) )
    {
        //printf( "bicubic_interp: element not found at index:\tx = %f\ty = %f\n", x, y);
        return 1;
    }
    
    // Obtain an array with all node indexces.
    node1 = mesh->e1[ei] -1;    // -1 for matlab to C indexes.
    node2 = mesh->e2[ei] -1;    
    node3 = mesh->e3[ei] -1;
    node4 = mesh->e4[ei] -1;    //not used
    
    // Calculate the space between two nodes
    delta_x = mesh->n1[node2] - mesh->n1[node1];
    delta_y = mesh->n2[node3] - mesh->n2[node1];
    
    
    // Calculate the local coordinate (between 0 and 1)
    xl = (x -  mesh->n1[node1]) / delta_x;
    yl = (y -  mesh->n2[node1]) / delta_y;
    
    
    x2 = xl*xl;
    x3 = xl*xl*xl;
    
    y2 = yl*yl;
    y3 = yl*yl*yl;
    
    for ( i = 0; i < 16; i += 4 ) 
    {
        x_vec[ i   ]  = 1.0;
        x_vec[ i+1 ]  = xl;
        x_vec[ i+2 ]  = x2;
        x_vec[ i+3 ]  = x3;
        dx_vec[ i   ] = 0.0;
        dx_vec[ i+1 ] = 1.0;
        dx_vec[ i+2 ] = 2.0*xl;
        dx_vec[ i+3 ] = 3.0*x2;
    }
    
    for ( i = 0; i < 4; i++ ) 
    {
        y_vec[ i    ]  = 1.0;
        y_vec[ i+4  ]  = yl;
        y_vec[ i+8  ]  = y2;
        y_vec[ i+12 ]  = y3;
        dy_vec[ i    ] = 0.0;
        dy_vec[ i+4  ] = 1.0;
        dy_vec[ i+8  ] = 2.0*yl;
        dy_vec[ i+12 ] = 3.0*y2;
    }
    
    *z      = 0.0;
    *dzdx   = 0.0;
    *dzdy   = 0.0;
    *dzdxy  = 0.0;
    
    
    
    
    for ( i = 0; i < 16; i++ )
    {
        *z      +=  mesh->coef[ i + ei*16 ] *  x_vec[i] *  y_vec[i];    
        *dzdx   +=  mesh->coef[ i + ei*16 ] * dx_vec[i] *  y_vec[i];    
        *dzdy   +=  mesh->coef[ i + ei*16 ] *  x_vec[i] * dy_vec[i];    
        *dzdxy  +=  mesh->coef[ i + ei*16 ] * dx_vec[i] * dy_vec[i];    
    };
    
    
    
    
    
    
    *dzdx   = *dzdx / delta_x;
    *dzdy   = *dzdy / delta_y;
    *dzdxy  = *dzdxy / (delta_x*delta_y);
    
    return 0;
    
};  

// The DotX equivalent of NCMDERIVS
int dotxderivs(     const bicu_mesh *Cq_mesh,           // IN   Mesh data of Cq
                    const bicu_mesh *Ct_mesh,           // IN   Mesh data of Ct
                        REAL lambda,                    // IN   Value of lambda to interpolate at.
                        REAL theta,                     // IN   Value of theta to interpolate at.
                        REAL *dCq_dLam, REAL *dCq_dTh,  // OUT  Derivatives of Cq along lambda and theta.
						REAL *dCt_dLam, REAL *dCt_dTh,  // OUT  Derivatives of Ct along lambda and theta.
						REAL *Cq,                         // OUT  Value of Cq at the given lambda and theta.
						REAL *Ct                          // OUT  Value of Ct at the given lambda and theta.
                )
{
    int err = 0;
    
    // The derivative in x and y direction is not required from dotxderivs but is required by the interp function.
    REAL dxy;
    
    if( lambda < Cq_mesh->r[0] ){   lambda = Cq_mesh->r[0];}    //  printf ("dotxderivs: Warning modifing lambda!\n"); 	}
    if( Cq_mesh->r[1] < lambda ){   lambda = Cq_mesh->r[1];}    //  printf ("dotxderivs: Warning modifing lambda!\n"); 	}	
    if( theta  < Cq_mesh->r[2] ){   theta  = Cq_mesh->r[2];}   //  printf ("dotxderivs: Warning modifing theta!\n"); 	}
    if( Cq_mesh->r[3] < theta  ){   theta  = Cq_mesh->r[3];}   //  printf ("dotxderivs: Warning modifing theta!\n");	}
    
    
    // Return an error incase lambda or theta are out of range.
    if( (lambda < Cq_mesh->r[0] || Cq_mesh->r[1] < lambda) || 
        (theta  < Cq_mesh->r[2] || Cq_mesh->r[3] < theta ) )
    {
        //printf ("dotxderivs: Error lambda or theta are out of range: \n \tlambda:\t%f >= %f <= %f \n \ttheta:\t%f >= %f <= %f \n", 
        //      Cq_mesh->r[0], lambda, Cq_mesh->r[1], Cq_mesh->r[2], theta, Cq_mesh->r[3] );
        return 1;
    };
    
    err += bicubic_interp( Cq_mesh, lambda, theta, Cq, dCq_dLam, dCq_dTh, &dxy );
    err += bicubic_interp( Ct_mesh, lambda, theta, Ct, dCt_dLam, dCt_dTh, &dxy );
    
    return err;

};









