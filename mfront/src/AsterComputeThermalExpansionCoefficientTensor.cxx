/*!
 * \file   AsterComputeThermalExpansionCoefficientTensor.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   18 mar 2008
 */

#include<algorithm>

#include"TFEL/Math/stensor.hxx"
#include"MFront/Aster/AsterComputeThermalExpansionCoefficientTensor.hxx"

namespace aster
{

  static void
  AsterComputeIsotropicThermalExpansionCoefficientTensor2D(const AsterReal* const props,
					       tfel::config::Types<2u,AsterReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    const double a = props[0];
    A(0)=A(1)=A(2)=a;
    A(3)=0.;
  }

  static void
  AsterComputeOrthotropicThermalExpansionCoefficientTensor2D(const AsterReal* const props,
						 tfel::config::Types<2u,AsterReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    const double a0 = props[0];
    const double a1 = props[1];
    const double a2 = props[2];
    A(0)=a0;
    A(1)=a1;
    A(2)=a2;
    A(3)=0.;
  } // end of struct AsterComputeThermalExpansionCoefficientTensor
 
  void
  AsterComputeThermalExpansionCoefficientTensor<1u,ISOTROPIC>::exe(const AsterReal* const props,
							tfel::config::Types<1u,AsterReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    const double a = props[0];
    A(0)=A(1)=A(2)=a;
  } // end of struct AsterComputeThermalExpansionCoefficientTensor

  void
  AsterComputeThermalExpansionCoefficientTensor<2u,ISOTROPIC>::exe(const AsterReal* const props,
							 tfel::config::Types<2u,AsterReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    AsterComputeIsotropicThermalExpansionCoefficientTensor2D(props,A);
  } // end of struct AsterComputeThermalExpansionCoefficientTensor

  void
  AsterComputeThermalExpansionCoefficientTensor<3u,ISOTROPIC>::exe(const AsterReal* const props,
						       tfel::config::Types<3u,AsterReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    const double a = props[0];
    A(0)=A(1)=A(2)=a;
    A(3)=A(4)=A(5)=0.;
  } // end of struct AsterComputeThermalExpansionCoefficientTensor

  void
  AsterComputeThermalExpansionCoefficientTensor<1u,ORTHOTROPIC>::exe(const AsterReal* const props,
							  tfel::config::Types<1u,AsterReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    const double a0 = props[0];
    const double a1 = props[1];
    const double a2 = props[2];
    A(0)=a0;
    A(1)=a1;
    A(2)=a2;
  } // end of struct AsterComputeThermalExpansionCoefficientTensor

  void
  AsterComputeThermalExpansionCoefficientTensor<2u,ORTHOTROPIC>::exe(const AsterReal* const props,
							  tfel::config::Types<2u,AsterReal,false>::ThermalExpansionCoefficientTensor& C)
  {
    AsterComputeOrthotropicThermalExpansionCoefficientTensor2D(props,C);
  } // end of struct AsterComputeThermalExpansionCoefficientTensor

  void
  AsterComputeThermalExpansionCoefficientTensor<3u,ORTHOTROPIC>::exe(const AsterReal* const props,
							 tfel::config::Types<3u,AsterReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    const double a0 = props[0];
    const double a1 = props[1];
    const double a2 = props[2];
    A(0)=a0;
    A(1)=a1;
    A(2)=a2;
    A(3)=A(4)=A(5)=0.;
  } // end of struct AsterComputeThermalExpansionCoefficientTensor

} // end of namespace aster