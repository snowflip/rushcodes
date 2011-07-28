/*************************************************************/
/*                           MODEL.H                         */
/*                                                           */
/* Purpose: Pure virtual class used for loading and rendering*/
/*          3d models.                                       */
/*      Evan Pipho (evan@codershq.com)                       */
/*                                                           */
/*************************************************************/
#ifndef MODEL_H
#define MODEL_H

#include "../math/math.h"

//-------------------------------------------------------------
//                        CMODEL                              -
// author: Evan Pipho (evan@codershq.com)                     -
// date  : Jul 09, 2002                                       -
//-------------------------------------------------------------
class CModel
{
public:
	
	//Load the model from file
	virtual bool Load(const char * szFile) = 0;
	//Render the model, generally at its initial position
	virtual void Render() = 0;

};

//-------------------------------------------------------------
//- SwapEndian
//- reverses the order of bytes in a variable
//-------------------------------------------------------------
inline void SwapEndian(unsigned int& i)
{
	unsigned int j  = (( i << 24 ) & 0xFF000000) + ((i << 8) & 0x00FF0000) + ((i >> 8) & 0x0000FF00) + ((i >> 24) & 0x000000FF);
	i = j;
}

inline void SwapEndian(unsigned short& i)
{
	unsigned short j  = (( i << 8 ) & 0xFF00) + ((i >> 8) & 0x00FF);
	i = j;
}

inline CVector3 CalcFaceNormal(CVector3& p0, CVector3& p1, CVector3& p2)
{
	CVector3 v0 = p1 - p0;
	CVector3 v1 = p2 - p0;
	CVector3 vF = CrossProduct(v0, v1);
	vF.Normalize();
	return vF;
}

#endif //MODEL_H