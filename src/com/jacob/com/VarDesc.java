package com.jacob.com;

public class VarDesc {
    private final int varkind;
    private final Variant constant;
    private final int memid;

    public VarDesc(int varkind, Variant constant, int memid) {
        this.varkind = varkind;
        this.constant = constant;
        this.memid = memid;
    }

    public Variant getConstant() {
        return constant;
    }
    
    public int getVarkind() {
        return varkind;
    }

    public int getMemid() {
        return memid;
    }
}
