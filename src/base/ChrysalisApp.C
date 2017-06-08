#include "ChrysalisApp.h"

#include "DopplerHeat.h"
#include "DopplerHeatAux.h"
#include "FECoefficientsUserObject.h"
#include "FEToAuxVariable.h"

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

ChrysalisApp::ChrysalisApp(InputParameters parameters)
  : MooseApp(parameters)
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

std::string
ChrysalisApp::header() const
{
  return  "MOOSE Wrapper for:                                  ==┴╮╭┴==      \n"
          "  _                   .-=-.           .-=-.          .-==-.       \n"
          " { }      __        .' O o '.       .' O o '.       ╱  <*  )--<   \n"
          " { }    .' O'.     ╱ o .-. O ╲     ╱ o .-. O ╲     ╱  .---`       \n"
          " { }   ╱ .-. o╲   ╱O  ╱   ╲  o╲   ╱O  ╱   ╲  o╲   ╱O ╱            \n"
          "  ╲ `-` ╱   ╲ O`-'o  ╱     ╲  O`-'o  ╱     ╲  O`-`o ╱             \n"
          "   `-.-`     '.____.'       `._____.'       `.____.'              \n"
                                                                            "\n"
          "Serpent 2.1.29 (© Jaakko Leppänen, VTT)"                         "\n"
          "Compatibility with subsequent Serpent versions is not guaranteed  \n"
                                                                            "\n";
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
  registerKernel(DopplerHeat);
  registerAuxKernel(DopplerHeatAux);
  registerAuxKernel(FEToAuxVariable);
  registerUserObject(FECoefficientsUserObject);
}

// External entry point for dynamic syntax association
extern "C" void ChrysalisApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory) { ChrysalisApp::associateSyntax(syntax, action_factory); }
void
ChrysalisApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
  // Nothing here
}
