/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 2001, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** Fire & Heat Transfer modules developed by:                         **
**   Yaqiang Jiang (y.jiang@ed.ac.uk)                                 **
**   Asif Usmani (asif.usmani@ed.ac.uk)                               **
**                                                                    **
** ****************************************************************** */

//
// Written by Yaqiang Jiang (y.jiang@ed.ac.uk)
//

#ifndef QuadFour_h
#define QuadFour_h

#include <HeatTransferElement.h>
#include <Matrix.h>
#include <Vector.h>
#include <ID.h>

class HeatTransferNode;
class HeatTransferMaterial;

class QuadFour : public HeatTransferElement
{
  public:
    QuadFour(int tag, int nd1, int nd2, int nd3, int nd4,
		     HeatTransferMaterial& m, 
			 bool phaseTransformation = false);
    ~QuadFour();

    const char* getClassType(void) const {return "QuadFour";};

    int getNumExternalNodes(void) const;
    const ID& getExternalNodes(void);
    HeatTransferNode** getNodePtrs(void);
    int getNumDOF(void);
	const ID& getNodesOnFace(int faceTag);
	int getNumNodesperFace(void) {return 2;};

    void setDomain(HeatTransferDomain* theDomain);

    // public methods to set the state of the element    
    int commitState(void);
    int revertToLastCommit(void);
    int revertToStart(void);
    int update(void);

    // public methods to obtain tangent matrices    
	const Matrix& getConductionTangent(void);  // K_k
	const Matrix& getCapacityTangent(void);  // K_c   
	const Matrix& getRadiationTangent(void);  // K_qr
	const Matrix& getConvectionTangent(void);  // K_qc

    void zeroFlux();
    int addPrecribedSurfFlux(PrescribedSurfFlux* theFlux, double factor);

	// public methods for calculating flux vectors
    const Vector& get_Q_Transient();  //computeQcp(void); getTransient 
    const Vector& get_Q_Conduction(); //computeQk(void);  getConductionFlux  
    const Vector& get_Q_Radiation();  //computeQqr(void); getRadiation  
	const Vector& get_Q_Convection();  //computeQqc(void); getConvection  

	//const Vector& getEleResidual(void);      

    Response* setResponse(const char** argv, int argc, OPS_Stream& output);
    int getResponse(int responseID, Information& eleInfo);

  protected:
    
  private:
    // private attributes - a copy for each object of the class
    HeatTransferMaterial** theMaterial;  // pointer to material objects
    
    ID connectedExternalNodes;  // Tags of quad nodes

    HeatTransferNode* theNodes[4];

    static double matrixData[16];  // array data for matrix
    static Matrix K;  // Element stiffness matrix
    static Vector Q;  // Flux vector
	Vector Qp;  // Stores the PrecribedSurfFlux

    static double shp[3][4];  // Stores shape functions and derivatives (overwritten)
    static double pts[4][2];  // Stores quadrature points
    static double wts[4];  // Stores quadrature weights

	static double shp2[2];  // Shape function and derivatives for line quadrature
	static double pts2[2];
	static double wts2[2];
	static int npface[4][2];
	bool phaseTransformation;

    // private member functions - only objects of this class can call these
    double shapeFunction(double xi, double eta);
	double shapeFunction(double xi, int faceTag);
	double getInterpolatedTemp(int tag);  // return interpolated temperature at quadrature point at boundary
};

#endif