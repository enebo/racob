package com.jacob.com;

public class VarDesc
{
    private final int varkind;
    private final Variant constant;
    private final int memid;

    public VarDesc(int memid, Variant constant, int varkind)
    {
        this.constant = constant;
        this.memid = memid;
        this.varkind = varkind;
    }

    public Variant getConstant()
    {
        return constant;
    }
    
    public int getVarkind()
    {
        return varkind;
    }

    public int getMemid()
    {
        return memid;
    }
}
