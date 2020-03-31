//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.gui;

/**
 * Creation date: (10/10/2000 12:38:49 PM)
 * @author: Andrew Bachmann
 */
public class Application {
	private Environment environment=new Environment();
/**
 * Application constructor comment.
 */
public Application() {
	super();
}
/**
 * Creation date: (10/10/2000 4:41:17 PM)
 * @return gov.nasa.arc.l2tools.gui.Environment
 */
public Environment getEnvironment() {
	return environment;
}
/**
 * Creation date: (10/10/2000 12:39:30 PM)
 * @param args java.lang.String[]
 */
public static void main(String[] args) {
	try {
		Boolean GUI=null;
		/* parse the command line arguments into variables */

		/* read defaults out of file */

		/* create the Skunk Works application */	
		Application application = new Application();
		/* initialize the application with the command line variables or file defaults */
		if (GUI!=null) {
			application.getEnvironment().setGUI(GUI);
		}
		/* run the application */
		application.run();
	} catch (Throwable exception) {
		System.err.println("Exception occurred in main() of Application");
		exception.printStackTrace(System.out);
	}
}
/**
 * Creation date: (9/22/2000 4:16:58 PM)
 */
public void run() {
	if (getEnvironment().isGUI().booleanValue()) {
		runGUI();
	}
}
/**
 * Creation date: (9/22/2000 4:16:58 PM)
 */
public void runGUI() {
	try { // Try for metal look and feel first
		javax.swing.UIManager.setLookAndFeel(new javax.swing.plaf.metal.MetalLookAndFeel());
	} catch (javax.swing.UnsupportedLookAndFeelException noMetalLookAndFeelException) {
		try { // Try for cross platform look and feel next
			javax.swing.UIManager.setLookAndFeel(javax.swing.UIManager.getCrossPlatformLookAndFeelClassName());
		} catch (Exception noCrossPlatformLookAndFeelException) {
			try { // Try for system look and feel next
				javax.swing.UIManager.setLookAndFeel(javax.swing.UIManager.getSystemLookAndFeelClassName());
			} catch (Exception noSystemLookAndFeelException) {
				// Accept default
			}
		}
	}
	gov.nasa.arc.l2tools.swing.Frame window=new gov.nasa.arc.l2tools.swing.Frame();
//	window.setSize(java.awt.Toolkit.getDefaultToolkit().getScreenSize());
	WorkspaceGUI workspaceGUI=new WorkspaceGUI(window);
	// Add a windowListener for the windowClosedEvent
	window.addWindowListener(new java.awt.event.WindowAdapter() {
		public void windowClosed(java.awt.event.WindowEvent e) {
			System.exit(0);
		};
	});
	window.setVisible(true);
	javax.swing.JOptionPane.showInternalMessageDialog(workspaceGUI.getView().getContentPane(),
		"Welcome to Skunk Works Desktop.","Hello",javax.swing.JOptionPane.PLAIN_MESSAGE);
/*	
		// Calculate the screen size
		java.awt.Dimension screenSize = java.awt.Toolkit.getDefaultToolkit().getScreenSize();

		// Create the splash screen
		ApplicationSplashScreen aApplicationSplashScreen = new ApplicationSplashScreen();
		aApplicationSplashScreen.pack();

		// Center splash screen
		Dimension splashScreenSize = aApplicationSplashScreen.getSize();
		if (splashScreenSize.height > screenSize.height)
				splashScreenSize.height = screenSize.height;
		if (splashScreenSize.width > screenSize.width)
				splashScreenSize.width = screenSize.width;
		aApplicationSplashScreen.setLocation((screenSize.width - splashScreenSize.width) / 2, (screenSize.height - splashScreenSize.height) / 2);
		aApplicationSplashScreen.setVisible(true);
		try {;
				Thread.sleep(3000);
		} catch (InterruptedException ie) {};
		aApplicationSplashScreen.dispose();
		*/
}
/**
 * Creation date: (10/10/2000 4:41:17 PM)
 * @param newEnvironment gov.nasa.arc.l2tools.gui.Environment
 */
protected void setEnvironment(Environment newEnvironment) {
	environment = newEnvironment;
}
}
