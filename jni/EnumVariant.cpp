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

/**
 * An implementation of IEnumVariant based on code submitted by
 * Thomas Hallgren (mailto:Thomas.Hallgren@eoncompany.com)
 */
extern "C" {

 #define MAX_VALUES 50

JNIEXPORT jint JNICALL
Java_com_jacob_com_EnumVariant_Next(JNIEnv* env, jobject _this, jint pointer, jobjectArray values, jint valuesSize) {
  IEnumVARIANT* enumVariant = (IEnumVARIANT *) pointer;
  if (enumVariant == NULL) return 0;

  if (valuesSize > MAX_VALUES) {
     ThrowComFail(env, "IEnumVariant::Next -- too many values requested", -1);
     return 0;
  }

  VARIANT sink[MAX_VALUES];
  ULONG fetchCount = 0;

  for (int i = 0; i < valuesSize; i++) VariantInit(&sink[i]);

  HRESULT hr = enumVariant->Next(valuesSize, sink, &fetchCount);
  if (FAILED(hr)) {
    ThrowComFail(env, "IEnumVARIANT::Next", hr);
    return 0;
  }

  if (fetchCount == 0) return 0; // Nothing retrieved.

  for (int i = 0; i < fetchCount; i++) {
     jobject variant = createVariant(env, &sink[i]);
     VariantClear(&sink[i]);
     env->SetObjectArrayElement(values, fetchCount - i - 1, variant);
     env->DeleteLocalRef(variant);
  }

  return fetchCount;
}

JNIEXPORT void JNICALL
Java_com_jacob_com_EnumVariant_Reset(JNIEnv* env, jobject _this, jint pointer) {
  IEnumVARIANT* self = (IEnumVARIANT *) pointer;
  if(self == NULL) return;

  HRESULT hr = self->Reset();
  if(FAILED(hr)) ThrowComFail(env, "IEnumVARIANT::Reset", hr);
}

JNIEXPORT void JNICALL
Java_com_jacob_com_EnumVariant_Skip(JNIEnv* env, jobject _this, jint pointer, jint count) {
  IEnumVARIANT* self = (IEnumVARIANT *) pointer;
  if(self == NULL) return;

  HRESULT hr = self->Skip((ULONG)count);
  if(FAILED(hr)) ThrowComFail(env, "IEnumVARIANT::Skip", hr);
}

}
