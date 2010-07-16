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

    public TypeInfo(int m_pType, int typekind, int functionCount,
            int implementationCount, int variableCount)
    {
        this.m_pTypeInfo = m_pType;
        this.typekind = typekind;
        this.funcsCount = functionCount;
        this.implTypesCount = implementationCount;
        this.varsCount = variableCount;
    }

    public int getTypekind() {
        return typekind;
    }

    public int getFuncsCount() {
        return funcsCount;
    }

    public int getImplTypesCount() {
        return implTypesCount;
    }

    public int getVarsCount() {
        return varsCount;
    }

    public native TypeLib getContainingTypeLib();
    public native String[] getNames(int index);
    public native VarDesc getVarDesc(int index);
}
