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
package com.jacob.com;

/**
 * The superclass of all Jacob objects. It is used to create a standard API
 * framework and to facilitate memory management for Java and COM memory
 * elements.
 * <p>
 * All instances of this class and subclasses are automatically managed by the
 * ROT. This means the ROT cannot be a subclass of JacobObject.
 * <p>
 * All COM object created by JACOB extend this class so that we can
 * automatically release them when the thread is detached from COM - if we leave
 * it to the finalizer it will call the release from another thread, which may
 * result in a segmentation violation.
 */
public abstract class JacobObject {
    protected int pointer;

    /**
     * Standard constructor that adds this JacobObject to the memory management
     * pool.
     */
    public JacobObject() {
        ROT.addObject(this);
    }

    protected abstract void release(int pointer);
    
    public boolean isAlive() {
        return pointer != 0;
    }

    /**
     * Finalizers call this method. This method should release any COM data
     * structures in a way that it can be called multiple times. This can happen
     * if someone manually calls this and then a finalizer calls it.
     */
    public void safeRelease() {
        if (isAlive()) {
            System.out.println("RELEASING: " + this);
            release(pointer);
            pointer = 0;
        } else if (isDebugEnabled()) { // looks like a double release
            debug(getClass().getName() + ":" + hashCode() + " double release");
        }
        
        // currently does nothing - subclasses may do something
        if (isDebugEnabled()) {
            // this used to do a toString() but that is bad for SafeArray
            debug("SafeRelease: " + this.getClass().getName());
        }
    }

    /**
     * When things go wrong, it is useful to be able to debug the ROT.
     */
    private static final boolean DEBUG = "true".equalsIgnoreCase(System.getProperty("com.jacob.debug"));

    protected static boolean isDebugEnabled() {
        return DEBUG;
    }

    /**
     * Very basic debugging function.
     *
     * @param istrMessage
     */
    protected static void debug(String istrMessage) {
        if (isDebugEnabled()) {
            System.out.println(Thread.currentThread().getName() + ": " + istrMessage);
        }
    }

    /**
     * force the jacob DLL to be loaded whenever this class is referenced
     */
    static {
        LibraryLoader.loadJacobLibrary();
    }

}