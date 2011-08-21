#include "jni.h"
#include <wchar.h>
#include <string.h>
#include <atlbase.h>
#include <objbase.h>
#include <oleauto.h>
#include <olectl.h>
#include "util.h"

extern "C" {
    SAFEARRAY *makeSingleArray(int vt, long lowerBounds, long size) {
        SAFEARRAYBOUND safeArrayBounds;
        
        safeArrayBounds.lLbound = lowerBounds;
        safeArrayBounds.cElements = size;

        return SafeArrayCreate(vt, 1, &safeArrayBounds);
    }

    SAFEARRAY *makeArray(int vt, int dimensions, long *lowerBounds, long *elementSizes) {
        if (dimensions == 1) return makeSingleArray(vt, lowerBounds[0], elementSizes[0]);

        SAFEARRAYBOUND* safeArrayBounds = new SAFEARRAYBOUND[dimensions];

        for (int i = 0; i < dimensions; i++) {
            safeArrayBounds[i].lLbound = lowerBounds[i];
            safeArrayBounds[i].cElements = elementSizes[i];
        }

        SAFEARRAY *sa = SafeArrayCreate(vt, dimensions, safeArrayBounds);
        delete[] safeArrayBounds;
        return sa;
    }

    SAFEARRAY *newSingleArray(JNIEnv* env, int vt, jobjectArray data) {
        long size = env->GetArrayLength(data);
        SAFEARRAY *newArray = makeSingleArray(vt, 0, size);

        DLOG("newSingleArray of length %d\n", size);
        for (long i = 0; i < size; i++) {
        DLOG("newSingleArray iter %d\n", i);
            VARIANT variant;
            VariantInit(&variant);
            jobject object = env->GetObjectArrayElement(data, i);
            populateVariant(env, object, &variant);
            SafeArrayPutElement(newArray, &i, &variant);
            VariantClear(&variant);
        }
        DLOG("newSingleArray of length %d (done)\n", size);
        return newArray;
    }
}