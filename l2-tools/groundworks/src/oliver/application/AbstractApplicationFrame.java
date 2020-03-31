// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * AbstractApplicationFrame.java
 *
 * Created on December 19, 2002, 10:23 AM
 */

package oliver.application;

/**
 *
 * @author  stephen
 */
public abstract class AbstractApplicationFrame extends javax.swing.JFrame {
    
    protected UserProperties userProperties;
    protected String propertiesFileName;
    
    protected SplashWindow sw;
    
    public static int EDITOR_CONTEXT = 1;
    public static int DEBUGGER_CONTEXT = 2;
    
    public AbstractApplicationFrame() throws java.io.IOException {
        showSplash();
        this.propertiesFileName = getApplicationName()+".ini";
        addComponentListener(new java.awt.event.ComponentAdapter() {
            public void componentResized(java.awt.event.ComponentEvent evt) {
                formComponentResized(evt);
            }
        });

        openPropertiesFile();
    }
    
    public AbstractApplicationFrame(UserProperties up) throws java.io.IOException {
        showSplash();
        this.propertiesFileName = getApplicationName()+".ini";
        addComponentListener(new java.awt.event.ComponentAdapter() {
            public void componentResized(java.awt.event.ComponentEvent evt) {
                formComponentResized(evt);
            }
        });

        openPropertiesFile(up);
    }
    
    private void formComponentResized(java.awt.event.ComponentEvent evt) {
        userProperties.setDimension(getApplicationName()+".windowSize", getSize().width, getSize().height);
    }

    protected void showSplash() {
        final java.awt.Image splashIm = getSplashImage();
        Thread thread = new Thread(new Runnable() {
            public void run() {
                java.awt.MediaTracker mt = new java.awt.MediaTracker(AbstractApplicationFrame.this);
                mt.addImage(splashIm,0);
                try {
                    mt.waitForID(0);
                } catch(InterruptedException ie){}
                sw = new SplashWindow(AbstractApplicationFrame.this,splashIm);
                try {
                    Thread.currentThread().sleep(2000);
                } catch(java.lang.InterruptedException ie){}
                sw.dispose();
            }
        });
        if(splashIm != null) thread.start();
    }
    
    public UserProperties getUserProperties() { return userProperties; }
    
    private void openPropertiesFile() throws java.io.IOException {
        UserProperties p = UserProperties.openPropertiesFile(propertiesFileName);
        p.setRootComponent(getContentPane());
        p.setFrame(this);
        p.setOutputStream(new java.io.OutputStream() {
            public void write(int b) {
                AbstractApplicationFrame.this.log(String.valueOf((char)b));
            }
        });
        userProperties = p;
    }
    
    private void openPropertiesFile(UserProperties up) throws java.io.IOException {
        UserProperties p = UserProperties.openPropertiesFile(propertiesFileName);
        p.setRootComponent(getContentPane());
        p.setFrame(this);
        p.setOutputStream(new java.io.OutputStream() {
            public void write(int b) {
                AbstractApplicationFrame.this.log(String.valueOf((char)b));
            }
        });
        java.util.Enumeration enum = up.propertyNames();
        while(enum.hasMoreElements()) {
            String key = (String)enum.nextElement();
            String value = (String)up.get(key);
            p.put(key, value);
        }
        userProperties = p;
    }
    
    protected void exit() {
        userProperties.saveProperties(getApplicationName());
    }
    
    protected abstract void log(String line);
    
    protected abstract String getApplicationName();
    
    protected abstract java.awt.Image getSplashImage();
    
    class SplashWindow extends java.awt.Window {
        java.awt.Image splashIm;
        
        SplashWindow(java.awt.Frame parent, java.awt.Image splashIm) {
            super(parent);
            this.splashIm = splashIm;
            setSize(200,200);
            
            /* Center the window */
            java.awt.Dimension screenDim =
            java.awt.Toolkit.getDefaultToolkit().getScreenSize();
            java.awt.Rectangle winDim = getBounds();
            setLocation((screenDim.width - winDim.width) / 2,
            (screenDim.height - winDim.height) / 2);
            setVisible(true);
        }
        
        public void paint(java.awt.Graphics g) {
            if (splashIm != null) {
                g.drawImage(splashIm,0,0,this);
            }
        }
    }
}
