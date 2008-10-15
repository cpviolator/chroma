// -*- C++ -*-
// $Id: inline_prop_3pt_w.h,v 1.1 2008-10-15 18:56:48 kostas Exp $
/*! \file
 * \brief Inline measurement of stochastic hadron operator (mesons and baryons).
 *
 * spectroscopy
 */

#ifndef __inline_prop_3pt_h__
#define __inline_prop_3pt_h__

#include "chromabase.h"
#include "meas/inline/abs_inline_measurement.h"
#include "io/qprop_io.h"
//#include <map>

namespace Chroma 
{ 
  /*! \ingroup inlinehadron */
  namespace InlineProp3ptEnv 
  {
    extern const std::string name;
    bool registerAll();
    
    // The flavors
    enum Flavor {up, down, strange, charm, bottom};

    //! Parameter structure
    /*! \ingroup inlinehadron */
    struct Params 
    {
      Params();
      Params(XMLReader& xml_in, const std::string& path);
       
      unsigned long      frequency;
    
      struct Param_t
      {
	multi1d<int> p ; /*!< the momentum inserted */
	int gamma ; /*!< insersion operator gamma Matrix 0..15 */
	Complex f  ; /*!< overal factor */

	multi1d<GroupXML_t> quarks ;     /*! dilutions */
	
      } param;
    
    
      struct NamedObject_t
      {
	std::string         gauge_id;
	std::string         prop_id;
	std::string         prop3pt_id;
      } named_obj;
      
      std::string xml_file;  // Alternate XML file pattern

      void write(XMLWriter& xml_out, const std::string& path);

    };
  

  //! Inline measurement of stochastic baryon operators
  /*! \ingroup inlinehadron */
    class InlineMeas : public AbsInlineMeasurement{
    protected:
      //! Do the measurement
      void func(const unsigned long update_no,
		XMLWriter& xml_out); 
      
    private:
 

      Params params;

    public:
      ~InlineMeas() {}
      InlineMeas(const Params& p) : params(p) {setUpMaps();}
      InlineMeas(const InlineMeas& p) : params(p.params) {setUpMaps();}
      
      unsigned long getFrequency(void) const {return params.frequency;}
      
      //! Do the measurement
      void operator()(const unsigned long update_no,
		      XMLWriter& xml_out); 
      
    };

  }; // name space InlineProp3ptEnv

};

#endif
