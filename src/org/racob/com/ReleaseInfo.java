package org.racob.com;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

public class ReleaseInfo {
    private static String buildVersion;
    private static String buildDate;
    private static final String PROPERTY_FILE = "META-INF/version.properties";

    private static void loadVersionProperties() {
        // Use local class loader to make sure we have security permissions
        ClassLoader loader = ReleaseInfo.class.getClassLoader();
        Properties info = new Properties();
        InputStream stream = loader.getResourceAsStream(PROPERTY_FILE);

        // Work-around for Websphere
        if (stream == null) stream = loader.getResourceAsStream("/" + PROPERTY_FILE);

        if (stream != null) {
            try {
                info.load(stream);
                stream.close();
                buildVersion = info.getProperty("version");
                buildDate = info.getProperty("build.date");
            } catch (IOException e) {}
        }
        if (buildVersion == null) buildVersion = "unknown";
        if (buildDate == null) buildDate = "unknown";
    }

    public static String getBuildDate() {
        if (buildDate == null) loadVersionProperties();
        return buildDate;
    }

    public static String getBuildVersion() {
        if (buildVersion == null) loadVersionProperties();
        return buildVersion;
    }
}
