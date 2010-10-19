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
/* Header for class TypeInfo */

#ifndef _Included_TypeInfo
#define _Included_TypeInfo
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     TypeInfo
 * Method:    getContainingTypeLibNative
 * Signature: (I)Lorg/racob/com/TypeLib;
 */
JNIEXPORT jobject JNICALL Java_org_racob_com_TypeInfo_getContainingTypeLib
  (JNIEnv *, jobject, jint);

/*
 * Class:     TypeInfo
 * Method:    getDocumentationNative
 * Signature: (II)Lorg/racob/com/Documentation;
 */
JNIEXPORT jobject JNICALL Java_org_racob_com_TypeInfo_getDocumentation
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     TypeInfo
 * Method:    getFuncDesc
 * Signature: (II)Lorg/racob/com/FuncDesc;
 */
JNIEXPORT jobject JNICALL Java_org_racob_com_TypeInfo_getFuncDesc
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     TypeInfo
 * Method:    getNames
 * Signature: (II)Lorg/racob/com/VarDesc;
 */
JNIEXPORT jobjectArray JNICALL Java_org_racob_com_TypeInfo_getNames
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     TypeInfo
 * Method:    getImplTypeFlags
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_org_racob_com_TypeInfo_getImplTypeFlags
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     TypeInfo
 * Method:    getRefTypeOfImplType
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_org_racob_com_TypeInfo_getRefTypeOfImplType
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     TypeInfo
 * Method:    getRefTypeInfo
 * Signature: (II)Lorg/racob/com/TypeInfo;
 */
JNIEXPORT jobject JNICALL Java_org_racob_com_TypeInfo_getRefTypeInfo
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     TypeInfo
 * Method:    getVarDesc
 * Signature: (II)Lorg/racob/com/VarDesc;
 */
JNIEXPORT jobject JNICALL Java_org_racob_com_TypeInfo_getVarDesc
  (JNIEnv *, jobject, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
