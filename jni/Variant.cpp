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
#include "Variant.h"
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

/* Initialized by initializeNative */
jclass BOOLEAN_CLASS = 0;
jclass DISPATCH_CLASS = 0;
jclass VARIANT_CLASS = 0;

jfieldID POINTER_FIELD = 0;
jfieldID VARIANT_TYPE = 0;

jmethodID BOOLEAN_CONSTRUCTOR = 0;
jmethodID DISPATCH_CONSTRUCTOR = 0;
jmethodID VARIANT_CONSTRUCTOR = 0;
jmethodID VARIANT_GETSTRING = 0;
jmethodID VARIANT_GETSHORT = 0;
jmethodID VARIANT_GETINT = 0;
jmethodID VARIANT_GETFLOAT = 0;
jmethodID VARIANT_GETDOUBLE = 0;
jmethodID VARIANT_GETDISPATCH = 0;
jmethodID VARIANT_GETDATEASDOUBLE = 0;
jmethodID VARIANT_GETCURRENCYASLONG = 0;
jmethodID VARIANT_GETBOOLEAN = 0;
jmethodID VARIANT_GETBYTE = 0;
jmethodID VARIANT_GETVARIANT = 0;
jmethodID VARIANT_CREATEDISPATCH = 0;
jmethodID VARIANT_CREATEDATE = 0;
jmethodID VARIANT_CREATEINT = 0;
jobject TRUE_VARIANT = 0;
jobject FALSE_VARIANT = 0;

JNIEXPORT jobject JNICALL Java_com_jacob_com_Variant_initializeNative
  (JNIEnv *env, jclass clazz) {
    // prepare a new return value
    VARIANT_CLASS = (jclass) env->NewGlobalRef(env->FindClass("com/jacob/com/Variant"));
    DISPATCH_CLASS = (jclass) env->NewGlobalRef(env->FindClass("com/jacob/com/Dispatch"));
    BOOLEAN_CLASS = (jclass) env->NewGlobalRef(env->FindClass("java/lang/Boolean"));
    
    jfieldID trueField = env->GetStaticFieldID(VARIANT_CLASS, "VT_TRUE", "Lcom/jacob/com/Variant;");
    jfieldID falseField = env->GetStaticFieldID(VARIANT_CLASS, "VT_FALSE", "Lcom/jacob/com/Variant;");
    TRUE_VARIANT =  env->NewGlobalRef(env->GetStaticObjectField(VARIANT_CLASS, trueField));
    FALSE_VARIANT = env->NewGlobalRef(env->GetStaticObjectField(VARIANT_CLASS, falseField));


    BOOLEAN_CONSTRUCTOR = env->GetMethodID(BOOLEAN_CLASS, "<init>", "(Z)V");
    DISPATCH_CONSTRUCTOR = env->GetMethodID(DISPATCH_CLASS, "<init>", "(I)V");
    VARIANT_CONSTRUCTOR = env->GetMethodID(VARIANT_CLASS, "<init>", "(Ljava/lang/Object;S)V");
    VARIANT_TYPE = env->GetFieldID(VARIANT_CLASS, "type", "S");
    VARIANT_GETSTRING = env->GetMethodID(clazz, "getString", "()Ljava/lang/String;");
    VARIANT_GETSHORT = env->GetMethodID(clazz, "getShort", "()S");
    VARIANT_GETINT = env->GetMethodID(clazz, "getInt", "()I");
    VARIANT_GETFLOAT = env->GetMethodID(clazz, "getFloat", "()F");
    VARIANT_GETDOUBLE = env->GetMethodID(clazz, "getDouble", "()D");
    VARIANT_GETDISPATCH = env->GetMethodID(clazz, "getDispatchPointer", "()I");
    VARIANT_GETDATEASDOUBLE = env->GetMethodID(clazz, "getDateAsDouble", "()D");
    VARIANT_GETCURRENCYASLONG = env->GetMethodID(clazz, "getCurrencyAsLong", "()J");
    VARIANT_GETBOOLEAN = env->GetMethodID(clazz, "getBoolean", "()Z");
    VARIANT_GETBYTE = env->GetMethodID(clazz, "getByte", "()B");
    VARIANT_GETVARIANT = env->GetMethodID(clazz, "getVariant", "()Lcom/jacob/com/Variant;");

    VARIANT_CREATEDISPATCH = env->GetStaticMethodID(clazz, "createDispatchVariant", "(I)Lcom/jacob/com/Variant;");
    VARIANT_CREATEDATE = env->GetStaticMethodID(clazz, "createDateVariant", "(D)Lcom/jacob/com/Variant;");
    VARIANT_CREATEINT = env->GetStaticMethodID(clazz, "createIntVariant", "(I)Lcom/jacob/com/Variant;");

    return NULL;
 }

jobject createDateVariant(JNIEnv *env, jdouble value) {
  return env->CallStaticObjectMethod(VARIANT_CLASS, VARIANT_CREATEDATE, value);
}

jobject createDispatchVariant(JNIEnv *env, IDispatch* pointer) {
  if (pointer) pointer->AddRef();
  
  return env->CallStaticObjectMethod(VARIANT_CLASS, VARIANT_CREATEDISPATCH, (jint) pointer);
}

jobject createBooleanVariant(JNIEnv *env, jboolean value) {
 return value == 0 ? FALSE_VARIANT : TRUE_VARIANT;
}

jobject createIntVariant(JNIEnv *env, jint value) {
  return env->CallStaticObjectMethod(VARIANT_CLASS, VARIANT_CREATEINT, value);
}

jobject createBoxedByte(JNIEnv *env, jbyte value) {
  jclass clazz = env->FindClass("java/lang/Byte");
  jmethodID constructor = env->GetMethodID(clazz, "<init>", "(B)V");
  return env->NewObject(clazz, constructor, value);
}

jobject createBoxedBoolean(JNIEnv *env, jboolean value) {
  return env->NewObject(BOOLEAN_CLASS, BOOLEAN_CONSTRUCTOR, value);
}

jobject createBoxedDouble(JNIEnv *env, jdouble value) {
  jclass clazz = env->FindClass("java/lang/Double");
  jmethodID constructor = env->GetMethodID(clazz, "<init>", "(D)V");
  return env->NewObject(clazz, constructor, value);
}

jobject createBoxedFloat(JNIEnv *env, jfloat value) {
  jclass clazz = env->FindClass("java/lang/Float");
  jmethodID constructor = env->GetMethodID(clazz, "<init>", "(F)V");
  return env->NewObject(clazz, constructor, value);
}

jobject createBoxedInt(JNIEnv *env, jint value) {
  jclass clazz = env->FindClass("java/lang/Integer");
  jmethodID constructor = env->GetMethodID(clazz, "<init>", "(I)V");
  return env->NewObject(clazz, constructor, value);
}

jobject createBoxedShort(JNIEnv *env, jshort value) {
  jclass clazz = env->FindClass("java/lang/Short");
  jmethodID constructor = env->GetMethodID(clazz, "<init>", "(S)V");
  return env->NewObject(clazz, constructor, value);
}

jobject createCurrency(JNIEnv *env, CY cy) {
  jlong jl;
  memcpy(&jl, &cy, sizeof (jl)); // was 64. should be sizeof(x) SF 1690420
  jclass clazz = env->FindClass("com/jacob/com/Currency");
  jmethodID constructor = env->GetMethodID(clazz, "<init>", "(J)V");
  return env->NewObject(clazz, constructor, jl);
}

jobject createDate(JNIEnv *env, jdouble date) {
  jclass clazz = env->FindClass("com/jacob/com/DateUtilities");
  jmethodID constructor = env->GetStaticMethodID(clazz, "convertWindowsTimeToDate", "(D)Ljava/util/Date;");
  return env->CallStaticObjectMethod(clazz, constructor, date);
}

jobject createDispatch(JNIEnv *env, IDispatch* value) {
  if (value) value->AddRef();
  
  return env->NewObject(DISPATCH_CLASS, DISPATCH_CONSTRUCTOR, (jint) value);
}

jobject createString(JNIEnv *env, BSTR value) {
  return env->NewString((jchar*) value, SysStringLen(value));
}

jobject createUnknown(JNIEnv *env, IUnknown *value) {
  jclass clazz = env->FindClass("com/jacob/com/IUnknown");
  jmethodID constructor = env->GetMethodID(clazz, "<init>", "(I)V");
  return env->NewObject(clazz, constructor, value);
}

/* Creates a Java object which represents the variant. */
jobject variantToObject(JNIEnv *env, VARIANT* v) {
  if (!v) return NULL;
//  printf("variantToObject: %d\n", V_VT(v)); fflush(stdout);

  switch (V_VT(v)) {
     case VT_I2:
        return createBoxedShort(env, (jshort) V_I2(v));
     case VT_I2|VT_BYREF:
        return createBoxedShort(env, (jshort) *V_I2REF(v));
     case VT_I4:
        return createBoxedInt(env, (jint) V_I4(v));
     case VT_I4|VT_BYREF:
        return createBoxedInt(env, (jint) *V_I4REF(v));
     case VT_R4:
        return createBoxedFloat(env, (jfloat) V_R4(v));
     case VT_R4|VT_BYREF:
        return createBoxedFloat(env, (jfloat) *V_R4REF(v));
     case VT_R8:
        return createBoxedDouble(env, (jdouble) V_R8(v));
     case VT_R8|VT_BYREF:
        return createBoxedDouble(env, (jdouble) *V_R8REF(v));
      case VT_BOOL:
         return createBoxedBoolean(env, (jboolean) V_BOOL(v));
      case VT_BOOL|VT_BYREF:
         return createBoxedBoolean(env, (jboolean) *V_BOOLREF(v));
     case VT_CY:
        return createCurrency(env, V_CY(v));
     case VT_CY|VT_BYREF:
       return createCurrency(env, *V_CYREF(v));
      case VT_DATE:
          return createDate(env, (jdouble) V_DATE(v));
     case VT_BSTR:
        return createString(env, (BSTR) V_BSTR(v));
     case VT_BSTR|VT_BYREF:
        return createString(env, (BSTR) *V_BSTRREF(v));
     case VT_DISPATCH:
        return createDispatch(env, (IDispatch *) V_DISPATCH(v));
     case VT_DISPATCH|VT_BYREF:
        return createDispatch(env, (IDispatch *) V_DISPATCHREF(v));
     case VT_EMPTY:
     case VT_NULL:
        return NULL;
     case VT_UI1:
        return createBoxedByte(env, (jbyte) V_UI1(v));
     case VT_UI1|VT_BYREF:
        return createBoxedByte(env, (jbyte) *V_UI1REF(v));
      case VT_ERROR:
        return createBoxedInt(env, (jbyte) V_ERROR(v));
     case VT_ERROR|VT_BYREF:
        return createBoxedInt(env, (jbyte) *V_ERRORREF(v));
     case VT_VARIANT: // Not sure this is possible
     case VT_VARIANT|VT_BYREF:
        return createVariant(env, V_VARIANTREF(v));
     case VT_UNKNOWN:
        return createUnknown(env, (IUnknown *) V_UNKNOWN(v));
     case VT_UNKNOWN|VT_BYREF:
        return createUnknown(env, (IUnknown *) *V_UNKNOWNREF(v));
     default:
        char mess[1024];
        sprintf(mess, "variantToObject: unhandled VT %d\n", V_VT(v));
        printf("variantToObject: unhandled VT %d\n", V_VT(v)); fflush(stdout);
        ThrowComFail(env, mess, -1);
  }
}

jobject createVariant(JNIEnv *env, VARIANT* v) {
//    printf("createVariant: %d\n", V_VT(v)); fflush(stdout);
    switch (V_VT(v)) {
      case VT_BOOL:
         return createBooleanVariant(env, (jboolean) V_BOOL(v));
      case VT_BOOL|VT_BYREF:
         return createBooleanVariant(env, (jboolean) *V_BOOLREF(v));
      case VT_DATE:
          return createDateVariant(env, (jdouble) V_DATE(v));
       case VT_I4:
          return createIntVariant(env, (jint) V_I4(v));
      case VT_DATE|VT_BYREF:
          return createDateVariant(env, (jdouble) *V_DATEREF(v));
      case VT_DISPATCH:
          return createDispatchVariant(env, V_DISPATCH(v));
      case VT_DISPATCH|VT_BYREF:
          return createDispatchVariant(env, *V_DISPATCHREF(v));
    }

    jobject object = variantToObject(env, v);
    return env->NewObject(VARIANT_CLASS, VARIANT_CONSTRUCTOR, object, V_VT(v));
}

jint getVariantType(JNIEnv *env, jobject obj){
  return env->GetIntField(obj, VARIANT_TYPE);
}

jboolean getValueAsBoolean(JNIEnv *env, jobject obj) {
  return env->CallBooleanMethod(obj, VARIANT_GETBOOLEAN);
}

jbyte getValueAsByte(JNIEnv *env, jobject obj) {
  return env->CallBooleanMethod(obj, VARIANT_GETBYTE);
}

jlong getValueAsCurrency(JNIEnv *env, jobject obj) {
  return env->CallLongMethod(obj, VARIANT_GETCURRENCYASLONG);
}

jdouble getValueAsDate(JNIEnv *env, jobject obj) {
  return (jdouble) env->CallDoubleMethod(obj, VARIANT_GETDATEASDOUBLE);
}

jobject getValueAsDispatch(JNIEnv *env, jobject obj) {
  return env->CallObjectMethod(obj, VARIANT_GETDISPATCH);
}

double getValueAsDouble(JNIEnv *env, jobject obj) {
  return (double) env->CallDoubleMethod(obj, VARIANT_GETDOUBLE);
}

float getValueAsFloat(JNIEnv *env, jobject obj) {
  return (float) env->CallFloatMethod(obj, VARIANT_GETFLOAT);
}

int getValueAsInt(JNIEnv *env, jobject obj) {
  return (int) env->CallIntMethod(obj, VARIANT_GETINT);
}

short getValueAsShort(JNIEnv *env, jobject obj) {
  return (short) env->CallShortMethod(obj, VARIANT_GETSHORT);
}

jstring getValueAsString(JNIEnv *env, jobject obj) {
  return (jstring) env->CallObjectMethod(obj, VARIANT_GETSTRING);
}

jobject getValueAsVariant(JNIEnv *env, jobject obj) {
  return env->CallObjectMethod(obj, VARIANT_GETVARIANT);
}

void populateVariant(JNIEnv *env, jobject javaVariant, VARIANT* v) {
  jint variantType = getVariantType(env, javaVariant);

//  printf("populateVariant: %d\n", variantType); fflush(stdout);
  // Add byRef logic
  VariantClear(v);
  V_VT(v) = (VARTYPE) variantType;
  switch(variantType) {
     case VT_I2:
          V_I2(v) = getValueAsShort(env, javaVariant); break;
     case VT_I4:
          V_I4(v) = getValueAsInt(env, javaVariant); break;
     case VT_R4:
          V_R4(v) = getValueAsFloat(env, javaVariant); break;
     case VT_R8:
          V_R8(v) = getValueAsDouble(env, javaVariant); break;
     case VT_CY:
          CY pf;
          pf.int64 = (LONGLONG) getValueAsCurrency(env, javaVariant);
          V_CY(v) = pf;
          break;
     case VT_DATE:
//        printf("VT_DATE\n");
          V_DATE(v) = getValueAsDate(env, javaVariant); break;
     case VT_BSTR: {
          jstring s = getValueAsString(env, javaVariant);
          const jchar *cStr = env->GetStringChars(s, NULL);
          // SF 1314116
          // DBeck: 2005-09-23: changed CComBSTR c-tor to accept
          //	Unicode string (no terminating NULL) provided by GetStringChars
          const jsize numChars = env->GetStringLength(s);
          CComBSTR bs(numChars, (LPCOLESTR) cStr); // SR cast SF 1689061
          V_BSTR(v) = bs.Copy();

          env->ReleaseStringChars(s, cStr);
     }
          break;
     case VT_BOOL:
          V_BOOL(v) = getValueAsBoolean(env, javaVariant) == JNI_TRUE ? VARIANT_TRUE : VARIANT_FALSE;
          break;
     case VT_DISPATCH: {
          IDispatch *disp = (IDispatch *) getValueAsDispatch(env, javaVariant);
          if (disp) {
             V_DISPATCH(v) = disp;
             disp->AddRef();     // I am handing the pointer to COM
          } else {
             ThrowComFail(env, "putObject failed", -1);
          }
     }
          break;
     case VT_UI1:
          V_UI1(v) = getValueAsByte(env, javaVariant); break;
     case VT_ERROR:
          V_ERROR(v) = getValueAsInt(env, javaVariant); break;
     case VT_VARIANT: // This may be error case
     case VT_VARIANT|VT_BYREF: {
          VARIANT *inner = (VARIANT *) CoTaskMemAlloc(sizeof(VARIANT));
          populateVariant(env, getValueAsVariant(env, javaVariant), inner);
          V_VARIANTREF(v) = inner;
     }
     break;
     case VT_EMPTY:
     case VT_NULL:
        break;
     default:
        printf("DEFAULTING ON POPULATE VARIANT: %d\n", V_VT(v)); fflush(stdout);
          break;
  }
}

#define VARIANT_FLD "m_pVariant"

// extract a VARIANT from a Variant object
VARIANT *extractVariant(JNIEnv *env, jobject arg)
{
  jclass argClass = env->GetObjectClass(arg);
  jfieldID ajf = env->GetFieldID( argClass, VARIANT_FLD, "I");
  jint anum = env->GetIntField(arg, ajf);
  VARIANT *v = (VARIANT *)anum;
  return v;
}

JNIEXPORT void JNICALL Java_com_jacob_com_Variant_putVariantNull
  (JNIEnv *env, jobject _this)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    VariantClear(v); // whatever was there before
    V_VT(v) = VT_NULL;
  }
}

JNIEXPORT void JNICALL Java_com_jacob_com_Variant_putVariantShortRef
  (JNIEnv *env, jobject _this, jshort s)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    VariantClear(v); // whatever was there before
    short *ps = (short *)CoTaskMemAlloc(sizeof(short));
    *ps = s;
    V_VT(v) = VT_I2|VT_BYREF;
    V_I2REF(v) = ps;
  }
}

JNIEXPORT void JNICALL Java_com_jacob_com_Variant_putVariantIntRef
  (JNIEnv *env, jobject _this, jint s)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    VariantClear(v); // whatever was there before
    long *ps = (long *)CoTaskMemAlloc(sizeof(long));
    *ps = s;
    V_VT(v) = VT_I4|VT_BYREF;
    V_I4REF(v) = ps;
  }
}

JNIEXPORT void JNICALL Java_com_jacob_com_Variant_putVariantDoubleRef
  (JNIEnv *env, jobject _this, jdouble s)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    double *ps = (double *)CoTaskMemAlloc(sizeof(double));
    *ps = s;
    V_VT(v) = VT_R8|VT_BYREF;
    V_R8REF(v) = ps;
  }
}

JNIEXPORT void JNICALL Java_com_jacob_com_Variant_putVariantDateRef
  (JNIEnv *env, jobject _this, jdouble s)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    VariantClear(v); // whatever was there before
    double *ps = (double *)CoTaskMemAlloc(sizeof(double));
    *ps = s;
    V_VT(v) = VT_DATE|VT_BYREF;
    V_DATEREF(v) = ps;
  }
}

// SF 1065533  added unicode support
JNIEXPORT void JNICALL Java_com_jacob_com_Variant_putVariantStringRef
  (JNIEnv *env, jobject _this, jstring s)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    VariantClear(v); // whatever was there before

	const jchar *cStr = env->GetStringChars(s,NULL);
	// SF 1314116 
	// DBeck: 2005-09-23: changed CComBSTR c-tor to accept
	//	Unicode string (no terminating NULL) provided by GetStringChars
	const jsize numChars = env->GetStringLength(s);
    //CComBSTR bs(cStr);
	CComBSTR bs( numChars, (LPCOLESTR)cStr ); // SR cast SF 1689061

    BSTR *pbs = (BSTR *)CoTaskMemAlloc(sizeof(BSTR));
    bs.CopyTo(pbs);
    V_VT(v) = VT_BSTR|VT_BYREF;
    V_BSTRREF(v) = pbs;

    env->ReleaseStringChars(s,cStr);  }
}

JNIEXPORT jshort JNICALL Java_com_jacob_com_Variant_getVariantShortRef
  (JNIEnv *env, jobject _this)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    if (V_VT(v) !=  (VT_I2|VT_BYREF)) {
      return NULL;
    }
    return (jshort)*V_I2REF(v);
  }
  return NULL;
}

JNIEXPORT jint JNICALL Java_com_jacob_com_Variant_getVariantIntRef
  (JNIEnv *env, jobject _this)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    if (V_VT(v) !=  (VT_I4|VT_BYREF)) {
      return NULL;
    }
    return (jint)*V_I4REF(v);
  }
  return NULL;
}

JNIEXPORT jdouble JNICALL Java_com_jacob_com_Variant_getVariantDoubleRef
  (JNIEnv *env, jobject _this)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    if (V_VT(v) !=  (VT_R8|VT_BYREF)) {
      return NULL;
    }
    return (jdouble)*V_R8REF(v);
  }
  return NULL;
}

JNIEXPORT jdouble JNICALL Java_com_jacob_com_Variant_getVariantDateRef
  (JNIEnv *env, jobject _this)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    if (V_VT(v) !=  (VT_DATE|VT_BYREF)) {
      return NULL;
    }
    return (jdouble)*V_DATEREF(v);
  }
  return NULL;
}

JNIEXPORT jstring JNICALL Java_com_jacob_com_Variant_getVariantStringRef
  (JNIEnv *env, jobject _this)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    if (V_VT(v) !=  (VT_BSTR|VT_BYREF)) {
      return NULL;
    }
    BSTR *bs = V_BSTRREF(v);
    jstring js = env->NewString((jchar*)*bs, SysStringLen(*bs)); // SR cast SF 1689061
    return js;
  }
  return NULL;
}

JNIEXPORT void JNICALL Java_com_jacob_com_Variant_putVariantEmpty
  (JNIEnv *env, jobject _this)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    VariantClear(v); // whatever was there before
    V_VT(v) = VT_EMPTY;
  }
}

/**
 * Sets the variant type to dispatch with no value object
 **/
JNIEXPORT void JNICALL Java_com_jacob_com_Variant_putVariantNothing
  (JNIEnv *env, jobject _this)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    VariantClear(v); // whatever was there before
    V_VT(v) = VT_DISPATCH;
  }
}

JNIEXPORT void JNICALL Java_com_jacob_com_Variant_putVariantFloatRef
  (JNIEnv *env, jobject _this, jfloat val)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    VariantClear(v); // whatever was there before
    float *pf = (float *)CoTaskMemAlloc(sizeof(float));
    *pf = val;
    V_VT(v) = VT_R4|VT_BYREF;
    V_R4REF(v) = pf;
  }
}

JNIEXPORT void JNICALL Java_com_jacob_com_Variant_putVariantCurrencyRef
  (JNIEnv *env, jobject _this, jlong cur)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    VariantClear(v); // whatever was there before
    CY *pf = (CY *)CoTaskMemAlloc(sizeof(CY));
    memcpy(pf, &cur, sizeof(*pf)); // was 64. should be sizeof(x) SF 1690420
    V_VT(v) = VT_BYREF|VT_CY;
    V_CYREF(v) = pf;
  }
}

JNIEXPORT void JNICALL Java_com_jacob_com_Variant_putVariantLongRef
  (JNIEnv *env, jobject _this, jlong longValue)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    VariantClear(v); // whatever was there before
    LONGLONG *ps = (LONGLONG *)CoTaskMemAlloc(sizeof(LONGLONG));
    *ps = longValue;
    V_VT(v) = VT_I8|VT_BYREF;
    V_I8REF(v) = ps;
  }
}

JNIEXPORT void JNICALL Java_com_jacob_com_Variant_putVariantErrorRef
  (JNIEnv *env, jobject _this, jint i)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    VariantClear(v); // whatever was there before
    V_VT(v) = VT_ERROR|VT_BYREF;
    V_ERROR(v) = (int)i;
  }
}

JNIEXPORT void JNICALL Java_com_jacob_com_Variant_putVariantBooleanRef
  (JNIEnv *env, jobject _this, jboolean b)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    VariantClear(v); // whatever was there before
    VARIANT_BOOL *br = (VARIANT_BOOL *)CoTaskMemAlloc(sizeof(VARIANT_BOOL));
    *br = b ? VARIANT_TRUE : VARIANT_FALSE;
    V_VT(v) = VT_BOOL|VT_BYREF;
    V_BOOLREF(v) = br;
  }
}

JNIEXPORT void JNICALL Java_com_jacob_com_Variant_putVariantByteRef
  (JNIEnv *env, jobject _this, jbyte b)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    VariantClear(v); // whatever was there before
    unsigned char *br = (unsigned char *)CoTaskMemAlloc(sizeof(char));
    *br = b;
    V_VT(v) = VT_UI1|VT_BYREF;
    V_UI1REF(v) = br;
  }
}

JNIEXPORT jfloat JNICALL Java_com_jacob_com_Variant_getVariantFloatRef
  (JNIEnv *env, jobject _this)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    if (V_VT(v) !=  (VT_R4|VT_BYREF)) {
      return NULL;
    }
    return (jfloat)*V_R4REF(v);
  }
  return NULL;
}

JNIEXPORT jlong JNICALL Java_com_jacob_com_Variant_getVariantCurrencyRef
  (JNIEnv *env, jobject _this)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    if (V_VT(v) !=  (VT_CY|VT_BYREF)) {
      return NULL;
    }
    CY *cy;
    cy = V_CYREF(v);
    jlong jl;
    memcpy(&jl, cy, sizeof(jl)); // was 64. should be sizeof(x) SF 1690420
    return jl;
  }
  return NULL;
}

JNIEXPORT jlong JNICALL Java_com_jacob_com_Variant_getVariantLongRef
  (JNIEnv *env, jobject _this)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    if (V_VT(v) !=  (VT_I8|VT_BYREF)) {
      return NULL;
    }
    return (jlong)*V_I8REF(v);
  }
  return NULL;
}

JNIEXPORT jint JNICALL Java_com_jacob_com_Variant_getVariantErrorRef
  (JNIEnv *env, jobject _this)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    if (V_VT(v) !=  (VT_ERROR|VT_BYREF)) {
      return NULL;
    }
    return (jint)V_ERROR(v);
  }
  return NULL;
}

JNIEXPORT jboolean JNICALL Java_com_jacob_com_Variant_getVariantBooleanRef
  (JNIEnv *env, jobject _this)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    if (V_VT(v) !=  (VT_BOOL|VT_BYREF)) {
      return NULL;
    }
    return (jboolean)*V_BOOLREF(v);
  }
  return NULL;
}


JNIEXPORT jbyte JNICALL Java_com_jacob_com_Variant_getVariantByteRef
  (JNIEnv *env, jobject _this)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    if (V_VT(v) !=  (VT_UI1|VT_BYREF)) {
      return NULL;
    }
    return (jbyte)*V_UI1REF(v);
  }
  return NULL;
}

/**
 * Converts the data to a Safe Array object and then returns it as a Dispatch
 */
JNIEXPORT jobject JNICALL Java_com_jacob_com_Variant_toVariantSafeArray
  (JNIEnv *env, jobject _this, jboolean deepCopy)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    if ((V_VT(v) & VT_ARRAY) == 0) 
    {
      ThrowComFail(env, "Variant not array", -1);
      return NULL;
    }
    // prepare a new sa obj
    jclass saClass = env->FindClass("com/jacob/com/SafeArray");
    jmethodID saCons = env->GetMethodID(saClass, "<init>", "()V");
    // construct an SA to return
    jobject newSA = env->NewObject(saClass, saCons);
    // pass in the deep copy indicator
    setSA(env, newSA, V_ARRAY(v), deepCopy == JNI_TRUE ? 1 : 0);
    return newSA;
  }
  return NULL;
}

JNIEXPORT void JNICALL Java_com_jacob_com_Variant_putVariantSafeArrayRef
  (JNIEnv *env, jobject _this, jobject sa)
{
  SAFEARRAY *psa = extractSA(env, sa);
  if (psa) 
  {
    VARIANT *v = extractVariant(env, _this);
    if (v) {
      VARTYPE vt;
      SAFEARRAY **sa = (SAFEARRAY **)CoTaskMemAlloc(sizeof(SAFEARRAY*));
      *sa = psa;
      SafeArrayGetVartype(psa, &vt);
      V_VT(v) = VT_ARRAY | vt | VT_BYREF;
      V_ARRAYREF(v) = sa;
      return;
    }
    ThrowComFail(env, "Can't get variant pointer", -1);
    return;
  }
  ThrowComFail(env, "Can't get sa pointer", -1);
  return;
}

JNIEXPORT void JNICALL Java_com_jacob_com_Variant_putVariantSafeArray
  (JNIEnv *env, jobject _this, jobject sa)
{
  SAFEARRAY *psa = extractSA(env, sa);
  if (psa) 
  {
    VARIANT *v = extractVariant(env, _this);
    if (v) {
      VARTYPE vt;
      SafeArrayGetVartype(psa, &vt);
      V_VT(v) = VT_ARRAY | vt;
      V_ARRAY(v) = copySA(psa);
      return;
    }
    ThrowComFail(env, "Can't get variant pointer", -1);
    return;
  }
  ThrowComFail(env, "Can't get sa pointer", -1);
  return;
}

/**
 * sets the type to VT_ERROR and the error message to DISP_E_PARAMNOTFOIUND
 * */
JNIEXPORT void JNICALL Java_com_jacob_com_Variant_putVariantNoParam
  (JNIEnv *env, jobject _this)
{
  VARIANT *v = extractVariant(env, _this);
  if (v) {
    V_VT(v) = VT_ERROR;
    V_ERROR(v) = DISP_E_PARAMNOTFOUND;
  }
}

/**
 * this is a big cover method that returns TRUE if 
 * the variant type is 
 * 	VT_EMPTY, VT_NULL, VT_ERROR or VT_DISPATCH with no dispatch object
 * */
JNIEXPORT jboolean JNICALL Java_com_jacob_com_Variant_isVariantConsideredNull
  (JNIEnv *env, jobject _this)
{
  VARIANT *v = extractVariant(env, _this);
  if (!v) return JNI_TRUE;
  if ((V_VT(v) & VT_ARRAY)) 
  {
    // is it a null safearray
    // prior to 4 Dec 2005 the squiggle brackets were missing 
    // so this did the wrong thing for the else statement
    if ((V_VT(v) & VT_BYREF)) {
       if (!V_ARRAYREF(v)) return JNI_TRUE;
    } else {
       if (!V_ARRAY(v)) return JNI_TRUE;
    }
  }
  switch (V_VT(v))
  {
    case VT_EMPTY:
    case VT_NULL:
    case VT_ERROR:
            return JNI_TRUE;
    // is it a null dispatch (Nothing in VB)
    case VT_DISPATCH:
            if (!V_DISPATCH(v)) return JNI_TRUE;
  }
  return JNI_FALSE;
}

 /** 
  * puts a VT_DECIMAL by reference
  * Added 1.13M4
  * */
 JNIEXPORT void JNICALL Java_com_jacob_com_Variant_putVariantDecRef
    (JNIEnv *env, jobject _this, jint signum, jbyte scale, jint lo, jint mid, jint hi)
  {
    VARIANT *v = extractVariant(env, _this);
    if (v) {
      VariantClear(v); // whatever was there before
      DECIMAL *pd = (DECIMAL *)CoTaskMemAlloc(sizeof(DECIMAL));
      pd->scale = scale;
      if (signum == 1 || signum == 0){
    	  pd->sign = 0;
      } else {
    	  pd->sign = 0x80;
      }
      pd->Hi32 = hi;
      pd->Mid32 = mid;
      pd->Lo32 = lo;
      V_VT(v) = VT_DECIMAL | VT_BYREF;
      V_DECIMALREF(v) = pd;
    }
  }


 /**
  * puts a VT_DECIMAL
  * Added 1.13M4
  * */
 JNIEXPORT void JNICALL Java_com_jacob_com_Variant_putVariantDec
    (JNIEnv *env, jobject _this, jint signum, jbyte scale, jint lo, jint mid, jint hi)
  {
    VARIANT *v = extractVariant(env, _this);
    DECIMAL *d;
    if (v) {
      VariantClear(v); // whatever was there before
      d = (DECIMAL*)v;
      d->scale = scale;
      if (signum == 1 || signum == 0){
    	  d->sign = 0;
      } else {
    	  d->sign = 0x80;
      }
      d->Hi32 = hi;
      d->Mid32 = mid;
      d->Lo32 = lo;
      V_VT(v) = VT_DECIMAL;
    }
  }

/**
 * utility method used by the getVariantXXX() methods to convert VT_DECIMAL to BigDecimal
 * */
jobject extractDecimal
     (JNIEnv *env, DECIMAL* d) {
     jclass bigIntegerClass = env->FindClass("java/math/BigInteger");
     if (bigIntegerClass == NULL) return NULL;

     jclass bigDecimalClass = env->FindClass("java/math/BigDecimal");
     if (bigDecimalClass == NULL) {
        env->DeleteLocalRef(bigIntegerClass);
        return NULL;
     }
          
     jmethodID bigIntegerConstructor = env->GetMethodID(bigIntegerClass, "<init>", "(I[B)V");
     if (bigIntegerConstructor == NULL) {
        env->DeleteLocalRef(bigIntegerClass);
        env->DeleteLocalRef(bigDecimalClass);
        return NULL;
     }

     jmethodID bigDecimalConstructor = env->GetMethodID(bigDecimalClass, "<init>", "(Ljava/math/BigInteger;I)V");
     if (bigIntegerConstructor == NULL) {
        env->DeleteLocalRef(bigIntegerClass);
        env->DeleteLocalRef(bigDecimalClass);
        return NULL;
     }

     jbyteArray bArray = env->NewByteArray(12);
     if (bArray == NULL) {
        env->DeleteLocalRef(bigIntegerClass);
        env->DeleteLocalRef(bigDecimalClass);
        return NULL;
     }

     /* Unfortunately the byte ordering is completely wrong, so we remap it into buffer */
     jbyte* buffer = (jbyte*)malloc(12);
     buffer[11] = (byte)(d->Lo32 & 255);
     buffer[10] = (byte)((d->Lo32 >> 8) & 255);
     buffer[9] =  (byte)((d->Lo32 >> 16) & 255);
     buffer[8] =  (byte)((d->Lo32 >> 24) & 255);
     buffer[7] =  (byte)((d->Mid32) & 255);
     buffer[6] =  (byte)((d->Mid32 >> 8) & 255);
     buffer[5] =  (byte)((d->Mid32 >> 16) & 255);
     buffer[4] =  (byte)((d->Mid32 >> 24) & 255);
     buffer[3] =  (byte)((d->Hi32) & 255);
     buffer[2] =  (byte)((d->Hi32 >> 8) & 255);
     buffer[1] =  (byte)((d->Hi32 >> 16) & 255);
     buffer[0] =  (byte)((d->Hi32 >> 24) & 255);

     /* Load buffer into the actual array */
     env->SetByteArrayRegion(bArray, 0, 12, buffer);
     /* then clean up the C array */
     free(buffer);

     /* instantiate the BigInteger */
     jobject integer = env->NewObject(bigIntegerClass, bigIntegerConstructor, d->sign == 0x80?-1:1, bArray);
     jobject result = env->NewObject(bigDecimalClass, bigDecimalConstructor, integer, (jint)(d->scale));
      
     /* Clean up the Java global references */
     env->DeleteLocalRef(bArray);
     env->DeleteLocalRef(integer);
     env->DeleteLocalRef(bigIntegerClass);

     return result;
  }

/**
  * gets a VT_DECIMAL by ref as a BigDecimal
  * Added 1.13M4
  * */
JNIEXPORT jobject JNICALL Java_com_jacob_com_Variant_getVariantDecRef
    (JNIEnv *env, jobject _this)
  {
    VARIANT *v = extractVariant(env, _this);
    if (v) {
      if (V_VT(v) !=  (VT_DECIMAL|VT_BYREF)) {
        return NULL;
      }
      return extractDecimal(env, v->pdecVal);
    }
    return NULL;
  }

/**
  * gets a VT_DECIMAL as a BigDecimal
  * Added 1.13M4
  * */
JNIEXPORT jobject JNICALL Java_com_jacob_com_Variant_getVariantDec
    (JNIEnv *env, jobject _this)
  {
    VARIANT *v = extractVariant(env, _this); 
    if (v) {
      if (V_VT(v) !=  VT_DECIMAL) {
        return NULL;
      }
      return extractDecimal(env, (DECIMAL*)v);
    }
    return NULL;
  }

}
