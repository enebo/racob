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
 * Object which represents a *live* IDispatch structure.
 */
public class Dispatch extends IUnknown {
    public static final int LOCALE_SYSTEM_DEFAULT = 2048;
    public static final int LSD = LOCALE_SYSTEM_DEFAULT;

    public static final int Method = 1;
    public static final int Get = 2;
    public static final int Put = 4;
    public static final int PutRef = 8;
    public static final int MGet = Method | Get; // Convenience
    
    /** program Id passed in by ActiveX components in their constructor */
    private String programId = null;

    private final static int[] NO_INT_ARGS = new int[0];
    public final static Variant[] NO_VARIANT_ARGS = new Variant[0];

    /**
     * zero argument constructor that sets the dispatch pointer to 0 This is the
     * only way to create a Dispatch without a value in the pointer field.
     */
    public Dispatch() {
        super();
    }

    /**
     * This constructor calls CoCreate with progid. This is the constructor
     * used by the ActiveXComponent or by programs that don't like the activeX
     * interface but wish to create new connections to windows programs.
     *
     * @param id is the name of the program you wish to cocreate to.
     * @throws IllegalArgumentException if an invalid progid is specified
     */
    public Dispatch(String id) {
        super();
        pointer.set(createInstanceNative(setProgramId(id)));
    }
    private native int createInstanceNative(String progid);

    /**
     * Constructor that only gets called internally from jni.  Do not use!!!
     */
    public Dispatch(int pointer) {
        super(pointer);
    }

    /**
     * Constructor to be used by subclass that want to swap themselves in for
     * the default Dispatch class. Usually you will have a class like
     * WordDocument that is a subclass of Dispatch and it will have a
     * constructor public WordDocument(Dispatch). That constructor should just
     * call this constructor as super(Dispatch)
     *
     * @param dispatchToBeDisplaced
     */
    public Dispatch(Dispatch dispatchToBeDisplaced) {
        super(dispatchToBeDisplaced.pointer.get()); // TAKE OVER THE IDispatch POINTER
        dispatchToBeDisplaced.pointer.invalidate(); // NULL OUT THE INPUT POINTER
    }
    
    /**
     * Make dispatch get assigned to whatever GetActiveObject() returns in
     * Windows.
     *
     * @param id is the name of the program you wish to connect to.
     */
    protected void getActiveInstance(String id) {
        pointer.set(getActiveInstanceNative(setProgramId(id)));
    }
    private native int getActiveInstanceNative(String progid);

    /**
     * Make Dispatch by calling CoCreate on windows side.
     *
     * @param id is the name of the program you wish to cocreate to.
     */
    protected void coCreateInstance(String id) {
        pointer.set(coCreateInstanceNative(setProgramId(id)));
    }
    private native int coCreateInstanceNative(String progid);

    private String setProgramId(String id) {
        if (id == null || id.equals("")) {
            throw new IllegalArgumentException("Empty/null progId");
        }
        programId = id;

        return id;
    }

    /**
     * @return the program id if instantiated by a progid string.
     */
    public String getProgramId() {
        return programId;
    }

    /**
     * Run QueryInterface() on this dispatch object to query for another
     * interface it may implement.
     *
     * @param id is {xxxx-xxxx-xxxx-xxx} format id
     * @return Dispatch a new dispatch object based on new interface
     */
    public Dispatch queryInterface(String id) {
        return queryInterface(pointer.get(), id);
    }
    private native Dispatch queryInterface(int pointer, String iid);

    public TypeInfo getTypeInfo() {
        return getTypeInfo(pointer.get());
    }
    private native TypeInfo getTypeInfo(int pointer);

    private int livePointer() {
        int value = pointer.get();

        if (!pointer.isAlive()) {
            throw new IllegalStateException("IDispatch is dead pointer");
        }

        return value;
    }

    private Variant[] vargs(Object[] args) {
        return VariantUtilities.objectsToVariants(args);
    }

    public void invokeSubv(String name, int dispID, int lcid, int flags,
            Variant[] args, int[] errs) {
        invokev(livePointer(), name, dispID, lcid, flags, args, errs);
    }

    public void invokeSubv(String name, int flags, Variant[] args, int[] errs) {
        invokev(livePointer(), name, 0, LSD, flags, args, errs);
    }

    public void invokeSubv(int dispID, int flags, Variant[] args, int[] errs) {
        invokev(livePointer(), null, dispID, LSD, flags, args, errs);
    }

    public void callSubN(String name, Object[] args) {
        invokeSubv(name, MGet, vargs(args), new int[args.length]);
    }

    public void callSubN(int dispID, Object[] args) {
        invokeSubv(dispID, MGet, vargs(args), new int[args.length]);
    }

    public int getIDOfName(String name) {
        return getIDsOfNames(LSD, new String[]{name})[0];
    }

    public int[] getIDsOfNames(int lcid, String[] names) {
        return getIDsOfNames(pointer.get(), lcid, names);
    }
    private static native int[] getIDsOfNames(int pointer, int lcid, String[] names);

    public int[] getIDsOfNames(String[] names) {
        return getIDsOfNames(LSD, names);
    }

    public Variant callN(String name, Object[] args) {
        return invokev(name, MGet, vargs(args), new int[args.length]);
    }

    public Variant callN(int dispID, Object[] args) {
        return invokev(dispID, MGet, vargs(args), new int[args.length]);
    }

    public Variant invoke(String name, int dispID, int lcid, int flags,
            Object[] args, int[] errs) {
        return invokev(livePointer(), name, dispID, lcid, flags, vargs(args), errs);
    }

    public Variant invoke(String name, int flags, Object[] args, int[] errs) {
        return invokev(name, flags, vargs(args), errs);
    }

    public Variant invoke(int dispID, int flags, Object[] args, int[] errs) {
        return invokev(dispID, flags, vargs(args), errs);
    }

    public Object callO(String name) {
        return invokev0(livePointer(), name, 0, LSD, MGet);
    }

    public Object callO(int dispid) {
        return invokev0(livePointer(), null, dispid, LSD, MGet);
    }

    public Variant call(String name, Object... args) {
        return callN(name, args);
    }

    public Variant call(int dispid, Object... args) {
        return callN(dispid, args);
    }

    public void put(String name, Object val) {
        invoke(name, Put, new Object[]{val}, new int[1]);
    }

    public void put(int dispid, Object val) {
        invoke(dispid, Put, new Object[]{val}, new int[1]);
    }

    public Variant invokev(String name, int dispID, int lcid, int flags,
            Variant[] args, int[] errs) {
        return invokev(livePointer(), name, dispID, lcid, flags, args, errs);
    }

    private static native Object invokev0(int pointer, String name,
            int dispID, int lcid, int flags);

    private static native Variant invokev(int pointer, String name,
            int dispID, int lcid, int flags, Variant[] args, int[] errs);

    public Variant invokev(String name, int flags, Variant[] args, int[] errs) {
        return invokev(livePointer(), name, 0, LSD, flags, args, errs);
    }

    public Variant invokev(int dispID, int flags, Variant[] args, int[] errs) {
        return invokev(livePointer(), null, dispID, LSD, flags, args, errs);
    }

    public void invokeSub(String name, int dispid, int lcid, int flags,
            Object[] args, int[] errs) {
        invokeSubv(name, dispid, lcid, flags, vargs(args), errs);
    }

    public void invokeSub(String name, int flags, Object[] args, int[] errs) {
        invokeSub(name, 0, LSD, flags, args, errs);
    }

    public void invokeSub(int dispid, int flags, Object[] args, int[] errs) {
        invokeSub(null, dispid, LSD, flags, args, errs);
    }

    public void callSub(String name, Object... args) {
        callSubN(name, args);
    }

    public void callSub(int dispid, Object... args) {
        callSubN(dispid, args);
    }

    public Variant get(String name) {
        return invokev(name, Get, NO_VARIANT_ARGS, NO_INT_ARGS);
    }

    public Variant get(int dispid) {
        return invokev(dispid, Get, NO_VARIANT_ARGS, NO_INT_ARGS);
    }

    public void putRef(String name, Object val) {
        invoke(name, PutRef, new Object[]{val}, new int[1]);
    }

    public void putRef(int dispid, Object val) {
        invoke(dispid, PutRef, new Object[]{val}, new int[1]);
    }
}