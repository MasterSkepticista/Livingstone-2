// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * LivingstoneAdapter.java
 *
 * Created on November 14, 2002, 1:50 PM
 */

package livdll;

/**
 *
 * @author  stephen
 */
public class LivingstoneAdapter implements EngineControlInterface {
    
    private static String modelName = "test";
    private java.io.File dir;
    private LivingstoneEngine engine = null;
    private String modelFileName = null;
    private java.io.File xmpl;
    private java.io.File hrn;
    private java.io.File ini;
    private java.io.File params;
    
    private int search_tracker_type;
    
    /** Utility field used by event firing mechanism. */
    private javax.swing.event.EventListenerList listenerList =  null;
    
    /** Creates a new instance of LivingstoneAdapter */
    public LivingstoneAdapter() {
        stopEngine();
    }
    
    public void startEngine() throws java.io.IOException, L2NativeException, L2JavaException {
        if(engine != null) engine.deallocateEngine();
        engine = new LivingstoneEngine();
        engine.setType(search_tracker_type);
        try {
            engine.setModelFilename(dir.getAbsolutePath()+System.getProperty("file.separator")+modelName);
            java.util.SortedSet attributes = engine.getAttributeNames();
            fireEngineListenerEchoLoadModel(this, modelFileName, attributes);
        } catch(L2NativeException e) {
            engine.deallocateEngine();
            engine = null;
            throw e;
        }
    }
    
    public void stopEngine() {
        if(engine != null) {
            engine.deallocateEngine();
            engine = null;
        }
        String fullname = System.getProperty("user.home")+System.getProperty("file.separator")+"oliver";
        dir = new java.io.File(fullname);
        if(!dir.exists()) dir.mkdir();
        this.xmpl = new java.io.File(dir.getAbsolutePath()+System.getProperty("file.separator")+modelName+".xmpl");
        if(xmpl.exists()) xmpl.delete();
        this.hrn = new java.io.File(dir.getAbsolutePath()+System.getProperty("file.separator")+modelName+".hrn");
        if(hrn.exists()) hrn.delete();
        this.ini = new java.io.File(dir.getAbsolutePath()+System.getProperty("file.separator")+modelName+".ini");
        if(ini.exists()) ini.delete();
        this.params = new java.io.File(dir.getAbsolutePath()+System.getProperty("file.separator")+modelName+".params");
        if(params.exists()) params.delete();
    }
    
    // returns the name of the root component type
    public String loadModel(java.io.File file) throws Exception {
        net.n3.nanoxml.StdXMLBuilder builder = new net.n3.nanoxml.StdXMLBuilder();
        net.n3.nanoxml.IXMLParser parser = net.n3.nanoxml.XMLParserFactory.createDefaultXMLParser(builder);
        net.n3.nanoxml.IXMLReader reader = new net.n3.nanoxml.StdXMLReader(new java.io.FileReader(file));
        parser.setReader(reader);
        net.n3.nanoxml.IXMLElement xml = (net.n3.nanoxml.IXMLElement) parser.parse();
        
        net.n3.nanoxml.IXMLElement child = xml.getFirstChildNamed("componentinstance");
        if(child != null) writeXmpToFile(child, xmpl);
        String type = child.getAttribute("type", null);
        
        child = xml.getFirstChildNamed("mplHarness");
        if(child != null) writeXmpToFile(child, hrn);
        
        child = xml.getFirstChildNamed("mplInit");
        if(child != null) writeXmpToFile(child, ini);
        
        return type;
    }
    
    private void writeXmpToFile(net.n3.nanoxml.IXMLElement node, java.io.File file) throws java.io.IOException {
        if(file.exists()) file.delete();
        java.io.FileOutputStream out = new java.io.FileOutputStream(file);
        net.n3.nanoxml.XMLWriter writer = new net.n3.nanoxml.XMLWriter(out);
        net.n3.nanoxml.XMLElement xml = new net.n3.nanoxml.XMLElement();
        xml.setName("xmpl");
        if(node.getName().equals("componentinstance"))
            xml.setAttribute("xmlns:ci","http://www.w3.org/1999/XSL/Format");
        xml.addChild(node);
        writer.write(xml,true);
        out.flush();
        out.close();
    }
    
    public void loadParameters(java.io.File file) throws L2JavaException, java.io.IOException {
        readInputStreamIntoFile(new java.io.FileInputStream(file), params);
        java.io.BufferedReader r = new java.io.BufferedReader(new java.io.FileReader(params));
        try {
            String line = r.readLine();
            while(line != null) {
                int i = line.indexOf("L2SearchMethod");
                if(i > -1) {
                    i = line.indexOf("cbfs");
                    if(i > -1) search_tracker_type = livdll.LivingstoneEngine.CBFS;
                    i = line.indexOf("cover");
                    if(i > -1) search_tracker_type = livdll.LivingstoneEngine.COVER;
                }
                line = r.readLine();
            }
        } catch(java.io.IOException e) {}
        r.close();
    }
    
    public void readInputStreamIntoFile(java.io.InputStream in, java.io.File file) throws L2JavaException {
        if(engine != null) throw new L2JavaException("LivingstoneAdapter stop engine before loading new model.");
        java.io.FileOutputStream fos = null;
        try {
            fos = new java.io.FileOutputStream(file);
            while(true) {
                int b = in.read();
                if(b == -1) break;
                fos.write(b);
            }
            fos.flush();
            fos.close();
        } catch(java.io.EOFException e) {
            try {
                fos.flush();
                fos.close();
            } catch(java.io.IOException ee) {
                throw new L2JavaException(ee.getMessage());
            }
        } catch(java.io.IOException e) {
            throw new L2JavaException(e.getMessage());
        }
    }
    
    public static void main(String[] args) throws java.io.IOException {
        LivingstoneAdapter a = new LivingstoneAdapter();
        try {
            a.startEngine();
        } catch(L2Exception e) {
            System.out.println(e);
        }
    }
    
    public FindCandidates getDiagnosis() throws L2JavaException, L2NativeException {
        if(engine == null) throw new L2JavaException("LivingstoneAdapter.assign() engine not running.");
        return engine.getDiagnosis();
    }
    
    public synchronized boolean parseCmd(livdll.LivingstoneCommand command) throws L2JavaException {
        if(engine == null) throw new L2JavaException("LivingstoneAdapter.parseCmd() engine not running.");
        boolean r = false;
        try {
            if(command.getType()==command.FIND_CANDIDATES) {
                fireEngineListenerEchoFindCandidates(this);
                r = engine.parseCmd(command);
                FindCandidates fc = getDiagnosis();
                if(r && fc!=null) fireEngineListenerReplyFoundCandidates(this, fc);
            } else if(command.getType()==command.ASSIGNMENT) {
                r = engine.parseCmd(command);
                fireEngineListenerEchoMonitor(this, command);
            } else if(command.getType()==command.PROGRESS) {
                r = engine.parseCmd(command);
                if(r) fireEngineListenerEchoCommand(this, command);
            } else if(command.getType()==command.PROPAGATE) {
                r = engine.parseCmd(command);
                FindCandidates fc = getDiagnosis();
                if(r && fc!=null) fireEngineListenerEchoPropagate(this, r, fc);
            } else if(command.getType()==command.UNKNOWN) {
                r = engine.parseCmd(command);
            } else if(command.getType()==command.RESTART) {
                //r = engine.parseCmd(command);
                if(engine != null) {
                    engine.deallocateEngine();
                    engine = null;
                }
                try {
                    startEngine();
                } catch(java.io.IOException e) {
                    System.out.println(e);
                }
                String type = "";
                if(search_tracker_type == livdll.LivingstoneEngine.CBFS) type = "cbfs";
                else if(search_tracker_type == livdll.LivingstoneEngine.COVER) type = "cover";
                fireEngineListenerEchoNewEngine(this, type);
            }
            return r;
        } catch(L2NativeException e) {
            stopEngine();
            throw new L2JavaException(e.getMessage());
        }
    }
    
    /** Registers EngineListener to receive events.
     * @param listener The listener to register.
     *
     */
    public synchronized void addEngineListener(livdll.EngineListener listener) {
        if (listenerList == null ) {
            listenerList = new javax.swing.event.EventListenerList();
        }
        listenerList.add(livdll.EngineListener.class, listener);
    }
    
    /** Removes EngineListener from the list of listeners.
     * @param listener The listener to remove.
     *
     */
    public synchronized void removeEngineListener(livdll.EngineListener listener) {
        listenerList.remove(livdll.EngineListener.class, listener);
    }
    
    public synchronized void removeAllEngineListeners() {
        listenerList = new javax.swing.event.EventListenerList();
    }
    
    /** Notifies all registered listeners about the event.
     *
     * @param event The event to be fired
     *
     */
    private void fireEngineListenerEchoNewEngine(Object source, String search_method) {
        if (listenerList == null) return;
        Object[] listeners = listenerList.getListenerList();
        for (int i = listeners.length-2; i>=0; i-=2) {
            if (listeners[i]==livdll.EngineListener.class) {
                ((livdll.EngineListener)listeners[i+1]).echoNewEngine(source, search_method);
            }
        }
    }
    
    /** Notifies all registered listeners about the event.
     *
     * @param event The event to be fired
     *
     */
    private void fireEngineListenerEchoDestroyEngine(Object source) {
        if (listenerList == null) return;
        Object[] listeners = listenerList.getListenerList();
        for (int i = listeners.length-2; i>=0; i-=2) {
            if (listeners[i]==livdll.EngineListener.class) {
                ((livdll.EngineListener)listeners[i+1]).echoDestroyEngine(source);
            }
        }
    }
    
    /** Notifies all registered listeners about the event.
     *
     * @param event The event to be fired
     *
     */
    private void fireEngineListenerEchoLoadModel(Object source, String fileName, java.util.SortedSet attributes) {
        if (listenerList == null) return;
        Object[] listeners = listenerList.getListenerList();
        for (int i = listeners.length-2; i>=0; i-=2) {
            if (listeners[i]==livdll.EngineListener.class) {
                ((livdll.EngineListener)listeners[i+1]).echoLoadModel(source, fileName, attributes);
            }
        }
    }
    
    /** Notifies all registered listeners about the event.
     *
     * @param event The event to be fired
     *
     */
    private void fireEngineListenerEchoCommand(Object source, LivingstoneCommand command) {
        if (listenerList == null) return;
        Object[] listeners = listenerList.getListenerList();
        for (int i = listeners.length-2; i>=0; i-=2) {
            if (listeners[i]==livdll.EngineListener.class) {
                ((livdll.EngineListener)listeners[i+1]).echoCommand(source, command);
            }
        }
    }
    
    /** Notifies all registered listeners about the event.
     *
     * @param event The event to be fired
     *
     */
    private void fireEngineListenerEchoMonitor(Object source, LivingstoneCommand command) {
        if (listenerList == null) return;
        Object[] listeners = listenerList.getListenerList();
        for (int i = listeners.length-2; i>=0; i-=2) {
            if (listeners[i]==livdll.EngineListener.class) {
                ((livdll.EngineListener)listeners[i+1]).echoMonitor(source, command);
            }
        }
    }
    
    /** Notifies all registered listeners about the event.
     *
     * @param event The event to be fired
     *
     */
    private void fireEngineListenerEchoPropagate(Object source, boolean consistent, FindCandidates fc) {
        if (listenerList == null) return;
        Object[] listeners = listenerList.getListenerList();
        for (int i = listeners.length-2; i>=0; i-=2) {
            if (listeners[i]==livdll.EngineListener.class) {
                ((livdll.EngineListener)listeners[i+1]).echoPropagate(source, consistent, fc);
            }
        }
    }
    
    /** Notifies all registered listeners about the event.
     *
     * @param event The event to be fired
     *
     */
    private void fireEngineListenerEchoFindCandidates(Object source) {
        if (listenerList == null) return;
        Object[] listeners = listenerList.getListenerList();
        for (int i = listeners.length-2; i>=0; i-=2) {
            if (listeners[i]==livdll.EngineListener.class) {
                ((livdll.EngineListener)listeners[i+1]).echoFindCandidates(source);
            }
        }
    }
    
    /** Notifies all registered listeners about the event.
     *
     * @param event The event to be fired
     *
     */
    private void fireEngineListenerReplyFoundCandidates(Object source, livdll.FindCandidates fc) {
        if (listenerList == null) return;
        Object[] listeners = listenerList.getListenerList();
        for (int i = listeners.length-2; i>=0; i-=2) {
            if (listeners[i]==livdll.EngineListener.class) {
                ((livdll.EngineListener)listeners[i+1]).replyFoundCandidates(source, fc);
            }
        }
    }
    
}
