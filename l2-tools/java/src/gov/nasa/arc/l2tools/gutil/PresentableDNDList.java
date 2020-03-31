//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : PresentableDNDList.java
 *
 * This is patterned after a DND example in the java tutorial.
 *
 * CHANGELOG:
 * 12/31/99      Mhs     Created
 */

package gov.nasa.arc.l2tools.gutil;

import gov.nasa.arc.l2tools.util.EventLog;
import java.awt.*;
import java.awt.datatransfer.*;
import java.awt.dnd.*;
import java.io.*;
import java.io.IOException;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import javax.swing.DefaultListModel;
import javax.swing.JList;

/** A list JComponent that can hold presentables.  Objects are copied (not moved)
    of of this presently.  This behavior will be an option later. */
public class PresentableDNDList extends JList 
implements DropTargetListener, DragSourceListener, DragGestureListener 
{

    /** enables this component to be a dropTarget. */
    DropTarget dropTarget = null;

    /** enables this component to be a Drag Source. */
    DragSource dragSource = null;

    /** constructor - initializes the DropTarget and DragSource. */
    public PresentableDNDList() {
        dropTarget = new DropTarget(this, this);
        dragSource = new DragSource();
        dragSource.createDefaultDragGestureRecognizer(this, DnDConstants.ACTION_COPY_OR_MOVE, this);
        // Is this necessary?
        setModel(new DefaultListModel());
    }

    // Just for the debugging message
    public void setDropTarget(DropTarget dt) {
        System.out.println("setDropTarget");
        super.setDropTarget(dt);
    }

    /** A drag gesture has been initiated. */
    public void dragGestureRecognized(DragGestureEvent event) {
        Object selected = getSelectedValue();
        if (selected != null && selected instanceof Presentable) {

            System.out.println("dataFlavor="+Presentable.dataFlavor);
            TransferablePresentable tp = new TransferablePresentable((Presentable)selected);

            //StringSelection tp = new StringSelection("test");

            System.out.println("Start drag: "+event+" "+tp+" "+this);
            dragSource.startDrag(event, DragSource.DefaultCopyDrop, tp, this);
        } else {
            System.out.println("nothing was selected");
        }
    }

    /** This is invoked when you are dragging over the DropSite. */
    public void dragEnter(DropTargetDragEvent event) {
        DataFlavor[] flavors = event.getCurrentDataFlavors();
        int len = flavors.length;
        System.out.println("dtl dragEnter length="+len+" flavors="+flavors);
        for (int i=0;i<len;i++) {
            if (flavors[i].equals(Presentable.dataFlavor)) {
                System.out.println("  accept");
                event.acceptDrag(DnDConstants.ACTION_COPY);
                return;
            }
        }
        System.out.println("  !!REJECT!!");
        event.rejectDrag();
    }

    /** This is invoked when you are exit the DropSite without dropping. */
    public void dragExit(DropTargetEvent event) {
        System.out.println("dtl dragExit");
    }

    /** This is invoked when a drag operation is going on. */
    public void dragOver(DropTargetDragEvent event) {
        System.out.println("dtl dragOver");
    }

    /** A drop has occurred. */
    public void drop(DropTargetDropEvent event) {
        System.out.println("dtl Drop");
        try {
                Transferable transferable = event.getTransferable();
                if (!transferable.isDataFlavorSupported(Presentable.dataFlavor)) {
                    event.rejectDrop();
                    return;
                }
                event.acceptDrop(DnDConstants.ACTION_COPY);
                Presentable o = (Presentable)transferable.getTransferData(Presentable.dataFlavor);
                addElement(o);
                event.getDropTargetContext().dropComplete(true);
        } catch (IOException exception) {
            EventLog.report(exception);
            event.rejectDrop();
        } catch (UnsupportedFlavorException ufException) {
            EventLog.report(ufException);
            event.rejectDrop();
            event.rejectDrop();
        }
    }

    /** This is invoked if the use modifies the current drop gesture. */
    public void dropActionChanged (DropTargetDragEvent event) { 
        System.out.println("dtl dropActionChanged");
    }

    /** This message goes to DragSourceListener, informing it that the dragging
     *  has ended. */
    public void dragDropEnd(DragSourceDropEvent event) {   
        System.out.println("dtl dragDropEnd");
        //if (event.getDropSuccess()) removeElement();
    }

    /** This message goes to DragSourceListener, informing it that the dragging 
     * has entered the DropSite. */
    public void dragEnter(DragSourceDragEvent event) { 
        System.out.println("dsl dragEnter");
    }

    /** This message goes to DragSourceListener, informing it that the dragging 
     * has exited the DropSite. */
    public void dragExit(DragSourceEvent event) { 
        System.out.println("dsl dragExit"); 
    }

    /** This message goes to DragSourceListener, informing it that the
     * dragging is currently ocurring over the DropSite. */
    public void dragOver(DragSourceDragEvent event) {
        System.out.println("dsl dragOver getTargetActions="+event.getTargetActions());
        if (event.getTargetActions()!=0) {
            // Accept
            event.getDragSourceContext().setCursor(DragSource.DefaultCopyDrop);
        } else {
            // Reject
            event.getDragSourceContext().setCursor(DragSource.DefaultCopyNoDrop);
        }
    }

    /** This is invoked when the user changes the dropAction. */
    public void dropActionChanged( DragSourceDragEvent event) { 
        System.out.println("dsl dropActionChanged");
    }

    /** Add elements. */
    public void addElement(Object o) {
        System.out.println("model="+getModel());
        ((DefaultListModel)getModel()).addElement(o);
        //getModel().addElement(o);
    }

    /** Remove an element. */
    public void removeElement(Object o) {
        ((DefaultListModel)getModel()).removeElement(o);
        //getModel().removeElement(o);
    }
  
}
