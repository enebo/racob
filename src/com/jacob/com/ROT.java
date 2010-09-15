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

import java.lang.ref.Reference;
import java.lang.ref.ReferenceQueue;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * The Running Object Table (ROT) maps each thread to a collection of all the
 * IUnknowns that were created in that thread. It always operates on the
 * current thread so all the methods are static and they implicitly get the
 * current thread.
 * <p>
 * The clearObjects method is used to release all the COM objects created by
 * Jacob in the current thread prior to uninitializing COM for that thread.
 * <p>
 */
public abstract class ROT {
    private static ThreadLocal<Boolean> initMTA = new ThreadLocal<Boolean>() {
        @Override
        public Boolean initialValue() {
            return FALSE;
        }
    };
    public static Map<PointerWeakReference,Boolean> objects = new ConcurrentHashMap<PointerWeakReference, Boolean>();
    private static final Boolean FALSE = new Boolean(false);
    private static final Boolean TRUE = new Boolean(true);
    private static int count = 0;
    private static final int CULL_COUNT;
    private static final int GC_COUNT;

    static {
        String cull_count = System.getProperty("com.jacob.cull_count");
        if (cull_count == null) cull_count = "2000";
        CULL_COUNT = Integer.parseInt(cull_count);

        String gc_count = System.getProperty("com.jacob.gc_count");
        if (gc_count == null) gc_count = "-1";
        GC_COUNT = Integer.parseInt(gc_count);

    }

    private static ThreadLocal<ReferenceQueue<IUnknown>> deadPool = new ThreadLocal<ReferenceQueue<IUnknown>>() {
        @Override
        protected ReferenceQueue<IUnknown> initialValue() {
            return new ReferenceQueue<IUnknown>();
        }
    };

    public static Map<PointerWeakReference,Boolean> getThreadObjects(boolean ignored) {
        return objects;
    }
    /**
     * safeRelease all remaining alive objects.
     */
    protected static void clearObjects() {
        if (IUnknown.isDebugEnabled()) {
            IUnknown.debug("ROT: " + objects.size() + " objects to clear in this thread's ROT ");
        }

        // walk the values
        for (PointerWeakReference reference : objects.keySet()) {
            IUnknown value = (IUnknown) reference.get();

            if (value != null) {
                value.safeRelease();
            }
        }
        objects.clear();
    }

    /**
     * Stores object so it can be released in COM when it is no longer
     * referenced.
     *
     * @param o
     */
    protected static void addObject(IUnknown o) {
        // If a new thread joins we need to add it to the apartment
        if (initMTA.get() == FALSE) {
            ComThread.InitMTA(false);
            initMTA.set(TRUE);
        }
        
        ReferenceQueue<IUnknown> deadObjects = deadPool.get();

        objects.put(new PointerWeakReference(o, deadObjects), FALSE);

        if (GC_COUNT != -1 && (count % GC_COUNT) == 0) {
            System.gc();
        }

        if ((count++ % CULL_COUNT) == 0) {
            int numberCulled = cullDeadPool(deadObjects, objects);

            if (IUnknown.isDebugEnabled()) {
                if (numberCulled > 0) {
                    IUnknown.debug("ROT: added instance of " +
                    o.getClass().getSimpleName() + "->[+1, -" +
                    numberCulled + "] with " + objects.size() +
                    " remaining live objects");
                }
            }
        }
    }

    @SuppressWarnings("element-type-mismatch")
    protected static int cullDeadPool(ReferenceQueue<IUnknown> deadObjects,
            Map<PointerWeakReference, Boolean> liveList) {
        int numberReleased = 0;
        Reference<? extends IUnknown> deadReference;
        while ((deadReference = deadObjects.poll()) != null) {
            ((PointerWeakReference) deadReference).safeRelease();
            liveList.remove(deadReference);
            numberReleased++;
        }

        return numberReleased;
    }

    /**
     * ROT can't be a subclass of IUnknown because of the way ROT pools are
     * managed so we force a DLL load here by referencing IUnknown
     */
    static {
        LibraryLoader.loadJacobLibrary();
    }
}
