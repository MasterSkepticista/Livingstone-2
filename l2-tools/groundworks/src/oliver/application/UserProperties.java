// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * UserProperties.java
 *
 * Created on September 23, 2002, 2:11 PM
 */

package oliver.application;

/**
 *
 * @author  stephen
 */
public class UserProperties extends java.util.Properties {
    
    public static final String OLIVER_HOME = "oliver";
    public static final String IMAGE_DIRECTORY = "images";
    public static final String CURRENT_DIRECTORY = "currentDirectory";
    public static final String CURRENT_MODEL_FILE = "currentModelFile";
    public static final String SLASH;
    
    public String filename;
    
    private java.util.Properties def = new java.util.Properties();
    
    /** Holds value of property rootComponent. */
    private java.awt.Component rootComponent;
    
    /** Holds value of property outputStream. */
    private java.io.OutputStream outputStream;
    
    /** Holds value of property frame. */
    private javax.swing.JFrame frame;
    
    static {
        SLASH = System.getProperty("file.separator");
    }
    
    /** Creates a new instance of UserProperties */
    protected UserProperties() {        
        String home = System.getProperty("user.home")+SLASH+OLIVER_HOME;
        def.setProperty(OLIVER_HOME, home);
        def.setProperty(IMAGE_DIRECTORY, home+SLASH+IMAGE_DIRECTORY);
    }
    
    public static UserProperties openPropertiesFile(String name) throws java.io.IOException {
        UserProperties u = new UserProperties();
        java.io.File home = new java.io.File(u.getProperty(u.OLIVER_HOME));
        if(!home.exists()) {
            home.mkdir();
        }
        java.io.File file = new java.io.File(home.getAbsolutePath()+u.SLASH+name);
        java.io.FileInputStream in = null;
        try {
            in = new java.io.FileInputStream(file);
            u.load(in);
        } catch(java.io.FileNotFoundException e) {
        }
        u.setFilename(name);
        return u;
    }
        
    public void saveProperties(String applicationName) {
        try {
            java.io.FileOutputStream out = new java.io.FileOutputStream(getProperty(OLIVER_HOME)+SLASH+filename);
            store(out, applicationName+" saved properties file");
            out.flush();
            out.close();
        } catch(Exception e) {
            System.out.println(e);
        }
    }

    public void setFilename(String filename) { this.filename = filename; }
    
    public String getFilename() { return filename; }

    public String getProperty(String key) {
        String p = super.getProperty(key);
        if(p == null) {
            p = def.getProperty(key);
            if(p != null) setProperty(key, p);
        }
        return p;
    }
    
    public java.awt.Dimension getDimension(String name, int defaultWidth, int defaultHeight) {
        String size = getProperty(name);
        if(size != null) {
            try {
                java.util.StringTokenizer tok = new java.util.StringTokenizer(size,":");
                defaultWidth = Integer.parseInt(tok.nextToken());
                defaultHeight = Integer.parseInt(tok.nextToken());
            } catch(Exception e) {
                System.out.println(e);
            }
        }
        return new java.awt.Dimension(defaultWidth,defaultHeight);
    }

    public void setDimension(String name, int width, int height) {
        setProperty(name, width+":"+height);
    }
    
    public int getInt(String name, int defaultValue) {
        int n = defaultValue;
        String p = getProperty(name);
        if(p == null) return n;
        else {
            try {
                n = Integer.valueOf(p).intValue();
            } catch(NumberFormatException e) {}
        }
        return n;
    }
    
    public void setInt(String name, int n) {
        setProperty(name, String.valueOf(n));
        
    }
    
    /** Getter for property rootComponent.
     * @return Value of property rootComponent.
     *
     */
    public java.awt.Component getRootComponent() {
        return this.rootComponent;
    }
    
    /** Setter for property rootComponent.
     * @param rootComponent New value of property rootComponent.
     *
     */
    public void setRootComponent(java.awt.Component rootComponent) {
        this.rootComponent = rootComponent;
    }
    
    /** Getter for property outputStream.
     * @return Value of property outputStream.
     *
     */
    public java.io.OutputStream getOutputStream() {
        return this.outputStream;
    }
    
    /** Setter for property outputStream.
     * @param outputStream New value of property outputStream.
     *
     */
    public void setOutputStream(java.io.OutputStream outputStream) {
        this.outputStream = outputStream;
    }
    
    public java.io.PrintStream getPrintStream() {
        return new java.io.PrintStream(outputStream);
    }
    
    /** Getter for property frame.
     * @return Value of property frame.
     *
     */
    public javax.swing.JFrame getFrame() {
        return this.frame;
    }
    
    /** Setter for property frame.
     * @param frame New value of property frame.
     *
     */
    public void setFrame(javax.swing.JFrame frame) {
        this.frame = frame;
    }
    
}
