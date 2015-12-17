// Author:  John Cairns <john@2ad.com>
// Date:    June 21, 2006


//     GZKFast - An environment for astrophysical simulation.
//     Copyright (C) 2006 John A Cairns <john@2ad.com>

//     This program is free software; you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation; either version 2 of the License, or
//     (at your option) any later version.

//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.

//     You should have received a copy of the GNU General Public License
//     along with this program; if not, write to the Free Software
//     Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//     02110-1301  USA

//     John Cairns
//     2271 Coventry Rd    
//     Upper Arlington, OH, 43221


#include <stdlib.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_plain.h>
#include <gsl/gsl_monte_miser.h>
#include <gsl/gsl_monte_vegas.h>

#include "gintegrate.h"
#include "gmath.h"

#include "gguard.h"

using namespace GZKFast;

#ifdef GINTEGRATE2D
class GFunc1d : public GFunction {
    GFunction *func;
    GFloat     y2;
  public:
    GFunc1d(GFunction *f, GFloat val) {
        func = f;
        y2=val;
    }
    
    GFloat operator()(GFloat y1) {
        return (*func)(y1, y2);
    }

    GFloat operator()(GFloat y1, GFloat y2) {
        return (*func)(y1, y2);
    }
};

class GFunctionAdapter : public GFunction {
    GFunction *f;
    GFloat a2;
    GFloat b2;
  public:
    GFunctionAdapter(GFunction *func, GFloat a, GFloat b) {
        f = func;
        a2=a;
        b2=b;
    }
    
    GFloat operator()(GFloat val) {
        GFunc1d func1d(f, val);
        return GIntegrate::integrate(*f, a2, b2);
    }

    GFloat operator()(GFloat x1, GFloat x2) {
        return (*this)(x1);
    }
};

#endif


GFloat GZKFast::GIntegrate::integrate(GFunction &func,
                                      GFloat a, 
                                      GFloat b)
    throw( GError )
/* Returns the integral of the function func from a to b. 
*/
{

    static GMutex fxnMutex;
    GGuard fxnGuard(fxnMutex);

    static GFunction *f = &func;
    struct AdaptFunc {
        static GFloat fxn(GFloat val, void*) {
            return (*f)(val);
        }
    };

    const int GSL_INTEGRATE_NSTEP = 1000;

    gsl_integration_workspace * w 
        = gsl_integration_workspace_alloc (GSL_INTEGRATE_NSTEP);

    
    GFloat sum=0.0;
    GFloat error;
    const GFloat DELTA = PrecisionGoal;
    gsl_function F;

    F.function = AdaptFunc::fxn;
    F.params = NULL;
        
    if(gsl_integration_qags(&F, a, b, 0.0, DELTA, GSL_INTEGRATE_NSTEP, w, &sum, &error) == GSL_SUCCESS) {
        return sum;
    }
    throw GError("GIntegrate::integrate not sucessful.");
    return 0.0; // Never get here but MSVC can't tell
}

#ifdef GINTEGRATE2D

// integrate a 2d function using simpsons rule
GFloat GIntegrate::integrate2d(GFunction &func,
                               GFloat a1, GFloat b1,
                               GFloat a2, GFloat b2)
    throw( GError ) {

    // adapt a 2d function 2 1d and then integrate

    // make a 2d function 1d
    GFunctionAdapter g(&func, a2, b2);

    return integrate(g, a1, b1);
}

#endif

GFloat GZKFast::GIntegrate::vegasIntegrate(GFunction &func,
                                           GFloat a, 
                                           GFloat b,
                                           int maxIter)
    throw( GError ) {
    
    static GMutex fxnMutex;
    GGuard fxnGuard(fxnMutex);
    
    static GFunction *f = &func;
    struct AdaptFunc {
        static GFloat fxn(GFloat *vals, size_t dim, void *params) {
            return (*f)(vals[0]);
        }
    };

    GFloat res, err;

    const gsl_rng_type *T;
    gsl_rng *r;
     
    gsl_monte_function G = { &AdaptFunc::fxn, 1, 0 };
     
    gsl_rng_env_setup ();
     
    T = gsl_rng_default;
    r = gsl_rng_alloc (T);
    
    GFloat xl[1] = {a};
    GFloat xu[1] = {b};

    gsl_monte_vegas_state *s = gsl_monte_vegas_alloc (1);
     
    gsl_monte_vegas_integrate (&G, xl, xu, 1, maxIter, r, s,
                               &res, &err);
    do
    {
        gsl_monte_vegas_integrate (&G, xl, xu, 1, maxIter>>2, r, s,
                                   &res, &err);
#ifdef GZKFAST_DEBUG
        fprintf(stderr, "result = % .6f sigma = % .6f "
                "chisq/dof = %.1f\n", res, err, s->chisq);
#endif
    }
    while ((fabs (s->chisq - 1.0) > 0.5)
           && (!GMath::isApproxEqual(err, 0.0, 1e-4)));
     
    gsl_monte_vegas_free (s);

    return res;
}
