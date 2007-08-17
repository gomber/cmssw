#include <memory>

#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <FWCore/Framework/interface/EDAnalyzer.h>
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/EventSetup.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/Framework/interface/MakerMacros.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>

#include <Geometry/Records/interface/MuonGeometryRecord.h>
#include <Geometry/CSCGeometry/interface/CSCGeometry.h>
#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <DataFormats/GeometryVector/interface/GlobalPoint.h>

#include <string>
#include <cmath>
#include <iomanip> // for setw() etc.
#include <vector>

class CSCGeometryAsChambers : public edm::EDAnalyzer {

   public:
 
     explicit CSCGeometryAsChambers( const edm::ParameterSet& );
      ~CSCGeometryAsChambers();

      virtual void analyze( const edm::Event&, const edm::EventSetup& );
 
      const std::string& myName() { return myName_;}

   private: 

      const int dashedLineWidth_;
      const std::string dashedLine_;
      const std::string myName_;
};

CSCGeometryAsChambers::CSCGeometryAsChambers( const edm::ParameterSet& iConfig )
 : dashedLineWidth_( 132 ), dashedLine_( std::string(dashedLineWidth_, '-') ), 
  myName_( "CSCGeometryAsChambers" )
{
}


CSCGeometryAsChambers::~CSCGeometryAsChambers()
{
}

void
 CSCGeometryAsChambers::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
   std::cout << myName() << ": Analyzer..." << std::endl;
   std::cout << "start " << dashedLine_ << std::endl;

   edm::ESHandle<CSCGeometry> pDD;
   iSetup.get<MuonGeometryRecord>().get( pDD );     
   std::cout << " Geometry node for CSCGeom is  " << &(*pDD) << std::endl;   
   std::cout << " I have "<<pDD->dets().size() << " detectors" << std::endl;
   std::cout << " I have "<<pDD->detTypes().size() << " types" << "\n" << std::endl;
   std::cout << " I have "<<pDD->detUnits().size()    << " detUnits" << std::endl;
   std::cout << " I have "<<pDD->dets().size()        << " dets" << std::endl;
   std::cout << " I have "<<pDD->layers().size()      << " layers" << std::endl;
   std::cout << " I have "<<pDD->chambers().size()    << " chambers" << std::endl;

   std::cout << myName() << ": Begin iteration over geometry..." << std::endl;



   int icount = 0;

   std::vector<CSCChamber*> vc = pDD->chambers();
   std::cout << "No. of chambers stored = " << vc.size() << std::endl;

   std::cout << "\n  #     id(dec)      id(oct)    labels      length       width      thickness   "
     "  g(x=0)   g(y=0)   g(z=0)  g(z=-1)  g(z=+1) "
     "  phi(0)" << std::endl;
   std::cout << dashedLine_ << std::endl;

   for( std::vector<CSCChamber*>::const_iterator it = vc.begin(); it != vc.end(); ++it ){

      const CSCChamber* chamber = *it;

      if( chamber ){
        ++icount;

	// What's its DetId?

        CSCDetId detId = chamber->id();
        int id = detId(); // or detId.rawId()

	//	std::cout << "GeomDetUnit is of type " << detId.det() << " and raw id = " << id << std::endl;

	// There's going to be a lot of messing with field width (and precision) so
	// save input values...
        int iw = std::cout.width(); // save current width
        int ip = std::cout.precision(); // save current precision

	//	std::cout << "\n" << "Parameters of chamber# " << 
	std::cout <<
           std::setw( 4 ) << icount << 
  	   std::setw(12) << id << std::oct << std::setw(12) << id << std::dec << std::setw( iw ) <<
           "   E" << detId.endcap() << 
           " S" << detId.station() << 
           " R" << detId.ring() << 
	   " C" << std::setw( 2 ) << detId.chamber() << std::setw( iw );

	// What's its surface?
	// The surface knows how to transform local <-> global

	const BoundSurface& bSurface = chamber->surface();

	//	std::cout << " length=" << bSurface.bounds().length() << 
	//       	             ", width=" << bSurface.bounds().width() << 
	//                     ", thickness=" << bSurface.bounds().thickness() << std::endl;

	std::cout << std::setw( 12 ) << std::setprecision( 8 ) << bSurface.bounds().length() << 
 	             std::setw( 12 ) << std::setprecision( 8 ) << bSurface.bounds().width() << 
	             std::setw( 12 ) << std::setprecision( 6 ) << bSurface.bounds().thickness() ;

	// Check global coordinates of centre of CSCChamber, and how
	// local z direction relates to global z direction

        LocalPoint  lCentre( 0., 0., 0. );
        GlobalPoint gCentre = bSurface.toGlobal( lCentre );

        LocalPoint  lCentre1( 0., 0., -1.);
        GlobalPoint gCentre1 = bSurface.toGlobal( lCentre1 );

        LocalPoint  lCentre2( 0., 0., 1.);
        GlobalPoint gCentre2 = bSurface.toGlobal( lCentre2 );

	//	std::cout << "local(0,0,-1) = global " << gCentre1 << std::endl;
	//	std::cout << "local(0,0)    = global " << gCentre  << std::endl;
	//	std::cout << "local(0,0,+1) = global " << gCentre2 << std::endl;

        double gx  =  gCentre.x();
        double gy  =  gCentre.y();
        double gz  =  gCentre.z();
        double gz1 =  gCentre1.z();
        double gz2 =  gCentre2.z();
        if ( fabs( gx )  < 1.e-06 ) gx = 0.;
        if ( fabs( gy )  < 1.e-06 ) gy = 0.;
        if ( fabs( gz )  < 1.e-06 ) gz = 0.;
        if ( fabs( gz1 ) < 1.e-06 ) gz1 = 0.;
        if ( fabs( gz2 ) < 1.e-06 ) gz2 = 0.;

	int now = 9;
	int nop = 5;
	std::cout << 
	    std::setw( now ) << std::setprecision( nop ) << gx <<  
            std::setw( now ) << std::setprecision( nop ) << gy << 
            std::setw( now ) << std::setprecision( nop ) << gz << 
	    std::setw( now ) << std::setprecision( nop ) << gz1 << 
            std::setw( now ) << std::setprecision( nop ) << gz2;

	// Global Phi of centre of CSCChamber

	//@@ CARE The following attempted conversion to degrees can be easily
	// subverted by GeometryVector/Phi.h enforcing its range convention!
	// Either a) use a separate local double before scaling...
	//        double cphi = gCentre.phi();
	//        double cphiDeg = cphi * radToDeg;
	// or b) use Phi's degree conversion...
        double cphiDeg = gCentre.phi().degrees();

	// I want to display in range 0 to 360

        // Handle some occasional ugly precision problems around zero
        if ( fabs(cphiDeg) < 1.e-06 ) {
          cphiDeg = 0.;
	}
        else if ( cphiDeg < 0. ) {
          cphiDeg += 360.;
	}
        else if ( cphiDeg >= 360. ) {
	  std::cout << "WARNING: resetting phi= " << cphiDeg << " to zero." << std::endl;
          cphiDeg = 0.;
	}


	//        int iphiDeg = static_cast<int>( cphiDeg );
	//	std::cout << "phi(0,0,0) = " << iphiDeg << " degrees" << std::endl;

	now = 9;
	nop = 4;
        std::cout 
          << std::setw( now ) << std::setprecision( nop ) << cphiDeg << std::endl;

	// Reset the values we changed
	std::cout << std::setprecision( ip ) << std::setw( iw );
  
    }
    else {
      std::cout << "*** DANGER WILL ROBINSON! *** Stored a null CSCChamber*?  " << std::endl;
    }
  }	

   std::cout << dashedLine_ << " end" << std::endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(CSCGeometryAsChambers);
