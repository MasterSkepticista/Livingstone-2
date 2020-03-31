// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * MyStandardDrawingView.java
 *
 * Created on December 5, 2002, 12:18 PM
 */

package oliver.schematic;

/**
 *
 * @author  stephen
 */
public class MyStandardDrawingView extends CH.ifa.draw.standard.StandardDrawingView {
    
    protected boolean editable = false;
    
    public MyStandardDrawingView(CH.ifa.draw.framework.DrawingEditor editor, int width, int height) {
        super(editor, width, height);
    }
    public boolean getEditable() { return editable; }
    public void setEditable(boolean editable) { this.editable = editable; }
    public void mousePressed(java.awt.event.MouseEvent e) {
        if(editable) super.mousePressed(e);
    }
    public void mouseDragged(java.awt.event.MouseEvent e) {
        if(editable) super.mouseDragged(e);
    }
    public void mouseMoved(java.awt.event.MouseEvent e) {
        if(editable) super.mouseMoved(e);
    }
    public void mouseReleased(java.awt.event.MouseEvent e) {
        if(editable) super.mouseReleased(e);
    }
    public void keyPressed(java.awt.event.KeyEvent e) {
        if(editable) super.keyPressed(e);
    }
}
