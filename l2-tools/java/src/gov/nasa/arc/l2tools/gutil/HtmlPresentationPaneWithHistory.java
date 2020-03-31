//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : HtmlPresentationPaneWithHistory.java
*
* CHANGELOG:
* 12/03/99      Mhs     Created
*/

package gov.nasa.arc.l2tools.gutil;

import gov.nasa.arc.l2tools.util.Vector;
import java.awt.Component;

/** This adds history management to HtmlPresentationPane. */
public class HtmlPresentationPaneWithHistory extends SwingHtmlPresentationPane {

    /** A history of objects that have been viewed in this viewer. */
    protected Vector history = new Vector(3);

    /** The position of the currently visible object in the history. */
    protected int currentIndex = -1;

    /** The currently displayed object. */
    Object currentObject = null;

    ///
    /// Interface for editing from outside
    ///

    protected boolean isDisplayingObject() {
        return !(history.size()>0 
                 && ((history.elementAt(history.size()-1))
                     instanceof GeneratedHtml));
    }

    protected GeneratedHtml getGeneratedHtml() {
        if (history.size()<=0) return null;
        if (isDisplayingObject()) return null;
        return (GeneratedHtml)history.elementAt(history.size()-1);
    }

    /** Clear the window. */
    public void htmlClear(boolean clearHistory) { 
        super.htmlClear();
        if (clearHistory) history.removeAllElements();
        history.addElement(new GeneratedHtml());
        currentIndex = history.size()-1;
    }

    public void htmlClear() {
        htmlClear(false);
    }

    public void present(Object o) {
        if (presentInternal(o))
            updateHistory(o);
    }

    public Object getCurrentObject() {
        if (currentIndex<0 || currentIndex >= history.size())
            return null;
        return history.elementAt(currentIndex);
    }

    protected void updateHistory(Object o) {
        history.addElement(o);
        currentIndex = history.size()-1;
    }

    ///
    /// Menu handling
    ///

    ///
    /// Browser default menu
    ///

    protected void exposeDefaultPopupMenu(int x, int y) {
        PopupMenu.popup("HTML Object Viewer",
                        new Vector(menuItemBack(),menuItemForward(),menuItemCopy(),menuItemPrintHistory()),
                        this, this,x,y);
    }

    protected PopupMenuItem menuItemBack() {
        return new PopupMenuItem("Back",
                                 new PopupMenuCallback() {
            public void callback(Component context) {
                if (currentIndex<1) return;
                presentInternal(history.elementAt(--currentIndex));
            }
        });
    }

    protected PopupMenuItem menuItemForward() {
        return new PopupMenuItem("Forward",
                                 new PopupMenuCallback() {
            public void callback(Component context) {
                if (currentIndex>=history.size()-1) return;
                presentInternal(history.elementAt(++currentIndex));
            }
        });
    }

    protected PopupMenuItem menuItemPrintHistory() {
        return new PopupMenuItem("Print History",
                                 new PopupMenuCallback() {
            public void callback(Component context) { 
                int max=history.size();
                System.out.println("history (currentIndex="+currentIndex+"):");
                for (int i=0;i<max;i++) 
                    System.out.println(i+" --> "+history.elementAt(i));
            }
        });
    }

}
