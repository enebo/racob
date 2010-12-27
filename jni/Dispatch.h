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
/* Header for class Dispatch */

#ifndef _Included_Dispatch
#define _Included_Dispatch
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_racob_com_Dispatch
 * Method:    QueryInterface
 * Signature: (ILjava/lang/String;)Lorg/racob/com/Dispatch;
 */
JNIEXPORT jobject JNICALL Java_org_racob_com_Dispatch_QueryInterface
  (JNIEnv *, jobject, jint, jstring);

/*
 * Class:     Dispatch
 * Method:    createInstance
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_racob_com_Dispatch_createInstanceNative
  (JNIEnv *, jobject, jstring);

/*
 * Class:     Dispatch
 * Method:    getActiveInstance
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_racob_com_Dispatch_getActiveInstanceNative
  (JNIEnv *, jobject, jstring);

/*
 * Class:     Dispatch
 * Method:    coCreateInstance
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT jint JNICALL Java_org_racob_com_Dispatch_coCreateInstanceNative
  (JNIEnv *, jobject, jstring);

/*
 * Class:     Dispatch
 * Method:    getIDsOfNames
 * Signature: (II[Ljava/lang/String;)[I
 */
JNIEXPORT jintArray JNICALL Java_org_racob_com_Dispatch_getIDsOfNames
  (JNIEnv *, jclass, jint, jint, jobjectArray);

/*
 * Class:     Dispatch
 * Method:    invokev0
 * Signature: (ILjava/lang/String;III[LVariant;[I)LVariant;
 */
JNIEXPORT jobject JNICALL Java_org_racob_com_Dispatch_invokev0
  (JNIEnv *, jclass, jint, jstring, jint, jint, jint);

/*
 * Class:     Dispatch
 * Method:    invokev
 * Signature: (ILjava/lang/String;III[LVariant;[I)LVariant;
 */
JNIEXPORT jobject JNICALL Java_org_racob_com_Dispatch_invokev
  (JNIEnv *, jclass, jint, jstring, jint, jint, jint, jobjectArray, jintArray);

/*
 * Class:     Dispatch
 * Method:    getTypeInfo
 * Signature: (I)Lorg/racob/com/TypeInfo;
 */
JNIEXPORT jobject JNICALL Java_org_racob_com_Dispatch_getTypeInfo
  (JNIEnv *, jobject, jint);

#ifdef __cplusplus
}
#endif
#endif
