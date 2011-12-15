#ifndef CORE_H
#define CORE_H
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


//
// includes everything from the core library
// (link with core.lib)
//

#include "data_types.h"
#include "numeric_tools.h"
#include "memory.h"
#include "bit_flags.h"
#include "constants.h"
#include "debug.h"
#include "file.h"
#include "reference_counter.h"
#include "singleton.h"
#include "string.h"
#include "system_info.h"
#include "timer.h"
#include "com_helpers.h"
#include "data_pool.h"
#include "resource_pool_item.h"
#include "resource_pool.h"
#include "resource_pool_manager.h"
#include "application_timer.h"
#include "code_timer.h"
#include "quick_sort.h"
#include "random_numbers.h"

#endif  // end of file      ( core.h )

//----------------------------------------------------------
//$Log: $