add_subdirectory(umatxx)
add_subdirectory(mtest)

mfront_python_module(_mfront mfront.cxx
  MaterialKnowledgeDescriptionAttribute.cxx
  AbstractDSL.cxx
  FileDescription.cxx
  TargetsDescription.cxx
  CompiledTargetDescriptionBase.cxx
  LibraryDescription.cxx
  DSLFactory.cxx
  MaterialPropertyDSL.cxx
  MaterialPropertyDescription.cxx
  AbstractBehaviourDSL.cxx
  VariableBoundsDescription.cxx
  VariableDescription.cxx
  BehaviourData.cxx
  BehaviourSymmetryType
  BehaviourDescription.cxx
  SearchPathsHandler.cxx
  GeneratorOptions.cxx
  MakefileGenerator.cxx
  CMakeGenerator.cxx
  OverridableImplementation.cxx)
target_link_libraries(py_mfront__mfront
  PRIVATE
  TFELMFront
  MFrontLogStream TFELMaterial
  TFELMathParser  TFELSystem
  TFELUtilities   TFELGlossary
  TFELException   TFELConfig
  ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})

tfel_python_script(mfront __init__.py)
