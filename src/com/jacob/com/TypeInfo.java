package com.jacob.com;

/**
 *
 */
public class TypeInfo extends JacobObject {
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
    private final int functionCount;
    private final int implementationCount;
    private final int variableCount;

    public TypeInfo(int m_pType, int typekind, int functionCount,
            int implementationCount, int variableCount)
    {
        this.m_pTypeInfo = m_pType;
        this.typekind = typekind;
        this.functionCount = functionCount;
        this.implementationCount = implementationCount;
        this.variableCount = variableCount;
    }

    /**
     * @return the typekind
     */
    public int getTypekind() {
        return typekind;
    }

    /**
     * @return the functionCount
     */
    public int getFunctionCount() {
        return functionCount;
    }

    /**
     * @return the implementationCount
     */
    public int getImplementationCount() {
        return implementationCount;
    }

    /**
     * @return the variableCount
     */
    public int getVariableCount() {
        return variableCount;
    }

    public native TypeLib getContainingTypeLib();
    public native VarDesc getVarDesc(int index);
}
