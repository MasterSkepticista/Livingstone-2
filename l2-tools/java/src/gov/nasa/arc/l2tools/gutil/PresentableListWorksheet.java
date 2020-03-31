//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : PresentableListWorksheet.java
 *
 * This is patterned after a DND example in the java tutorial.
 *
 * CHANGELOG:
 * 12/31/99      Mhs     Created
 */

package gov.nasa.arc.l2tools.gutil;

import java.awt.*;
import javax.swing.*;

public class PresentableListWorksheet extends JFrame {

    PresentableDNDList list;

    public PresentableListWorksheet() {
        super();
        initialize();
    }

    public PresentableListWorksheet(String title) {
        super(title);
        initialize();
    }

    protected void initialize() {
        list = new PresentableDNDList();
        JScrollPane scrollPane = new JScrollPane(list);
        getContentPane().setLayout(new BorderLayout());
        getContentPane().add(scrollPane,BorderLayout.CENTER);
        setSize(100,300);
    }

    /** Add elements. */
    public void addElement(Object o) {
        list.addElement(o);
    }

    /** Remove an element. */
    public void removeElement(Object o) {
        list.removeElement(o);
    }


}
