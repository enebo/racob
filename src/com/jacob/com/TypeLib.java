package com.jacob.com;

public class TypeLib
{
    private final int m_pTypeLib;
    private int count;

    public TypeLib(int pointer, int count)
    {
        this.m_pTypeLib = pointer;
        System.out.println("POINTER: " + pointer);
        this.count = count;
    }

    public TypeInfo[] getTypeInfo() 
    {
        TypeInfo[] list = new TypeInfo[count];
        
        for (int i = 0; i < count; i++) {
            list[i] = getTypeInfo(i);
        }

        return list;
    }

    public native TypeInfo getTypeInfo(int index);
}
