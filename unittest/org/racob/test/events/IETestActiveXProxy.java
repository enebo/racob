package org.racob.test.events;

import org.racob.activeX.ActiveXComponent;
import org.racob.activeX.ActiveXDispatchEvents;
import org.racob.com.ComThread;
import org.racob.com.Dispatch;
import org.racob.com.Variant;
import org.racob.test.BaseTestCase;

/**
 * This demonstrates the new event handling code in racob 1.7 This example will
 * open up IE and print out some of the events it listens to as it havigates to
 * web sites. contributed by Niels Olof Bouvin mailto:n.o.bouvin@daimi.au.dk and
 * Henning Jae jehoej@daimi.au.dk
 * <p>
 * May need to run with some command line options (including from inside
 * Eclipse). Look in the docs area at the Racob usage document for command line
 * options.
 */
public class IETestActiveXProxy extends BaseTestCase {
    public void testIEActiveProxyCallback() {
        // this line starts the pump but it runs fine without it
        ComThread.startMainSTA();
        // remove this line and it dies
        // /ComThread.InitMTA(true);
        IETestActiveProxyThread aThread = new IETestActiveProxyThread();
        aThread.start();
        while (aThread.isAlive()) {
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                // doen with the sleep
                // e.printStackTrace();
            }
        }
        System.out.println("Main: Thread quit, about to quit main sta in thread "
                + Thread.currentThread().getName());
        // this line only does someting if startMainSTA() was called
        ComThread.quitMainSTA();
        System.out.println("Main: did quit main sta in thread "
                + Thread.currentThread().getName());
        if (aThread.threadFailedWithException != null) {
            fail("caught an unexpected exception "
                    + aThread.threadFailedWithException);
        }
    }
}

class IETestActiveProxyThread extends Thread {

    /** says that the quit message has been received from the target application */
    public static boolean quitHandled = false;
    private static final String WEBSITE = "http://www.github.com/enebo/racob";
    /**
     * holds any caught exception so the main/test case can see them
     */
    public Throwable threadFailedWithException = null;

    /** the thread's constructor */
    public IETestActiveProxyThread() {
        super();
    }

    @Override
    public void run() {
        // this used to be 5 seconds but sourceforge is slow
        int delay = 5000; // msec
        // paired with statement below that blows up
        ComThread.InitMTA();
        ActiveXComponent ie = new ActiveXComponent("InternetExplorer.Application");
        try {
            ie.put("Visible", true);
            ie.put("AddressBar", true);
            System.out.println("IETestActiveProxyThread: " + ie.get("Path"));
            ie.put("StatusText", "My Status Text");

            System.out.println("IETestActiveProxyThread: About to hookup event listener");
            IEEventsActiveProxy ieE = new IEEventsActiveProxy();
            new ActiveXDispatchEvents(ie, ieE, "InternetExplorer.Application.1");
            System.out.println("IETestActiveProxyThread: Did hookup event listener");

            System.out.println("IETestActiveProxyThread: About to call navigate to sourceforge");
            ie.call("Navigate", WEBSITE);
            System.out.println("IETestActiveProxyThread: Did call navigate to sourceforge");
            try {
                Thread.sleep(delay);
            } catch (Exception e) {
            }
            System.out.println("IETestActiveProxyThread: About to call navigate to yahoo");
            ie.call("Navigate", WEBSITE);
            System.out.println("IETestActiveProxyThread: Did call navigate to yahoo");
            try {
                Thread.sleep(delay);
            } catch (Exception e) {
            }
        } catch (Exception e) {
            threadFailedWithException = e;
            e.printStackTrace();
        } catch (Throwable re) {
            threadFailedWithException = re;
            re.printStackTrace();
        } finally {
            System.out.println("IETestActiveProxyThread: About to send Quit");
            ie.invoke("Quit");
            System.out.println("IETestActiveProxyThread: Did send Quit");
        }
        // this blows up when it tries to release a DispatchEvents object
        // I think this is because there is still one event we should get back
        // "OnQuit" that will came after we have released the thread pool
        // this is probably messed up because DispatchEvent object will have
        // been
        // freed before the callback
        // commenting out ie.invoke(quit...) causes this to work without error
        // this code tries to wait until the quit has been handled but that
        // doesn't work
        System.out.println("IETestActiveProxyThread: Waiting until we've received the quit callback");
        while (!quitHandled) {
            try {
                Thread.sleep(delay / 5);
            } catch (InterruptedException e) {
            }
        }
        System.out.println("IETestActiveProxyThread: Received the quit callback");
        // wait a little while for it to end
        // try {Thread.sleep(delay); } catch (InterruptedException e) {}
        System.out.println("IETestActiveProxyThread: about to call ComThread.Release in thread "
                + Thread.currentThread().getName());

        ComThread.Release();
    }

    /**
     * The events class must be publicly accessable for reflection to work. The
     * list of available events is located at
     * http://msdn2.microsoft.com/en-us/library/aa768280.aspx
     */
    public class IEEventsActiveProxy {
        private void received(String eventName, Object extraValue) {
            System.out.println("IEEventsActiveProxy Received (" +
                    Thread.currentThread().getName() + "): " +
                    eventName+" " + (extraValue != null ? extraValue : ""));
        }

        public void BeforeNavigate2(Dispatch pDisp, String url, Integer flags,
                String targetFrameName, Variant postData, String headers,
                Boolean cancel) {
            received("BeforeNavigate2", url);
        }

        public void CommandStateChange(Integer command, Boolean enable) {
            received("CommandStateChange", command);
        }

        public void DocumentComplete(Dispatch pDisp, String url) {
            received("DocumentComplete", url);
        }

        public void DownloadBegin() { received("DownloadBegin", null); }
        public void DownloadComplete() { received("DownloadComplete", null); }

        public void NavigateComplete2(Dispatch pDisp, String url) {
            received("NavigateComplete2", url);
        }

        public void NavigateError(Dispatch pDispatch, String url,
                String targetFrameName, Integer statusCode, Boolean Cancel) {
            received("NavigateError", statusCode);
        }

        public void NewWindow2(Dispatch pDisp, Boolean cancel) {
            received("NewWindow2", pDisp);
        }

        public void OnFullScreen(Boolean fullScreen) {
            received("OnFullScreen", fullScreen);
        }

        public void OnMenuBar(Boolean menuBar) { received("OnMenuBlur", menuBar); }

        public void OnQuit() {
            received("OnQuit", null);
            IETestActiveProxyThread.quitHandled = true;
        }

        public void OnStatusBar(Boolean statusBar) { received("OnStatusBar", statusBar); }
        public void OnTheaterMode(Boolean theaterMode) { received("OnTheaterMode", theaterMode); }
        public void OnToolBar(Boolean onToolBar) { received("OnToolBar", onToolBar); }
        public void OnVisible(Boolean onVisible) { received("OnVisible", onVisible); }
        public void ProgressChange() { received("ProgressChange", null); }
        public void PropertyChange() { received("PropertyChange", null); }

        public void SetSecureLockIcon(Integer secureLockIcon) {
            received("SetSecureLockIcon", secureLockIcon);
        }

        public void StatusTextChange() { received("StatusTextChange", null); }
        public void TitleChange() { received("TitleChange", null); }

        public void WindowClosing(Boolean isChildWindow) {
            received("WindowClosing", isChildWindow);
        }
    }
}