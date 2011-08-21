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

/**
 * This implementation has two obvious problems:
 * 1. It only works with a single dimension
 * 2. It boxes all primitive types in the lists it contains.  Not only that
 * it double boxes them into Variants.
 *
 * These two details may require a re-write if it becomes obvious to anyone
 * that we are creating so many objects.  A different design would be to support
 * n SafeArray classes so we can have one for each primitive type and one for
 * all Object/Variant-based ones.
 */
public class SafeArray {
    private Variant[] values;

    public SafeArray(Variant[] values) {
        this.values = values;
    }

    public int determinevt() {
        if (values == null || values.length == 0) return Variant.VariantVariant;

        int variantType = values[0].getvt();
        for (int i = 1; i < values.length; i++) {
            if (values[i].getvt() != variantType) return Variant.VariantVariant;
        }

        return variantType;
    }

    public static SafeArray create(Object value) {
        Class componentType = value.getClass().getComponentType();
        Variant[] values;
        if (Variant.class.isAssignableFrom(componentType)) {
            values = (Variant[]) value;
        } else {
            values = VariantUtilities.objectsToVariants((Object[]) value);
        }
        return new SafeArray(values);
    }

    public Variant[] getValues() {
        return values;
    }
}