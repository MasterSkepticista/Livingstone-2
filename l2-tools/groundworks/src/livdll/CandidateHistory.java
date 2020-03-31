// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * CandidateHistory.java
 *
 * Created on November 12, 2002, 3:28 PM
 */

package livdll;

/**
 *
 * @author  stephen
 */
public class CandidateHistory {
    
    private short counter = 0;
    private SmallAttributeAssignment[][] data;
    private java.util.HashMap tokenkeys = new java.util.HashMap();
    private java.util.HashMap numberkeys = new java.util.HashMap();
    
    /** Creates a new instance of CandidateHistory */
    public CandidateHistory(AttributeAssignment[][] assignments) {
        data = new SmallAttributeAssignment[assignments.length][];
        for(int i = 0; i < assignments.length; i++) {
            data[i] = new SmallAttributeAssignment[assignments[i].length];
            for(int j = 0; j < assignments[i].length; j++) {
                data[i][j] = new SmallAttributeAssignment(assignments[i][j]);
            }
        }
    }
    
    /** Creates a new instance of CandidateHistory */
    public CandidateHistory(java.util.HashMap tokenkeys, java.util.HashMap numberkeys, AttributeAssignment[][] assignments) {
        this.tokenkeys = tokenkeys;
        this.numberkeys = numberkeys;
        data = new SmallAttributeAssignment[assignments.length][];
        for(int i = 0; i < assignments.length; i++) {
            data[i] = new SmallAttributeAssignment[assignments[i].length];
            for(int j = 0; j < assignments[i].length; j++) {
                data[i][j] = new SmallAttributeAssignment(assignments[i][j]);
            }
        }
    }
    
    public AttributeAssignment[][] getAttributeAssignment() {
        AttributeAssignment[][] assignments = new AttributeAssignment[data.length][];
        for(int i = 0; i < data.length; i++) {
            assignments[i] = new AttributeAssignment[data[i].length];
            for(int j = 0; j < data[i].length; j++) {
                String name = getTokenPath(data[i][j].name);
                String value = getToken(data[i][j].value);
                int time = (int)data[i][j].time;
                assignments[i][j] = new AttributeAssignment(name,value,time);
            }
        }
        return assignments;
    }
    
    private short getTokenShort(String tok) {
        Short num = (Short)tokenkeys.get(tok);
        if(num == null) {
            num = new Short(counter++);
            tokenkeys.put(tok, num);
            numberkeys.put(num, tok);
        }
        return num.shortValue();
    }
    
    private String getToken(short num) {
        return (String)numberkeys.get(new Short(num));
    }
    
    private short[] getTokenPathShortArray(String path) {
        int j = path.indexOf("#");
        if(j > -1) path = path.substring(j+1);
        java.util.Vector v = new java.util.Vector();
        java.util.StringTokenizer tonenizer = new java.util.StringTokenizer(path, ".");
        while(tonenizer.hasMoreElements()) {
            v.addElement(tonenizer.nextToken());
        }
        short[] a = new short[v.size()];
        for(int i = 0; i < v.size(); i++) {
            a[i] = getTokenShort((String)v.elementAt(i));
        }
        return a;
    }
    
    private String getTokenPath(short[] a) {
        java.util.Vector v = new java.util.Vector();
        StringBuffer buf = new StringBuffer();
        for(int i = 0; i < a.length; i++) {
            buf.append(getToken(a[i]));
            if(i+1 < a.length) buf.append(".");
        }
        return buf.toString();
    }
        
    private class SmallAttributeAssignment {
        public short[] name;
        public short value;
        public short time;
        public SmallAttributeAssignment(AttributeAssignment a) {
            name = getTokenPathShortArray(a.getName());
            value = getTokenShort(a.getValue());
            time = (short)a.getTime();
        }
    }
    
}
