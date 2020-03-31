// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

package livdll;

public class CandidateAssignment implements Comparable {
    
    protected String attribute;
    protected String value;
    protected int weight;
    protected int time = -1;
    
    public CandidateAssignment(String attribute, String value, int weight) {
        int i = attribute.indexOf("#");
        if(i > -1) {
            String t = attribute.substring(0,i);
            attribute = attribute.substring(i+1);
            try {
                time = Integer.valueOf(t).intValue();
            } catch(NumberFormatException e) {}
        }
        this.attribute = attribute;
        this.value = value;
        this.weight = weight;
    }
    
    public String getAttribute() { return attribute; }
    public String getValue() { return value; }
    public int getWeight() { return weight; }
    public int getTime() { return time; }
    
    public String toString() {
        int i = attribute.indexOf(".")+1;
        int ii = attribute.lastIndexOf(".");
        if(ii == -1) ii = attribute.length();
        String n = attribute.substring(i,ii);
        return n+" -> "+value+"  weight="+getWeight()+" time="+time;
    }
    
    public boolean equals(Object obj) {
        if(obj instanceof CandidateAssignment) {
            CandidateAssignment c = (CandidateAssignment)obj;
            if(c.getAttribute().equals(attribute) &&
            c.getValue().equals(value) &&
            c.getTime() == getTime()) return true;
            else return false;
        } else return super.equals(obj);
    }
    
    public int compareTo(java.lang.Object obj) {
        if(obj != null && obj instanceof CandidateAssignment) {
            int diff = 0;
            CandidateAssignment a = (CandidateAssignment)obj;
            try {
                if(attribute!=null && a.getAttribute()!=null && !a.getAttribute().equals(attribute))
                    diff = attribute.compareTo(a.getAttribute());
                else if(value!=null && a.getValue()!=null && !a.getValue().equals(value))
                    diff = value.compareTo(a.getValue());
                else diff = time - a.getTime();
            } catch(java.lang.NullPointerException e) {
                diff = -1;
            }
            return diff;
        } else {
            return -1;
        }
    }
}
