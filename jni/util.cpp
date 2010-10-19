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
#include "Dispatch.h"
// Win32 support for Ole Automation
#include <wchar.h>
#include <string.h>
#include <atlbase.h>
#include <objbase.h>
#include <oleauto.h>
#include <olectl.h>
#include "util.h"

extern "C" 
{

void ThrowComFail(JNIEnv *env, const char* desc, jint hr)
{
  jclass failClass = env->FindClass("org/racob/com/ComFailException");
  // call the constructor that takes hr and message
  jmethodID failCons = 
     env->GetMethodID(failClass, "<init>", "(ILjava/lang/String;)V");
  if (!desc) {
	  desc = "Java/COM Error";
  }
  jstring js = env->NewStringUTF(desc);
  jthrowable fail = (jthrowable)env->NewObject(failClass, failCons, hr, js);
  env->Throw(fail);
}

void ThrowComFailUnicode(JNIEnv *env, const wchar_t* desc, jint hr)
{
  if (!desc) {
	  ThrowComFail(env, "Java/COM Error", hr);
  }
  jclass failClass = env->FindClass("org/racob/com/ComFailException");
  // call the constructor that takes hr and message
  jmethodID failCons = 
     env->GetMethodID(failClass, "<init>", "(ILjava/lang/String;)V");
  jstring js = env->NewString((const jchar *) desc, wcslen(desc));
  jthrowable fail = (jthrowable)env->NewObject(failClass, failCons, hr, js);
  env->Throw(fail);
}

// if env's are different throw on the 1st env
int CheckEnv(JNIEnv *env1, JNIEnv *env2)
{
  if (env1 != env2) {
    jclass failClass = env1->FindClass("org/racob/com/WrongThreadException");
    // call the constructor that takes hr and message
    jmethodID failCons = 
       env1->GetMethodID(failClass, "<init>", "()V");
    env1->ThrowNew(failClass, "Wrong Thread");
		return 0;
  }
	return 1;
}

 void freeDocumentationStrings(BSTR name, BSTR docString, BSTR helpFile)
 {
  if (name != NULL) SysFreeString(name);
  if (docString != NULL) SysFreeString(docString);
  if (helpFile != NULL) SysFreeString(helpFile);
 }

 jstring makeString(JNIEnv *env, BSTR value)
 {
  int length = SysStringLen(value);
  return env->NewString((const jchar *) value, length);
 }

 jstring makeGUIDString(JNIEnv *env, GUID guid)
 {
  // GUID as string
  OLECHAR bstr[MAX_GUID_LENGTH];
  int length = StringFromGUID2(guid, bstr, sizeof(bstr)/sizeof(OLECHAR));
  BSTR copy = SysAllocString(bstr);
  jstring guidString = length > 3 ? makeString(env, copy) : NULL;

  return guidString;
 }

 BSTR makeBStr(JNIEnv *env, jstring value)
   {
    const jchar *chars = env->GetStringChars(value, NULL);
    int length = env->GetStringLength(value);
    BSTR newBstr = SysAllocStringLen((const OLECHAR *) chars, length);

    env->ReleaseStringChars(value, chars);
    
    return newBstr;

 }

 jobject makeTypeInfo(JNIEnv *env, ITypeInfo *typeInfo) {
   TYPEATTR *typeAttributes = NULL;
   HRESULT hr = typeInfo->GetTypeAttr(&typeAttributes);
   if (!SUCCEEDED(hr)) {
      ThrowComFail(env, "GetTypeInfo.GetTypeAttr failed", hr);
      return NULL;
   }

   jstring progid = 0;
   LPOLESTR buffer;
   hr = ProgIDFromCLSID(typeAttributes->guid, &buffer);
   if (SUCCEEDED(hr)) {
      BSTR bstr = OLE2BSTR(buffer);
      progid = makeString(env, bstr);
      SysFreeString(bstr);
      CoTaskMemFree(buffer);
   }

   jint aliasVT = -1;
   if (typeAttributes->typekind == 6 /*TYPEKIND_ALIAS where is this defined */) {
      aliasVT = typeAttributes->tdescAlias.vt;
   }

   jclass autoClass = env->FindClass("org/racob/com/TypeInfo");
   jmethodID autoCons = env->GetMethodID(autoClass, "<init>", "(ILjava/lang/String;Ljava/lang/String;IIIIIIII)V");
   jobject newAuto = env->NewObject(autoClass, autoCons, (jint) typeInfo,
           makeGUIDString(env, typeAttributes->guid), progid,
           typeAttributes->typekind, typeAttributes->cFuncs,
           typeAttributes->cImplTypes, typeAttributes->cVars,
           typeAttributes->wTypeFlags, typeAttributes->wMajorVerNum,
           typeAttributes->wMinorVerNum, aliasVT);

   typeInfo->ReleaseTypeAttr(typeAttributes);

   return newAuto;
 }
}
