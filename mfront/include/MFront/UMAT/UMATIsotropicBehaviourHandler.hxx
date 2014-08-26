/*! 
 * \file  UMATIsotropicBehaviourHandler.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 janv. 2013
 */

#ifndef _LIB_MFRONT_UMAT_UMATISOTROPICBEHAVIOURHANDLER_H_
#define _LIB_MFRONT_UMAT_UMATISOTROPICBEHAVIOURHANDLER_H_ 

#ifndef _LIB_MFRONT_UMAT_UMATBEHAVIOURHANDLER_H_
#error "This header shall not be called directly"
#endif

#include"TFEL/FSAlgorithm/copy.hxx"

namespace umat
{

  template<UMATBehaviourType type,
	   tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATIsotropicBehaviourHandlerBase
    : public UMATBehaviourHandler<type,H,Behaviour>
  {
    TFEL_UMAT_INLINE static
      void exe(const UMATReal *const DTIME ,
	       const UMATReal *const,
	       UMATReal *const DDSOE,
	       const UMATReal *const STRAN ,
	       const UMATReal *const DSTRAN,
	       const UMATReal *const TEMP  ,
	       const UMATReal *const DTEMP,
	       const UMATReal *const PROPS ,
	       const UMATInt  *const NPROPS,
	       const UMATReal *const PREDEF,
	       const UMATReal *const DPRED,
	       UMATReal *const STATEV,
	       const UMATInt  *const NSTATV,
	       UMATReal *const STRESS) 
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      typedef MechanicalBehaviourTraits<Behaviour<H,UMATReal,false> > MTraits;
      typedef UMATTraits<Behaviour<H,UMATReal,false> > Traits;
      typedef UMATBehaviourHandler<type,H,Behaviour> UMATBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessOperator;
      const bool ba = Traits::requiresThermalExpansionTensor;
      typedef typename IF<
	is_defined_,
	typename IF<
	  Traits::useTimeSubStepping,
	  typename UMATBehaviourHandler::template IntegratorWithTimeStepping<bs,ba>,
	  typename UMATBehaviourHandler::template Integrator<bs,ba>
	  >::type,
	typename UMATBehaviourHandler::Error>::type Handler;
      UMATBehaviourHandler::checkNPROPS(*NPROPS);
      UMATBehaviourHandler::checkNSTATV(*NSTATV);
      Handler handler(DTIME,STRAN,
		      DSTRAN,TEMP,DTEMP,PROPS,
		      PREDEF,DPRED,STATEV,STRESS);
      handler.exe(DDSOE,STRESS,STATEV);
    } // end of UMATIsotropicBehaviourHandlerBase::exe

  }; // end of struct UMATIsotropicBehaviourHandlerBase

  template<UMATBehaviourType type,
	   tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATIsotropicBehaviourHandler
    : public UMATIsotropicBehaviourHandlerBase<type,H,Behaviour>
  {
    using UMATIsotropicBehaviourHandlerBase<type,H,Behaviour>::exe;
  };

  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATIsotropicBehaviourHandler<COHESIVEZONEMODEL,H,Behaviour>
    : public UMATIsotropicBehaviourHandlerBase<COHESIVEZONEMODEL,H,Behaviour>
  {
    TFEL_UMAT_INLINE static
      void exe(const UMATReal *const DTIME ,
	       const UMATReal *const DROT,
	             UMATReal *const DDSOE,
	       const UMATReal *const STRAN ,
	       const UMATReal *const DSTRAN,
	       const UMATReal *const TEMP  ,
	       const UMATReal *const DTEMP,
	       const UMATReal *const PROPS ,
	       const UMATInt  *const NPROPS,
	       const UMATReal *const PREDEF,
	       const UMATReal *const DPRED,
	             UMATReal *const STATEV,
	       const UMATInt  *const NSTATV,
	             UMATReal *const STRESS) 
    {
      // local material properties to match castem conventions
      // In Cast3M, the tangential stiffness is given by PROPS[0],
      // the normal stiffness by PROPS[1].
      // In MFront, we want the normal stiffness to be the first one
      using namespace tfel::material;
      using tfel::fsalgo::copy;
      typedef Behaviour<H,UMATReal,false>  BV;
      typedef MechanicalBehaviourTraits<BV> MTraits;
      const unsigned short offset  = UMATTraits<BV>::propertiesOffset;
      const unsigned short nprops  = MTraits::material_properties_nb;
      const unsigned short NPROPS_ = offset+nprops == 0 ? 1u : offset+nprops; 
      UMATReal mp[NPROPS_];
      copy<NPROPS_>::exe(PROPS,mp);
      mp[0] = PROPS[1];
      mp[1] = PROPS[0];
      UMATIsotropicBehaviourHandlerBase<COHESIVEZONEMODEL,H,Behaviour>::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,
									    TEMP,DTEMP,mp,NPROPS,PREDEF,
									    DPRED,STATEV,NSTATV,STRESS);
    }
  };

}

#endif /* _LIB_MFRONT_UMAT_UMATISOTROPICBEHAVIOURHANDLER_H */

