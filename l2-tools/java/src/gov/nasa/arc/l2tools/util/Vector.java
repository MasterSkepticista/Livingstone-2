//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : Vector.java
 *
 * Adds convenience constructors
 *
 * CHANGELOG:
 * 12/01/99      Mhs     Created
 */

package gov.nasa.arc.l2tools.util;

public class Vector extends java.util.Vector {

    public Vector() {
        super();
    }

    public Vector(int initialCapacity) {
        super(initialCapacity);
    }

    public Vector(int initialCapacity, int capacityIncrement) {
        super(initialCapacity,capacityIncrement);
    }

    public Vector(Object o) {
        super(1);
        addElement(o);
    }

    public Vector(Object o1, Object o2) {
        super(2);
        addElement(o1);
        addElement(o2);
    }

    public Vector(Object o1, Object o2, Object o3) {
        super(3);
        addElement(o1);
        addElement(o2);
        addElement(o3);
    }

    public Vector(Object o1, Object o2, Object o3, Object o4) {
        super(4);
        addElement(o1);
        addElement(o2);
        addElement(o3);
        addElement(o4);
    }
}
