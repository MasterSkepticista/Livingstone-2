// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * RelConnection.java
 *
 * Created on October 8, 2002, 5:58 PM
 */

package oliver.schematic;

/**
 *
 * @author  stephen
 */
public class RelConnection extends ConnectionFigure {
    
    private net.n3.nanoxml.IXMLElement line;
    
    /** Holds value of property endAttribute. */
    private String endAttribute;    
    
    /** Holds value of property startAttribute. */
    private String startAttribute;    
    
    /** Creates a new instance of RelConnection */
    protected RelConnection(oliver.xmpl.Rel rel, net.n3.nanoxml.IXMLElement line) {
        super(rel);
        this.line = line;
        setStartDecoration(null);
        setEndDecoration(null);
        startAttribute = line.getAttribute("startAttribute",null);
        endAttribute = line.getAttribute("endAttribute",null);
        /*  This is done by setLine() after the ends have been connected in the ComponentDrawing
        java.util.Enumeration enum = line.getChildrenNamed("gr:point").elements();
        while(enum.hasMoreElements()) {
            Object obj = enum.nextElement();
            if(obj instanceof net.n3.nanoxml.IXMLElement) {
                net.n3.nanoxml.IXMLElement xml = (net.n3.nanoxml.IXMLElement)obj;
                int x = Integer.parseInt(xml.getAttribute("x", "100"));
                int y = Integer.parseInt(xml.getAttribute("y", "100"));
                addPoint(x, y);
            }
        }*/
    }
        
    public void setLine() {
        super.setLine(line);
    }
    
    /*
        <ex:rel name="tk02.pneumaticsLineOut.getPressure" attributes="mpre102p.sensedPressure">
          <gr:line startAttribute="tk02.pneumaticsLineOut.getPressure" endAttribute="mpre102p.sensedPressure">
            <gr:point x="80" y="320"/>
            <gr:point x="55" y="300"/>
            <gr:point x="165" y="300"/>
            <gr:point x="165" y="160"/>
            <gr:point x="150" y="155"/>
          </gr:line>
        </ex:rel>
     */
    
    public static java.util.Enumeration createRelConnections(oliver.xmpl.Rel rel) {
        // make a line for each arguments from the rel to the var
        String from = rel.getAttribute("name",null);
        // look for gr:lines if there are any
        java.util.HashMap hash = new java.util.HashMap();
        java.util.Enumeration enum = rel.getChildrenNamed("gr:line").elements();
        while(enum.hasMoreElements()) {
            Object obj = enum.nextElement();
            if(obj instanceof net.n3.nanoxml.IXMLElement) {
                net.n3.nanoxml.IXMLElement xml = (net.n3.nanoxml.IXMLElement)obj;
                String endAttributeName = xml.getAttribute("endAttribute", null);
                if(endAttributeName != null) hash.put(endAttributeName, xml);
            }
        }
        
        java.util.Vector v = new java.util.Vector();
        String args = rel.getAttribute("arguments","");
        java.util.StringTokenizer tok = new java.util.StringTokenizer(args, " ");
        while(tok.hasMoreElements()) {
            String arg = tok.nextToken();
            net.n3.nanoxml.IXMLElement xml = (net.n3.nanoxml.IXMLElement)hash.get(arg);
            if(xml == null) {
            xml = new net.n3.nanoxml.XMLElement();
            
                xml.setName("gr:line");
                xml.setAttribute("startAttribute", from);
                xml.setAttribute("endAttribute", arg);
                rel.addChild(xml);
            }
            v.addElement(new RelConnection(rel, xml));
        }
        //System.out.println("RelConnection.createRelConnections() returning "+v.size()+" connections");
        return v.elements();
    }
            
    /** Getter for property endAttribute.
     * @return Value of property endAttribute.
     *
     */
    public String getEndAttribute() {
        return this.endAttribute;
    }    
            
    /** Setter for property endAttribute.
     * @param endAttribute New value of property endAttribute.
     *
     */
    public void setEndAttribute(String endAttribute) {
        this.endAttribute = endAttribute;
    }
    
    /** Getter for property startAttribute.
     * @return Value of property startAttribute.
     *
     */
    public String getStartAttribute() {
        return this.startAttribute;
    }
    
    /** Setter for property startAttribute.
     * @param startAttribute New value of property startAttribute.
     *
     */
    public void setStartAttribute(String startAttribute) {
        this.startAttribute = startAttribute;
    }
    
}
