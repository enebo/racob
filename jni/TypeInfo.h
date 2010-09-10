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
 * Signature: (I)Lcom/jacob/com/TypeLib;
 */
JNIEXPORT jobject JNICALL Java_com_jacob_com_TypeInfo_getContainingTypeLib
  (JNIEnv *, jobject, jint);

/*
 * Class:     TypeInfo
 * Method:    getDocumentationNative
 * Signature: (II)Lcom/jacob/com/Documentation;
 */
JNIEXPORT jobject JNICALL Java_com_jacob_com_TypeInfo_getDocumentation
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     TypeInfo
 * Method:    getFuncDesc
 * Signature: (II)Lcom/jacob/com/FuncDesc;
 */
JNIEXPORT jobject JNICALL Java_com_jacob_com_TypeInfo_getFuncDesc
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     TypeInfo
 * Method:    getNames
 * Signature: (II)Lcom/jacob/com/VarDesc;
 */
JNIEXPORT jobjectArray JNICALL Java_com_jacob_com_TypeInfo_getNames
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     TypeInfo
 * Method:    getRefTypeOfImplType
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_com_jacob_com_TypeInfo_getRefTypeOfImplType
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     TypeInfo
 * Method:    getRefTypeInfo
 * Signature: (II)Lcom/jacob/com/TypeInfo;
 */
JNIEXPORT jobject JNICALL Java_com_jacob_com_TypeInfo_getRefTypeInfo
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     TypeInfo
 * Method:    getVarDesc
 * Signature: (II)Lcom/jacob/com/VarDesc;
 */
JNIEXPORT jobject JNICALL Java_com_jacob_com_TypeInfo_getVarDesc
  (JNIEnv *, jobject, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
