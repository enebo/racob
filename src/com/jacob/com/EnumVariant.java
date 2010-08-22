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

import java.util.Enumeration;

/**
 * An implementation of IEnumVariant based on code submitted by Thomas Hallgren
 * (mailto:Thomas.Hallgren@eoncompany.com)
 */
public class EnumVariant extends JacobObject implements Enumeration<Variant> {
    private Variant[] values;
    int i = -1;

    protected EnumVariant(int pointer) {
        this(pointer, 5);
    }

    protected EnumVariant(int pointer, int size) {
        this.pointer = pointer;
        values = new Variant[size];
    }

    /**
     * Implements java.util.Enumeration
     *
     * @return boolean true if there are more elements in this enumeration
     */
    public boolean hasMoreElements() {
        if (i == -1) i = Next(pointer, values, values.length) - 1;

        return i > -1;
    }

    /**
     * Implements java.util.Enumeration
     *
     * @return next element in the enumeration
     */
    public Variant nextElement() {
        if (hasMoreElements()) {
            Variant returnValue = values[i];

            i--;
            
            return returnValue;
        }

        throw new java.util.NoSuchElementException();
    }

    /**
     */
    public native int Next(int pointer, Variant[] values, int size);

    /**
     * This should be private and wrapped to protect JNI layer.
     *
     * @param count
     *            number to skip
     */
    public native void Skip(int pointer, int count);

    /**
     * This should be private and wrapped to protect JNI layer
     */
    public native void Reset(int pointer);

    /**
     * now private so only this object can access was: call this to explicitly
     * release the com object before gc
     *
     */
    protected native void release(int pointer);
}