// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * State.java
 *
 * Created on December 10, 2002, 10:47 AM
 */

package oliver.scenario;

/**
 *
 * @author  stephen
 */
public class State {
    
    private oliver.application.UserProperties userProperties;
    private org.mozilla.javascript.Scriptable scope;
    private java.util.HashMap images = new java.util.HashMap();
    private StanleyImageFilter sif = new StanleyImageFilter();
    private java.util.HashMap state = new java.util.HashMap();
    
    public State(oliver.application.UserProperties userProperties) {
        this.userProperties = userProperties;
        org.mozilla.javascript.Context cx = org.mozilla.javascript.Context.enter();
        scope = cx.initStandardObjects(null);
        try {
            org.mozilla.javascript.ScriptableObject.defineClass(scope, oliver.scenario.ScriptableHashMap.class);
            org.mozilla.javascript.ScriptableObject.defineClass(scope, oliver.scenario.MyString.class);
            String[] args = new String[0];
            org.mozilla.javascript.Scriptable myHash = cx.newObject(scope, "ScriptableHashMap", args);
            scope.put("state", scope, myHash);
            //livdll.AttributeAssignment a = new livdll.AttributeAssignment("test.key", "bulb_off.gif");
            //livdll.AttributeAssignment[] aa = new livdll.AttributeAssignment[1];
            //aa[0] = a;
            //setAttributeAssignments(aa, cx);
        } catch(Exception e) {
            System.out.println(e);
        }
    }
    
    public synchronized void setAttributeAssignments(livdll.AttributeAssignment[] data) throws Exception {
        org.mozilla.javascript.Context cx = org.mozilla.javascript.Context.enter();
        setAttributeAssignments(data, cx);
        cx.exit();
        cx = null;
    }
    public void setAttributeAssignments(livdll.AttributeAssignment[] data, org.mozilla.javascript.Context cx) throws Exception {
        String[] args = new String[0];
        state = new java.util.HashMap();
        if(data != null && data.length > 0) {
            args = new String[data.length*2];
            for(int i = 0; i < data.length; i++) {
                String key = data[i].getName();
                String value = data[i].getValue();
                args[i*2] = key;
                args[i*2+1] = value;
                state.put(key, value);
            }
        }
        org.mozilla.javascript.Scriptable myHash = cx.newObject(scope, "ScriptableHashMap", args);
        scope.put("state", scope, myHash);
    }
    
    public synchronized javax.swing.ImageIcon getImage(net.n3.nanoxml.IXMLElement componentType, String instanceName) {
        javax.swing.ImageIcon image = null;
        org.mozilla.javascript.Context cx = org.mozilla.javascript.Context.enter();
        java.awt.MediaTracker tracker = new java.awt.MediaTracker(userProperties.getFrame());
        try {
            Object[] args = new Object[1];
            args[0] = instanceName;
            org.mozilla.javascript.Scriptable scriptableInstanceName = cx.newObject(scope, "MyString", args);
            //scriptableInstanceName.setPrototype(scope);
            //scriptableInstanceName.setParentScope(null);
            //scriptableInstanceName.put("instance", scriptableInstanceName, scriptableInstanceName);
            scope.put("instance", scope, scriptableInstanceName);
            String src = "";
            net.n3.nanoxml.IXMLElement n = oliver.schematic.ImageDialog.getGlobalScriptNode(componentType);
            if(Boolean.valueOf(n.getAttribute("useglobalscript",Boolean.toString(true))).booleanValue()) {
                src = n.getAttribute("src", "");            
            }
            src = src + oliver.schematic.ImageDialog.getScriptNode(componentType).getAttribute("src", "default.gif");
            Object result = cx.evaluateString(scope, src, "<cmd>", 1, null);
            if(result.toString().startsWith("org.mozilla")) result = "default.gif";
            image = (javax.swing.ImageIcon)images.get(result);
            if(image == null) {
                String dir = userProperties.getProperty(userProperties.IMAGE_DIRECTORY)+userProperties.SLASH;
                java.io.File file = new java.io.File(dir+result.toString());
                if(file.exists()) {
                    image = new javax.swing.ImageIcon(file.getAbsolutePath());
                    tracker.addImage(image.getImage(),0);
                    images.put(result, image);
                } else {
                    int i1 = result.toString().lastIndexOf("_");
                    int i2 = result.toString().lastIndexOf(".");
                    if(i1 > -1 && i2 > -1) {
                        String whiteimage = result.toString().substring(0, i1)+"_white.gif";
                        String colorname = result.toString().substring(i1+1,i2);
                        java.io.File file2 = new java.io.File(dir+whiteimage);
                        if(file2.exists()) {
                            image = new javax.swing.ImageIcon(file2.getAbsolutePath());
                            sif.setX11Color(colorname);
                            java.awt.image.FilteredImageSource filteredImageSource = new java.awt.image.FilteredImageSource(image.getImage().getSource(), sif);
                            image = new javax.swing.ImageIcon(java.awt.Toolkit.getDefaultToolkit().createImage(filteredImageSource));
                        }
                    }
                    if(image == null) {
                        image = (javax.swing.ImageIcon)images.get("unknown.gif");
                        if(image == null) {
                            java.net.URL url = getClass().getResource("/oliver/scenario/unknown.gif");
                            image = new javax.swing.ImageIcon(url);
                            tracker.addImage(image.getImage(),0);
                            images.put("unknown.gif",  image);
                        }
                    }
                }
            }
        } catch(Exception e) {
            System.out.println(e);
            if(image == null) {
                image = (javax.swing.ImageIcon)images.get("unknown.gif");
                if(image == null) {
                    java.net.URL url = getClass().getResource("/oliver/scenario/unknown.gif");
                    image = new javax.swing.ImageIcon(url);
                    tracker.addImage(image.getImage(),0);
                    images.put("unknown.gif",  image);
                }
            }
        }
        if(cx != null) {
            cx.exit();
            cx = null;
        }
        try {
            tracker.waitForAll();
        } catch(Exception e) {}
        return image;
    }
    
    public String getAttributeAssignment(String key) {
        return (String)state.get(key);
    }
    
    /*
    public static void main(String args[]) throws Exception {
     
        javax.swing.JFrame frame = new javax.swing.JFrame();
        oliver.application.UserProperties userProperties = new oliver.application.UserProperties();
        userProperties.setRootComponent(frame);
     
        net.n3.nanoxml.XMLElement node = new net.n3.nanoxml.XMLElement();
        javax.swing.JDialog d = new oliver.schematic.ImageDialog(frame, node, true);
        d.show();
     
        State state = new State(userProperties);
     
        javax.swing.ImageIcon image = state.getImage(node, "test");
        d = new javax.swing.JDialog(frame, true);
        d.getContentPane().add(new javax.swing.JLabel(image));
        d.setVisible(true);
     
    }
     */
}
