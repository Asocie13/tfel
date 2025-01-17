/*!
 * \file   mfront/src/EuroplexusInterfaceExceptions.cxx
 * \brief  This file implements the EuroplexusInterfaceExceptions class.
 * \author Thomas Helfer
 * \date   12/12/2011
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <sstream>
#include <iostream>

#include "TFEL/Raise.hxx"
#include "MFront/Europlexus/EuroplexusInterfaceExceptions.hxx"

namespace epx {

  void EuroplexusInterfaceExceptions::throwUnMatchedNumberOfMaterialProperties(
      const std::string& b, const unsigned short n1, const EuroplexusInt n2) {
    std::ostringstream msg;
    msg << "EuroplexusInterfaceExceptions::"
           "throwUnMatchedNumberOfMaterialProperties : "
        << "the number of material properties does not match. The behaviour '"
        << b << "' requires " << n1 << " material properties, and " << n2
        << " material properties were declared";
    tfel::raise<EuroplexusException>(msg.str());
  }  // end of throwUnMatchedNumberOfMaterialProperties

  void EuroplexusInterfaceExceptions::throwUnMatchedNumberOfStateVariables(
      const std::string& b, const unsigned short n1, const EuroplexusInt n2) {
    std::ostringstream msg;
    msg << "EuroplexusInterfaceExceptions::"
           "throwUnMatchedNumberOfStateVariables : "
        << "the number of internal state variables does not match. The "
           "behaviour '"
        << b << "' requires " << n1 << " state variables, and " << n2
        << " state variables were declared";
    tfel::raise<EuroplexusException>(msg.str());
  }  // end of throwUnMatchedNumberOfStateVariables

  void
  EuroplexusInterfaceExceptions::throwUnMatchedNumberOfExternalStateVariables(
      const std::string& b, const unsigned short n1, const EuroplexusInt n2) {
    std::ostringstream msg;
    msg << "EuroplexusInterfaceExceptions::"
           "throwUnMatchedNumberOfExternalStateVariables : "
        << "the number of external state variables does not match. The "
           "behaviour '"
        << b << "' requires " << n1 << " state variables, and " << n2
        << " state variables were declared";
    tfel::raise<EuroplexusException>(msg.str());
  }  // end of throwUnMatchedNumberOfExternalStateVariables

  void EuroplexusInterfaceExceptions::treatEuroplexusException(
      const std::string& b, const EuroplexusException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown an EuroplexusException : " << e.what()
              << std::endl;
  }  // end of EuroplexusInterfaceExceptions::treatEuroplexusException

  void EuroplexusInterfaceExceptions::treatMaterialException(
      const std::string& b, const tfel::material::MaterialException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown an MaterialException : " << e.what()
              << std::endl;
  }  // end of treatMaterialException

  void EuroplexusInterfaceExceptions::treatTFELException(
      const std::string& b, const tfel::exception::TFELException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown a generic tfel exception : " << e.what()
              << std::endl;
  }  // end of treatTFELException

  void EuroplexusInterfaceExceptions::treatStandardException(
      const std::string& b, const std::exception& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown a generic standard exception : " << e.what()
              << std::endl;
  }  // end of treatStandardException

  void EuroplexusInterfaceExceptions::treatUnknownException(
      const std::string& b) {
    std::cout << "The behaviour '" << b << "' has thrown an unknown exception"
              << std::endl;
  }  // end of treatUnknownException

  void EuroplexusInterfaceExceptions::throwNegativeTimeStepException(
      const std::string& b) {
    tfel::raise(
        "EuroplexusInterfaceExceptions::throwNegativeTimeStepException: "
        "negative time step detected for behaviour '" +
        b + "'");
  }  // end of EuroplexusInterfaceExceptions::throwNegativeTimeStepException

  void EuroplexusInterfaceExceptions::throwInvalidDDSDDEValueException(
      const std::string& b, const EuroplexusReal v) {
    tfel::raise(
        "EuroplexusInterfaceExceptions::throwInvalidDDSDDEValueException: "
        "invalid value " +
        std::to_string(v) +
        " for DDSDDE. "
        "Invalid call to behaviour '" +
        b + "'");
  }  // end of EuroplexusInterfaceExceptions::throwInvalidDDSDDEValueException

  void
  EuroplexusInterfaceExceptions::throwInvalidTimeStepScalingFactorOnFailure(
      const std::string& b, const EuroplexusReal v) {
    tfel::raise(
        "EuroplexusInterfaceExceptions::"
        "throwInvalidTimeStepScalingFactorOnFailure: "
        "invalid value '" +
        std::to_string(v) +
        "' for time"
        "step scaling factor on failure. "
        "Invalid call to behaviour '" +
        b + "'");
  }  // end of EuroplexusInterfaceExceptions::throwInvalidDDSDDEValueException

  void EuroplexusInterfaceExceptions::throwPredictionComputationFailedException(
      const std::string& b) {
    tfel::raise(
        "EuroplexusInterfaceExceptions::"
        "throwPredictionComputationFailedException: "
        "prediction computation failed for behaviour '" +
        b + "'");
  }

  void EuroplexusInterfaceExceptions::throwPredictionOperatorIsNotAvalaible(
      const std::string& b) {
    tfel::raise(
        "EuroplexusInterfaceExceptions::throwPredictionOperatorIsNotAvalaible: "
        "behaviour '" +
        b + "' can't compute a prediction operator");
  }  // end of
     // EuroplexusInterfaceExceptions::throwBehaviourIntegrationFailedException

  void
  EuroplexusInterfaceExceptions::throwConsistentTangentOperatorIsNotAvalaible(
      const std::string& b) {
    tfel::raise(
        "EuroplexusInterfaceExceptions::"
        "throwConsistentTangentOperatorIsNotAvalaible: "
        "behaviour '" +
        b + "' can't compute a consistent tangent operator");
  }  // end of
     // EuroplexusInterfaceExceptions::throwBehaviourIntegrationFailedException

  void
  EuroplexusInterfaceExceptions::throwUnsupportedStressFreeExpansionException(
      const std::string& b) {
    tfel::raise(
        "EuroplexusInterfaceExceptions::"
        "throwUnsupportedStressFreeExpansionException: "
        "behaviour '" +
        b +
        "' can handle stress-free expansion "
        "but the Europlexus interface can't");
  }  // end of
     // EuroplexusInterfaceExceptions::throwUnsupportedStressFreeExpansionException

  void EuroplexusInterfaceExceptions::displayUnsupportedHypothesisMessage() {
    std::cout << "EuroplexusInterfaceExceptions::"
                 "displayUnsupportedHypothesisMessage : "
              << "unsupported hypothesis" << std::endl;
  }

  void EuroplexusUnSupportedCaseHandler::exe(
      const EuroplexusReal* const,
      const EuroplexusReal* const,
      const EuroplexusReal* const,
      const EuroplexusReal* const,
      const EuroplexusReal* const,
      const EuroplexusReal* const,
      const EuroplexusInt* const,
      const EuroplexusReal* const,
      const EuroplexusReal* const,
      EuroplexusReal* const,
      const EuroplexusInt* const,
      EuroplexusReal* const,
      const StressFreeExpansionHandler&) {
    tfel::raise(
        "EuroplexusUnSupportedCaseHandler::exe : "
        "we fall in a case that the Europlexus interface "
        "is not able to handle.");
  }  // end of exe

}  // end of namespace epx
