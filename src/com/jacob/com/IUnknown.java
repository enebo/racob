package com.jacob.com;

/**
 * IUnknown...the top of COM
 */
public class IUnknown {
    // Holds a reference to an integer value which when passed to JNI
    // is cast to a pointer which COM can use.
    protected final Pointer pointer;

    // TODO: Remove this and change classes which cannot cope
    public IUnknown() {
        this(new Pointer(0));
    }

    public IUnknown(int pointerValue) {
        this(new Pointer(pointerValue));
    }
    
    private IUnknown(Pointer pointer) {
        this.pointer = pointer;

        // Register this object so we can release it when it is unreachable
        ROT.addObject(this);
    }

    public boolean isAlive() {
        return pointer.isAlive();
    }

    /**
     * Anything can call this to release COM references to the pointer
     * associated with this IUnknown object.  If you do explicitly call this
     * the ROT will try and perform this implicitly when the Java object is
     * no longer reachable.
     */
    public void safeRelease() {
        pointer.safeRelease();
    }

    // Lots of debugging output
    private static final boolean DEBUG = "true".equalsIgnoreCase(System.getProperty("com.jacob.debug"));

    protected static boolean isDebugEnabled() {
        return DEBUG;
    }

    /**
     * Very basic debugging function.
     */
    protected static void debug(String message) {
        if (isDebugEnabled()) {
            System.out.println(Thread.currentThread().getName() + ": " + message);
        }
    }

    private native int toEnumVariant(int pointer);
    
    public EnumVariant toEnumVariant() {
        return new EnumVariant(toEnumVariant(pointer.get()));
    }

    protected static native void release(int pointer);

    /**
     * force the jacob DLL to be loaded whenever this class is referenced
     */
    static {
        LibraryLoader.loadJacobLibrary();
    }
}