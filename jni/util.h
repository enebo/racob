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
#include <objbase.h>
extern "C" {
#define MAX_GUID_LENGTH 80

#ifdef RACOB_DEBUG
  #define DLOG(fmt, ...) printf("%s:%d(%s) "fmt,__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__); fflush(stdout);
#else
  #define DLOG(fmt, ...)
#endif


  /* Initialized by initializeNative in variant.cpp */
  extern jclass VARIANT_CLASS;
  extern jmethodID VARIANT_CONSTRUCTOR;
  extern jclass DISPATCH_CLASS;
  extern jfieldID DISPATCH_FIELD;
  extern jfieldID POINTER_FIELD;

  unsigned long referenceCountFor(IUnknown *unknown);
  void IDispatchAddRef(char* message, IDispatch *obj);

  /* Defined in variant.cpp */
  jobject createVariant(JNIEnv *env, VARIANT* variant);
  void populateVariant(JNIEnv *env, jobject javaVariant, VARIANT* variant);
  VARIANT *extractVariant(JNIEnv *env, jobject arg);
  jobject variantToObject(JNIEnv *env, VARIANT* variant);

  void ThrowComFail(JNIEnv *env, const char* desc, jint hr);
  void ThrowComFailUnicode(JNIEnv *env, const wchar_t* desc, jint hr);
  SAFEARRAY *extractSA(JNIEnv *env, jobject arg);
  void setSA(JNIEnv *env, jobject arg, SAFEARRAY *sa, int copy);
  SAFEARRAY *copySA(SAFEARRAY *psa);
  void freeDocumentationStrings(BSTR name, BSTR docString, BSTR helpFile);
  jstring makeGUIDString(JNIEnv *env, GUID guid);
  jstring makeString(JNIEnv *env, BSTR value);
  BSTR makeBStr(JNIEnv *env, jstring value);
  jobject makeTypeInfo(JNIEnv *env, ITypeInfo *typeInfo);

  /* SafeArray.cpp */
  SAFEARRAY *makeSingleArray(int vt, long lowerBounds, long size);
  SAFEARRAY *makeArray(int vt, int dimensions, long *lowerBounds, long *elementSizes);
  SAFEARRAY *newSingleArray(JNIEnv* env, int vt, jobjectArray data);
}
