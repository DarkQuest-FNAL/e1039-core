#ifndef PHG4FPbScSteppingAction_h
#define PHG4FPbScSteppingAction_h

#include "g4main/PHG4SteppingAction.h"

class PHCompositeNode;
class PHG4FPbScDetector;
class PHG4Hit;
class PHG4HitContainer;

class PHG4FPbScSteppingAction : public PHG4SteppingAction
{
  public:
    PHG4FPbScSteppingAction( PHG4FPbScDetector* );
    virtual ~PHG4FPbScSteppingAction(){}
    
    virtual bool UserSteppingAction(const G4Step*, bool);
    
    virtual void SetInterfacePointers( PHCompositeNode* );
    
  private:
    PHG4FPbScDetector* detector_;
    PHG4HitContainer* hits_;
    PHG4Hit* hit;
};



#endif

