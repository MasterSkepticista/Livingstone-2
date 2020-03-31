// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * SchematicView.java
 *
 * Created on October 1, 2002, 11:51 AM
 */

package oliver.schematic;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.io.*;
import CH.ifa.draw.framework.*;
import CH.ifa.draw.standard.*;
import CH.ifa.draw.figures.*;
import CH.ifa.draw.util.*;

/**
 *
 * @author  stephen
 */
public class DrawingEditorPanel extends javax.swing.JPanel implements DrawingEditor, PaletteListener {
    
    private oliver.project.ProjectModel model;
    private oliver.application.UserProperties userProperties;
    
    /** Creates a new instance of DrawingEditorPanel */
    public DrawingEditorPanel() {
        super();
        open();
    }
    
    private Drawing              fDrawing;
    private Tool                 fTool;
    private Iconkit              fIconkit;
    
    private JTextField           fStatusLine;
    private StandardDrawingView  fView;
    private ToolButton           fDefaultToolButton;
    private ToolButton           fSelectedToolButton;
    
    private String               fApplicationName;
    private String               fDrawingFilename;
    private StorageFormatManager fStorageFormatManager;
    static String                fgUntitled = "untitled";
    
    // the image resource path
    private static final String fgDrawPath = "/CH/ifa/draw/";
    public static final String IMAGES = fgDrawPath + "images/";
    
    /**
     * The index of the file menu in the menu bar.
     */
    public static final int    FILE_MENU = 0;
    /**
     * The index of the edit menu in the menu bar.
     */
    public static final int    EDIT_MENU = 1;
    /**
     * The index of the alignment menu in the menu bar.
     */
    public static final int    ALIGNMENT_MENU = 2;
    /**
     * The index of the attributes menu in the menu bar.
     */
    public static final int    ATTRIBUTES_MENU = 3;
    
    /**
     * Opens the window and initializes its contents.
     * Clients usually only call but don't override it.
     */
    public void open() {
        fIconkit = new Iconkit(this);
        setLayout(new BorderLayout());
        // Panel in which a JToolBar can be placed using a BoxLayout
        JPanel fullPanel = new JPanel();
        fullPanel.setLayout(new BoxLayout(fullPanel, BoxLayout.X_AXIS));
        fView = createDrawingView();
        JComponent contents = createContents((StandardDrawingView)view());
        contents.setAlignmentX(LEFT_ALIGNMENT);
        
        JToolBar tools = createToolPalette();
        createTools(tools);
        
        JPanel activePanel = new JPanel();
        activePanel.setAlignmentX(LEFT_ALIGNMENT);
        activePanel.setAlignmentY(TOP_ALIGNMENT);
        activePanel.setLayout(new BorderLayout());
        activePanel.add(tools, BorderLayout.NORTH);
        activePanel.add(contents, BorderLayout.CENTER);
        
        fullPanel.add(activePanel);
        
        fStatusLine = createStatusLine();
        add(fullPanel, BorderLayout.CENTER);
        add(fStatusLine, BorderLayout.SOUTH);
        
        /*
        JMenuBar mb = new JMenuBar();
        createMenus(mb);
        setJMenuBar(mb);
         */
        initDrawing();
        
        Dimension d = defaultSize();
        //if (d.width > mb.getPreferredSize().width) {
        setSize(d.width, d.height);
        //}
        //else {
        //    setSize(mb.getPreferredSize().width, d.height);
        //}
        //addListeners();
        //setVisible(true);
        //fStorageFormatManager = createStorageFormatManager();
    }
    
    protected void initDrawing() {
        setDrawing(createDrawing());
        //setDrawingTitle(fgUntitled);
        view().setDrawing(drawing());
        toolDone();
    }
    
    
    // Gotta to use a thread for this
    private MouseOverThread mouseOverThread = null;
    private class MouseOverThread extends Thread {
        private java.awt.Point point;
        private boolean running = true;
        public MouseOverThread(java.awt.Point point) {
            super();
            setPriority(MIN_PRIORITY);
            this.point = point;
        }
        public void setRunning(boolean running) {
            this.running = running;
            if(!running) interrupt();
        }
        public void run() {
            try {
                sleep(100);
                if(running) {
                    Object obj = DrawingEditorPanel.this.view().drawing().findFigure(point.x, point.y);
                    if(running && obj != null && obj instanceof oliver.schematic.XmplFigure) {
                        String text = ((oliver.schematic.XmplFigure)obj).getMouseOverText();
                        showStatus(text);
                    }
                }
            } catch(InterruptedException e) {}
        }
    }
    
    private void mouseOverDrawing(java.awt.Point p) {
        if(mouseOverThread != null) mouseOverThread.setRunning(false);
        mouseOverThread = new MouseOverThread(p);
        mouseOverThread.start();
    }
    
    /**
     * Creates the tool palette.
     */
    protected JToolBar createToolPalette() {
        JToolBar palette = new JToolBar();
        palette.setBackground(Color.lightGray);
        // use standard FlowLayout for JToolBar
        // palette.setLayout(new PaletteLayout(2,new Point(2,2)));
        return palette;
    }
    
    /**
     * Creates the tools. By default only the selection tool is added.
     * Override this method to add additional tools.
     * Call the inherited method to include the selection tool.
     * @param palette the palette where the tools are added.
     */
    protected void createTools(JToolBar palette) {
        Tool tool = createSelectionTool();
        fDefaultToolButton = createToolButton(IMAGES+"SEL", "Selection Tool", tool);
        palette.add(fDefaultToolButton);
    }
    
    /**
     * Creates the selection tool used in this editor. Override to use
     * a custom selection tool.
     */
    protected Tool createSelectionTool() {
        return new SelectionTool(view());
    }
    
    /**
     * Creates a tool button with the given image, tool, and text
     */
    protected ToolButton createToolButton(String iconName, String toolName, Tool tool) {
        return new ToolButton(this, iconName, toolName, tool);
    }
    
    /**
     * Creates the drawing view used in this application.
     * You need to override this method to use a DrawingView
     * subclass in your application. By default a standard
     * DrawingView is returned.
     */
    protected StandardDrawingView createDrawingView() {
        Dimension d = getDrawingViewSize();
        StandardDrawingView v = new MyStandardDrawingView(this, d.width, d.height);
        v.addBackground(new MyBackgroundPainter());
        v.addMouseMotionListener(new java.awt.event.MouseMotionAdapter() {
            public void mouseMoved(java.awt.event.MouseEvent e) {
                DrawingEditorPanel.this.mouseOverDrawing(e.getPoint());
            }
        });
        return v;
    }
    
    /**
     * Override to define the dimensions of the drawing view.
     */
    protected Dimension getDrawingViewSize() {
        return new Dimension(1280, 1024);
    }
    
    /**
     * Creates the drawing used in this application.
     * You need to override this method to use a Drawing
     * subclass in your application. By default a standard
     * Drawing is returned.
     */
    protected Drawing createDrawing() {
        return new StandardDrawing();
    }
    
    /**
     * Creates the contents component of the application
     * frame. By default the DrawingView is returned in
     * a JScrollPane.
     */
    protected JComponent createContents(StandardDrawingView view) {
        JScrollPane sp = new JScrollPane(view);
        sp.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        sp.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
        
        return sp;
    }
    
    /**
     * Sets the drawing to be edited.
     */
    public void setDrawing(Drawing drawing) {
        view().setDrawing(drawing);
        fDrawing = drawing;
    }
    
    /**
     * Gets the default size of the window.
     */
    protected Dimension defaultSize() {
        return new Dimension(600,450);
    }
    
    /**
     * Creates the status line.
     */
    protected JTextField createStatusLine() {
        JTextField field = new JTextField("No Tool", 40);
        field.setBackground(Color.white);
        field.setEditable(false);
        return field;
    }
    
    /**
     * Handles a user selection in the palette.
     * @see PaletteListener
     */
    public void paletteUserSelected(PaletteButton button) {
        ToolButton toolButton = (ToolButton) button;
        setTool(toolButton.tool(), toolButton.name());
        setSelected(toolButton);
    }
    
    /**
     * Handles when the mouse enters or leaves a palette button.
     * @see PaletteListener
     */
    public void paletteUserOver(PaletteButton button, boolean inside) {
        ToolButton toolButton = (ToolButton) button;
        if (inside)
            showStatus(toolButton.name());
        else
            showStatus(fSelectedToolButton.name());
    }
    
    /**
     * Gets the current drawing.
     * @see DrawingEditor
     */
    public Drawing drawing() {
        return fDrawing;
    }
    
    /**
     * Gets the current tool.
     * @see DrawingEditor
     */
    public Tool tool() {
        return fTool;
    }
    
    /**
     * Gets the current drawing view.
     * @see DrawingEditor
     */
    public DrawingView view() {
        return fView;
    }
    
    /**
     * Sets the default tool of the editor.
     * @see DrawingEditor
     */
    public void toolDone() {
        if (fDefaultToolButton != null) {
            setTool(fDefaultToolButton.tool(), fDefaultToolButton.name());
            setSelected(fDefaultToolButton);
        }
    }
    
    /**
     * Handles a change of the current selection. Updates all
     * menu items that are selection sensitive.
     * @see DrawingEditor
     */
    public void selectionChanged(DrawingView view) {
        //JMenuBar mb = getJMenuBar();
        //CommandMenu editMenu = (CommandMenu)mb.getMenu(EDIT_MENU);
        //editMenu.checkEnabled();
        //CommandMenu alignmentMenu = (CommandMenu)mb.getMenu(ALIGNMENT_MENU);
        //alignmentMenu.checkEnabled();
    }
    
    
    /**
     * Shows a status message.
     * @see DrawingEditor
     */
    public void showStatus(String string) {
        fStatusLine.setText(string);
    }
    
    private void setTool(Tool t, String name) {
        if (tool() != null)
            tool().deactivate();
        fTool = t;
        if (tool() != null) {
            fStatusLine.setText(name);
            tool().activate();
        }
    }
    
    private void setSelected(ToolButton button) {
        if (fSelectedToolButton != null)
            fSelectedToolButton.reset();
        fSelectedToolButton = button;
        if (fSelectedToolButton != null)
            fSelectedToolButton.select();
    }
    
    private class MyBackgroundPainter implements CH.ifa.draw.framework.Painter {
        //private javax.swing.ImageIcon image = new javax.swing.ImageIcon("C:/PITEX_1_04/sshot.jpg");
        public MyBackgroundPainter() {
        }
        public void draw(java.awt.Graphics g, CH.ifa.draw.framework.DrawingView view) {
            int gridsize = oliver.xmpl.Project.getGridSize();
            java.awt.Rectangle clip = g.getClipBounds();
            if(clip == null) {
                clip = new java.awt.Rectangle(0,0);
                clip.setSize(view.getSize());
            }
            g.clearRect(clip.x,clip.y,clip.width,clip.height);
            //g.drawImage(image.getImage(),0,0,image.getImageObserver());
            g.setColor(java.awt.Color.white);
            for(int x = (clip.x/(gridsize*2))*gridsize*2; x < clip.x+clip.width; x += gridsize*2) {
                g.drawLine(x,clip.y,x,clip.y+clip.height);
            }
            for(int y = (clip.y/(gridsize*2))*gridsize*2; y < clip.y+clip.height; y += gridsize*2) {
                g.drawLine(clip.x,y,clip.x+clip.width,y);
            }
        }
    }
    
}
