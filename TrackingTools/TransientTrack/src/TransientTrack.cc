#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/PatternTools/interface/TransverseImpactPointExtrapolator.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
//#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <iostream>

using namespace reco;

TransientTrack::TransientTrack( const Track & tk , const MagneticField* field) : 
  //  Track(tk), tk_(&tk), tkr_(0), stateAtVertexAvailable(false) 
  Track(tk), tkr_(0), theField(field), stateAtVertexAvailable(false) 
{
  originalTSCP = TrajectoryStateClosestToPoint
    (parameters(), pt(), covariance(), GlobalPoint(0.,0.,0.), theField);
}


TransientTrack::TransientTrack( const TrackRef & tk , const MagneticField* field) : 
  //  Track(*tk), tk_(&(*tk)), tkr_(&tk), stateAtVertexAvailable(false) 
  Track(*tk), tkr_(&tk), theField(field), stateAtVertexAvailable(false) 
{
  originalTSCP = TrajectoryStateClosestToPoint
    (parameters(), pt(), covariance(), GlobalPoint(0.,0.,0.), theField);
}


TransientTrack::TransientTrack( const TransientTrack & tt ) :
  Track(tt), tkr_(tt.persistentTrackRef()), theField(tt.field()), stateAtVertexAvailable(false) 
{
//   std::cout << "construct from TransientTrack" << std::endl;
  originalTSCP = tt.impactPointTSCP();
  if (tt.stateAtVertexAvailable) theStateAtVertex= tt.impactPointState();
//   originalTSCP = TrajectoryStateClosestToPoint
//     (parameters(), covariance(), GlobalPoint(0.,0.,0.), theField);
//   std::cout << "construct from TransientTrack OK" << std::endl;
}


TransientTrack& TransientTrack::operator=(const TransientTrack & tt)
{
//   std::cout << "assign op." << std::endl;
  if (this == &tt) return *this;
  //
  //  std::cout << tt.tk_ << std::endl;
//   std::cout << "assign base." << std::endl;
  Track::operator=(tt);
//   std::cout << "done assign base." << std::endl;
  //  tk_ = &(tt.persistentTrack());
  //  tk_ = tt.tk_;
//   std::cout << "assign ref." << std::endl;
  tkr_ = tt.persistentTrackRef();
//   std::cout << "done assign ref." << std::endl;
  originalTSCP = tt.originalTSCP;
  stateAtVertexAvailable = tt.stateAtVertexAvailable;
  theStateAtVertex = tt.theStateAtVertex;
  theField = tt.field();
//   std::cout << "assign op. OK" << std::endl;
  
  return *this;
}


TrajectoryStateOnSurface TransientTrack::impactPointState() const
{
  if (!stateAtVertexAvailable) calculateStateAtVertex();
  return theStateAtVertex;
}


void TransientTrack::calculateStateAtVertex() const
{
  //  edm::LogInfo("TransientTrack") 
  //    << "initial state validity:" << originalTSCP.theState() << "\n";
  TransverseImpactPointExtrapolator tipe(theField);
  theStateAtVertex = tipe.extrapolate(
     originalTSCP.theState(), originalTSCP.position());
  //  edm::LogInfo("TransientTrack") 
  //    << "extrapolated state validity:" 
  //    << theStateAtVertex.isValid() << "\n";
  
  stateAtVertexAvailable = true;
}

