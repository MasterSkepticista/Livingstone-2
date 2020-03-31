//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : thumbwheel control (very simple for now)
*
* CHANGELOG:
* 11/18/99      Mhs     Created
* 11/19/99      Mhs     Beafed up.  Added observable pattern.  (Sigh,
*                       can't use java.util.Observable directly.)
* 11/21/99      Mhs     Made the drawing more like a real thumbwheel.
*                       Could be more efficient, though.
*/

package gov.nasa.arc.l2tools.gutil;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.event.MouseEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseMotionAdapter;
import java.awt.event.MouseAdapter;
import java.util.Vector;
import java.util.Enumeration;
import javax.swing.JComponent;

public class Thumbwheel extends JComponentWithInputMode implements MouseAdjustment {

    /** The minimum level */
    protected float minLevel = 0f;

    /** The maximum level */
    protected float maxLevel = 1f;

    /** The current level */
    protected float level = 0.5f;

    /** The percentage of the total range traversed by one sweep of the mouse
        across this widget */
    protected float gain = 50f;

    public double conversion = (float)(Math.PI / 180D);
    public double pi2 = Math.PI / 2d;

    protected double rotationFactor = 1d;
    protected double dragAngle = 0d;

    public final double grooveStep = 20d * conversion;

    /** Whether this is a horizontal or a vertical thumbwheel */
    protected boolean horizontal = false;

    /** A vector of java.util.Observers */
    Vector observers = new Vector(1);

    public Thumbwheel(boolean horizontal) {
        super();
        this.horizontal = horizontal;
        setMinimumSize(new Dimension(3,3));
    }

    protected InputMode defaultInputMode() {
        return new ThumbwheelInputMode();
    }

    public void setBounds(int x, int y, int width, int height) {
        super.setBounds(x,y,width,height);
        double h=0d;
        if (horizontal)
            h = getSize().width - 8d;
        else
            h = getSize().height - 8d;
        if (h < 1d) h = 1d;
        rotationFactor = Math.asin(1d/h);
    }

    public void paint(Graphics g) {
        if (horizontal)
            paintHorizontal(g);
        else
            paintVertical(g);
    }

    protected void paintVertical(Graphics g) {
        Dimension d = getSize();

        // Paint background
        g.setColor(Color.lightGray);
        g.fillRect(0,0,d.width,d.height);

        // Paint border
        Color light = Color.darkGray;
        Color dark = Color.white;
    
        g.setColor(dark);
        g.drawLine(d.width-1,0,d.width-1,d.height);
        g.drawLine(0,d.height-1,d.width,d.height-1);
        g.setColor(light);
        g.drawLine(0,0,d.width,0);
        g.drawLine(0,0,0,d.height);

        int x = 1+ (d.width-2) / 4;
        int w = d.width - x * 2;
        if (w<1) w=1;
        int y = 4;
        int h = d.height - y * 2;
        // Draw inner border
        g.setColor(Color.black);
        g.drawRect(x,y,w,h);

        int midY = y + h / 2;

        for (double i = -pi2;i<pi2; i = i+grooveStep){
            double ang = i + dragAngle;
            if (ang < -pi2 || ang > pi2) continue;
            double o = ((double)h)/2d * Math.sin(ang);
            //System.out.println("i="+i+"  ang="+ang+"  o="+o);
            g.setColor(Color.black);
            int y1 = (int)(midY+o);
            g.drawLine(x,y1,x+w,y1);
        }

    }

    protected void paintHorizontal(Graphics g) {
        Dimension d = getSize();

        // Paint background
        g.setColor(Color.lightGray);
        g.fillRect(0,0,d.width,d.height);

        // Paint border
        Color light = Color.darkGray;
        Color dark = Color.white;
    
        g.setColor(dark);
        g.drawLine(d.width-1,0,d.width-1,d.height);
        g.drawLine(0,d.height-1,d.width,d.height-1);
        g.setColor(light);
        g.drawLine(0,0,d.width,0);
        g.drawLine(0,0,0,d.height);

        int x = 1+ (d.height-2) / 4;
        int w = d.height - x * 2;
        if (w<1) w=1;
        int y = 4;
        int h = d.width - y * 2;
        // Draw inner border
        g.setColor(Color.black);
        g.drawRect(y,x,h,w);

        int midY = y + h / 2;

        for (double i = -pi2;i<pi2; i = i+grooveStep){
            double ang = i + dragAngle;
            if (ang < -pi2 || ang > pi2) continue;
            double o = ((double)h)/2d * Math.sin(ang);
            g.setColor(Color.black);
            int y1 = (int)(midY+o);
            g.drawLine(y1,x,y1,x+w);
        }

    }

    class ThumbwheelInputMode extends InputModeIdle {
        int startx, starty;
        float startLevel;
        boolean finished = false;
        public void mousePressed(MouseEvent e) {
            startx = e.getX();
            starty = e.getY();
            startLevel = level;
            dragAngle = 0d;
            //System.out.println("---level="+level);
            finished = false;
            notifyObserversBegin();
        }
        public void mouseDragged(MouseEvent e) {
            if (finished) return;
            int d;
            float p;
            if (horizontal) {
                d = e.getX() - startx;
                p = (float)d / (float)(getSize().width-2);
            } else {
                d = e.getY() - starty;
                p = (float)d / (float)(getSize().height-2);
            }

            level = startLevel + (maxLevel-minLevel)*p*(gain/100f);

            // Combine the clipping and the dragAngle computation
            if (level>=minLevel && level<=maxLevel) {
                dragAngle = rotationFactor * d;
                int j = (int)(dragAngle/grooveStep);
                dragAngle = dragAngle - (j*grooveStep);
            } else {
                if (level>maxLevel) 
                    level = maxLevel;
                else
                    level = minLevel;
            }

            //System.out.println("d="+d+"  p="+p+"  level="+level);
            //System.out.println("level="+level);
            //System.out.println("  maxLevel="+maxLevel+"  minLevel="+minLevel);

            notifyObserversAdjust();
            repaint();
        }
        public void mouseReleased(MouseEvent e) {
            notifyObserversEnd();
        }
        public void keyPressed(KeyEvent e) {
            if (KeyEvent.VK_ESCAPE == e.getKeyCode()) {
                level = startLevel;
                notifyObserversAdjust();
                repaint();
                finished = true;
            }
        }
    }

    float screenYtoLevel(int y) {
        Dimension d = getSize();
        float result = 1f - ((float)y / (float)d.height);
        if (result<0f) result = 0f;
        if (result>1f) result = 1f;
        return result;
    }

    float screenXtoLevel(int x) {
        Dimension d = getSize();
        float result = 1f - ((float)x / (float)d.width);
        if (result<0f) result = 0f;
        if (result>1f) result = 1f;
        return result;
    }

    //
    // Observers
    //

    public void addMouseAdjustmentListener(MouseAdjustmentListener source) {
        if (!observers.contains(source))
            observers.addElement(source);
    }

    public void deleteMouseAdjustmentListener(MouseAdjustmentListener source) {
        observers.removeElement(source);
    }
        
    public void deleteMouseAdjustmentListeners() {
        observers.removeAllElements();
    }

    public void notifyObserversBegin() {
        for (Enumeration e=observers.elements(); e.hasMoreElements(); )
            ((MouseAdjustmentListener)e.nextElement()).begin(this);
    }

    public void notifyObserversAdjust() {
        for (Enumeration e=observers.elements(); e.hasMoreElements(); )
            ((MouseAdjustmentListener)e.nextElement()).adjust(this);
    }

    public void notifyObserversEnd() {
        for (Enumeration e=observers.elements(); e.hasMoreElements(); )
            ((MouseAdjustmentListener)e.nextElement()).end(this);
    }

    //
    // Properties
    //

    public float getLevel() { return level; }
    public float getMaxLevel() { return maxLevel; }
    public float getMinLevel() { return minLevel; }
    public float getGain() { return gain; }

    public void setMaxLevel(float maxLevel) {
        this.maxLevel = maxLevel;
        if (level > maxLevel) level = maxLevel;
    }
    public void setMinLevel(float minLevel) {
        this.minLevel = minLevel;
        if (level < minLevel) level = minLevel;
    }
    public void setLevel(float level) { 
        if (level == this.level || level < minLevel || level > maxLevel) return;
        this.level = level;
        repaint();
        notifyObserversAdjust();
    }

    public void setGain(float gain) { this.gain = gain; }

}
