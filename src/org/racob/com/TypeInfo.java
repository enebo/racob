package org.racob.com;

/**
 * Name Note: cVars -> varsCount.  The getCVars looked strange to me so I
 * broke from convention and gave it a more readable name.
 */
public class TypeInfo extends IUnknown {
    public static final int FUNCFLAG_NONE = 0;
    public static final int FUNCFLAG_FRESTRICTED = 1;
    public static final int FUNCFLAG_FSOURCE = 2;
    public static final int FUNCFLAG_FBINDABLE = 4;
    public static final int FUNCFLAG_FREQUESTEDIT = 8;
    public static final int FUNCFLAG_FDISPLAYBIND = 16;
    public static final int FUNCFLAG_FDEFAULTBIND = 32;
    public static final int FUNCFLAG_FHIDDEN = 64;
    public static final int FUNCFLAG_FUSESGETLASTERROR = 128;
    public static final int FUNCFLAG_FDEFAULTCOLLELEM = 256;
    public static final int FUNCFLAG_FUIDEFAULT = 512;
    public static final int FUNCFLAG_FNONBROWSABLE = 1024;
    public static final int FUNCFLAG_FREPLACEABLE = 2048;
    public static final int FUNCFLAG_FIMMEDIATEBIND = 4096;
    public static final int FUNCFLAG_DEFAULTFILTER = 65;

    public static final int IMPLTYPEFLAG_FDEFAULT = 1;
    public static final int IMPLTYPEFLAG_FSOURCE = 2;
    public static final int IMPLTYPEFLAG_FRESTRICTED = 4;
    public static final int IMPLTYPEFLAG_FDEFAULTVTABLE = 8;
    
    public static final int TYPEKIND_ENUM = 0;
    public static final int TYPEKIND_RECORD = 1;
    public static final int TYPEKIND_MODULE = 2;
    public static final int TYPEKIND_INTERFACE = 3;
    public static final int TYPEKIND_DISPATCH = 4;
    public static final int TYPEKIND_COCLASS = 5;
    public static final int TYPEKIND_ALIAS = 6;
    public static final int TYPEKIND_UNION = 7;
    public static final int TYPEKIND_MAX = 8;
    
    public static final int TYPEFLAG_NONE = 0;
    public static final int TYPEFLAG_FAPPOBJECT = 1;
    public static final int TYPEFLAG_FCANCREATE = 2;
    public static final int TYPEFLAG_FLICENSED = 4;
    public static final int TYPEFLAG_FPREDECLID = 8;
    public static final int TYPEFLAG_FHIDDEN = 16;
    public static final int TYPEFLAG_FCONTROL = 32;
    public static final int TYPEFLAG_FDUAL = 64;
    public static final int TYPEFLAG_FNONEXTENSIBLE = 128;
    public static final int TYPEFLAG_FOLEAUTOMATION = 256;
    public static final int TYPEFLAG_FRESTRICTED = 512;
    public static final int TYPEFLAG_FAGGREGATABLE = 1024;
    public static final int TYPEFLAG_FREPLACEABLE = 2048;
    public static final int TYPEFLAG_FDISPATCHABLE = 4096;
    public static final int TYPEFLAG_FREVERSEBIND = 8192;
    public static final int TYPEFLAG_FPROXY = 16384;
    public static final int TYPEFLAG_DEFAULTFILTER = 528;
    public static final int TYPEFLAG_COCLASSATTRIBUTES = 1599;
    public static final int TYPEFLAG_INTERFACEATTRIBUTES = 31696;
    public static final int TYPEFLAG_DISPATCHATTRIBUTES = 23184;
    public static final int TYPEFLAG_ALIASATTRIBUTES = 528;
    public static final int TYPEFLAG_MODULEATTRIBUTES = 528;
    public static final int TYPEFLAG_ENUMATTRIBUTES = 528;
    public static final int TYPEFLAG_RECORDATTRIBUTES = 528;
    public static final int TYPEFLAG_UNIONATTRIBUTES = 528;

    private final int typekind;
    private final String guid;
    private final String progid;
    private final int funcsCount;
    private final int implTypesCount;
    private final int varsCount;
    private final int flags;
    private final int minorVersion;
    private final int majorVersion;
    private final int aliasVt;

    public TypeInfo(int pointer, String guid, String progid, int typekind,
            int functionCount, int implementationCount, int variableCount,
            int flags, int minorVersion, int majorVersion, int aliasVt) {
        super(pointer);
        this.guid = guid;
        this.progid = progid;
        this.typekind = typekind;
        this.funcsCount = functionCount;
        this.implTypesCount = implementationCount;
        this.varsCount = variableCount;
        this.flags = flags;
        this.minorVersion = minorVersion;
        this.majorVersion = majorVersion;
        this.aliasVt = aliasVt;
    }

    public int getTypekind() {
        return typekind;
    }

    public int getAliasVt() {
        return aliasVt;
    }

    public int getFlags() {
        return flags;
    }

    public int getFuncsCount() {
        return funcsCount;
    }

    public String getGUID() {
        return guid;
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

    public String getProgid() {
        return progid;
    }

    public int getVarsCount() {
        return varsCount;
    }

    private native TypeLib getContainingTypeLib(int pointer);
    private native Documentation getDocumentation(int pointer, int index);
    private native FuncDesc getFuncDesc(int pointer, int index);
    private native int getImplTypeFlags(int pointer, int index);
    private native String[] getNames(int pointer, int index);
    private native int getRefTypeOfImplType(int pointer, int index);
    private native TypeInfo getRefTypeInfo(int pointer, int index);
    private native VarDesc getVarDesc(int pointer, int index);

    public TypeLib getContainingTypeLib() {
        return getContainingTypeLib(pointer.get());
    }
    
    public Documentation getDocumentation(int index) {
        return getDocumentation(pointer.get(), index);
    }

    public FuncDesc getFuncDesc(int index) {
        FuncDesc funcDesc = getFuncDesc(pointer.get(), index);

        funcDesc.setOwner(this); // We set mutable state here to avoid passing
                                 // typeinfo into JNI.
        
        return funcDesc;
    }

    public int getImplTypeFlags(int index) {
        return getImplTypeFlags(pointer.get(), index);
    }

    public String[] getNames(int index) {
        return getNames(pointer.get(), index);
    }

    public int getRefTypeOfImplType(int index) {
        return getRefTypeOfImplType(pointer.get(), index);
    }

    public TypeInfo getRefTypeInfo(int index) {
        return getRefTypeInfo(pointer.get(), index);
    }

    public VarDesc getVarDesc(int index) {
        return getVarDesc(pointer.get(), index);
    }
}
