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
jclass SAFEARRAY_CLASS = 0;
jclass VARIANT_CLASS = 0;

jfieldID POINTER_FIELD = 0;
jfieldID VARIANT_TYPE = 0;

jmethodID BOOLEAN_CONSTRUCTOR = 0;
jmethodID DISPATCH_CONSTRUCTOR = 0;
jmethodID SAFEARRAY_CONSTRUCTOR = 0;
jmethodID SAFEARRAY_ADD = 0;
jmethodID VARIANT_CONSTRUCTOR = 0;
jmethodID VARIANT_GETSTRING = 0;
jmethodID VARIANT_GETSHORT = 0;
jmethodID VARIANT_GETINT = 0;
jmethodID VARIANT_GETFLOAT = 0;
jmethodID VARIANT_GETDOUBLE = 0;
jmethodID VARIANT_GETLONG = 0;
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

JNIEXPORT jobject JNICALL Java_org_racob_com_Variant_initializeNative
  (JNIEnv *env, jclass clazz) {
    // prepare a new return value
    VARIANT_CLASS = (jclass) env->NewGlobalRef(env->FindClass("org/racob/com/Variant"));
    DISPATCH_CLASS = (jclass) env->NewGlobalRef(env->FindClass("org/racob/com/Dispatch"));
    BOOLEAN_CLASS = (jclass) env->NewGlobalRef(env->FindClass("java/lang/Boolean"));
    SAFEARRAY_CLASS = (jclass) env->NewGlobalRef(env->FindClass("org/racob/com/SafeArray"));

    jfieldID trueField = env->GetStaticFieldID(VARIANT_CLASS, "VT_TRUE", "Lorg/racob/com/Variant;");
    jfieldID falseField = env->GetStaticFieldID(VARIANT_CLASS, "VT_FALSE", "Lorg/racob/com/Variant;");
    TRUE_VARIANT =  env->NewGlobalRef(env->GetStaticObjectField(VARIANT_CLASS, trueField));
    FALSE_VARIANT = env->NewGlobalRef(env->GetStaticObjectField(VARIANT_CLASS, falseField));


    BOOLEAN_CONSTRUCTOR = env->GetMethodID(BOOLEAN_CLASS, "<init>", "(Z)V");
    DISPATCH_CONSTRUCTOR = env->GetMethodID(DISPATCH_CLASS, "<init>", "(I)V");
    SAFEARRAY_CONSTRUCTOR = env->GetMethodID(SAFEARRAY_CLASS, "<init>", "(I)V");
    SAFEARRAY_ADD = env->GetMethodID(SAFEARRAY_CLASS, "add", "(Ljava/lang/Object;)Z");
    VARIANT_CONSTRUCTOR = env->GetMethodID(VARIANT_CLASS, "<init>", "(Ljava/lang/Object;S)V");
    VARIANT_TYPE = env->GetFieldID(VARIANT_CLASS, "type", "S");
    VARIANT_GETSTRING = env->GetMethodID(clazz, "getString", "()Ljava/lang/String;");
    VARIANT_GETSHORT = env->GetMethodID(clazz, "getShort", "()S");
    VARIANT_GETINT = env->GetMethodID(clazz, "getInt", "()I");
    VARIANT_GETFLOAT = env->GetMethodID(clazz, "getFloat", "()F");
    VARIANT_GETDOUBLE = env->GetMethodID(clazz, "getDouble", "()D");
    VARIANT_GETLONG = env->GetMethodID(clazz, "getLong", "()J");
    VARIANT_GETDISPATCH = env->GetMethodID(clazz, "getDispatchPointer", "()I");
    VARIANT_GETDATEASDOUBLE = env->GetMethodID(clazz, "getDateAsDouble", "()D");
    VARIANT_GETCURRENCYASLONG = env->GetMethodID(clazz, "getCurrencyAsLong", "()J");
    VARIANT_GETBOOLEAN = env->GetMethodID(clazz, "getBoolean", "()Z");
    VARIANT_GETBYTE = env->GetMethodID(clazz, "getByte", "()B");
    VARIANT_GETVARIANT = env->GetMethodID(clazz, "getVariant", "()Lorg/racob/com/Variant;");

    VARIANT_CREATEDISPATCH = env->GetStaticMethodID(clazz, "createDispatchVariant", "(I)Lorg/racob/com/Variant;");
    VARIANT_CREATEDATE = env->GetStaticMethodID(clazz, "createDateVariant", "(D)Lorg/racob/com/Variant;");
    VARIANT_CREATEINT = env->GetStaticMethodID(clazz, "createIntVariant", "(I)Lorg/racob/com/Variant;");

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

jobject createBoxedLong(JNIEnv *env, jlong value) {
  jclass clazz = env->FindClass("java/lang/Long");
  jmethodID constructor = env->GetMethodID(clazz, "<init>", "(J)V");
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
  jclass clazz = env->FindClass("org/racob/com/Currency");
  jmethodID constructor = env->GetMethodID(clazz, "<init>", "(J)V");
  return env->NewObject(clazz, constructor, jl);
}

jobject createDate(JNIEnv *env, jdouble date) {
  jclass clazz = env->FindClass("org/racob/com/DateUtilities");
  jmethodID constructor = env->GetStaticMethodID(clazz, "convertWindowsTimeToDate", "(D)Ljava/util/Date;");
  return env->CallStaticObjectMethod(clazz, constructor, date);
}

jobject createDispatch(JNIEnv *env, IDispatch* value) {
  if (value) value->AddRef();
  
  return env->NewObject(DISPATCH_CLASS, DISPATCH_CONSTRUCTOR, (jint) value);
}

jobject createSafeArray(JNIEnv *env, VARIANT *vt, SAFEARRAY *array) {
    VARTYPE varType;
    SafeArrayGetVartype(array, &varType);

    jobject newArray = env->NewObject(SAFEARRAY_CLASS, SAFEARRAY_CONSTRUCTOR, (jint) vt);
    int dimensions = SafeArrayGetDim(array);
    unsigned int i = 0;
    long *lowerBounds, *upperBounds, *indexes;    
    lowerBounds = (long *) malloc(sizeof(long) * dimensions);
    upperBounds = (long *) malloc(sizeof(long) * dimensions);
    indexes = (long *) malloc(sizeof(long) * dimensions);
    
    if (!lowerBounds || !upperBounds || !indexes) {
        if (lowerBounds) free(lowerBounds);
        if (upperBounds) free(upperBounds);
        if (indexes) free(indexes);
        
        return NULL; //TODO: An error or ok?
    }

    for(i = 0; i < dimensions; ++i) {
        SafeArrayGetLBound(array, i+1, &lowerBounds[i]);
        SafeArrayGetLBound(array, i+1, &indexes[i]);
        SafeArrayGetUBound(array, i+1, &upperBounds[i]);
    }

    HRESULT hr = SafeArrayLock(array);

    i = 0;
    VARIANT variant;
    VariantInit(&variant);
    V_VT(&variant) = (V_VT(vt) & ~VT_ARRAY) | VT_BYREF;

    while (i < dimensions) {
        hr = SafeArrayPtrOfIndex(array, indexes, &V_BYREF(&variant));
        if (!SUCCEEDED(hr)) {
            printf("FDAILED\n");
        } else {
//        hr = SafeArrayGetElement(array, indexes, V_BYREF(&variant);

        // TODO: Add multi-dim support
        env->CallBooleanMethod(newArray, SAFEARRAY_ADD, createVariant(env, &variant));
        }

        for (i = 0; i < dimensions; ++i) {
            if (++indexes[i] <= upperBounds[i]) break;
            indexes[i] = lowerBounds[i];
        }
    }
    SafeArrayUnlock(array);

    return newArray;
}

jobject createString(JNIEnv *env, BSTR value) {
  return env->NewString((jchar*) value, SysStringLen(value));
}

jobject createUnknown(JNIEnv *env, IUnknown *value) {
  jclass clazz = env->FindClass("org/racob/com/IUnknown");
  jmethodID constructor = env->GetMethodID(clazz, "<init>", "(I)V");
  return env->NewObject(clazz, constructor, value);
}

/* Creates a Java object which represents the variant. */
jobject variantToObject(JNIEnv *env, VARIANT* v) {
  if (!v) return NULL;
//  printf("variantToObject: %d\n", V_VT(v)); fflush(stdout);

  if (V_VT(v) & VT_ARRAY) {
      if (V_ISBYREF(v)) {
         return createSafeArray(env, v, (SAFEARRAY *) *V_ARRAYREF(v));
      } else {
         return createSafeArray(env, v, (SAFEARRAY *) V_ARRAY(v));
      }
  }

  switch (V_VT(v)) {
     case VT_I2:
        return createBoxedShort(env, (jshort) V_I2(v));
     case VT_I2|VT_BYREF:
        return createBoxedShort(env, (jshort) *V_I2REF(v));
     case VT_UI8:
        return createBoxedInt(env, (jint) V_UI8(v));
     case VT_I4:
        return createBoxedInt(env, (jint) V_I4(v));
     case VT_UI8|VT_BYREF:
        return createBoxedInt(env, (jint) *V_UI8REF(v));
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
     case VT_I8:
        return createBoxedLong(env, (jdouble) V_I8(v));
     case VT_UI4:
        return createBoxedLong(env, (jdouble) V_UI4(v));
     case VT_I8|VT_BYREF:
        return createBoxedLong(env, (jdouble) *V_I8REF(v));
     case VT_UI4|VT_BYREF:
        return createBoxedLong(env, (jdouble) *V_UI4REF(v));
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
        return createDispatch(env, (IDispatch *) *V_DISPATCHREF(v));
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
jlong getValueAsLong(JNIEnv *env, jobject obj) {
  return (jlong) env->CallLongMethod(obj, VARIANT_GETLONG);
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
     case VT_UI8:
         V_UI8(v) = getValueAsInt(env, javaVariant); break;
     case VT_I4:
          V_I4(v) = getValueAsInt(env, javaVariant); break;
     case VT_R4:
          V_R4(v) = getValueAsFloat(env, javaVariant); break;
     case VT_R8:
          V_R8(v) = getValueAsDouble(env, javaVariant); break;
     case VT_I8:
          V_I8(v) = getValueAsLong(env, javaVariant); break;
     case VT_UI4:
          V_UI4(v) = getValueAsLong(env, javaVariant); break;
     case VT_CY:
          CY pf;
          pf.int64 = (LONGLONG) getValueAsCurrency(env, javaVariant);
          V_CY(v) = pf;
          break;
     case VT_DATE:
          V_DATE(v) = getValueAsDate(env, javaVariant); break;
     case VT_BSTR|VT_BYREF:
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

}
