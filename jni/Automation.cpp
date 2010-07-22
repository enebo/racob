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
 
// Note: We use the 'Ex' version of this file so it won't try auto-registering
// any component:
// http://chrisbensen.blogspot.com/2007/02/when-most-people-use-com-they-stumble.html

/*
 * Class:     Automation
 * Method:    loadTypeLib
 * Signature: (Ljava/lang/String;)Lcom/jacob/com/TypeLib;
 */
JNIEXPORT jobject JNICALL Java_com_jacob_com_Automation_loadTypeLib
  (JNIEnv *env, jclass obj, jstring file)
{
 BSTR filename = makeBStr(env, file);
 ITypeLib *typeLib = NULL;
 HRESULT hr = LoadTypeLibEx(filename, REGKIND_NONE, &typeLib);
 SysFreeString(filename);
 if (!SUCCEEDED(hr)) {
    ThrowComFail(env, "Automation.loadTypeLibEx failed", hr);
    return NULL;
 }

 int typeCount = typeLib->GetTypeInfoCount();
 if (typeCount == E_NOTIMPL) {
    ThrowComFail(env, "loadTypeLib failed to get count", hr);
    return NULL;
 }

 TLIBATTR *libAttr = NULL;
 hr = typeLib->GetLibAttr(&libAttr);
 if (!SUCCEEDED(hr)) {
    ThrowComFail(env, "Automation.loadTypeLibEx failed", hr);
    return NULL;
 }

 // GUID as string
 OLECHAR bstr[MAX_GUID_LENGTH];
 int length = StringFromGUID2(libAttr->guid, bstr, sizeof(bstr)/sizeof(OLECHAR));
 BSTR copy = SysAllocString(bstr);
 jstring guid = length > 3 ? makeString(env, copy) : NULL;

 jclass autoClass = env->FindClass("com/jacob/com/TypeLib");
 jmethodID autoCons = env->GetMethodID(autoClass, "<init>", "(ILjava/lang/String;IIII)V");
 jobject newAuto = env->NewObject(autoClass, autoCons, (jint) typeLib,
         guid, typeCount, libAttr->wLibFlags, libAttr->wMajorVerNum,
         libAttr->wMinorVerNum);

 typeLib->ReleaseTLibAttr(libAttr);

 return newAuto;
}

}

