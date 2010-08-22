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
import java.util.ArrayList;
import java.util.List;

/**
 * The Running Object Table (ROT) maps each thread to a collection of all the
 * JacobObjects that were created in that thread. It always operates on the
 * current thread so all the methods are static and they implicitly get the
 * current thread.
 * <p>
 * The clearObjects method is used to release all the COM objects created by
 * Jacob in the current thread prior to uninitializing COM for that thread.
 * <p>
 */
public abstract class ROT {
    private static ThreadLocal<List<WeakReference<JacobObject>>> objectTable = new ThreadLocal() {
        @Override
        protected List<WeakReference<JacobObject>> initialValue() {
            return null;
        }  
    };

    private static ThreadLocal<ReferenceQueue<JacobObject>> deadPool = new ThreadLocal<ReferenceQueue<JacobObject>>() {
        @Override
        protected ReferenceQueue<JacobObject> initialValue() {
            return new ReferenceQueue<JacobObject>();
        }
    };

    /**
     * safeRelease all remaining alive objects in the soon-to-be-dead-thread.
     */
    protected static void clearObjects() {
        List<WeakReference<JacobObject>> objects = objectTable.get();
        if (JacobObject.isDebugEnabled()) {
            JacobObject.debug("ROT: " + objects.size() + " objects to clear in this thread's ROT ");
        }

        // walk the values
        for (WeakReference<JacobObject> reference : objects) {
            JacobObject value = reference.get();
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
    protected static void addObject(JacobObject o) {
        List<WeakReference<JacobObject>> objects = objectTable.get();
        ReferenceQueue<JacobObject> deadObjects = deadPool.get();

        if (objects == null) {
            // System.out.println("Creating new Thread: " + Thread.currentThread().getName());
            ComThread.InitMTA(false);
            objects = new ArrayList<WeakReference<JacobObject>>();
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
        objects.add(new WeakReference<JacobObject>(o, deadObjects));

        cullDeadPool(deadObjects);

        if (JacobObject.isDebugEnabled()) {
            JacobObject.debug("ROT: adding " + o + "->"
                    + o.getClass().getSimpleName() + " table size prior to addition:" + objects.size());
        }
    }

    protected static void cullDeadPool(ReferenceQueue<JacobObject> deadObjects) {
        Reference<? extends JacobObject> deadReference;
        while ((deadReference = deadObjects.poll()) != null) {
            // System.out.println("Clearing dead reference");
            JacobObject deadObject = deadReference.get();
            if (deadObject != null) deadObject.safeRelease();
        }
    }

    /**
     * ROT can't be a subclass of JacobObject because of the way ROT pools are
     * managed so we force a DLL load here by referencing JacobObject
     */
    static {
        LibraryLoader.loadJacobLibrary();
    }
}
