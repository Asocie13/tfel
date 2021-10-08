/*!
 * \file   mfront/include/MFront/DefaultModelDSL.hxx
 * \brief
 * \author Thomas Helfer
 * \date   08 nov 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTDEFAULTMODELDSL_HXX
#define LIB_MFRONT_MFRONTDEFAULTMODELDSL_HXX

#include <string>
#include "MFront/DefaultDSLBase.hxx"

namespace mfront {

  /*!
   * \brief default implementation of a generic model
   */
  struct DefaultModelDSL : public DefaultDSLBase {
    //! \return the name of the DSL
    static std::string getName();
    //! \return a description of the DSL
    static std::string getDescription();
    //! \brief default constructor
    DefaultModelDSL();
    //! \return a description of the DSL
    BehaviourDSLDescription getBehaviourDSLDescription() const override;
    //! \brief destructor
    ~DefaultModelDSL() override;
  };  // end of struct DefaultModelDSL

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTDEFAULTMODELDSL_HXX */
