// $Id: formfac_w.cc,v 1.11 2003-06-25 16:11:44 edwards Exp $
/*! \file
 *  \brief Form-factors 
 *
 *  Form factors constructed from a quark and a sequential quark propagator
 */
// $Log: formfac_w.cc,v $
// Revision 1.11  2003-06-25 16:11:44  edwards
// Changed from nml to xml.
//
// Revision 1.10  2003/04/02 22:28:22  edwards
// Changed proto.h to qdp_util.h
//
// Revision 1.9  2003/04/01 03:01:28  edwards
// Fixed doxygen comments.
//
// Revision 1.8  2003/04/01 02:38:26  edwards
// Added doxygen comments.
//
// Revision 1.7  2003/03/31 19:54:15  edwards
// Fixed doxygen comments.
//
// Revision 1.6  2003/03/20 19:34:25  flemingg
// Evolved formfac_w.cc to use SftMom class, which included some bug fixes
// in features in SftMom which had been previously untested and evolution
// of the corresponding test program.
//

#include "chromabase.h"
#include "util/ft/sftmom.h"
#include "meas/hadron/formfac_w.h"
#include "qdp_util.h"                  // part of QDP++, for crtesn()

using namespace QDP;

//! Compute contractions for current insertion 3-point functions.
/*!
 * \ingroup hadron
 *
 * This routine is specific to Wilson fermions!
 *
 * \param u        -- gauge fields (used for non-local currents) ( Read )
 * \param quark_propagator -- quark propagator ( Read )
 * \param seq_quark_prop -- sequential quark propagator ( Read )
 * \param phases   -- fourier transofmr phase factors ( Read )
 * \param t0       -- cartesian coordinates of the source ( Read )
 * \param xml      -- xml file object ( Read )
 */

void FormFac(const multi1d<LatticeColorMatrix>& u, 
             const LatticePropagator& quark_propagator,
             const LatticePropagator& seq_quark_prop, 
             const SftMom& phases,
             int t0,
             XMLWriter& xml)
{
  START_CODE("FormFac");

  // Length of lattice in j_decay direction and 3pt correlations fcns
  int length = phases.numSubsets();

  multi1d<Complex> local_cur3ptfn(length);
  multi1d<Complex> nonlocal_cur3ptfn(length);
  
  int G5 = Ns*Ns-1;
  
  // Construct the anti-quark propagator from the seq. quark prop.
  LatticePropagator anti_quark_prop = Gamma(G5) * seq_quark_prop * Gamma(G5);

  // Rough timings (arbitrary units):
  //   Variant 1: 120
  //   Variant 2: 140
  // See previous cvs versions (before 1.10) for Variant 2 - only keeping Variant 1

  XMLArrayWriter xml_array(xml, Nd*Nd);
  push(xml_array, "FormFac");

  // Loop over gamma matrices of the insertion current of insertion current
  for(int gamma_value = 0; gamma_value < Nd*Nd; ++gamma_value)
  {
    push(xml_array);
    Write(xml_array, gamma_value) ;

    //  For the case where the gamma value indicates we are evaluating either
    //  the vector or axial vector currents, we will also evaluate
    //  the non-local currents.  The non-local vector current is the conserved
    //  current.  The non-local axial vector current would be partially
    //  conserved but for the Wilson term.  In these cases we will set
    //  mu = corresponding direction.  In all other cases, we will set mu = -1.

    bool compute_nonlocal ;
    int mu;

    switch(gamma_value){
    case  1:
    case 14:
      mu = 0;
      compute_nonlocal = true ;
      break;
    case  2:
    case 13:
      mu = 1;
      compute_nonlocal = true ;
      break;
    case  4:
    case 11:
      mu = 2;
      compute_nonlocal = true ;
      break;
    case  8:
    case  7:
      mu = 3;
      compute_nonlocal = true ;
      break;
    default:
      mu = -1;
      compute_nonlocal = false ;
    }

    // The local non-conserved vector-current matrix element 
    LatticeComplex corr_local_fn =
      trace(adj(anti_quark_prop) * Gamma(gamma_value) * quark_propagator);

    multi2d<DComplex> hsum, hsum_nonlocal ;
    hsum = phases.sft(corr_local_fn) ;

    // Construct the non-local current matrix element 
    //
    // The form of J_mu = (1/2)*[psibar(x+mu)*U^dag_mu*(1+gamma_mu)*psi(x) -
    //                           psibar(x)*U_mu*(1-gamma_mu)*psi(x+mu)]
    // NOTE: the 1/2  is included down below in the sumMulti stuff
    LatticeComplex corr_nonlocal_fn;
    if(compute_nonlocal){
      corr_nonlocal_fn =
        trace(adj(u[mu] * shift(anti_quark_prop, FORWARD, mu)) *
          (quark_propagator + Gamma(gamma_value) * quark_propagator));
      LatticePropagator tmp_prop1 = u[mu] *
        shift(quark_propagator, FORWARD, mu);
      corr_nonlocal_fn -= trace(adj(anti_quark_prop) *
                            (tmp_prop1 - Gamma(gamma_value) * tmp_prop1));

      hsum_nonlocal = phases.sft(corr_nonlocal_fn) ;
    }

    XMLArrayWriter xml_inser_mom(xml_array, phases.numMom());
    push(xml_inser_mom, "Momenta");

    // Loop over insertion momenta and print out results
    for(int inser_mom_num=0; inser_mom_num<phases.numMom(); ++inser_mom_num) 
    {
      push(xml_inser_mom);
      Write(xml_inser_mom, inser_mom_num);
      write(xml_inser_mom, "inser_mom", phases.numToMom(inser_mom_num)) ;

      for (int t=0; t < phases.numSubsets(); ++t) 
      {
        int t_eff = (t - t0 + length) % length ;

        local_cur3ptfn[t_eff] = Complex(hsum[inser_mom_num][t]) ;

        if (compute_nonlocal) {
          nonlocal_cur3ptfn[t_eff]
            = 0.5 * Complex(hsum_nonlocal[inser_mom_num][t]) ;
        }
      } // end for(t)

      // Print out the results
      push(xml_inser_mom, "Wilson_Local_Current_3Pt_fn") ;
      Write(xml_inser_mom, local_cur3ptfn) ;
      pop(xml_inser_mom) ;

      if (compute_nonlocal) {
        push(xml_inser_mom, "Wilson_NonLocal_Current_3Pt_fn") ;
        Write(xml_inser_mom, nonlocal_cur3ptfn) ;
        pop(xml_inser_mom) ;
      }

      pop(xml_inser_mom);  // elem
    } // end for(inser_mom_num)

    pop(xml_inser_mom);    // Momenta
    pop(xml_array);        // elem
  } // end for(gamma_value)
                            
  pop(xml_array);          // FormFac

  END_CODE("FormFac");
}
