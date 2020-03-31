//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.gutil;

import java.awt.*;

/** This class provides some graphics methods left out of JDK 1.1.  The
    functionality here is also in the 2D framework in 1.2, but we're not using
    that yet.  The methods are all static, so one shouldn't need to
    instantiate this class.  */
public class GraphicsExtension {

    /** A constant used by the thick line drawing code */
    protected final static int DDA_SCALE = 8192;

    /** Draw a line more than 1 pixel wide. */
    public static void drawThickLine(Graphics g, int x1, int y1, int x2, int y2, int linewidth) {
        g.fillOval(x1 - linewidth / 2, y1 - linewidth / 2, linewidth, linewidth);
        if (x1 == x2 && y1 == y2)
            return;
        if (Math.abs(x2 - x1) > Math.abs(y2 - y1)) {
            int dy, srow;
            int dx, col, row, prevrow;
            if (x2 > x1)
                dx = 1;
            else
                dx = -1;
            dy = (y2 - y1) * DDA_SCALE / Math.abs(x2 - x1);
            prevrow = row = y1;
            srow = row * DDA_SCALE + DDA_SCALE / 2;
            col = x1;
            for ( ; ; ) {
                if (row != prevrow) {
                    g.drawOval(col - linewidth / 2, prevrow - linewidth / 2, linewidth, linewidth);
                    prevrow = row;
                }
                g.drawOval(col - linewidth / 2, row - linewidth / 2, linewidth, linewidth);
                if (col == x2)
                    break;
                srow += dy;
                row = srow / DDA_SCALE;
                col += dx;
            }
        }
        else {
            int dx, scol;
            int dy, col, row, prevcol;
            if (y2 > y1)
                dy = 1;
            else
                dy = -1;
            dx = (x2 - x1) * DDA_SCALE / Math.abs(y2 - y1);
            row = y1;
            prevcol = col = x1;
            scol = col * DDA_SCALE + DDA_SCALE / 2;
            for ( ; ; ) {
                if (col != prevcol) {
                    g.drawOval(prevcol - linewidth / 2, row - linewidth / 2, linewidth, linewidth);
                    prevcol = col;
                }
                g.drawOval(col - linewidth / 2, row - linewidth / 2, linewidth, linewidth);
                if (row == y2)
                    break;
                row += dy;
                scol += dx;
                col = scol / DDA_SCALE;
            }
        }
    }

    public static void drawThickRectangle(Graphics g, int x, int y, int width, int height, int linewidth) {
        drawThickLine(g, x, y, x + width, y, linewidth);
        drawThickLine(g, x + width, y, x + width, y + height, linewidth);
        drawThickLine(g, x + width, y + height, x, y + height, linewidth);
        drawThickLine(g, x, y + height, x, y, linewidth);
    }

    public static void drawThickOval(Graphics g, int x1, int y1, int width, int height, int linewidth) {
        int xd = x1 + (width - linewidth) / 2;
        int yd = y1 + (height - linewidth) / 2;
        double a = width / 2;
        double as = a * a;
        double b = height / 2;
        double bs = b * b;
        int x = 0;
        int y = (int)b;
        double d = bs - as * b + as / 4.0;
        drawThickOvalPoints(g, x, y, xd, yd, linewidth);
        while (as * (y - 0.5) > bs * (x + 1.0)) {
            if (d < 0) {
                d = d + bs * (2.0 * x + 3.0);
                x++;
            }
            else {
                d = d + bs * (2.0 * x + 3.0) + as * (2.0 - 2.0 * y);
                x++;
                y--;
            }
            drawThickOvalPoints(g, x, y, xd, yd, linewidth);
        }
        d = bs * (x + 0.5) * (x + 0.5) + as * (y - 1) * (y - 1) - as * bs;
        while (y > 0) {
            if (d < 0) {
                d = d + bs * (2.0 * x + 2.0) + as * (3.0 - 2.0 * y);
                x++;
                y--;
            }
            else {
                d = d + as * (3.0 - 2.0 * y);
                y--;
            }
            drawThickOvalPoints(g, x, y, xd, yd, linewidth);
        }
    }

    public static void drawThickOvalPoints(Graphics g, int x, int y, int xd, int yd, int w) {
        g.fillOval(xd + x, yd + y, w, w);
        g.fillOval(xd + x, yd - y, w, w);
        g.fillOval(xd - x, yd - y, w, w);
        g.fillOval(xd - x, yd + y, w, w);
    }

    public static void drawThickPolygon(Graphics g, Polygon p, int linewidth) {
        for (int i = 0; i < p.npoints; i++)
            drawThickLine(g, p.xpoints[i], p.ypoints[i], p.xpoints[(i + 1) % p.npoints],
            p.ypoints[(i + 1) % p.npoints], linewidth);
    }

    public static void drawCurve(Graphics g, int cx0, int cy0, int cx1, int cy1, int cx2, int cy2, int cx3,
        int cy3, int lineSize) {
        double cx = 3 * (cx1 - cx0);
        double bx = 3 * (cx2 - cx1) - cx;
        double ax = cx3 - cx0 - cx - bx;
        double cy = 3 * (cy1 - cy0);
        double by = 3 * (cy2 - cy1) - cy;
        double ay = cy3 - cy0 - cy - by;
        int x1 = cx0;
        int y1 = cy0;
        for (double t = .01; t < (1.005); t += .01) {
            double t2 = t * t;
            double t3 = t2 * t;
            int x2 = (int)(ax * t3 + bx * t2 + cx * t + cx0);
            int y2 = (int)(ay * t3 + by * t2 + cy * t + cy0);
            if (lineSize == 1)
                g.drawLine(x1, y1, x2, y2);
            else
                drawThickLine(g, x1, y1, x2, y2, lineSize);
            x1 = x2;
            y1 = y2;
        }
    }

    public static void drawFilledCurve(Graphics g, int cx0, int cy0, int cx1, int cy1, int cx2, int cy2, int cx3,
        int cy3, int lineSize) {
        Polygon p = new Polygon();
        double cx = 3 * (cx1 - cx0);
        double bx = 3 * (cx2 - cx1) - cx;
        double ax = cx3 - cx0 - cx - bx;
        double cy = 3 * (cy1 - cy0);
        double by = 3 * (cy2 - cy1) - cy;
        double ay = cy3 - cy0 - cy - by;
        p.addPoint(cx0, cy0);
        for (double t = .01; t < (1.005); t += .01) {
            double t2 = t * t;
            double t3 = t2 * t;
            int x = (int)(ax * t3 + bx * t2 + cx * t + cx0);
            int y = (int)(ay * t3 + by * t2 + cy * t + cy0);
            p.addPoint(x, y);
        }
        p.addPoint(cx0, cy0);
        g.fillPolygon(p);
    }

    /** a version of rectangle intersection that doesn't require consing */
    public static boolean intersectsRectangleRectangle(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
        return !(x1 + w1 < x2 || x2 + w2 < x1 || y1 + h1 < y2 || y1 + h2 < y1);
        // for debugging
        //if (x1+w1<x2) return false;
        //if (x2+w2<x1) return false;
        //if (y1+h1<y2) return false;
        //if (y2+h2<y1) return false;
        //return true;
    }

    /** A version of rectangle intersection that doesn't require consing */
    public static boolean intersectsPointRectangle(int x1, int y1, int x2, int y2, int w2, int h2) {
        return !(x1 < x2 || x2 + w2 < x1 || y1 < y2 || y2 + h2 < y1);
        // for debugging
        //if (x1<x2) return false;
        //if (x2+w2<x1) return false;
        //if (y1<y2) return false;
        //if (y2+h2<y1) return false;
        //return true;
    }

    /** A version of sqrt for floats. */
    public static float sqrt(float a) {
        return (float)Math.sqrt((double)a);
    }

    /** A constant used to draw arrowheads quickly */
    protected static final float ARROWHEAD_SINX = 0.5f;

    /** A constant used to draw arrowheads quickly */
    protected static final float ARROWHEAD_COSX = 0.866f;

    /** Draw an arrow pointing from x1,y1 to x2,y2 * */
    public static void drawArrow(Graphics g, int x1, int y1, int x2, int y2, int arrowhead_length, int strokeWidth) {
        float d = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
        if (d == 0) d = .01f;
        float px = arrowhead_length * (x1 - x2) / d;
        float py = arrowhead_length * (y1 - y2) / d;
        int px1 = (int)(px * ARROWHEAD_COSX - py * ARROWHEAD_SINX) + x2;
        int py1 = (int)(px * ARROWHEAD_SINX + py * ARROWHEAD_COSX) + y2;
        int px2 = (int)(px * ARROWHEAD_COSX + py * ARROWHEAD_SINX) + x2;
        int py2 = (int)(px * (-ARROWHEAD_SINX) + py * ARROWHEAD_COSX) + y2;
        if (strokeWidth == 1) {
            g.drawLine(x1, y1, x2, y2);
            g.drawLine(x2, y2, (int)px1, (int)py1);
            g.drawLine(x2, y2, (int)px2, (int)py2);
        } else {
            drawThickLine(g, x1, y1, x2, y2, strokeWidth);
            drawThickLine(g, x2, y2, (int)px1, (int)py1, strokeWidth);
            drawThickLine(g, x2, y2, (int)px2, (int)py2, strokeWidth);
        };
    }

    /** Modify the first argument so it contains the point px,py */
    public static void growBoundingBox(Rectangle b, int px, int py) {
        if (px < b.x) {
            b.width = b.width + b.x - px;
            b.x = px;
        }
        if (py < b.y) {
            b.height = b.height + b.y - py;
            b.y = py;
        }
        if (px > b.x + b.width)
            b.width = py - b.x;
        if (py > b.y + b.height)
            b.height = py - b.y;
    }

    /** Modify the first argument so that it contains the second argument. */
    public static void growBoundingBox(Rectangle b, Rectangle r) {
        int px = r.x;
        if (px < b.x) {
            b.width = b.width + b.x - px;
            b.x = px;
        }
        int py = r.y;
        if (py < b.y) {
            b.height = b.height + b.y - py;
            b.y = py;
        }
        px = r.x + r.width;
        if (px > b.x + b.width) {
            b.width = px - b.x;
        }
        py = r.y + r.height;
        if (py > b.y + b.height)
            b.height = py - b.y;
    }

    public static void drawDashedLine(Graphics g,int x1,int y1,int x2,int y2,
                                      double dashlength, double spacelength) {
        double linelength=Math.sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
        double yincrement=(y2-y1)/(linelength/(dashlength+spacelength));
        double xincdashspace=(x2-x1)/(linelength/(dashlength+spacelength));
        double yincdashspace=(y2-y1)/(linelength/(dashlength+spacelength));
        double xincdash=(x2-x1)/(linelength/(dashlength));
        double yincdash=(y2-y1)/(linelength/(dashlength));
        int counter=0;
        for (double i=0;i<linelength-dashlength;i+=dashlength+spacelength){
            g.drawLine((int) (x1+xincdashspace*counter),
                       (int) (y1+yincdashspace*counter),
                       (int) (x1+xincdashspace*counter+xincdash),
                       (int) (y1+yincdashspace*counter+yincdash));
            counter++;
        }
        if ((dashlength+spacelength)*counter<=linelength)
            g.drawLine((int) (x1+xincdashspace*counter),
                       (int) (y1+yincdashspace*counter),
                       x2,y2);
    }

}
