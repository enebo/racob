/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package com.jacob.com;

import java.util.concurrent.atomic.AtomicInteger;

/**
 *
 */
public class Pointer extends AtomicInteger {
    public Pointer(int pointerValue) {
        super(pointerValue);
    }

    public void safeRelease() {
        boolean debug = IUnknown.isDebugEnabled();

        if (isAlive()) {
// Happens waaaay too often
//            if (debug) IUnknown.debug("SafeRelease: " + this.getClass().getSimpleName());
            IUnknown.release(invalidate());
        } else {
            if (debug) IUnknown.debug(getClass().getSimpleName() + ":" + hashCode() + " double release");
        }
    }

    public boolean isAlive() {
        return get() != 0;
    }

    public int invalidate() {
        return getAndSet(0);
    }
}
