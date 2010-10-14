package com.jacob.com;

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
    private final int invkind;
    private final int flags;
    private final Parameter[] parameters;
    private final int optParamsCount;

    public FuncDesc(int memid, int invkind, int flags, Parameter[] parameters,
            int optParamsCount) {
        this.memid = memid;
        this.invkind = invkind;
        this.flags = flags;
        this.parameters = parameters;
        this.optParamsCount = optParamsCount;
    }

    public int getFlags() {
        return flags;
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

    public Parameter[] getParameters() {
        return parameters;
    }

    public int getParamsCount() {
        return parameters.length;
    }
}
