import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;

/*******************************************************************************
This class represents a single bouncing ball in the animation.
*******************************************************************************/
public class Ball {
    private Color color;
    private int radius;
    private int x,y;
    private int dx,dy;

    /**
     * Constructor for the class
     * Create a ball depending on color, radius, x and y coordinations, and dx 
     * and dx which are the amount of change when the ball moves.
     */
    public Ball(Color c, int r, int xVal, int yVal, int dxVal, int dyVal)
    {
        color = c;
        radius = r;
        x = xVal;
        y = yVal;
        dx = dxVal;
        dy = dyVal;
    }

    /**
     * Method move simulates the moving of a ball
     */
    public void move(Dimension panelDim)
    {
        if ((x <= radius)||(x >= panelDim.width-radius))
        {
            dx *= -1;
        }
        if ((y <= radius)||(y >= panelDim.height-radius))
        {
            dy *= -1;
        }
        x += dx;
        y += dy;
    }

    /**
     * This method draws the ball on an off-screen image using its color and 
     * position coordinates.
     */
    public void draw(Graphics g)
    {
        g.setColor(color);
        g.fillOval(x-radius, y-radius, 2*radius, 2*radius);
    }
}
