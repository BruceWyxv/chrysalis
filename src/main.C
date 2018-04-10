#include "ChrysalisApp.h"
#include "MooseInit.h"
#include "Moose.h"
#include "MooseApp.h"
#include "AppFactory.h"

// Create a performance log
PerfLog Moose::perf_log("Chrysalis");

// Begin the main program.
int
main(int argc, char * argv[])
{
  // Initialize MPI, solvers and MOOSE
  MooseInit init(argc, argv);

  // Register this application's MooseApp and any it depends on
  ChrysalisApp::registerApps();

  // Create the shared app
  MooseAppPtr app = AppFactory::createAppShared("ChrysalisApp", argc, argv);

  // Execute the application
  app->run();

  return 0;
}
