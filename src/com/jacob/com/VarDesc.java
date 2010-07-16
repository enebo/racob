package com.jacob.com;

public class VarDesc {
    private final Variant constant;

    public VarDesc(int varkind, Variant constant) {
        this.constant = constant;
    }

    public Variant getConstant() {
        return constant;
    }
}
