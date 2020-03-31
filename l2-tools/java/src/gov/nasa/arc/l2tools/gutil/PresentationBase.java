//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : PresentationBase.java
 *
 * CHANGELOG:
 * 11/27/99      Mhs     Created
 */

package gov.nasa.arc.l2tools.gutil;

import gov.nasa.arc.l2tools.util.EventLog;
import java.awt.Component;
import java.util.Vector;

/** Base class for presentations. */
public class PresentationBase implements Presentation {

    protected Object object;

    public PresentationBase(Object object) {
        this.object = object;
    }

    /** Return the object this presentation represents. */
    public Object getObject() { return object; }

    /** Return a short string to present this object in a wholine */
    public String stringForWholine() { 
        if (object==null)
            return "";
        else
            return object.toString();
    }

    /** Add PopupMenuItem's for this object's right click behavior to items. */
    public void addPopupMenuItems(Component c, Vector items) {
        Object o = getObject();
        if (o instanceof Presentable)
            ((Presentable)o).addPopupMenuItems(c, items);
        items.addElement(menuItemDescribe());
        items.addElement(menuItemInspect());
    }

    // 
    // Common popup menu commands
    // 

    protected PopupMenuItem menuItemDescribe() {
        return new PopupMenuItem("Describe",
                                 new PopupMenuCallback() {
            public void callback(Component context) { describe(context); }
        });
    }

    /** Default describe operation: pop up an HtmlObjectBrowser. */
    public void describe(Component context) {
        HtmlObjectBrowser.presentObject(getObject());
    }

    protected PopupMenuItem menuItemOpen() {
        return new PopupMenuItem("Open",
                                 new PopupMenuCallback() {
            public void callback(Component context) { open(context); }
        });
    }

    public void open(Component context) {
        EventLog.report(new Throwable("default open menu item called"));
    }

    protected PopupMenuItem menuItemInspect() {
        return new PopupMenuItem("Inspect",
                                 new PopupMenuCallback() {
            public void callback(Component context) { inspect(context); }
        });
    }

    public void inspect(Component context) {
        HtmlInspector.inspectObject(getObject());
    }

}
