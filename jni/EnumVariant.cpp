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

JNIEXPORT jobject JNICALL
Java_com_jacob_com_EnumVariant_Next(JNIEnv* env, jobject _this, jint pointer) {
  IEnumVARIANT* enumVariant = (IEnumVARIANT *) pointer;
  if (enumVariant == NULL) return NULL;

  VARIANT sink;
  ULONG fetchCount = 0;

  VariantInit(&sink);
  HRESULT hr = enumVariant->Next(1, &sink, &fetchCount);
  if (FAILED(hr)) {
    ThrowComFail(env, "IEnumVARIANT::Next", hr);
    return NULL;
  }

  return createVariant(env, &sink);
}

JNIEXPORT void JNICALL
Java_com_jacob_com_EnumVariant_release(JNIEnv* env, jobject _this, jint pointer) {
  IEnumVARIANT* self = (IEnumVARIANT *) pointer;
  if(self != NULL) self->Release();
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
