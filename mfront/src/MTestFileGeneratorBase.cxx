/*!
 * \file  mfront/src/MTestFileGeneratorBase.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 09 juil. 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <fstream>
#include <iterator>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "MFront/MTestFileGeneratorBase.hxx"

// fixing a bug on current glibc++ cygwin versions (19/08/2015)
#if defined __CYGWIN__ && (!defined _GLIBCXX_USE_C99)
#include <sstream>
namespace std {
  template <typename T>
  std::string to_string(const T& v) {
    std::ostringstream s;
    s << v;
    return s.str();
  }
}  // namespace std
#endif /* defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99) */

namespace mfront {

  MTestFileGeneratorBase::InternalStateVariable::~InternalStateVariable() =
      default;

  MTestFileGeneratorBase::ExternalStateVariable::~ExternalStateVariable() =
      default;

  unsigned int MTestFileGeneratorBase::getIdentifier() {
    static unsigned int i = 0;
    return ++i;
  }

  MTestFileGeneratorBase::MTestFileGeneratorBase()
      : hypothesis(tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS),
        handleThermalExpansion(false),
        hasRotationMatrix(false) {
  }  // end of MTestFileGeneratorBase::MTestFileGeneratorBase

  void MTestFileGeneratorBase::setHandleThermalExpansion(const bool b) {
    this->handleThermalExpansion = b;
  }

  void MTestFileGeneratorBase::setModellingHypothesis(
      const tfel::material::ModellingHypothesis::Hypothesis h) {
    using namespace tfel::material;
    tfel::raise_if(this->hypothesis != ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                   "MTestFileGeneratorBase::setModellingHypothesis: "
                   "modelling hypothesis already set");
    tfel::raise_if(h == ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                   "MTestFileGeneratorBase::setModellingHypothesis: "
                   "invalid modelling hypothesis");
    this->hypothesis = h;
  }  // end of MTestFileGeneratorBase::setModellingHypothesis

  void MTestFileGeneratorBase::addTime(const MTestFileGeneratorBase::real t) {
    tfel::raise_if(!this->times.insert(t).second,
                   "MTestFileGeneratorBase::addTime: "
                   "time '" +
                       std::to_string(t) + "' already defined");
  }  // end of MTestFileGeneratorBase::addTime

  void MTestFileGeneratorBase::setRotationMatrix(const real m00,
                                                 const real m01,
                                                 const real m02,
                                                 const real m10,
                                                 const real m11,
                                                 const real m12,
                                                 const real m20,
                                                 const real m21,
                                                 const real m22) {
    this->hasRotationMatrix = true;
    this->m[0] = m00;
    this->m[1] = m01;
    this->m[2] = m02;
    this->m[3] = m10;
    this->m[4] = m11;
    this->m[5] = m12;
    this->m[6] = m20;
    this->m[7] = m21;
    this->m[8] = m22;
  }  // end of MTestFileGeneratorBase::addTime

  void MTestFileGeneratorBase::addMaterialProperty(
      const std::string& n, const MTestFileGeneratorBase::real v) {
    tfel::raise_if(!(this->mps.insert({n, v}).second),
                   "MTestFileGeneratorBase::addMaterialProperty: "
                   "material property '" +
                       n + "' already defined");
  }  // end of MTestFileGeneratorBase::addMaterialProperty

  void MTestFileGeneratorBase::addInternalStateVariable(
      const std::string& n,
      const SupportedTypes::TypeFlag f,
      const real* const v) {
    for (const auto& iv : this->ivs) {
      tfel::raise_if(iv.name == n,
                     "MTestFileGeneratorBase::addInternalStateVariable: "
                     "variable already declared '" +
                         n + "'");
    }
    const auto s = this->getVariableSize(f);
    InternalStateVariable iv;
    iv.name = n;
    iv.type = f;
    iv.values.resize(s);
    std::copy(v, v + s, iv.values.begin());
    this->ivs.push_back(iv);
  }  // end of MTestFileGeneratorBase::addInternalStateVariable

  void MTestFileGeneratorBase::addExternalStateVariable(
      const std::string& n,
      const SupportedTypes::TypeFlag f,
      const real t0,
      const real* const v0,
      const real t1,
      const real* const v1,
      const bool increment_provided) {
    for (const auto& ev : this->evs) {
      tfel::raise_if(ev.name == n,
                     "MTestFileGeneratorBase::addExternalStateVariable: "
                     "variable already declared '" +
                         n + "'");
    }
    const auto s = this->getVariableSize(f);
    ExternalStateVariable ev;
    ev.name = n;
    ev.type = f;
    ev.initial_values.first = t0;
    ev.final_values.first = t1;
    ev.initial_values.second.resize(s);
    ev.final_values.second.resize(s);
    std::copy(v0, v0 + s, ev.initial_values.second.begin());
    if (increment_provided) {
      for (size_t i = 0; i != s; ++i) {
        ev.final_values.second[i] = v0[i] + v1[i];
      }
    } else {
      std::copy(v1, v1 + s, ev.final_values.second.begin());
    }
    this->evs.push_back(ev);
  }  // end of MTestFileGeneratorBase::addExternalStateVariable

  void MTestFileGeneratorBase::generate(const std::string& n) const {
    std::ofstream file(n + "-" + std::to_string(getIdentifier()) + ".mtest");
    tfel::raise_if(!file,
                   "MTestFileGeneratorBase::generate: "
                   "can't open file '" +
                       n + ".mtest'");
    file << "@MaximumNumberOfSubSteps 1;\n";
    if (this->handleThermalExpansion) {
      file << "@HandleThermalExpansion true;\n";
    } else {
      file << "@HandleThermalExpansion false;\n";
    }
    this->writeModellingHypothesis(file);
    this->writeBehaviourDeclaration(file);
    this->writeRotationMatrix(file);
    this->writeMaterialProperties(file);
    this->writeInternalStateVariables(file);
    this->writeExternalStateVariables(file);
    this->writeGradients(file);
    this->writeTimes(file);
  }  // end of MTestFileGeneratorBase::generate

  void MTestFileGeneratorBase::writeRotationMatrix(std::ostream& os) const {
    if (this->hasRotationMatrix) {
      os.precision(14);
      os << "@RotationMatrix {{" << m[0] << "," << m[1] << "," << m[2] << "},\n"
         << "                 {" << m[3] << "," << m[4] << "," << m[5] << "},\n"
         << "                 {" << m[6] << "," << m[7] << "," << m[8]
         << "}};\n\n";
    }
  }

  void MTestFileGeneratorBase::writeModellingHypothesis(
      std::ostream& os) const {
    using namespace tfel::material;
    tfel::raise_if(this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                   "MTestFileGeneratorBase::writeModellingHypothesis: "
                   "undefined modelling hypothesis");
    os << "@ModellingHypothesis '"
       << ModellingHypothesis::toString(this->hypothesis) << "';\n";
  }

  void MTestFileGeneratorBase::writeMaterialProperties(std::ostream& os) const {
    if (this->mps.empty()) {
      return;
    }
    os << "// Material properties\n";
    for (const auto& mp : this->mps) {
      os.precision(14);
      os << "@MaterialProperty<constant> '" << mp.first << "' " << mp.second
         << ";\n";
    }
    os << '\n';
  }  // end of MTestFileGeneratorBase::writeMaterialProperties

  void MTestFileGeneratorBase::writeTimes(std::ostream& os) const {
    tfel::raise_if(this->times.empty(),
                   "MTestFileGeneratorBase::writeTimes: "
                   "no times defined");
    tfel::raise_if(this->times.size() < 2,
                   "MTestFileGeneratorBase::writeTimes : "
                   "only one time given");
    os << "// Times\n"
       << "@Times {";
    os.precision(14);
    for (auto p = this->times.begin(); p != times.end();) {
      os << *p;
      if (++p != times.end()) {
        os << ", ";
      }
    }
    os << "};\n\n";
  }  // end of MTestFileGeneratorBase::writeTimes

  void MTestFileGeneratorBase::writeInternalStateVariables(
      std::ostream& os) const {
    if (this->ivs.empty()) {
      return;
    }
    os << "// Internal state variables\n";
    for (const auto& iv : this->ivs) {
      os << "@InternalStateVariable '" << iv.name << "' ";
      os.precision(14);
      if (iv.type == SupportedTypes::SCALAR) {
        os << iv.values[0] << ";\n";
      } else {
        const auto s = this->getVariableSize(iv.type);
        os << "{";
        for (unsigned short i = 0; i != s;) {
          os << iv.values[i];
          if (++i != s) {
            os << ",";
          }
        }
        os << "};\n";
      }
    }
    os << '\n';
  }  // end of MTestFileGeneratorBase::writeMaterialProperties

  void MTestFileGeneratorBase::writeExternalStateVariables(
      std::ostream& os) const {
    if (this->evs.empty()) {
      return;
    }
    os << "// External state variables\n";
    for (const auto& ev : this->evs) {
      const auto& n = ev.name;
      os.precision(14);
      os << "@ExternalStateVariable<evolution> '" << n << "' {";
      if (ev.type == SupportedTypes::SCALAR) {
        os << ev.initial_values.first << " : " << ev.initial_values.second[0]
           << ", " << ev.final_values.first << " : "
           << ev.final_values.second[0];
      } else {
        const auto& s = ev.initial_values.second.size();
        for (size_t i = 0; i != s; ++i) {
          if (i != 0) {
            os << ", ";
          }
          os << "{" << ev.initial_values.first << " : "
             << ev.initial_values.second[i] << ", " << ev.final_values.first
             << " : " << ev.final_values.second[i] << "}";
        }
      }
      os << "};\n";
    }
    os << '\n';
  }  // end of MTestFileGeneratorBase::writeExternalStateVariables

  std::vector<std::string>
  MTestFileGeneratorBase::getDeformationGradientComponentsNames() const {
    using namespace tfel::material;
    const std::string exts[9u] = {"FXX", "FYY", "FZZ", "FXY", "FYX",
                                  "FXZ", "FZX", "FYZ", "FZY"};
    const std::string aexts[5u] = {"FRR", "FZZ", "FTT", "FRZ", "FZR"};
    std::vector<std::string> n;
    tfel::raise_if(
        this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS,
        "MTestFileGeneratorBase::getDeformationGradientsComponentsNames: "
        "undefined modelling hypothesis");
    if ((this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
        (this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      copy(aexts, aexts + 3u, back_inserter(n));
    } else if (this->hypothesis == ModellingHypothesis::AXISYMMETRICAL) {
      copy(aexts, aexts + 5u, back_inserter(n));
    } else if ((this->hypothesis == ModellingHypothesis::PLANESTRESS) ||
               (this->hypothesis == ModellingHypothesis::PLANESTRAIN) ||
               (this->hypothesis ==
                ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      copy(exts, exts + 5u, back_inserter(n));
    } else if (this->hypothesis == ModellingHypothesis::TRIDIMENSIONAL) {
      copy(exts, exts + 9u, back_inserter(n));
    } else {
      tfel::raise(
          "MTestFileGeneratorBase::getDeformationGradientsComponentsNames: "
          "unsupported hypothesis");
    }
    return n;
  }

  std::vector<std::string> MTestFileGeneratorBase::getStrainComponentsNames()
      const {
    using namespace tfel::material;
    const std::string exts[6u] = {"EXX", "EYY", "EZZ", "EXY", "EXZ", "EYZ"};
    const std::string aexts[4u] = {"ERR", "EZZ", "ETT", "ERZ"};
    std::vector<std::string> n;
    tfel::raise_if(this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                   "MTestFileGeneratorBase::getStrainComponentsNames: "
                   "undefined modelling hypothesis");
    if ((this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
        (this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      copy(aexts, aexts + 3u, back_inserter(n));
    } else if (this->hypothesis == ModellingHypothesis::AXISYMMETRICAL) {
      copy(aexts, aexts + 4u, back_inserter(n));
    } else if ((this->hypothesis == ModellingHypothesis::PLANESTRESS) ||
               (this->hypothesis == ModellingHypothesis::PLANESTRAIN) ||
               (this->hypothesis ==
                ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      copy(exts, exts + 4u, back_inserter(n));
    } else if (this->hypothesis == ModellingHypothesis::TRIDIMENSIONAL) {
      copy(exts, exts + 6u, back_inserter(n));
    } else {
      tfel::raise(
          "MTestFileGeneratorBase::getStrainComponentsNames: "
          "unsupported hypothesis");
    }
    return n;
  }

  std::vector<std::string> MTestFileGeneratorBase::getStressComponentsNames()
      const {
    using namespace tfel::material;
    const std::string exts[6u] = {"SXX", "SYY", "SZZ", "SXY", "SXZ", "SYZ"};
    const std::string aexts[4u] = {"SRR", "SZZ", "STT", "SRZ"};
    std::vector<std::string> n;
    tfel::raise_if(this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                   "MTestFileGeneratorBase::getStressComponentsNames: "
                   "undefined modelling hypothesis");
    if ((this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
        (this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      copy(aexts, aexts + 3u, back_inserter(n));
    } else if (this->hypothesis == ModellingHypothesis::AXISYMMETRICAL) {
      copy(aexts, aexts + 4u, back_inserter(n));
    } else if ((this->hypothesis == ModellingHypothesis::PLANESTRESS) ||
               (this->hypothesis == ModellingHypothesis::PLANESTRAIN) ||
               (this->hypothesis ==
                ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      copy(exts, exts + 4u, back_inserter(n));
    } else if (this->hypothesis == ModellingHypothesis::TRIDIMENSIONAL) {
      copy(exts, exts + 6u, back_inserter(n));
    } else {
      tfel::raise(
          "MTestFileGeneratorBase::getStressComponentsNames: "
          "unsupported hypothesis");
    }
    return n;
  }

  unsigned short MTestFileGeneratorBase::getVariableSize(
      const SupportedTypes::TypeFlag& f) const {
    if (f == SupportedTypes::SCALAR) {
      return 1u;
    } else if (f == SupportedTypes::TVECTOR) {
      return this->getTVectorSize();
    } else if (f == SupportedTypes::STENSOR) {
      return this->getStensorSize();
    } else if (f == SupportedTypes::TENSOR) {
      return this->getTensorSize();
    }
    tfel::raise(
        "MTestFileGeneratorBase::getVariableSize: "
        "unsupported variable type");
  }  // end of MTestFileGeneratorBase::getVariableSize

  unsigned short MTestFileGeneratorBase::getTVectorSize() const {
    using namespace tfel::material;
    tfel::raise_if(this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                   "MTestFileGeneratorBase::getTVectorSize: "
                   "undefined modelling hypothesis");
    if ((this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
        (this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      return 1u;
    } else if ((this->hypothesis == ModellingHypothesis::AXISYMMETRICAL) ||
               (this->hypothesis == ModellingHypothesis::PLANESTRESS) ||
               (this->hypothesis == ModellingHypothesis::PLANESTRAIN) ||
               (this->hypothesis ==
                ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      return 2u;
    } else if (this->hypothesis == ModellingHypothesis::TRIDIMENSIONAL) {
      return 3u;
    }
    tfel::raise(
        "MTestFileGeneratorBase::getTVectorSize : "
        "unsupported modelling hypothesis");
  }  // end of MTestFileGeneratorBase::getTVectorSize

  unsigned short MTestFileGeneratorBase::getStensorSize() const {
    using namespace tfel::material;
    tfel::raise_if(this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                   "MTestFileGeneratorBase::getStensorSize: "
                   "undefined modelling hypothesis");
    if ((this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
        (this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      return 3u;
    } else if ((this->hypothesis == ModellingHypothesis::AXISYMMETRICAL) ||
               (this->hypothesis == ModellingHypothesis::PLANESTRESS) ||
               (this->hypothesis == ModellingHypothesis::PLANESTRAIN) ||
               (this->hypothesis ==
                ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      return 4u;
    } else if (this->hypothesis == ModellingHypothesis::TRIDIMENSIONAL) {
      return 6u;
    }
    tfel::raise(
        "MTestFileGeneratorBase::getStensorSize : "
        "unsupported modelling hypothesis");
  }  // end of MTestFileGeneratorBase::getStensorSize

  unsigned short MTestFileGeneratorBase::getTensorSize() const {
    using namespace tfel::material;
    tfel::raise_if(this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                   "MTestFileGeneratorBase::getTensorSize: "
                   "undefined modelling hypothesis");
    if ((this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
        (this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      return 3u;
    } else if ((this->hypothesis == ModellingHypothesis::AXISYMMETRICAL) ||
               (this->hypothesis == ModellingHypothesis::PLANESTRESS) ||
               (this->hypothesis == ModellingHypothesis::PLANESTRAIN) ||
               (this->hypothesis ==
                ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      return 5u;
    } else if (this->hypothesis == ModellingHypothesis::TRIDIMENSIONAL) {
      return 9u;
    }
    tfel::raise(
        "MTestFileGeneratorBase::getTensorSize : "
        "unsupported modelling hypothesis");
  }  // end of MTestFileGeneratorBase::getTensorSize

  MTestFileGeneratorBase::~MTestFileGeneratorBase() = default;

}  // end of namespace mfront
