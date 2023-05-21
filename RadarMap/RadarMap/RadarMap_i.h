

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __RadarMap_i_h__
#define __RadarMap_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IRadarMap_FWD_DEFINED__
#define __IRadarMap_FWD_DEFINED__
typedef interface IRadarMap IRadarMap;
#endif 	/* __IRadarMap_FWD_DEFINED__ */


#ifndef __IRT_FWD_DEFINED__
#define __IRT_FWD_DEFINED__
typedef interface IRT IRT;
#endif 	/* __IRT_FWD_DEFINED__ */


#ifndef __ISTC_FWD_DEFINED__
#define __ISTC_FWD_DEFINED__
typedef interface ISTC ISTC;
#endif 	/* __ISTC_FWD_DEFINED__ */


#ifndef __RadarMapCom_FWD_DEFINED__
#define __RadarMapCom_FWD_DEFINED__

#ifdef __cplusplus
typedef class RadarMapCom RadarMapCom;
#else
typedef struct RadarMapCom RadarMapCom;
#endif /* __cplusplus */

#endif 	/* __RadarMapCom_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IRadarMap_INTERFACE_DEFINED__
#define __IRadarMap_INTERFACE_DEFINED__

/* interface IRadarMap */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IRadarMap;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EDE2CDAD-E62B-4F10-BC9C-3F947B66A1DC")
    IRadarMap : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IRadarMapVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRadarMap * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRadarMap * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRadarMap * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRadarMap * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRadarMap * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRadarMap * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRadarMap * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IRadarMapVtbl;

    interface IRadarMap
    {
        CONST_VTBL struct IRadarMapVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRadarMap_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRadarMap_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRadarMap_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRadarMap_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IRadarMap_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IRadarMap_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IRadarMap_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRadarMap_INTERFACE_DEFINED__ */


#ifndef __IRT_INTERFACE_DEFINED__
#define __IRT_INTERFACE_DEFINED__

/* interface IRT */
/* [unique][dual][uuid][object] */ 


EXTERN_C const IID IID_IRT;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8DABC68E-D8B9-4bb8-B387-1A03BC0594F0")
    IRT : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ unsigned long time) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Run( 
            /* [in] */ unsigned long time) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Terminate( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRTVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRT * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRT * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRT * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRT * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRT * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRT * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRT * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IRT * This,
            /* [in] */ unsigned long time);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Run )( 
            IRT * This,
            /* [in] */ unsigned long time);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Terminate )( 
            IRT * This);
        
        END_INTERFACE
    } IRTVtbl;

    interface IRT
    {
        CONST_VTBL struct IRTVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRT_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRT_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRT_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRT_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IRT_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IRT_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IRT_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IRT_Initialize(This,time)	\
    ( (This)->lpVtbl -> Initialize(This,time) ) 

#define IRT_Run(This,time)	\
    ( (This)->lpVtbl -> Run(This,time) ) 

#define IRT_Terminate(This)	\
    ( (This)->lpVtbl -> Terminate(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRT_INTERFACE_DEFINED__ */


#ifndef __ISTC_INTERFACE_DEFINED__
#define __ISTC_INTERFACE_DEFINED__

/* interface ISTC */
/* [unique][dual][uuid][object] */ 


EXTERN_C const IID IID_ISTC;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E55FBD86-BB76-4332-B1C1-E9C980891215")
    ISTC : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE STConnect( 
            /* [in] */ IDispatch *pDisp,
            /* [in] */ unsigned long ID) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISTCVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISTC * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISTC * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISTC * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISTC * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISTC * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISTC * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISTC * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *STConnect )( 
            ISTC * This,
            /* [in] */ IDispatch *pDisp,
            /* [in] */ unsigned long ID);
        
        END_INTERFACE
    } ISTCVtbl;

    interface ISTC
    {
        CONST_VTBL struct ISTCVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISTC_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISTC_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISTC_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISTC_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISTC_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISTC_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISTC_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISTC_STConnect(This,pDisp,ID)	\
    ( (This)->lpVtbl -> STConnect(This,pDisp,ID) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISTC_INTERFACE_DEFINED__ */



#ifndef __RadarMapLib_LIBRARY_DEFINED__
#define __RadarMapLib_LIBRARY_DEFINED__

/* library RadarMapLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_RadarMapLib;

EXTERN_C const CLSID CLSID_RadarMapCom;

#ifdef __cplusplus

class DECLSPEC_UUID("5B332688-640C-4AF8-9901-AAC6F071616B")
RadarMapCom;
#endif
#endif /* __RadarMapLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


