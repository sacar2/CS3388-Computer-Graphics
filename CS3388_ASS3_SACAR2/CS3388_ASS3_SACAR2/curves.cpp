#include "curves.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
// #include <GL/glut.h>

#include <cmath>
#include <cassert>
#include <iostream>

Vector2D curveOperations::evaluateBezier(float t) const
{
    assert(t >= 0.0f && t <= 1.0f);
    assert(controlPoints.size() > 1);

    // Evaluate the Bezier curve at the given t parameter.
    // You may find the following functions useful:
    //  - curveOperations::binomialCoefficient(m,i) computes "m choose i", 
    //      aka: (m over i)
    //  - std::pow(t,i) computes t raised to the power i

    //@@@@@@@@@@
    
    float x = 0, y = 0;
    int numCtrlPoints = (int)controlPoints.size();
    // find sum of (control points * blending function), to calculate point on the bezier according to the value t and all the control points
    for(int i=0; i<numCtrlPoints ; i++)
    {
        unsigned long binomCoeff = binomialCoefficient((float)numCtrlPoints-1, (float)i);
        double  t_tothe_i = pow(t, (double)i),
                oneminutst_tothe_kminusi = pow((1 - t), (numCtrlPoints - 1 - i));
        double blendingFunction = binomCoeff * t_tothe_i * oneminutst_tothe_kminusi;
        x += blendingFunction * controlPoints[i][0];
        y += blendingFunction * controlPoints[i][1];
    }
	return Vector2D(x, y);

    //@@@@@@@@@@
}

void curveOperations::drawBezier() const
{
    // Draw this Bezier curve.
    // Do this by evaluating the curve at some finite number of t-values,
    // and drawing line segments between those points.
    // You may use the curveOperations::drawLine() function to do the actual
    // drawing of line segments.

    //@@@@@@@@@@
    
    // CALL evaluateBezier(float t) TO EVALUATE POINT FOR NUMBER OF T VALUES
    // DRAW THE point FOR EACH t VALUE
    Vector2D point1 = controlPoints[0];
    for(double t=0.0 ; t<=1.0 ; t+=0.01)
    {
        Vector2D point2 = evaluateBezier(t);
        drawLine(point1, point2);
        point1 = point2;
   }

    //@@@@@@@@@@
 
}


void curveOperations::drawControlPolygon() const
{
    for (size_t i = 1; i < controlPoints.size(); ++i)
    {
        drawLine(controlPoints[i-1], controlPoints[i]);
    }
}


unsigned long curveOperations::binomialCoefficient(int n, int k)
{
    // Compute nCk ("n choose k")
    // WARNING: Vulnerable to overflow when n is very large!

    assert(k >= 0);
    assert(n >= k);

    unsigned long result = 1;
    for (int i = 1; i <= k; ++i)
    {
        result *= n-(k-i);
        result /= i;
    }
    return result;
}


void curveOperations::drawLine(const Vector2D& p1, const Vector2D& p2)
{
    glBegin(GL_LINES);
    glVertex2f(p1[0], p1[1]);
    glVertex2f(p2[0], p2[1]);
    glEnd();
}



// draw cubic bspline
void curveOperations::drawCubicBspline() const
{
	//@@@@@@@@@@
    int numCtrlPoints = (int)controlPoints.size();

//    //First lets draw out the control lines so we know what it Should kind of follow
//    for(int i=0 ; i<numCtrlPoints-1 ; i++)
//    {
//        drawLine(controlPoints[i], controlPoints[i+1]);
//    }
//
//    Vector2D point1 = controlPoints[0];
//
//    // find sum of (control points * blending function), to calculate points on the B Spline
//    for(double t=0.0 ; t<=1.0 ; t+=0.01)
//    {
//        double  x = 0,
//                y = 0,
//                u = pow((1-t), 3)/6,
//                nu = numCtrlPoints * u;
//        
//            for(double i=floor(nu-2); i<=ceil(nu+2) ; i++)
//            {
//                //CALCULATE BLENDING FUNCTIONS
//                // B SPLINE IS LIKE BEZIER, EXCEPT THERE ARE 5 BLENDING FUNCTIONS
//                double tau = nu-i;
//                double blendingFunction = getBlendingFunction(tau);
//                x += blendingFunction * controlPoints[i][0];
//                y += blendingFunction * controlPoints[i][1];
//            }
////            std::cout<<count<<std::endl;
//
//            Vector2D point2 = Vector2D(x, y);
//            drawLine(point1, point2);
//            point1 = point2;
//    }

    
    
    // CALL evaluateBezier(float t) TO EVALUATE POINT FOR NUMBER OF T VALUES
    // DRAW THE point FOR EACH t VALUE
    Vector2D point1 = controlPoints[0];
    for(double t=0.0 ; t<=1.0 ; t+=0.01)
    {
        
        float x = 0, y = 0;
        int numCtrlPoints = (int)controlPoints.size();
        
        double  u = pow((1-t), 3)/6,
                nu = numCtrlPoints * u;
        
        for(int i=0; i<numCtrlPoints ; i++)
        {
            double tau = nu-i;
            double blendingFunction = getBlendingFunction(tau);
            
            x += blendingFunction * controlPoints[i][0];
            y += blendingFunction * controlPoints[i][1];
        }
        Vector2D point2 = Vector2D(x, y);
        
        drawLine(point1, point2);
        point1 = point2;
    }

    
    
    
    //@@@@@@@@@@
}

//returns the 4th order 3rd degree cubic B-Spline blendingfunction for the given tau value
double curveOperations::getBlendingFunction(double tau) const
{
    double  blendingFunction = 0.0;
    if ( tau >= -2.0 && tau <= -1.0 ){
        blendingFunction = pow((2.0+tau), 3)/6.0f;
    }
    else if ( tau >= -1.0 && tau <= 0 ){
        blendingFunction =  (4.0 - (6.0 * pow(tau, 2)) - (3.0 * pow(tau,3)))/6.0f;
    }
    else if ( tau >= 0 && tau <= 1 ){
        blendingFunction = (4.0 - (6.0 * pow(tau, 2)) + (3.0 * pow(tau, 3)))/6.0f;
    }
    else if ( tau >= 1.0 && tau <= 2.0 ){
        blendingFunction = pow((2.0 - tau), 3)/6.0f;
    }
    else{
        blendingFunction = 0;
    }
    return blendingFunction;
}


double curveOperations::getBetaBlendingFunction(double skew, double tension, double tau) const
{
    double  blendingFunction = 0.0;
    double  quadratic_ts = tension + (4.0 * skew) + (4.0*pow(skew, 2)),
            tau_squared = pow(tau,2),
            tau_cubed = pow(tau,3),
            skew_squared = pow(skew, 2),
            skew_cubed = pow(skew, 3),
            delta = tension + (2 * skew_cubed) + (4*skew_squared) + (4 * skew) + 2;
    
    if ( tau >= -2 && tau <= -1 ){
        blendingFunction = pow((2 + tau), 3)*2/delta;
    }
    else if ( tau >= -1 && tau <= 0 ){
        blendingFunction =  (quadratic_ts - (6 * tau * (1 - skew_squared)) - (3 * tau_squared * (2 + tension + (2 * skew))) - (2 * tau_cubed * (1 + tension + skew + skew_squared)) ) / delta;
    }
    else if ( tau >= 0 && tau <= 1 ){
        blendingFunction = (quadratic_ts - (6 * tau * (skew - skew_cubed)) - (3 * tau_squared * (tension + (2*(skew_squared + skew_cubed)) ) ) + (2 * tau_cubed * ( tension + skew + skew_squared + skew_cubed)) ) / delta;
    }
    else if ( tau >= 1 && tau <= 2 ){
        blendingFunction = pow((2 - tau), 3) * skew_cubed * 2 / delta;
    }
    else{
        blendingFunction = 0;
    }
    return blendingFunction;
}


// draw betaspline
void curveOperations::drawBetaspline() const
{
	//@@@@@@@@@@
    // YOUR CODE HERE
	

    double skew = 1.0, tension = 10.0;
    
    
    Vector2D point1 = controlPoints[0];
    for(double t=0.0 ; t<=1.0 ; t+=0.01)
    {
        
        
        float x = 0, y = 0;
        int numCtrlPoints = (int)controlPoints.size();
        
        double  u = pow((1-t), 3)/6,
        nu = numCtrlPoints * u;
        
        for(int i=0; i<numCtrlPoints ; i++)
        {
            double tau = nu-i;
            double blendingFunction = getBetaBlendingFunction(skew, tension, tau);
            
            x += blendingFunction * controlPoints[i][0];
            y += blendingFunction * controlPoints[i][1];
        }
        Vector2D point2 = Vector2D(x, y);
        
        
        drawLine(point1, point2);
        point1 = point2;
    }
    
    
    
    
	//@@@@@@@@@@
}


// draw betaspline for closed curve
void curveOperations::drawBetasplineClosedCurve() const
{
	//@@@@@@@@@@
    // YOUR CODE HERE
	


	//@@@@@@@@@@
}

// draw cubic bspline with phantom endpoints
void curveOperations::drawCubicBsplinePhantom() const
{
	//@@@@@@@@@@
    // YOUR CODE HERE
	


	//@@@@@@@@@@
}



/*************************** You don't need to modify this block ***************************/
void CurveManager::drawCurves() const
{
    if (points == NULL || points->size() < 2)
    {
        return;
    }

    if (curveMode == BEZIER_MODE)
    {
        // Basic Mode (default)
        // Create a Bezier curve from the entire set of points,
        // and then simply draw it to the screen.
        
        curveOperations curve(*points);
        curve.drawBezier();

    }
	else
		if (curveMode == CUBIC_BSPLINE_MODE)
		{
			// mode to draw a cubic b-spline
			curveOperations curve(*points);
			curve.drawCubicBspline();
		}
		else
			if (curveMode == BETA_SPLINE_MODE)
			{
				// mode to draw a beta-spline
				curveOperations curve(*points);
				curve.drawBetaspline();
			}
			else
				if (curveMode == BETA_SPLINE_CLOSEDCURVE_MODE)
				{
					// mode to draw a beta-spline for closed curve
					curveOperations curve(*points);
					curve.drawBetasplineClosedCurve();
				} 
				else
					if (curveMode == CUBIC_BSPLINE_PHANTOM_MODE)
					{
						// mode to draw a cubic b-spline with phantom endpoints
						curveOperations curve(*points);
						curve.drawCubicBsplinePhantom();
					}
}
/*************************** You don't need to modify this block ***************************/


