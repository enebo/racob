/*
 * Copyright (c) 1999-2004 Sourceforge JACOB Project.
 * All rights reserved. Originator: Dan Adler (http://danadler.com).
 * Get more information about JACOB at http://sourceforge.net/projects/jacob-project
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include "stdafx.h"
#include <objbase.h>
#include "Dispatch.h"
// Win32 support for Ole Automation
#include <wchar.h>
#include <string.h>
#include <atlbase.h>
#include <oleauto.h>
#include <olectl.h>
#include "util.h"

extern "C" {

JNIEXPORT jint JNICALL Java_org_racob_com_IUnknown_toEnumVariant
  (JNIEnv *env, jobject obj, jint pointer) {
    IDispatch *dispatch = (IDispatch *) pointer;
    LCID lcid = 2048; // <--- heh
    unsigned int argErr;
    EXCEPINFO excepinfo;
    DISPPARAMS dispParams;
    VARIANT result;
    IEnumVARIANT *pEnum = NULL;

    VariantInit(&result);
    dispParams.rgvarg = NULL;
    dispParams.rgdispidNamedArgs = NULL;
    dispParams.cNamedArgs = 0;
    dispParams.cArgs = 0;
    memset(&excepinfo, 0, sizeof(excepinfo));
    HRESULT hr = dispatch->Invoke(DISPID_NEWENUM, IID_NULL, lcid, 
         DISPATCH_METHOD | DISPATCH_PROPERTYGET, &dispParams, &result,
            &excepinfo, &argErr);

    if (FAILED(hr)) {
       VariantClear(&result);
       ThrowComFail(env, "failed to get IEnum Interface", hr);
    }

    if (V_VT(&result) == VT_UNKNOWN) {
        hr = V_UNKNOWN(&result)->QueryInterface(IID_IEnumVARIANT, (void **) &pEnum);
    } else if (V_VT(&result) == VT_DISPATCH) {
        hr = V_DISPATCH(&result)->QueryInterface(IID_IEnumVARIANT, (void **) &pEnum);
    }
    VariantClear(&result);
    
    if (FAILED(hr) || !pEnum) ThrowComFail(env, "failed to get IEnum Interface", hr);

    return (jint) pEnum;
}

/*
 * Class:     IUnknown
 * Method:    release
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_org_racob_com_IUnknown_release
  (JNIEnv *env, jclass obj, jint pointer) {
  IUnknown* self = (IUnknown *) pointer;
  if(self != NULL) self->Release();
}

}
