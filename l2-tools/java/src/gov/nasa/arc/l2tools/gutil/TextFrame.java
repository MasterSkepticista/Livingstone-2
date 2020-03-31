//
// * See the file "l2-tools/disclaimers-and-notices.txt" for
// * information on usage and redistribution of this file,
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * DiscrepancyFrame
 *
 * Created on January 23, 2003, 6:18 PM
 */

package gov.nasa.arc.l2tools.gutil;

/**
 *
 * @author  rgarcia
 */

import javax.swing.JTextArea;
import javax.swing.JScrollPane;
import java.util.Vector;
import javax.swing.BorderFactory;
import javax.swing.border.Border;
import java.awt.Insets;
import java.awt.Color;
import java.awt.Dimension;

 

/** Class TextFrame
*
*   Defines a scrollable text frame with a custom Bevelled border
*
**/

public class TextFrame extends JScrollPane{

    private JTextArea textPane = new JTextArea(0,0);
    private String title = "";

    /* Define size of the text frame */
    
    public TextFrame(int rows, int columns, String title){
        this.title = title;
        initDisplay(rows,columns);       
    }


    
    public void initDisplay(int rows, int columns) {
       
        setViewportView(textPane);
        setBorderOutline();
        setDisplayProperties(rows,columns);
        
    }
   

    /* Creates Custom Border for text area */
 
    private void setBorderOutline(){
        
        Border raisedbevel = BorderFactory.createRaisedBevelBorder();
        
        Border loweredbevel = BorderFactory.createLoweredBevelBorder();

        Border compound = BorderFactory.createCompoundBorder(raisedbevel, loweredbevel);
        
        Border titled = BorderFactory.createTitledBorder(compound,title);
        
        setBorder(titled);
        
    }


    /* Set all viewable properties for text area */

    private void setDisplayProperties(int x, int y){
        
        textPane.setMargin(new Insets(5,3,5,3));
        textPane.setBackground(getBackground());
        textPane.setEditable(false);  
        setPreferredSize(new Dimension(x,y));
        setMinimumSize(new Dimension(0,0)); 
    }
    

    /* Accepts a list of assignments to be displayed */
    
    public void setText(Vector assignments){
        textPane.setText("");
        if(assignments == null) return;
        for(int i=0; i < assignments.size(); i++){
            textPane.append(assignments.get(i).toString() + "\n");  
        }
        
    }
    
    public void setText(String text) {textPane.setText(text);}

    public void append(String text) {textPane.append(text);}
    /* Erase all text from display area */

    public void clear(){
        
         textPane.setText("");
    }
    
}
