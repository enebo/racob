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

  JNIEXPORT jobject JNICALL Java_org_racob_com_TypeLib_getDocumentation
  (JNIEnv *env, jobject obj, jint pointer, jint index) {
   ITypeLib *typeLib = (ITypeLib *) pointer;
   BSTR name;
   BSTR docString;
   unsigned long helpContext;
   BSTR helpFile;
   HRESULT hr = typeLib->GetDocumentation(index, &name, &docString, &helpContext,
           &helpFile);
   if (!SUCCEEDED(hr)) {
      freeDocumentationStrings(name, docString, helpFile);
      ThrowComFail(env, "getDocumentation failed", hr);
      return NULL;
   }

   jclass autoClass = env->FindClass("org/racob/com/Documentation");
   jmethodID autoCons = env->GetMethodID(autoClass, "<init>",
           "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");
   jobject newAuto = env->NewObject(autoClass, autoCons, makeString(env, name),
           makeString(env, docString), makeString(env, helpFile), helpContext);

   freeDocumentationStrings(name, docString, helpFile);

   return newAuto;
 }

JNIEXPORT jobject JNICALL Java_org_racob_com_TypeLib_getTypeInfo
  (JNIEnv *env, jobject obj, jint pointer, jint index) {
   ITypeLib *typelib = (ITypeLib *) pointer;
   ITypeInfo* typeInfo = 0;
   HRESULT hr = typelib->GetTypeInfo(index, &typeInfo);
   if (!SUCCEEDED(hr)) {
      ThrowComFail(env, "GetTypeInfo failed", hr);
      return NULL;
   }

   return makeTypeInfo(env, typeInfo);
}

JNIEXPORT jint JNICALL Java_org_racob_com_TypeLib_getTypeInfoCount
  (JNIEnv *env, jobject obj, jint pointer) {
   ITypeLib *typelib = (ITypeLib *) pointer;
   HRESULT hr = typelib->GetTypeInfoCount();
   if (hr == E_NOTIMPL) {
      ThrowComFail(env, "GetTypeInfoCount failed", hr);
      return NULL;
   }

   return (jint) hr;
}
}