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
package org.racob.com;

import java.util.ArrayList;

/**
 */
public class SafeArray extends ArrayList {
    private int variantType;

    // ENEBO: getting number of dimensions was here...we could add logic to
    // track as we add an SafeArray if we actually need it.

    public SafeArray(int variantType) {
        this.variantType = variantType;
    }

    // So-named based on same name in Variant
    public int getvt() {
        return variantType;
    }

    // Be damned sure you are really getting a variant...kaboom
    public Variant getVariant(int i) {
        return (Variant) get(i);
    }

    /**
     * Standard toString() Warning, this creates new Variant objects!
     *
     * @return String contents of variant
     */
    @Override
    public String toString() {
        StringBuilder s = new StringBuilder();

        for (Object elem: this) {
            if (elem instanceof SafeArray) {
                s.append("[").append(elem).append("]");
            } else if (elem instanceof Variant) {
                s.append(" ").append(elem);
            }
        }

        return s.toString();
    }
}