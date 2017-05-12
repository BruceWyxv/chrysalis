#ifndef MESHFEMULTIAPP_H
#define MESHFEMULTIAPP_H

#include "MultiApp.h"

// Forward delcarations
class MeshFEMultiApp;

template<>
InputParameters validParams<MeshFEMultiApp>();

/**
 * This base type of MultiApp will interface between master and sub apps that
 * contain mix of mesh and functional expansion based data.
 */
class MeshFEMultiApp : public MultiApp
{
public:
  MeshFEMultiApp(const InputParameters & parameters);

  virtual void preTransfer(Real dt, Real target_time) override;

  virtual bool solveStep(Real td, Real target_time, bool auto_advance = true) override;

  virtual void advanceStep() override;
};

#endif // MESHFEMULTIAPP_H
