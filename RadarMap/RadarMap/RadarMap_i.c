

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Wed Jul 11 14:57:25 2012
 */
/* Compiler settings for RadarMap.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IRadarMap,0xEDE2CDAD,0xE62B,0x4F10,0xBC,0x9C,0x3F,0x94,0x7B,0x66,0xA1,0xDC);


MIDL_DEFINE_GUID(IID, IID_IRT,0x8DABC68E,0xD8B9,0x4bb8,0xB3,0x87,0x1A,0x03,0xBC,0x05,0x94,0xF0);


MIDL_DEFINE_GUID(IID, IID_ISTC,0xE55FBD86,0xBB76,0x4332,0xB1,0xC1,0xE9,0xC9,0x80,0x89,0x12,0x15);


MIDL_DEFINE_GUID(IID, LIBID_RadarMapLib,0xADB4A802,0x23BC,0x44A0,0x8E,0x93,0x19,0x3F,0xEE,0x25,0xF6,0x7D);


MIDL_DEFINE_GUID(CLSID, CLSID_RadarMapCom,0x5B332688,0x640C,0x4AF8,0x99,0x01,0xAA,0xC6,0xF0,0x71,0x61,0x6B);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



