//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : TransferablePresentable.java
 *
 * I couldn't find a class that implements the Transferable interface for
 * transfering arbitrary object instances within the same JVM, so I built one.
 *
 * CHANGELOG:
 * 01/01/00    Mhs       Created
 */

package gov.nasa.arc.l2tools.gutil;

import java.awt.datatransfer.*;
import java.awt.dnd.*;

/** I couldn't find a class that implements the Transferable interface for
  transfering arbitrary object instances within the same JVM, so I built
  one specifically for presentables. */
public class TransferablePresentable implements Transferable {

    Presentable presentable;

    public TransferablePresentable(Presentable presentable) {
        this.presentable = presentable;
    }

    public Object getTransferData(DataFlavor flavor) {
        System.out.println("in getTransferData");
        return Presentable.dataFlavor.equals(flavor) ? presentable : null;
    }

    private static DataFlavor[] allowedFlavors;

    public DataFlavor[] getTransferDataFlavors() {
        System.out.println("in getTransferDataFlavors");
        System.out.println("allowed flavors="+allowedFlavors);
        return allowedFlavors;
    }

    public boolean isDataFlavorSupported(DataFlavor flavor) {
        System.out.println("in isDataFlavorSupported");
        return Presentable.dataFlavor.equals(flavor);
    }

    static {
        allowedFlavors = new DataFlavor[1];
        allowedFlavors[0] = Presentable.dataFlavor;
    }

}
