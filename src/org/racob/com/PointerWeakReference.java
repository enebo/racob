/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package org.racob.com;

import java.lang.ref.ReferenceQueue;
import java.lang.ref.WeakReference;

/**
 *
 */
public class PointerWeakReference extends WeakReference {
    private final Pointer pointer;
    
    public PointerWeakReference(IUnknown object, ReferenceQueue queue) {
        super(object, queue);
        this.pointer = object.pointer;
    }

    public void safeRelease() {
        pointer.safeRelease();
    }
}
