// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * TextFigure.java
 *
 * Created on March 19, 2003, 6:09 PM
 */

package oliver.schematic;

/**
 *
 * @author  stephen
 */
public class TextFigure extends CH.ifa.draw.figures.TextFigure {
    
    public TextFigure() {
        super();
    }
    
    /** Creates a new instance of TextFigure */
    public TextFigure(net.n3.nanoxml.IXMLElement n) {
        super();
        try {
            String text = n.getAttribute("text","");
            String fontname = n.getAttribute("fontname","dialog.plain");
            int x = Integer.valueOf(n.getAttribute("x","0")).intValue();
            int y = Integer.valueOf(n.getAttribute("y","0")).intValue();
            int fontStyle = Integer.valueOf(n.getAttribute("fontStyle","0")).intValue();
            int fontSize = Integer.valueOf(n.getAttribute("fontSize","0")).intValue();
            setText(text);
            setFont(new java.awt.Font(fontname, fontStyle, fontSize));
            basicMoveBy(x, y);
        } catch(NumberFormatException e) {
            System.out.println(e);
        }
    }
    
    public net.n3.nanoxml.IXMLElement toXMLElement() {
        net.n3.nanoxml.IXMLElement n = new net.n3.nanoxml.XMLElement("gr:text");
        n.setAttribute("text", getText());
        n.setAttribute("x", Integer.toString(displayBox().getLocation().x));
        n.setAttribute("y", Integer.toString(displayBox().getLocation().y));
        n.setAttribute("fontSize", Integer.toString(getFont().getSize()));
        n.setAttribute("fontStyle", Integer.toString(getFont().getStyle()));
        return n;
    }
    
    public void addToComponent(oliver.xmpl.ComponentType component) {
        component.addChild(toXMLElement());
    }
    
    public void removeFromComponent(oliver.xmpl.ComponentType component) {
        component.removeChild(toXMLElement());
    }
    
}
