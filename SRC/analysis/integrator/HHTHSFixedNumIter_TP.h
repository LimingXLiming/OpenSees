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

// $Revision$
// $Date$
// $URL$

#ifndef HHTHSFixedNumIter_TP_h
#define HHTHSFixedNumIter_TP_h

// Written: Andreas Schellenberg (andreas.schellenberg@gmail.com)
// Created: 10/05
// Revision: A
//
// Description: This file contains the class definition for HHTHSFixedNumIter_TP.
// HHTHSFixedNumIter_TP is an algorithmic class for performing a transient analysis
// using the HHT integration scheme based on the trapezoidal rule. This is a special
// integrator for hybrid simulation where the number of equilibrium iterations are
// fixed at a constant user-specified value. In addition, only parts of the calculated
// response increments are utilized to update the elements. The reduced command
// displacements are determined by means of Lagrange interpolation using the trial
// displacements of the current iteration step and the last n committed displacements.

#include <TransientIntegrator.h>

class DOF_Group;
class FE_Element;
class Vector;

class HHTHSFixedNumIter_TP : public TransientIntegrator
{
public:
    // constructors
    HHTHSFixedNumIter_TP();
    HHTHSFixedNumIter_TP(double rhoInf, int polyOrder,
        bool updDomFlag = true);
    HHTHSFixedNumIter_TP(double alphaI, double alphaF,
        double beta, double gamma, int polyOrder,
        bool updDomFlag = true);
    
    // destructor
    ~HHTHSFixedNumIter_TP();
    
    // method to set up the system of equations
    int formUnbalance(void);
    
    // methods which define what the FE_Element and DOF_Groups add
    // to the system of equation object.
    int formEleTangent(FE_Element *theEle);
    int formNodTangent(DOF_Group *theDof);
    int formEleResidual(FE_Element *theEle);
    int formNodUnbalance(DOF_Group *theDof);
    
    int domainChanged(void);
    int newStep(double deltaT);
    int revertToLastStep(void);
    int update(const Vector &deltaU);
    int commit(void);
    
    virtual int sendSelf(int commitTag, Channel &theChannel);
    virtual int recvSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);
    
    void Print(OPS_Stream &s, int flag = 0);
    
protected:
    
private:
    double alphaI;
    double alphaF;
    double beta;
    double gamma;
    int polyOrder;    // order of Lagrange interpolation polynomial
    bool updDomFlag;  // a flag indicating if response is updated at commit
    double deltaT;
    
    double c1, c2, c3;                      // some constants we need to keep
    double alphaM, alphaD, alphaR, alphaP;  // weighting factors we need to keep
    double x;                               // interpolation location 0<=x<=1
    Vector *Ut, *Utdot, *Utdotdot;          // response quantities at time t
    Vector *U, *Udot, *Udotdot;             // response quantities at time t + deltaT
    Vector *Utm1, *Utm2;                    // disp at time t-deltaT and t-2*deltaT
    Vector *scaledDeltaU;                   // scaled displacement increment
    Vector *Put;                            // unbalance at time t
};

#endif
