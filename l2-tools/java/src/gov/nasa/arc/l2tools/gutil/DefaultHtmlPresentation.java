//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : DefaultHtmlPresentation.java
 *
 * CHANGELOG:
 * 11/27/99      Mhs     Created
 */

package gov.nasa.arc.l2tools.gutil;

import gov.nasa.arc.l2tools.util.Vector;
import java.awt.Component;

/** A simple presentation for HTML that defaults to presenting the object as
    its toString() unless it's Presentable (with its own writeHTML()
    method). */
public class DefaultHtmlPresentation extends PresentationBase implements HtmlPresentation {

    /** Create an HtmlPresentation */
    public DefaultHtmlPresentation(Object object) { 
        super(object);
    }

    /** Write a representation of this object to the stream. */
    public void write(HtmlWriter writer) throws java.io.IOException {
        if (object==null)
            writer.write(toString()+" presenting a null object.");
        else if (getObject() instanceof Presentable)
            ((Presentable)getObject()).write(writer);
        else
            writer.presentAsLink(this,object.toString());
    }

    //
    // Menu handling
    //

    /** Add PopupMenuItem's for this object's right click behavior to items. */
    public void addPopupMenuItems(Component c, Vector items) {
        items.addElement(menuItemOpen());
        items.addElement(menuItemOpenNew());
        super.addPopupMenuItems(c, items);
    }

    protected PopupMenuItem menuItemOpen() {
        return new PopupMenuItem("Open",
                                 new PopupMenuCallback() {
            public void callback(Component context) { 
                if (context instanceof HtmlObjectBrowser)
                    ((HtmlObjectBrowser)context).presentObject(getObject());
                else
                    HtmlObjectBrowser.presentObject(getObject());
            }
        });
    }

    protected PopupMenuItem menuItemOpenNew() {
        return new PopupMenuItem("Open New",
                                 new PopupMenuCallback() {
            public void callback(Component context) { 
                HtmlObjectBrowser.presentObject(getObject());
            }
        });
    }


}
