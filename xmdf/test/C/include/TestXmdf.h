#ifndef TESTXMDFdotH
#define TESTXMDFdotH

#include <Math.h>

/* The following 3 macros ending with EPS should have an epsilon
   value passed to them.  This should be something like FLT_EPS or
   DBL_EPS or 1e-6 etc.  The epsilon value is multiplied by the
   sum of the two floats to compute a tolerance */
#define DBL_EPS    2.2204460492503131E-16

#define EQ_EPS(A, B, epsilon) (fabs((A) - (B)) <= fabs(((A) + (B)) * (epsilon)))
#define LT_EPS(A, B, epsilon) (((B) - (A)) > fabs(((A) + (B)) * (epsilon)))
#define GT_EPS(A, B, epsilon) (((A) - (B)) > fabs(((A) + (B)) * (epsilon)))
#define LTEQ_EPS(A, B, epsilon) (LT_EPS((A), (B), (epsilon)) || EQ_EPS((A), (B), (epsilon)))
#define GTEQ_EPS(A, B, epsilon) (GT_EPS((A), (B), (epsilon)) || EQ_EPS((A), (B), (epsilon)))

/* The following 3 macros ending with TOL should have a tolerance
   passed to them.  This tolerance should be something like
   g_triangletolerancexy, or a tolerance that has been computed
   from the range of the numbers involved.  The numbers are compared
   against the tolerance */

#define EQ_TOL(A, B, tolerance) (fabs((A) - (B)) <= (tolerance))
#define LT_TOL(A, B, tolerance) (((B) - (A)) > (tolerance))
#define GT_TOL(A, B, tolerance) (((A) - (B)) > (tolerance))
#define LTEQ_TOL(A, B, tol) (LT_TOL((A), (B), (tol)) || EQ_TOL((A), (B), (tol)))
#define GTEQ_TOL(A, B, tol) (GT_TOL((A), (B), (tol)) || EQ_TOL((A), (B), (tol)))

#endif