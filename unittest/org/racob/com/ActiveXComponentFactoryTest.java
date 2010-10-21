package org.racob.com;

import org.racob.activeX.ActiveXComponent;
import org.racob.test.BaseTestCase;

/**
 * This exercises the two Dispatch factor methods that let you control whether
 * you create a new running COM object or connect to an existing one
 * <p>
 * May need to run with some command line options (including from inside
 * Eclipse). Look in the docs area at the Jacob usage document for command line
 * options.
 */
public class ActiveXComponentFactoryTest extends BaseTestCase {
    private static final String appId = "InternetExplorer.Application";

    private void waitABit(String message) {
        try {
            Thread.sleep(10000);
            System.out.println(message);
        } catch (InterruptedException e) {}
    }

    /**
     * This test is supposed to verify we get multiple instances when we mean
     * too. Unfortunately, it requires that the runner of the test verify via
     * the "Windows Task Manager"
     */
    public void testMultipleInstances() {
        // CRASHER Associated with this test
        ComThread.InitMTA();
//        ActiveXComponent instance1 = ActiveXComponent.createNewInstance(appId);
//        ActiveXComponent instance2 = ActiveXComponent.createNewInstance(appId);
//        waitABit("Make sure two instances are running in task manager");

//        instance1.invoke("Quit", new Variant[]{});
//        instance2.invoke("Quit", new Variant[]{});
        ComThread.Release();
    }

    /**
     * This test is supposed to verify we can force multiple items through a
     * single running instance. It requires that a user physically watch the
     * "Windows Task Manager" to verify only one copy of is executing
     */
//    public void testOnlyOneInstance() {
//        ComThread.InitMTA();
//        ActiveXComponent instance1 = new ActiveXComponent(appId);
//        ActiveXComponent instance2 = ActiveXComponent.connectToActiveInstance(appId);
//        assertNotNull(instance2);
//
//        waitABit("Make sure only one instance is running in task manager");
//
//        instance1.invoke("Quit", new Variant[]{});
//        ComThread.Release();
//    }

//    public void testActiveXComponentFactory() {
//        ComThread.InitSTA(true);
//        try {
//            System.out.println("This test only works if " + appId + " is NOT already running");
//            ActiveXComponent connection = ActiveXComponent.connectToActiveInstance(appId);
//            assertNull(connection);
//
//            System.out.println("    " + appId + " Starting");
//            connection = ActiveXComponent.createNewInstance(appId);
//            assertNotNull(connection);
//
//            connection = ActiveXComponent.connectToActiveInstance(appId);
//            assertNotNull(connection);
//            System.out.println("Stopping " + appId);
//            // stop it so we can fail trying to connect to a running
//            connection.invoke("Quit", new Variant[]{});
//            // Give it time to stop
//            Thread.sleep(2000);
//            connection = ActiveXComponent.connectToActiveInstance(appId);
//            assertNull(connection);
//        } catch (InterruptedException ie) {
//        } catch (ComException e) {
//            e.printStackTrace();
//            fail("Caught COM exception");
//        } finally {
//            ComThread.Release();
//            ComThread.quitMainSTA();
//        }
//    }
}
