//
// * See the file "l2-tools/disclaimers-and-notices.txt" for
// * information on usage and redistribution of this file,
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Discrepancy.java
 *
 * Created on January 31, 2003, 7:45 PM
 */

package livdll;

/**
 * Class Discrepancy represents a Discrepant Observation or Command.
 *       Livingstone classifies an assignment as discrepant if it conflicts
 *       with the known model constraints.  
 * @author  rgarcia
 */
public class Discrepancy { 
    private String name;
    private String value;
    private int weight;
   
   
    public Discrepancy(String name, String value, int weight) {
    
        this.name = name;
        this.value = value;
        this.weight = weight;
        
    }


    public String getName() { return name; }

    public String getValue() { return value; }

    public int getWeight() { return weight; }    
    
    public String toString(){
        
        return name + " = " + value + " :"+ weight;
    }
}
