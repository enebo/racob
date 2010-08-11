package com.jacob.com;

/**
 * The interesting information returned by ITypeInfo.getDocumentation.
 */
public class Documentation
{
    private final String name;
    private final String docString;
    private final String helpFile;
    private final int helpContext;

    public Documentation(String name, String docString, String helpFile,
            int helpContext)
    {
        this.name = name;
        this.docString = docString;
        this.helpFile = helpFile;
        this.helpContext = helpContext;
    }

    public int getHelpContext() {
        return helpContext;
    }

    public String getName()
    {
        return name;
    }

    public String getDocString()
    {
        return docString;
    }

    public String getHelpFile()
    {
        return helpFile;
    }
}
