#ifndef PHG4VFPbScRegionSteppingAction_h
#define PHG4VFPbScRegionSteppingAction_h

#include <Geant4/G4UserSteppingAction.hh>


class PHCompositeNode;
class PHG4FPbScDetector;
class PHG4Hit;
class PHG4HitContainer;

class PHG4FPbScRegionSteppingAction : public G4UserSteppingAction
{

  public:

  //! constructor
  PHG4FPbScRegionSteppingAction( PHG4FPbScDetector* );

  //! destroctor
  virtual ~PHG4FPbScRegionSteppingAction()
  {}

  //! stepping action
  virtual void UserSteppingAction(const G4Step*);

  //! reimplemented from base class
  virtual void SetInterfacePointers( PHCompositeNode* );

  private:

  //! pointer to the detector
  PHG4FPbScDetector* detector_;

  //! pointer to hit container
  PHG4HitContainer * hits_;
  PHG4Hit *hit;
};


#endif //__G4PHPHYTHIAREADER_H__
