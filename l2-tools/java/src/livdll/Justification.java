//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : Justification.java
*
* A Justification
*
* CHANGELOG:
* 11/20/99      Mhs     Created.
*/

package livdll;

import java.io.*;

public class Justification implements Serializable {

    protected AttributeAssignment consequent;

    Justification(AttributeAssignment[] pos, AttributeAssignment[] neg) {
        // Copy the arrays
        this.pos = new AttributeAssignment[pos.length];
        System.arraycopy(pos,0,this.pos,0,pos.length);
        this.neg = new AttributeAssignment[neg.length];
        System.arraycopy(neg,0,this.neg,0,neg.length);
    }

    protected AttributeAssignment[] pos = null;
    protected AttributeAssignment[] neg = null;

    public AttributeAssignment[] getPositiveAntecedents() { return pos; }
    public AttributeAssignment[] getNegativeAntecedents() { return neg; }
    public int getPositiveAntecedentsSize() { return pos.length; }
    public int getNegativeAntecedentsSize() { return neg.length; }
    public AttributeAssignment getPositiveAntecedent(int i) { return pos[i]; }
    public AttributeAssignment getNegativeAntecedent(int i) { return neg[i]; }
        
    public String toString() { return "<justification pos="+(pos==null?0:pos.length)+" neg="+(neg==null?0:neg.length)+">"; }

    /*
    public void describe(PrintStream stream) {
        describe(stream,0);
    }

    public void describe(PrintStream stream, int indent) {
        indent(stream,indent);
        if (0==neg.length)  {
            // A disjunction with only a single positive literal
            // forces that literal to be true
            if (1==pos.length) {
                stream.print("Assertion: ");
                pos[0].describe(stream,indent);
            } else {
                stream.print("At least one: "); 
                for (int i=0;i<pos.length;i++)
                    stream.print(""+pos[i]+" ");
            }
            return;
        }
    }
    */

    public static void indent(PrintStream stream, int indent) {
        for (int i=0;i<indent;i++) stream.print(" ");
    }

}
