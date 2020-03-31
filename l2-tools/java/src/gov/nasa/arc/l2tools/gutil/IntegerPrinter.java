//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : IntegerPrinter.java
*
* CHANGELOG:
* 04/05/99      Mhs     Created
*/

package gov.nasa.arc.l2tools.gutil;

import java.awt.*;

/** Java's methods for printing objects with a Graphics aren't great.  This is
 * an attempt at printing integers without consing strings all the time (since
 * I'm using this at animation framerates).  */
public class IntegerPrinter implements java.io.Serializable {
    public final int INITIAL_CAPACITY = 30;
    char[] buf = new char[INITIAL_CAPACITY];
    int capacity = INITIAL_CAPACITY;
    int ptr = 0; // Where the next char will go

    public void reset() { ptr = 0; }

    public void removeAllChars() { ptr = 0; }

    public void insert(char c) {
        if (ptr >= capacity) {
            char buf1[] = new char[capacity * 2];
            for (int i = 0; i < ptr; i++) buf1[i] = buf[i];
            buf = buf1;
        }
        buf[ptr++] = c;
    }

    public void insert(String s) {
        int size = s.length();
        if (ptr + size >= capacity) {
            char buf1[] = new char[capacity + size + 20];
            for (int i = 0; i < ptr; i++) buf1[i] = buf[i];
            buf = buf1;
        }
        s.getChars(0, size, buf, ptr);
        ptr = ptr + size;
    }

    // convert to chars
    public void insert(int i) {
        int temp;
        if (i < 0) {
            insert('-');
            i = -i;
        }
        insertIntegerRecur(i);
    }

    public void insertIntegerRecur(int i) {
        if (i < 10) {
            insert((char)(48 + i));
        } else {
            int temp = i / 10;
            insertIntegerRecur(temp);
            insert((char)(48 + (i - temp * 10)));
        }
    }

    public void drawString(Graphics g, int x, int y) {
        g.drawChars(buf, 0, ptr, x, y);
    }
}

