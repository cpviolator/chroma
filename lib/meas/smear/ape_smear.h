//  $Id: ape_smear.h,v 1.1 2003-09-12 04:31:45 edwards Exp $

#ifndef __ape_smear__
#define __ape_smear__

//! Construct APE smeared links from:
/*!
 * \ingroup smear
 *
 *
 *      x               x           \           x------x
 *      +               |            \                 |
 *      +     =     c * |     +       \                |
 *      +               |             /                |
 *      x               x            /          x------x
 *                                  /______ 
 *
 *
 * Arguments:
 *
 * where c is the smearing factor sm_fact, and projected back onto SU(Nc)

 * Warning: this works only for Nc = 2 and 3 !

 * \param u        gauge field ( Read )
 * \param u_smear  smeared gauge field ( Write )
 * \param mu       direction of smeared gauge field ( Read )
 * \param bl_level blocking level (of the u's) ( Read )
 * \param sm_fact  smearing factor ( Read )
 * \param BlkAccu  accuracy in fuzzy link projection ( Read )
 * \param BlkMax   maximum number of iterations in fuzzy link projection ( Read )
 * \paramj_decay   no staple in direction j_decay ( Read )
 */

void APE_Smear(const multi1d<LatticeColorMatrix>& u,
	       multi1d<LatticeColorMatrix>& u_smear,
	       int mu, int bl_level, 
	       const Real& sm_fact, const Real& BlkAccu, 
	       int BlkMax, int j_decay);

#endif
