/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package com.jacob.com;

/**
 *
 * @author Fuebo
 */
public class IUnknown extends JacobObject {

    public IUnknown(int pointer) {
        this.pointer = pointer;
    }

    private native int toEnumVariant(int pointer);
    
    public EnumVariant toEnumVariant() {
        return new EnumVariant(toEnumVariant(pointer));
    }

    @Override
    protected native void release(int pointer);
}
