#include "ChrysalisApp.h"

#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

#include "header.h"
#define SV CODE_VERSION

template <>
InputParameters
validParams<ChrysalisApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

ChrysalisApp::ChrysalisApp(InputParameters parameters) : MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  ChrysalisApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  ChrysalisApp::associateSyntax(_syntax, _action_factory);
}

ChrysalisApp::~ChrysalisApp() {}

std::string
ChrysalisApp::header() const
{
  return "Chrysalis includes a MOOSE Wrapper for:\n"
         "Serpent " SV " (© Jaakko Leppänen, VTT)             ==┴╮ ╭┴==     \n"
         "  _                   .-=-.           .-=-.          .-===-.      \n"
         " { }      __        .' O o '.       .' O o '.       ╱   <*  )--<  \n"
         " { }    .' O'.     ╱ o .-. O ╲     ╱ o .-. O ╲     ╱ o .---`      \n"
         " { }   ╱ .-. o╲   ╱O  ╱   ╲  o╲   ╱O  ╱   ╲  o╲   ╱O  ╱           \n"
         "  ╲ `-` ╱   ╲ O`-'o  ╱     ╲  O`-'o  ╱     ╲  O`-`o  ╱            \n"
         "   `-.-`     '.____.'       `._____.'       `._____.'             \n"
         "Original wrapper was written for Serpent 2.1.30.\n"
         "Compatibility with future Serpent versions is not guaranteed (but likely).\n\n";
}

// External entry point for dynamic application loading
extern "C" void
ChrysalisApp__registerApps()
{
  ChrysalisApp::registerApps();
}
void
ChrysalisApp::registerApps()
{
  registerApp(ChrysalisApp);
}

// External entry point for dynamic object registration
extern "C" void
ChrysalisApp__registerObjects(Factory & factory)
{
  ChrysalisApp::registerObjects(factory);
}
void
ChrysalisApp::registerObjects(Factory & factory)
{
  Registry::registerObjectsTo(factory, {"ChrysalisApp"});
}

// External entry point for dynamic syntax association
extern "C" void
ChrysalisApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  ChrysalisApp::associateSyntax(syntax, action_factory);
}
void
ChrysalisApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
  // Nothing here
}
