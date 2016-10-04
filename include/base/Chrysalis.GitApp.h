#ifndef CHRYSALIS.GITAPP_H
#define CHRYSALIS.GITAPP_H

#include "MooseApp.h"

class Chrysalis.GitApp;

template<>
InputParameters validParams<Chrysalis.GitApp>();

class Chrysalis.GitApp : public MooseApp
{
public:
  Chrysalis.GitApp(InputParameters parameters);
  virtual ~Chrysalis.GitApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* CHRYSALIS.GITAPP_H */
