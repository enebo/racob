package com.jacob.com;

public class TypeLib extends IUnknown {
    public static final int LIBFLAG_FRESTRICTED = 1;
    public static final int LIBFLAG_FCONTROL = 2;
    public static final int LIBFLAG_FHIDDEN = 4;
    public static final int LIBFLAG_FHASDISKIMAGE = 8;

    private final String guid;
    private final int count;
    private final int flags;
    private final int majorVersion;
    private final int minorVersion;

    public TypeLib(int pointer, String guid, int count, int flags,
            int majorVersion, int minorVersion) {
        super(pointer);
        this.guid = guid;
        this.flags = flags;
        this.count = count;
        this.majorVersion = majorVersion;
        this.minorVersion = majorVersion;
    }

    public TypeInfo[] getTypeInfo() {
        TypeInfo[] list = new TypeInfo[count];
        
        for (int i = 0; i < count; i++) {
            list[i] = getTypeInfo(i);
        }

        return list;
    }

    public int getFlags() {
        return flags;
    }

    public String getGuid() {
        return guid;
    }

    public int getMajorVersion() {
        return majorVersion;
    }

    public int getMinorVersion() {
        return minorVersion;
    }

    private native Documentation getDocumentation(int pointer, int index);
    private native TypeInfo getTypeInfo(int pointer, int index);
    private native int getTypeInfoCount(int pointer);

    public Documentation getDocumentation(int index) {
        return getDocumentation(pointer.get(), index);
    }

    public TypeInfo getTypeInfo(int index) {
        return getTypeInfo(pointer.get(), index);
    }

    public int getTypeInfoCount() {
        return getTypeInfoCount(pointer.get());
    }
}
