// $Id: sn_jacob.h,v 1.3 2005-02-10 22:22:42 edwards Exp $
#ifndef __sn_jacob_h__
#define __sn_jacob_h__

namespace Chroma {

//! Single-node Jacobi rotation
/*!
 * \ingroup eig
 *
 * This subroutine contains a "single node" Jacobi routine
 * to be used with the Ritz functional eigenvialue/vector finder.
 *
 *
 *  \param psi		Eigenvectors			(Modify)
 *  \param N_eig	Eigenvalue number 		(Read)
 *  \param lambda	Diagonals / Eigenvalues		(Modify)
 *  \param off_diag	Upper triang off-diag matrix elems	(Modify)
 *  \param tolererance	Tolerance for off-diag elems	(Read)
 *  \param N_max	Maximal number of Jacobi iters	(Read)
 *  \param sub          Subset to use                   (Read) 
 *
 *  \return 	        Number of Jacobi iters		(Write) 
 */
int SN_Jacob(multi1d<LatticeFermion>& psi, 
	     const int N_eig, 
	     multi1d<Real>& lambda, 
	     multi1d<Complex>& off_diag, 
	     Real tolerance, 
	     int N_max,
	     const OrderedSubset& sub);

}  // end namespace Chroma

#endif
