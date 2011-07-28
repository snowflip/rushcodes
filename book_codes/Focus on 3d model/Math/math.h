/*************************************************************/
/*                           MATH.H                          */
/*                                                           */
/* Purpose: Header file to include in order to get full math */
/*          support for 2 and 3 element vectors, 3x3 and 4x4 */
/*          matrices and quaternions                         */
/*      Evan Pipho (May 27, 2002)                            */
/*                                                           */
/*************************************************************/
#ifndef MATH_H
#define MATH_H

#pragma warning(push)
#pragma warning(disable : 4244)

#define Pi 3.141592f

#include "matrix.h"
#include "vector.h"
#include "quaternion.h"

#pragma warning(pop)

#endif //MATH_H