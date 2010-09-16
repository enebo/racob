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
        if (isAlive()) IUnknown.release(invalidate());
    }

    public boolean isAlive() {
        return get() != 0;
    }

    public int invalidate() {
        return getAndSet(0);
    }
}
