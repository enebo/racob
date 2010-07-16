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

#define P_TYPEINFO_FLD "m_pTypeInfo"

 // extract a ITypeInfo from a jobject
ITypeInfo *extractTypeInfo(JNIEnv *env, jobject arg)
{
  jclass argClass = env->GetObjectClass(arg);
  jfieldID ajf = env->GetFieldID( argClass, P_TYPEINFO_FLD, "I");
  
  return (ITypeInfo *) env->GetIntField(arg, ajf);
}

 JNIEXPORT jobject JNICALL Java_com_jacob_com_TypeInfo_getContainingTypeLib
  (JNIEnv *env, jobject obj)
 {
   ITypeInfo *typeInfo = extractTypeInfo(env, obj);
   ITypeLib* typeLib = NULL;
   unsigned int index = 0;
   HRESULT hr = typeInfo->GetContainingTypeLib(&typeLib, &index);
   if (!SUCCEEDED(hr)) {
      ThrowComFail(env, "GetContainingTypeLib failed", hr);
      return NULL;
   }

   int typeCount = typeLib->GetTypeInfoCount();
   if (typeCount == E_NOTIMPL) {
      ThrowComFail(env, "GetContainingTypeLib failed to get count", hr);
      return NULL;
   }

   jclass autoClass = env->FindClass("com/jacob/com/TypeLib");
   jmethodID autoCons = env->GetMethodID(autoClass, "<init>", "(II)V");
   jobject newAuto = env->NewObject(autoClass, autoCons, (jint) typeLib,
           typeCount);

   return newAuto;
 }

 JNIEXPORT jobject JNICALL Java_com_jacob_com_TypeInfo_getVarDesc
  (JNIEnv *env, jobject obj, jint index)
 {
   ITypeInfo *typeInfo = extractTypeInfo(env, obj);

   VARDESC *varDesc = NULL;
   HRESULT hr = typeInfo->GetVarDesc(index, &varDesc);
    if (!SUCCEEDED(hr)) {
       ThrowComFail(env, "getVarDesc failed", hr);
       return NULL;
    }

   jobject cValue = NULL;
   // We have a constant so let's save the variant
   if (varDesc->varkind == VAR_CONST) {
       jclass variantClass = env->FindClass("com/jacob/com/Variant");
       jmethodID variantCons = env->GetMethodID(variantClass, "<init>", "()V");
       cValue = env->NewObject(variantClass, variantCons);
       VARIANT *v = extractVariant(env, cValue);
       hr = VariantCopy(v, varDesc->lpvarValue);
       if (!SUCCEEDED(hr)) {
          ThrowComFail(env, "getVarDesc failed to copy constant", hr);
          return NULL;
       }
   }

   jclass autoClass = env->FindClass("com/jacob/com/VarDesc");
   jmethodID autoCons = env->GetMethodID(autoClass, "<init>", "(ILcom/jacob/com/Variant;)V");
   jobject newAuto = env->NewObject(autoClass, autoCons, varDesc->varkind, cValue);

   typeInfo->ReleaseVarDesc(varDesc);

   return newAuto;
 }
}