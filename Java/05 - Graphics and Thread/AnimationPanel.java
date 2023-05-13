import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.util.ArrayList;
import java.security.SecureRandom;
import javax.swing.JPanel;

/*******************************************************************************
This class displays the animation in a separate thread.
*******************************************************************************/
public class AnimationPanel extends JPanel implements Runnable {
    private ArrayList<Ball> ballList = new ArrayList<>();
    private Dimension dim = new Dimension();

    Thread animationThread;  // embed a thread in the class
    
    SecureRandom ranNum = new SecureRandom();
    Color randomCol;
    
    /**
     * Constructor for the class
     */
    public AnimationPanel()
    {
        animationThread = null;
        dim = null;
    }
    
    public void startThread()
    {
        if (animationThread == null)
        {
            animationThread = new Thread(this);
            animationThread.start();    // responsible for start the thread
        }
    }
    
    public void stopThread()
    {
        animationThread = null;
    }
    
    @Override
    protected void paintComponent(Graphics g)
    {
        super.paintComponent(g);
        
        if (dim == null)
        {
            // create a set of balls
            // int numBalls = ranNum.nextInt(5)+7; // 7 to 12 balls
            int numBalls = 5000; // 7 to 12 balls
            int rad,x,y,dx,dy;
            for (int i=0;i<numBalls;i++)
            {
                // rad = ranNum.nextInt(40)+10;  // radius 10 to 50
                rad = 5;  // radius 10 to 50
                
                // position balls around center
                x = 150 - 50 + ranNum.nextInt(100);
                y = 250 - 50 + ranNum.nextInt(100);
                dx = 1+ranNum.nextInt(6);
                dy = 1+ranNum.nextInt(12);
                randomCol = new Color(ranNum.nextInt(255),ranNum.nextInt(255),ranNum.nextInt(255));
                ballList.add(new Ball(randomCol,rad,x,y,dx,dy));
            }
            
            // draw the blue background
            Color blueSky = new Color(135,206,235);
            this.setBackground(blueSky);
        }

        // get the dimension of the panel
        dim = this.getSize();
        
        // call the move() method for each ball, passing the panel dimension
        for (Ball b : ballList)
        {
            b.move(dim);
        }
        
        // call the draw() method for each ball, passing the Graphics context
        for (Ball b : ballList)
        {
            b.draw(g);
        }
    }
    
    public void run()
    {
        while (Thread.currentThread() == animationThread)
        {
            try {
                repaint();
                Thread.sleep(1000/60);  // 60 frames per second
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
