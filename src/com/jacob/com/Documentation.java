package com.jacob.com;

/**
 * The interesting information returned by ITypeInfo.getDocumentation.
 */
public class Documentation
{
    private final String name;
    private final String docString;
    private final String helpFile;

    public Documentation(String name, String docString, String helpFile)
    {
        this.name = name;
        this.docString = docString;
        this.helpFile = helpFile;
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
