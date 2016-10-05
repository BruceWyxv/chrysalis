#ifndef CHRYSALISAPP_H
#define CHRYSALISAPP_H

#include "MooseApp.h"

class ChrysalisApp;

template<>
InputParameters validParams<ChrysalisApp>();

class ChrysalisApp : public MooseApp
{
public:
  ChrysalisApp(InputParameters parameters);
  virtual ~ChrysalisApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* CHRYSALISAPP_H */
