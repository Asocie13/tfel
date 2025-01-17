/*!
 * \file   PragerKinematicHardeningRule.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04/04/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_PRAGERKINEMATICHARDENINGRULE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_PRAGERKINEMATICHARDENINGRULE_HXX

#include "MFront/BehaviourBrick/KinematicHardeningRuleBase.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief class describing the Prager kinematic hardening rule
     */
    struct PragerKinematicHardeningRule : KinematicHardeningRuleBase {
      std::string buildBackStrainImplicitEquations(
          const BehaviourDescription&,
          const StressPotential&,
          const StressCriterion&,
          const std::vector<std::shared_ptr<KinematicHardeningRule>>&,
          const std::string&,
          const std::string&,
          const bool) const override;
      //! destructor
      ~PragerKinematicHardeningRule() override;
    };  // end of struct KinematicHardeningRule

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_PRAGERKINEMATICHARDENINGRULE_HXX */
