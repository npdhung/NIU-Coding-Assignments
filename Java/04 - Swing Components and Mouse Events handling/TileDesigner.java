/*******************************************************************************
CSCI470 - Assignment 4 - Spring 2023

Programmer: Nguyen Le (z1940550) & Hung Nguyen (z1924897)
Section:    0001
Due Date:   04/06/2023

Name:       Tile Designer GUI App.

Purpose:    Write a GUI application that allows user to do tile design, pratice
            painting (draw lines and images), use advanced Swing component such 
            as JToolBar and handling mouse events.
*******************************************************************************/
import javax.swing.*;
import javax.swing.plaf.basic.BasicInternalFrameTitlePane.RestoreAction;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.FlowLayout;
import java.util.concurrent.Flow;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;

/*******************************************************************************
Demonstrate the main class Tile Designer
This class interacts with user throught an GUI interface. User can use a mouse 
to pick the tile pattern and then pick a spot for that pattern. It also includes
a RESET button to restart the process. 
*******************************************************************************/
public class TileDesigner {
    private static BufferedImage copiedImage;
    public static Image[] imAr;

    /**
     * Constructor for the TileDesigner class
     */
    public  TileDesigner() {
        JFrame frame = new JFrame("Tile Designer");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        JPanel topPanel = new JPanel();
        topPanel.setLayout(new FlowLayout(FlowLayout.LEFT));

        // create top Tool Bar
        imAr = new Image[4];
        Toolkit toolkit = Toolkit.getDefaultToolkit();
        imAr[0] = toolkit.getImage("img/pat1.gif");
        imAr[1] = toolkit.getImage("img/pat2.gif");
        imAr[2] = toolkit.getImage("img/pat3.gif");
        imAr[3] = toolkit.getImage("img/pat4.gif");

        for (int i = 0; i < imAr.length; i++) 
        {
            JButton tile = new JButton(new ImageIcon(imAr[i]));
            tile.addActionListener(new ActionListener() 
            {
                @Override
                public void actionPerformed(ActionEvent e) {
                    // Create a sample image of a button
                    BufferedImage image = new BufferedImage(tile.getWidth(), tile.getHeight(), BufferedImage.TYPE_INT_ARGB);
                    Graphics g = image.getGraphics();
                    tile.paint(g);
                    g.dispose();

                    // Copy the image
                    copiedImage = new BufferedImage(image.getWidth(), image.getHeight(), image.getType());
                    Graphics2D g2d = copiedImage.createGraphics();
                    g2d.drawImage(image, 0, 0, null);
                    g2d.dispose();
                }
            });
            tile.setPreferredSize(new Dimension(25, 25));
            tile.setBorder(BorderFactory.createLineBorder(Color.BLACK));
            topPanel.add(tile);
        }

        topPanel.setBorder(BorderFactory.createLineBorder(Color.GRAY));
        frame.getContentPane().add(topPanel,BorderLayout.NORTH);
        
        // create the Tile Canvas
        TileCanvas canvas = new TileCanvas();
        JPanel centerPanel = new JPanel();
        centerPanel.setLayout(new FlowLayout());
        centerPanel.add(canvas);
        centerPanel.setBorder(BorderFactory.createEmptyBorder(50,50,50,50));
        frame.getContentPane().add(centerPanel,BorderLayout.CENTER);
        
        // create the RESET button
        JButton resetButton = new JButton("RESET");
        resetButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                canvas.clearCanvas();
            }
        });

        JPanel bottomPanel = new JPanel();
        bottomPanel.add(resetButton);
        resetButton.setPreferredSize(new Dimension(80, 30));
        frame.getContentPane().add(bottomPanel,BorderLayout.SOUTH);
        bottomPanel.setBorder(BorderFactory.createLineBorder(Color.GRAY));
        frame.setSize(360,340);
        frame.setVisible(true);
    }
    
    /**
     * This method returns the tile pattern that was picked.
     */
    public static BufferedImage getcopiedImage() {
        return copiedImage;
    }
    
    /**
     * This method clears the previous picked pattern when user click the RESET 
     * button
     */
    public static void clearCopiedImage() {
        copiedImage = null;
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                new TileDesigner();
            }
        });
    }
}

