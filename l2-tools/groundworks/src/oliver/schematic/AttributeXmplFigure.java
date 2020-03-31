// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * AttributeXmplFigure.java
 *
 * Created on October 10, 2002, 10:15 AM
 */

package oliver.schematic;

/**
 *
 * @author  stephen
 */
public interface AttributeXmplFigure extends XmplFigure {

    public oliver.xmpl.Attribute getAttribute();
    
    public oliver.xmpl.ComponentType getType();
    
}
