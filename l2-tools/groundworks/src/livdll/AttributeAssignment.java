// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

package livdll;

public class AttributeAssignment {
    
    protected String name;
    protected String value;
    protected int time;
    
    // used int livdll
    public AttributeAssignment(String name, String value) {
        this(name, value, -1);
        int i = name.indexOf("#");
        if(i > -1) {
            String t = name.substring(0,i);
            name = name.substring(i+1);
            try {
                time = Integer.valueOf(t).intValue();
            } catch(NumberFormatException e) {}
        }
    }
    
    public AttributeAssignment(String name, String value, int time) {
        int i = name.indexOf("#");
        if(i > -1) this.name = name.substring(i+1);
        else this.name = name;
        this.value = value;
        this.time = time;
    }
    
    public String toString() {
        return name+"="+value;
    }
    
    public String getName() { return name; }
    public String getValue() { return value; }
    public int getTime() { return time; }
        
}