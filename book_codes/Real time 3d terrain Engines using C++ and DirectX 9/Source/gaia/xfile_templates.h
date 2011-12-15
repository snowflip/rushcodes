#ifndef XFILETEMPLATES_H
#define XFILETEMPLATES_H
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


#define EXTENDED_TEXTURE_COUNT 16

//
// Extended Material Templates
//
// {79E82357-9BEC-48a3-AA5A-6A90AEF6ECC0}
DEFINE_GUID(DXFILEOBJ_ExtendedMaterial, 
0x79e82357, 0x9bec, 0x48a3, 0xaa, 0x5a, 0x6a, 0x90, 0xae, 0xf6, 0xec, 0xc0);


#define XGAIA_TEMPLATES \
        "xof 0303txt 0032\
        template XGaiaExtendedMaterial \
        { \
            <79E82357-9BEC-48a3-AA5A-6A90AEF6ECC0> \
			STRING materialName;\
            DWORD nMaterialIndex; \
        } "



#endif  // end of file      ( XfileTemplates.h )

//----------------------------------------------------------
//$Log: $