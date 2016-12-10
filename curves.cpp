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
    // find sum of (control points * blending function), to calculate point on the bezier according to the value t and all the control points

    float   x = 0,
            y = 0;
    int numCtrlPoints = (int)controlPoints.size();
    
    for(int i=0; i<numCtrlPoints ; i++)
    {
        unsigned long binomCoeff = binomialCoefficient((float)numCtrlPoints-1, (float)i);
        float  t_tothe_i = pow(t, (float)i),
                oneminust_tothe_nminusi = pow((1 - t), (numCtrlPoints - 1 - i));
        float blendingFunction = binomCoeff * t_tothe_i * oneminust_tothe_nminusi;
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
    
    // call evaluateBezier(t) to evaluate point for number of t value
    // draw the point for each 't' value
    Vector2D point1 = controlPoints[0];
    for(float t=0.0 ; t<=1.0 ; t+=0.01)
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


//returns the 4th order 3rd degree cubic B-Spline blendingfunction for the given tau value
double curveOperations::getBlendingFunction(float tau)
{
    double  blendingFunction = 0.0f;
    if ( tau >= -2.0f && tau <= -1.0f ){
        blendingFunction = pow((2.0+tau), 3.0f)/6.0f;
    }
    else if ( tau >= -1.0f && tau <= 0.0f ){
        blendingFunction =  (4.0f - (6.0f * pow(tau, 2)) - (3.0f * pow(tau,3)))/6.0f;
    }
    else if ( tau >= 0.0f && tau <= 1.0f ){
        blendingFunction = (4.0f - (6.0f * pow(tau, 2)) + (3.0f * pow(tau, 3)))/6.0f;
    }
    else if ( tau >= 1.0f && tau <= 2.0f ){
        blendingFunction = pow((2.0f - tau), 3)/6.0f;
    }
    else{
        blendingFunction = 0;
    }
    return blendingFunction;
}

//returns the 4th order 3rd degree beta-Spline blendingfunction for the given skew, tension, and tau values
double curveOperations::getBetaBlendingFunction(float skew, float tension, float tau)
{
    float   blendingFunction = 0.0f;
    float   tau_squared = pow(tau,2),
            tau_cubed = pow(tau,3),
            skew_squared = pow(skew, 2),
            skew_cubed = pow(skew, 3),
            delta = tension + (2.0f * skew_cubed) + (4.0f * skew_squared) + (4.0f * skew) + 2.0f,
            quadratic_ts = tension + (4.0f * skew) + (4.0f * skew_squared);
    
    if ( tau >= -2.0f && tau <= -1.0f ){
        blendingFunction = (pow((2.0f + tau), 3)) * 2.0f / delta;
    }
    else if ( tau >= -1.0f && tau <= 0.0f ){
        blendingFunction =  (quadratic_ts - (6.0f * tau * (1 - skew_squared)) - (3 * tau_squared * (2.0f + tension + (2.0f * skew))) - (2.0f * tau_cubed * (1.0f + tension + skew + skew_squared)) ) / delta;
    }
    else if ( tau >= 0.0f && tau <= 1.0f ){
        blendingFunction = (quadratic_ts - (6.0f * tau * (skew - skew_cubed)) - (3.0f * tau_squared * (tension + (2.0f*skew_squared) + (2.0f * skew_cubed)) )  + (2.0f * tau_cubed * ( tension + skew + skew_squared + skew_cubed)) ) / delta;
    }
    else if ( tau >= 1.0f && tau <= 2.0f ){
        blendingFunction = pow((2.0f - tau), 3) * skew_cubed * 2.0f / delta;
    }
    else{
        blendingFunction = 0;
    }
    return blendingFunction;
}



void curveOperations::drawLine(const Vector2D& p1, const Vector2D& p2)
{
    glBegin(GL_LINES);
    glVertex2f(p1[0], p1[1]);
    glVertex2f(p2[0], p2[1]);
    glEnd();
}



// draws cubic b-spline by evaluating the curve at some finite number of u-values which are an element int he range of [0,1],
// and drawing line segments between those points.
// calls the getBlendingFunction() method to calculare the blending function to draw the curve
// 'nu' is the number of knots(control points minus one) multiplied by the current u-value
// 'tau' is used to retrieve the blending function. 'tau' is equal to nu minus the current index of the point affected by the cubic spline (since it is of order 4, only 4 points are affected)
void curveOperations::drawCubicBspline() const
{
	//@@@@@@@@@@
    
    Vector2D point1 = controlPoints[0];
    for(float u = 0.0 ; u <= 1.0 ; u += 0.01)
    {
        int numCtrlPoints = (int)controlPoints.size();
        float   x = 0,
                y = 0,
                nu = (numCtrlPoints-1) * u;
        
        for(int i = floor(nu-2) ; i < ceil(nu+2) ; i++)
        {
            float tau = nu-i;
            float blendingFunction = getBlendingFunction(tau);
            
            x += blendingFunction * controlPoints[i][0];
            y += blendingFunction * controlPoints[i][1];
        }
        if (u == 0.0)
        {
            point1 = Vector2D(x,y);
        }
        else
        {
            Vector2D point2 = Vector2D(x, y);
            drawLine(point1, point2);
            point1 = point2;
        }
    }

	//@@@@@@@@@@
}


// draw betaspline
void curveOperations::drawBetaspline() const
{
	//@@@@@@@@@@
    
    double skew = 1.0, tension = 10.0;
    Vector2D point1 = controlPoints[0];
    
    for(float u = 0.0 ; u <= 1.0 ; u += 0.01)
    {
        int numCtrlPoints = (int)controlPoints.size();
        float   x = 0,
        y = 0,
        nu = (numCtrlPoints-1) * u;
        
        for(int i = floor(nu-2) ; i < ceil(nu+2) ; i++)
        {
            float tau = nu-i;
            float blendingFunction = getBetaBlendingFunction(skew, tension, tau);
            
            x += blendingFunction * controlPoints[i][0];
            y += blendingFunction * controlPoints[i][1];
        }
        if (u == 0.0)
        {
            point1 = Vector2D(x,y);
        }
        else
        {
            Vector2D point2 = Vector2D(x, y);
            drawLine(point1, point2);
            point1 = point2;
        }
    }

	//@@@@@@@@@@
}


// draw betaspline for closed curve
void curveOperations::drawBetasplineClosedCurve() const
{
	//@@@@@@@@@@

    float skew = 1.0f, tension = 10.0f;
    Vector2D point1 = controlPoints[0];
    
    for(float u = 0.0f ; u <= 1.0f ; u += 0.01f)
    {
        int numCtrlPoints = (int)controlPoints.size();
        float   x = 0,
                y = 0,
                nu = (numCtrlPoints-1) * u;
        
        for(int i = floor(nu-2) ; i < ceil(nu+2) ; i++)
        {
            float tau = nu-i;
            float blendingFunction = getBetaBlendingFunction(skew, tension, tau);
            
            x += blendingFunction * controlPoints[i][0];
            y += blendingFunction * controlPoints[i][1];
        }
        if (u == 0.0)
        {
            point1 = Vector2D(x,y);
        }
        else
        {
            Vector2D point2 = Vector2D(x, y);
            drawLine(point1, point2);
            point1 = point2;
        }
    }

	//@@@@@@@@@@
}

// draw cubic bspline with phantom endpoints, which FORCES the spline to go through the end points
void curveOperations::drawCubicBsplinePhantom() const
{
	//@@@@@@@@@@

    Vector2D point1 = controlPoints[0];
    
    for(float u = 0.0 ; u <= 1.0 ; u += 0.01)
    {
        int numCtrlPoints = (int)controlPoints.size();
        float   x = 0,
                y = 0,
                nu = (numCtrlPoints-1) * u;
        
        for(int i = 0 ; i < numCtrlPoints+1 ; i++)
        {
            float tau = nu-i;
            float blendingFunction = getBlendingFunction(tau);
            
            x += blendingFunction * controlPoints[i][0];
            y += blendingFunction * controlPoints[i][1];
        }
        
        if (u == 0.0)
        {
            
            Vector2D point2 = Vector2D(x, y);
            float x_phantom0 = 2*point2[0] - controlPoints[1][0] ;
            float y_phantom0 = 2*point2[1] - controlPoints[1][1] ;
            point1 = Vector2D(x_phantom0,y_phantom0);

            drawLine(point1, point2);
            point1 = point2;
        }
        else if (u > 0.9)
        {
            //first you want to get the last point on the curve and draw it with a line coming from the previous point
            Vector2D point2 = Vector2D(x, y);
            drawLine(point1, point2);
            
            //then you want to calculate the phanton points based off of the previous point and the current point
            //
            float x_phantom1 = 2*point2[0] - point1[0];
            float y_phantom1 = 2*point2[1] - point1[1];
            Vector2D phantom_point = Vector2D(x_phantom1,y_phantom1);
            point1 = point2;
            drawLine(point1, phantom_point);
        }
        else
        {
            Vector2D point2 = Vector2D(x, y);
            drawLine(point1, point2);
            point1 = point2;
        }
    }

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


