#ifndef FUNCTIONALEXPANSIONAPPINTERFACE_H
#define FUNCTIONALEXPANSIONAPPINTERFACE_H

// Forward Declarations
class FunctionalExpansionAppInterface;

class FunctionalExpansionAppInterface
{
public:
  FunctionalExpansionAppInterface();
  virtual ~FunctionalExpansionAppInterface() = default;

  virtual bool IsFunctionalExpansionApp() = 0;

protected:
  // Choose best object to store expansion coefficients here
};

#endif // FUNCTIONALEXPANSIONAPPINTERFACE_H
