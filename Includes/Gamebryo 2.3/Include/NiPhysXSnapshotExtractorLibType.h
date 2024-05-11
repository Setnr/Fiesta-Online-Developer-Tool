// EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1996-2007 Emergent Game Technologies.
//      All Rights Reserved.
//
// Emergent Game Technologies, Chapel Hill, North Carolina 27517
// http://www.emergent.net

#ifndef NIPHYSXSNAPSHOTEXTRACTORLIBTYPE_H
#define NIPHYSXSNAPSHOTEXTRACTORLIBTYPE_H

#ifdef NIPHYSXSNAPSHOTEXTRACTOR_EXPORT
    // DLL library project uses this
    #define NIPHYSXSNAPSHOTEXTRACTOR_ENTRY __declspec(dllexport)
#else
#ifdef NIPHYSXSNAPSHOTEXTRACTOR_IMPORT
    // client of DLL uses this
    #define NIPHYSXSNAPSHOTEXTRACTOR_ENTRY __declspec(dllimport)
#else
    // static library project uses this
    #define NIPHYSXSNAPSHOTEXTRACTOR_ENTRY
#endif
#endif

#endif  // #ifndef NIPHYSXSNAPSHOTEXTRACTORLIBTYPE_H
