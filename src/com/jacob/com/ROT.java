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
import java.lang.ref.WeakReference;
import java.util.HashSet;
import java.util.Set;

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
    private static ThreadLocal<Set<PointerWeakReference>> objectTable = new ThreadLocal() {
        @Override
        protected Set<PointerWeakReference> initialValue() {
            return null;
        }  
    };

    private static ThreadLocal<ReferenceQueue<IUnknown>> deadPool = new ThreadLocal<ReferenceQueue<IUnknown>>() {
        @Override
        protected ReferenceQueue<IUnknown> initialValue() {
            return new ReferenceQueue<IUnknown>();
        }
    };

    public static Set<PointerWeakReference> getThreadObjects(boolean ignored) {
        return objectTable.get();
    }
    /**
     * safeRelease all remaining alive objects in the soon-to-be-dead-thread.
     */
    protected static void clearObjects() {
        Set<PointerWeakReference> objects = objectTable.get();
        if (IUnknown.isDebugEnabled()) {
            IUnknown.debug("ROT: " + objects.size() + " objects to clear in this thread's ROT ");
        }

        // walk the values
        for (WeakReference<IUnknown> reference : objects) {
            IUnknown value = reference.get();
            // System.out.println("Clearing live reference");

            if (value != null) {
                value.safeRelease();
            }
        }
        objects.clear();
    }

    /**
     * Stores reference to supplied object so it can be released when it is
     * no longer references by anything.
     * <p>
     * In addition, this method cannot be threaded because it calls
     * ComThread.InitMTA. The ComThread object has some methods that call ROT so
     * we could end up deadlocked. This method should be safe without the
     * synchronization because the ROT works on per thread basis and the methods
     * that add threads and remove thread related entries are all synchronized
     *
     *
     * @param o
     */
    protected static void addObject(IUnknown o) {
        Set<PointerWeakReference> objects = objectTable.get();
        ReferenceQueue<IUnknown> deadObjects = deadPool.get();

        if (objects == null) {
            // System.out.println("Creating new Thread: " + Thread.currentThread().getName());
            ComThread.InitMTA(false);
            objects = new HashSet<PointerWeakReference>();
            objectTable.set(objects);
        }

        /*
        Thread[] threads = new Thread[Thread.activeCount()];
        Thread.enumerate(threads);
        System.out.println("LIVE THREADS: " + threads.length);
        for (int i = 0; i < threads.length; i++) {
            System.out.println("T: " + threads[i].getName());
        }*/

        // System.out.println("Adding new Object");
        objects.add(new PointerWeakReference(o, deadObjects));

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

    @SuppressWarnings("element-type-mismatch")
    protected static int cullDeadPool(ReferenceQueue<IUnknown> deadObjects,
            Set<PointerWeakReference> liveList) {
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
