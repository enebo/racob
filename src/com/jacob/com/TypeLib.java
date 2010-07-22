package com.jacob.com;

public class TypeLib
{
    private final int m_pTypeLib;
    private final String guid;
    private final int count;
    private final int flags;
    private final int majorVersion;
    private final int minorVersion;

    public TypeLib(int pointer, String guid, int count, int flags,
            int majorVersion, int minorVersion)
    {
        this.m_pTypeLib = pointer;
        this.guid = guid;
        this.flags = flags;
        this.count = count;
        this.majorVersion = majorVersion;
        this.minorVersion = majorVersion;
    }

    public TypeInfo[] getTypeInfo() 
    {
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

    public native Documentation getDocumentation(int index);
    public native TypeInfo getTypeInfo(int index);
    public native int getTypeInfoCount();
}
