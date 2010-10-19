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
#include <jni.h>
/* Header for class org_racob_com_Variant */

#ifndef _Included_org_racob_com_Variant
#define _Included_org_racob_com_Variant
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     Variant
 * Method:    initializeNative
 * Signature: ()V
 */
JNIEXPORT jobject JNICALL Java_org_racob_com_Variant_initializeNative
  (JNIEnv *, jclass);

/*
 * Class:     org_racob_com_Variant
 * Method:    putVariantNull
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_racob_com_Variant_putVariantNull
  (JNIEnv *, jobject);

/*
 * Class:     org_racob_com_Variant
 * Method:    putVariantShortRef
 * Signature: (S)V
 */
JNIEXPORT void JNICALL Java_org_racob_com_Variant_putVariantShortRef
  (JNIEnv *, jobject, jshort);

/*
 * Class:     org_racob_com_Variant
 * Method:    putVariantIntRef
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_org_racob_com_Variant_putVariantIntRef
  (JNIEnv *, jobject, jint);

/*
 * Class:     org_racob_com_Variant
 * Method:    putVariantDoubleRef
 * Signature: (D)V
 */
JNIEXPORT void JNICALL Java_org_racob_com_Variant_putVariantDoubleRef
  (JNIEnv *, jobject, jdouble);

/*
 * Class:     org_racob_com_Variant
 * Method:    putVariantDateRef
 * Signature: (D)V
 */
JNIEXPORT void JNICALL Java_org_racob_com_Variant_putVariantDateRef
  (JNIEnv *, jobject, jdouble);

/*
 * Class:     org_racob_com_Variant
 * Method:    putVariantStringRef
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_racob_com_Variant_putVariantStringRef
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_racob_com_Variant
 * Method:    putVariantEmpty
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_racob_com_Variant_putVariantEmpty
  (JNIEnv *, jobject);

/*
 * Class:     org_racob_com_Variant
 * Method:    putVariantNothing
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_racob_com_Variant_putVariantNothing
  (JNIEnv *, jobject);

/*
 * Class:     org_racob_com_Variant
 * Method:    putVariantCurrencyRef
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_racob_com_Variant_putVariantCurrencyRef
  (JNIEnv *, jobject, jlong);

/*
 * Class:     org_racob_com_Variant
 * Method:    putVariantLongRef
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_racob_com_Variant_putVariantLongRef
  (JNIEnv *, jobject, jlong);

/*
 * Class:     org_racob_com_Variant
 * Method:    putVariantErrorRef
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_org_racob_com_Variant_putVariantErrorRef
  (JNIEnv *, jobject, jint);

/*
 * Class:     org_racob_com_Variant
 * Method:    putVariantBooleanRef
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_org_racob_com_Variant_putVariantBooleanRef
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     org_racob_com_Variant
 * Method:    putObjectRef
 * Signature: (Ljava/lang/Object;)V
 */
JNIEXPORT void JNICALL Java_org_racob_com_Variant_putObjectRef
  (JNIEnv *, jobject, jobject);

/*
 * Class:     org_racob_com_Variant
 * Method:    putVariantByteRef
 * Signature: (B)V
 */
JNIEXPORT void JNICALL Java_org_racob_com_Variant_putVariantByteRef
  (JNIEnv *, jobject, jbyte);

/*
 * Class:     org_racob_com_Variant
 * Method:    toVariantSafeArray
 * Signature: (Z)Lorg/racob/com/SafeArray;
 */
JNIEXPORT jobject JNICALL Java_org_racob_com_Variant_toVariantSafeArray
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     org_racob_com_Variant
 * Method:    putVariantSafeArrayRef
 * Signature: (LSafeArray;)V
 */
JNIEXPORT void JNICALL Java_org_racob_com_Variant_putVariantSafeArrayRef
  (JNIEnv *, jobject, jobject);

/*
 * Class:     org_racob_com_Variant
 * Method:    putVariantSafeArray
 * Signature: (LSafeArray;)V
 */
JNIEXPORT void JNICALL Java_org_racob_com_Variant_putVariantSafeArray
  (JNIEnv *, jobject, jobject);

/*
 * Class:     org_racob_com_Variant
 * Method:    putVariantNoParam
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_racob_com_Variant_putVariantNoParam
  (JNIEnv *, jobject);

/*
 * Class:     org_racob_com_Variant
 * Method:    putVariantDecRef
 * Signature: (Ljava.math.BigDecimal;)V
 */
JNIEXPORT void JNICALL Java_org_racob_com_Variant_putVariantDecRef
  (JNIEnv *env, jobject _this, jint signum, jbyte scale, jint lo, jint mid, jint hi);


/*
 * Class:     org_racob_com_Variant
 * Method:    putVariantDec
 * Signature: (Ljava.math.BigDecimal;)V
 */
JNIEXPORT void JNICALL Java_org_racob_com_Variant_putVariantDec
  (JNIEnv *env, jobject _this, jint signum, jbyte scale, jint lo, jint mid, jint hi);

#ifdef __cplusplus
}
#endif
#endif
