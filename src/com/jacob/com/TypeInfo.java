package com.jacob.com;

/**
 * Name Note: cVars -> varsCount.  The getCVars looked strange to me so I
 * broke from convention and gave it a more readable name.
 */
public class TypeInfo extends JacobObject
{
    public static final int TYPEKIND_ENUM = 0;
    public static final int TYPEKIND_RECORD = 1;
    public static final int TYPEKIND_MODULE = 2;
    public static final int TYPEKIND_INTERFACE = 3;
    public static final int TYPEKIND_DISPATCH = 4;
    public static final int TYPEKIND_COCLASS = 5;
    public static final int TYPEKIND_ALIAS = 6;
    public static final int TYPEKIND_UNION = 7;
    public static final int TYPEKIND_MAX = 8;

    public final int m_pTypeInfo;
    private final int typekind;
    private final int funcsCount;
    private final int implTypesCount;
    private final int varsCount;
    private final int flags;
    private final int minorVersion;
    private final int majorVersion;

    public TypeInfo(int m_pType, int typekind, int functionCount,
            int implementationCount, int variableCount, int flags,
            int minorVersion, int majorVersion)
    {
        this.m_pTypeInfo = m_pType;
        this.typekind = typekind;
        this.funcsCount = functionCount;
        this.implTypesCount = implementationCount;
        this.varsCount = variableCount;
        this.flags = flags;
        this.minorVersion = minorVersion;
        this.majorVersion = majorVersion;
    }

    public int getTypekind() {
        return typekind;
    }

    public int getFlags() {
        return flags;
    }

    public int getFuncsCount() {
        return funcsCount;
    }

    public int getImplTypesCount() {
        return implTypesCount;
    }

    public int getMinorVersion() {
        return minorVersion;
    }

    public int getMajorVersion() {
        return majorVersion;
    }

    public int getVarsCount() {
        return varsCount;
    }

    public native TypeLib getContainingTypeLib();
    public native Documentation getDocumentation(int index);
    public native FuncDesc getFuncDesc(int index);
    public native String[] getNames(int index);
    public native int getRefTypeOfImplType(int index);
    public native TypeInfo getRefTypeInfo(int index);
    public native VarDesc getVarDesc(int index);
}
