package org.racob.com;

public class Parameter {
    private final String name;
    private final boolean in;
    private final boolean out;
    private final boolean optional;
    private final boolean returnValue;
    private final int vt;
    private final boolean hasDefaultValue;
    private final Variant defaultValue;

    public Parameter(String name, boolean in, boolean out, boolean optional,
            boolean returnValue, int vt, boolean hasDefaultValue, Variant defaultValue) {
        this.name = name;
        this.in = in;
        this.out = out;
        this.optional = optional;
        this.returnValue = returnValue;
        this.vt = vt;
        this.hasDefaultValue = hasDefaultValue;
        this.defaultValue = defaultValue;
    }

    public Variant getDefaultValue() {
        return defaultValue;
    }
    
    public String getName() {
        return name;
    }

    public int getVt() {
        return vt;
    }

    public boolean isDefaultValue() {
        return hasDefaultValue;
    }

    public boolean isIn() {
        return in;
    }

    public boolean isOut() {
        return out;
    }

    public boolean isOptional() {
        return optional;
    }

    public boolean isReturnValue() {
        return returnValue;
    }
}
