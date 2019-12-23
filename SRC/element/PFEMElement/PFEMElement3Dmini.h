/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
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
** ****************************************************************** */
                                                                        
// $Revision: 1.0 $
// $Date: 2015/3/10 10:23:06 $
                                                                        
// Written: Minjie Zhu
//
// Description: This file contains the class definition for PFEMElement3Dmini.

#ifndef PFEMElement3Dmini_h
#define PFEMElement3Dmini_h

#include <Matrix.h>
#include <Vector.h>
#include <Element.h>
#include <vector>

class Pressure_Constraint;

class PFEMElement3Dmini : public Element
{
public:
    PFEMElement3Dmini();
    PFEMElement3Dmini(int tag, int nd1, int nd2, int nd3, int nd4,
                      double r, double m, double b1, double b2, double b3,
                      double ka, bool lmpd=false, bool chk=false);
    
    ~PFEMElement3Dmini();

    // methods dealing with nodes and number of external dof
    int getNumExternalNodes(void) const;
    const ID &getExternalNodes(void);
    Node **getNodePtrs(void);
    int getNumDOF(void);

    // public methods to set the state of the element    
    int revertToLastCommit(void);
    //int revertToStart(void);   
    int update(void);
    int commitState(void);    

    // public methods to obtain stiffness, mass, damping and residual information    
    const Matrix &getTangentStiff(void);
    const Matrix &getInitialStiff(void);    
    const Matrix &getDamp();
    const Matrix &getMass(void);    

    // methods for applying loads
    int addInertiaLoadToUnbalance(const Vector &accel);

    // methods for obtaining resisting force (force includes elemental loads)
    const Vector &getResistingForce(void);
    const Vector &getResistingForceIncInertia(void);   

    // MovableObject
    const char *getClassType(void) const;
    int sendSelf(int commitTag, Channel &theChannel);
    int recvSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

    // DomainComponent
    void setDomain(Domain *theDomain); 

    // TaggedObject
    void Print(OPS_Stream &s, int flag =0);
    int displaySelf(Renderer &theViewer, int displayMode, float fact);

    // sensitivity
    // int setParameter(const char **argv, int argc, Parameter &param);
    // int updateParameter (int parameterID, Information &info);
    // int activateParameter(int passedParameterID);
    // const Matrix& getDampSensitivity(int gradNumber);
    // const Matrix& getMassSensitivity(int gradNumber);
    // const Vector& getResistingForceSensitivity(int gradNumber);
    // int commitSensitivity(int gradNumber, int numGrads);

protected:

private:

    ID ntags; // Tags of nodes
    std::vector<Node*> nodes; // pointers of nodes
    std::vector<Pressure_Constraint*> thePCs;
    double rho;  // density
    double mu;   // viscocity
    Vector body; // body force
    double J, Jn;
    ID vdof, pdof;
    double kappa;
    int ndf;
    bool lumped, checkJ;
    Matrix dNdx;

    static Matrix K;
    static Vector P;

    double det(const double& a, const double& b, const double& c,
	       const double& d, const double& e, const double& f,
	       const double& g, const double& h, const double& i) {
	return a*e*i+b*f*g+c*d*h-c*e*g-b*d*i-a*f*h;
    }
    double kbb(int i, int j) {
	double res = 0;
	for(int a=0; a<4; a++) {
	    res += dNdx(i,a)*dNdx(j,a);
	}
	return res;
    }

};

#endif


