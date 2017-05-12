#ifndef CHRYSALISAPP_H
#define CHRYSALISAPP_H

#include "FunctionalExpansionAppInterface.h"

#include "MooseApp.h"

class ChrysalisApp;

template<>
InputParameters validParams<ChrysalisApp>();

class ChrysalisApp : public MooseApp,
                     public FunctionalExpansionAppInterface
{
public:
  ChrysalisApp(InputParameters parameters);
  virtual ~ChrysalisApp();

  virtual std::string header() const override;

  virtual bool IsFunctionalExpansionApp() override {return true;}

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* CHRYSALISAPP_H */
