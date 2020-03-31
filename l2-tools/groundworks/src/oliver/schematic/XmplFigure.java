// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * XmplFigure.java
 *
 * Created on October 3, 2002, 3:28 PM
 */

package oliver.schematic;

/**
 *
 * @author  stephen
 */
public interface XmplFigure extends CH.ifa.draw.framework.Figure {

    public static int EDITOR_CONTEXT = 1;
    public static int DEBUGGER_CONTEXT = 2;

    public javax.swing.Action[] getActions(final int context, oliver.application.UserProperties userProperties, java.awt.Point popupLocation, java.awt.Point drawingLocation);
    
    public String getMouseOverText();
    
    /** Used to support updating moves in SchematicView.
     *  For instances of ComponentTypes and AttributeTypes return the 
     *  upper left corner.  For lines return null, since they cannot
     *  be moved. */
    public java.awt.Point location();
    
}

