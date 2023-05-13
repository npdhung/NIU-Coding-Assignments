/*******************************************************************************
CSCI470 - Assignment 5 - Spring 2023

Programmer: Nguyen Le (z1940550) & Hung Nguyen (z1924897)
Section:    0001
Due Date:   04/22/2023

Name:       Tile Designer GUI App with added animation.

Purpose:    Add an animation that run in a background thread to the GUI 
            application of assignment 4.
*******************************************************************************/
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;

/*******************************************************************************
Demonstrate the main class Tile Designer
This class interacts with user throught an GUI interface. User can use a mouse 
to pick the tile pattern and then pick a spot for that pattern. It also includes
a RESET button to restart the process.
This class also include the Start and Stop button to start/resume or pause the 
animation.
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
        imAr = new Image[7];
        Toolkit toolkit = Toolkit.getDefaultToolkit();
        for (int i=0;i < imAr.length;i++)
        {
            imAr[i] = toolkit.getImage("pat"+(i+1)+".gif");
        }

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
        
        JPanel canvasPane = new JPanel();
        canvasPane.setBorder(BorderFactory.createEmptyBorder(150,0,0,0));
        canvasPane.add(canvas);

        JPanel centerPane = new JPanel(new GridLayout(1,2));
        centerPane.add(canvasPane);
        
        // create the BallAnimation object
        BallAnimation ballAnimationPanel = new BallAnimation();
        centerPane.add(ballAnimationPanel.aniPane);

        frame.getContentPane().add(centerPane,BorderLayout.CENTER);
        
        // add Reset button
        JButton resetButton = new JButton("Reset");
        resetButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                canvas.clearCanvas();
            }
        });

        JPanel leftBottomPane = new JPanel();
        resetButton.setPreferredSize(new Dimension(80, 30));
        leftBottomPane.add(resetButton);
        
        JPanel rightBottomPane = new JPanel();
        rightBottomPane.add(ballAnimationPanel.startButton);
        rightBottomPane.add(ballAnimationPanel.stopButton);

        JPanel bottomPane = new JPanel(new GridLayout(1, 2));
        bottomPane.add(leftBottomPane);
        bottomPane.add(rightBottomPane);

        frame.getContentPane().add(bottomPane,BorderLayout.SOUTH);
        bottomPane.setBorder(BorderFactory.createLineBorder(Color.GRAY));
        frame.setSize(600,620);
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

