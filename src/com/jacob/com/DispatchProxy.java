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
 * If you need to pass a COM Dispatch object between STA threads, you have to
 * marshall the interface. This class is used as follows: the STA that creates
 * the Dispatch object must construct an instance of this class. Another thread
 * can then call toDispatch() on that instance and get a Dispatch pointer which
 * has been marshalled. WARNING: You can only call toDispatch() once! If you
 * need to call it multiple times (or from multiple threads) you need to
 * construct a separate DispatchProxy instance for each such case!
 */
public class DispatchProxy extends IUnknown {

    /**
     * Marshals the passed in dispatch into the stream
     *
     * @param localDispatch
     */
    public DispatchProxy(Dispatch localDispatch) {
        super();
        pointer.set(MarshalIntoStream(localDispatch.pointer.get()));
    }

    /**
     *
     * @return Dispatch the dispatch retrieved from the stream
     */
    public Dispatch toDispatch() {
        Dispatch dispatch = MarshalFromStream(pointer.get());
        pointer.invalidate();  // Cannot marshal from stream more than once
        return dispatch;
    }

    private native int MarshalIntoStream(int pointer);

    private native Dispatch MarshalFromStream(int pointer);
}
