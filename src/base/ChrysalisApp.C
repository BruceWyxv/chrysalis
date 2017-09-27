#include "ChrysalisApp.h"
#include "ElementVariablesDifferenceVariance.h"
#include "FunctionElementIntegral.h"
#include "FunctionElementAverageValue.h"
#include "TREATHeat.h"
#include "TREATHeatAux.h"

// MOOSE includes
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

template<>
InputParameters validParams<ChrysalisApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

ChrysalisApp::ChrysalisApp(InputParameters parameters) :
    MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  ChrysalisApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  ChrysalisApp::associateSyntax(_syntax, _action_factory);
}

ChrysalisApp::~ChrysalisApp()
{
}

// External entry point for dynamic application loading
extern "C" void ChrysalisApp__registerApps() { ChrysalisApp::registerApps(); }
void
ChrysalisApp::registerApps()
{
  registerApp(ChrysalisApp);
}

// External entry point for dynamic object registration
extern "C" void ChrysalisApp__registerObjects(Factory & factory) { ChrysalisApp::registerObjects(factory); }
void
ChrysalisApp::registerObjects(Factory & factory)
{
  registerAuxKernel(TREATHeatAux);
  registerKernel(TREATHeat);
  registerPostprocessor(FunctionElementIntegral);
  registerPostprocessor(FunctionElementAverageValue);
  registerVectorPostprocessor(ElementVariablesDifferenceVariance);
}

// External entry point for dynamic syntax association
extern "C" void ChrysalisApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory) { ChrysalisApp::associateSyntax(syntax, action_factory); }
void
ChrysalisApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
}
