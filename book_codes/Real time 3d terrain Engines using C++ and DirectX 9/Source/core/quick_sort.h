#ifndef CQUICKSORT_H
#define CQUICKSORT_H
#pragma once
/***************************************************************\

    ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^^^^^
    ^^    ^^  ^^    ^^     ^^     ^^    ^^
    ^^        ^^^^^^^^     ^^     ^^^^^^^^
    ^^   ^^^  ^^    ^^     ^^     ^^    ^^
    ^^    ^^  ^^    ^^     ^^     ^^    ^^
    ^^^^^^^^  ^^    ^^  ^^^^^^^^  ^^    ^^

    sample code from the book...
    Real Time 3D Terrain Engines Using C++ and DirectX

    by Greg Snook
    greg@mightystudios.com

\***************************************************************/


//	Name Space declaration
namespace gaia
{


/*	QuickSort
-----------------------------------------------------------------
    
    A template function to perform quick sorting of a value list.
	Objects in the list must have a sort_predicate defined for them
    
-----------------------------------------------------------------
*/
template <class T>
struct sort_func
{
	bool operator()(const T& a, const T& b)const
	{
		debug_assert(0, "a custom sort predicate must be provided");
		return false;
	};
};

template <class T, class FUNC>
inline void QuickSort(T* array, uint32 members, const FUNC& pred)
{
	uint32 h = 1;

    //find the largest h value possible 
    while ((h * 3 + 1) < members) 
	{
		h = 3 * h + 1;
	}

    //while h remains larger than 0 
    while( h > 0 ) 
	{
		// for each set of elements (there are h sets)
        for (uint32 i = h - 1; i < members; i++) 
		{
            //pick the last element in the set
            T B = array[i];
            uint32 j = i;

			 //compare the element at B to the one before it in the set
             //if they are out of order continue this loop, moving
             //elements "back" to make room for B to be inserted.
            for( j = i; (j >= h) && pred(B,array[j-h]); j -= h)		
			{
                array[j] = array[j-h];
            }
            
             //  insert B into the correct place
            array[j] = B;
        }

        //all sets h-sorted, now decrease set size
        h = h / 3;
    }
}

//- End of cQuickSort -------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cQuickSort.h )

//----------------------------------------------------------
//$Log: $