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

#define P_TYPELIB_FLD "m_pTypeLib"

// extract a ITypeLib from a jobject
ITypeLib *extractTypeLib(JNIEnv *env, jobject arg)
{
  jclass argClass = env->GetObjectClass(arg);
  jfieldID ajf = env->GetFieldID( argClass, P_TYPELIB_FLD, "I");
  jint anum = env->GetIntField(arg, ajf);

  return (ITypeLib *) anum;
}

JNIEXPORT jobject JNICALL Java_com_jacob_com_TypeLib_getTypeInfo
  (JNIEnv *env, jobject _this, jint index)
{
   ITypeLib *typelib = extractTypeLib(env, _this);

   ITypeInfo* typeInfo = 0;
   HRESULT hr = typelib->GetTypeInfo(index, &typeInfo);
   if (!SUCCEEDED(hr)) {
      ThrowComFail(env, "GetTypeInfo failed", hr);
      return NULL;
   }

   TYPEATTR *typeAttributes = NULL;
   hr = typeInfo->GetTypeAttr(&typeAttributes);
   if (!SUCCEEDED(hr)) {
      ThrowComFail(env, "GetTypeInfo.GetTypeAttr failed", hr);
      return NULL;
   }

   jclass autoClass = env->FindClass("com/jacob/com/TypeInfo");
   jmethodID autoCons = env->GetMethodID(autoClass, "<init>", "(IIIII)V");
   jobject newAuto = env->NewObject(autoClass, autoCons, (jint) typeInfo,
           typeAttributes->typekind, typeAttributes->cFuncs,
           typeAttributes->cImplTypes, typeAttributes->cVars);

   typeInfo->ReleaseTypeAttr(typeAttributes);

   return newAuto;
}

}