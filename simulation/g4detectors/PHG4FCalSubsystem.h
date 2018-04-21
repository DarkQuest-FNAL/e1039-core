#ifndef PHG4FCalSubsystem_h
#define PHG4FCalSubsystem_h

#include "g4main/PHG4Subsystem.h"

class PHG4FCalDetector;
class PHG4FCalSteppingAction;

class PHG4FCalSubsystem: public PHG4Subsystem
{
  
  public:
    
    //! constructor
    PHG4FCalSubsystem( const char* name = "FCAL" );
    
    //! destructor
    virtual ~PHG4FCalSubsystem( void )
    {}
    
    //! init
    /*!
    creates the detector_ object and place it on the node tree, under "DETECTORS" node (or whatever)
    reates the stepping action and place it on the node tree, under "ACTIONS" node
    creates relevant hit nodes that will be populated by the stepping action and stored in the output DST
    */
    int Init(PHCompositeNode *);
    
    //! event processing
    /*!
    get all relevant nodes from top nodes (namely hit list)
    and pass that to the stepping action
    */
    int process_event(PHCompositeNode *);
    
    //! accessors (reimplemented)
    virtual PHG4Detector* GetDetector( void ) const;
    
  private:
    
    //! detector geometry
    /*! defives from PHG4Detector */
    PHG4FCalDetector* detector_;
    
    
};

#endif
