import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.image.BufferedImage;
import javax.swing.*;

/*******************************************************************************
The Tile Canvas class is responsible for the center area. It draws the grids, 
handles mouse events and draw image in respond with the mouse events.
*******************************************************************************/
public class TileCanvas extends JPanel implements MouseListener {
    private static final int TILE_SIZE = 25;
    private static final int NUM_TILES = 5;
    private static int tileX;
    private static int tileY;
    private int selectedSquareX = -1; // X-coordinate of selected square
    private int selectedSquareY = -1; // Y-coordinate of selected square
    private BufferedImage [][] tileImageAr = new BufferedImage [5][5];

    /**
     * Constructor for the class
     */
    public TileCanvas() 
    {
        setPreferredSize(new Dimension(NUM_TILES * TILE_SIZE, NUM_TILES * TILE_SIZE));
        addMouseListener(this);
    }

    @Override
    public void paintComponent(Graphics g) 
    {
        super.paintComponent(g);
        
        // draw tiles
        for (int i = 0; i < NUM_TILES; i++)
            for (int j = 0; j < NUM_TILES; j++) 
            {
                BufferedImage copiedImage = null;
                copiedImage = TileDesigner.getcopiedImage();
                
                // draw grids
                g.setColor(Color.LIGHT_GRAY);
                g.fillRect(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE);
                g.setColor(Color.BLACK);
                g.drawRect(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE);

                // draw the previous chosen tiles
                if (tileImageAr[i][j] != null)
                    g.drawImage(tileImageAr[i][j], i * TILE_SIZE, j * TILE_SIZE, null);
                
                // draw the select pattern
                if (i == selectedSquareX && j == selectedSquareY)
                {
                    if (copiedImage != null) 
                    {
                        // Clear tile if it's already filled with image
                        if (tileImageAr[i][j] != null)
                        {
                            g.setColor(Color.LIGHT_GRAY);
                            g.fillRect(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE);
                        }
                        // Draw the copied image on the selected square
                        g.drawImage(copiedImage, i * TILE_SIZE, j * TILE_SIZE, null);
                        // Save the tile pattern at the position to the tileImageAr array
                        tileImageAr[i][j] = copiedImage;
                    }
                    // reset values of selected square
                    selectedSquareX = -1;
                    selectedSquareY = -1;
                }
            }
    }

    /**
     * This method records the square clicked by the mouse
     */
    @Override
    public void mouseClicked(MouseEvent e) {
        int x = e.getX();
        int y = e.getY();
        tileX = x / TILE_SIZE;
        tileY = y / TILE_SIZE;
        if (tileX >= 0 && tileX < 10 && tileY >= 0 && tileY < 10) {
            selectedSquareX = tileX;
            selectedSquareY = tileY;
        }
        repaint();
    };

    /**
     * This method clear the canvas when user clicks the RESET button.
     */
    public void clearCanvas()
    {
        for (int i = 0; i < NUM_TILES; i++)
            for (int j = 0; j < NUM_TILES; j++) 
            {
                // reset the tileImage array
                tileImageAr[i][j] = null;
                // reset the selectedSquare values
                selectedSquareX = -1;
                selectedSquareY = -1;
                // reset the previous chosen pattern
                TileDesigner.clearCopiedImage();
            }
        repaint();
    }

    // implement other MouseListener methods
    @Override
    public void mousePressed(MouseEvent e) {}
    @Override
    public void mouseReleased(MouseEvent e) {}
    @Override
    public void mouseEntered(MouseEvent e) {}
    @Override
    public void mouseExited(MouseEvent e) {}
}