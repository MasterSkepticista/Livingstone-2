//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : HtmlInspector.java
*
* CHANGELOG:
* 11/29/99      Mhs     Created
*/

package gov.nasa.arc.l2tools.gutil;

/** A JFrame for holding an Html-based inspector. */
public class HtmlInspector extends HtmlObjectBrowser {

    protected static final String title = "Java Object Inspector";

    private HtmlInspector() {
        super(title);
    }

    protected HtmlPresentationPaneWithHistory getBrowserPane() { 
        return new HtmlPresentationPaneWithHistory() {
            protected Class getPresentationContext() { 
                return HtmlInspector.class; 
            }
        };
    }

    ///
    /// Static functionality
    ///

    /** Create and show a new inspector displaying any object. */
    public static void inspectObject(Object o) {
        HtmlInspector viewer = new HtmlInspector();
        viewer.setVisible(true);
        viewer.present(o,title); 
    }

}
