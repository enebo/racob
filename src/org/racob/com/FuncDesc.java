package org.racob.com;

public class FuncDesc {
    public static final int FUNCFLAG_FRESTRICTED = 1;
    public static final int FUNCFLAG_FSOURCE = 0x2;
    public static final int FUNCFLAG_FBINDABLE = 0x4;
    public static final int FUNCFLAG_FREQUESTEDIT = 0x8;
    public static final int FUNCFLAG_FDISPLAYBIND = 0x10;
    public static final int FUNCFLAG_FDEFAULTBIND = 0x20;
    public static final int FUNCFLAG_FHIDDEN = 0x40;
    public static final int FUNCFLAG_FUSESGETLASTERROR = 0x80;
    public static final int FUNCFLAG_FDEFAULTCOLLELEM = 0x100;
    public static final int FUNCFLAG_FUIDEFAULT = 0x200;
    public static final int FUNCFLAG_FNONBROWSABLE = 0x400;
    public static final int FUNCFLAG_FREPLACEABLE = 0x800;
    public static final int FUNCFLAG_FIMMEDIATEBIND = 0x1000;

    private final int memid;
    private final int index;
    private final int invkind;
    private final int flags;
    private final Parameter[] parameters;
    private final Parameter returnType;
    private final int optParamsCount;
    private final int vtableOffset;
    private TypeInfo owner = null;

    public FuncDesc(int memid, int index, int invkind, int flags,
            Parameter[] parameters, Parameter returnType,
            int optParamsCount, int vtableOffset) {
        this.memid = memid;
        this.index = index;
        this.invkind = invkind;
        this.flags = flags;
        this.parameters = parameters;
        this.returnType = returnType;
        this.optParamsCount = optParamsCount;
        this.vtableOffset = vtableOffset;
    }

    public int getFlags() {
        return flags;
    }

    public int getIndex() {
        return index;
    }

    public int getInvkind() {
        return invkind;
    }

    public int getMemid() {
        return memid;
    }

    public int getOptParamsCount() {
        return optParamsCount;
    }

    public TypeInfo getOwner() {
        return owner;
    }

    public void setOwner(TypeInfo owner) {
        this.owner = owner;
    }

    public Parameter[] getParameters() {
        return parameters;
    }

    public int getParamsCount() {
        return parameters.length;
    }

    public Parameter getReturnType() {
        return returnType;
    }

    public int getVtableOffset() {
        return vtableOffset;
    }
}
