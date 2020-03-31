// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * AttributeInstance.java
 *
 * Created on July 8, 2002, 10:25 AM
 */

package oliver.tree;

import oliver.xmpl.*;

/**
 *
 * @author  stephen
 */
public class AttributeInstance extends Instance {
    
    private String initialValue = "######";
    private java.util.Vector history = new java.util.Vector();
    private boolean isCommandable = false;
    
    /** Creates a new instance of AttributeInstance */
    public AttributeInstance(oliver.project.ProjectModel model, Xmpl parent, Attribute att) throws Exception {
        super(model);
        this.att = att;
        String name = att.getAttribute("type", null);
        Object obj = parent.findType(name);
        if(obj != null && obj instanceof AttributeType) {
            this.type = (AttributeType)obj;
        } else {
            throw new Exception("AttributeInstance.constructor type not AttributeType");
        }
        setUserObject(att.getAttribute("name",""));
    }
    
    public String getHarnessValue() {
        if(isInStateVector()) return null;
        else return super.getHarnessValue();
    }
    
    public boolean isInStateVector() {
        Object obj = getParent();
        if(obj == null) return false;
        oliver.xmpl.Xmpl parenttype = ((Instance)obj).getType();
        net.n3.nanoxml.IXMLElement ct = parenttype.getFirstChildNamed("ct:statevector");
        if(ct == null) return false;
        String vars = ct.getAttribute("vars", null);
        if(vars == null || vars.length() == 0) return false;
        java.util.StringTokenizer tok = new java.util.StringTokenizer(vars, " ");
        while(tok.hasMoreElements()) {
            String s = tok.nextToken();
            if(s.equals(att.getInstanceName())) return true;
        }
        return false;
    }
    
    public String getValueAtTime(int time) {
        String value = null;
        try {
            value = (String)history.elementAt(time);
        } catch(java.lang.ArrayIndexOutOfBoundsException e) {}
        return value;
    }
    
    public void updateChildChanged(oliver.project.ProjectModelUndoEvent event) {}
    
}
