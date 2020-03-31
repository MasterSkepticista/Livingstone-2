//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : Wholine.java
*
* See StatusLine also.  (StatusLine is a copy of this but should probably be
* the one to get further development.)
*
* CHANGELOG:
* 11/07/99 Mhs Created */

package gov.nasa.arc.l2tools.gutil;

import javax.swing.JLabel;
import java.io.StringWriter;
import java.awt.Dimension;
import javax.swing.border.BevelBorder;
import javax.swing.BorderFactory;
import java.awt.BorderLayout;

/** This is a placeholder for a more complex version of a wholine.
    Eventually, it'll be a combination message area / progress area. */
public class Wholine extends javax.swing.JPanel {
    static Dimension size = new Dimension(12,16); // for now

    // The actual wholine
    JLabel wholine;

	// com.symantec.itools.javax.swing.borders.BevelBorder bevelBorder1 = new com.symantec.itools.javax.swing.borders.BevelBorder();

    public Wholine() {
        super();
        setBorder(BorderFactory.createBevelBorder(BevelBorder.LOWERED));
        setLayout(new BorderLayout());
        wholine = new JLabel();
        wholine.setOpaque(true);
        add(BorderLayout.CENTER,wholine);
        setMinimumSize(size);
        setPreferredSize(size);
    }

    public void setText(String s) { wholine.setText(s); }

}
