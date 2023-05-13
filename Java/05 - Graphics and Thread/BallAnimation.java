import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JPanel;

/*******************************************************************************
This class contains a pair of buttons to start and stop the animation and an 
Animation Panel to display animation. This class also sets up and adds listeners
for the buttons.
*******************************************************************************/
public class BallAnimation extends JPanel{
    
    protected JButton startButton;
    protected JButton stopButton;
    protected AnimationPanel aniPane;

    public BallAnimation(){
        
        // create the Animation Panel
        aniPane = new AnimationPanel();
    
        // add Start and Stop button
        startButton = new JButton("Start");
        stopButton = new JButton("Stop");
        stopButton.setEnabled(false);
        
        startButton.addActionListener((ActionListener) new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                aniPane.startThread();
                startButton.setEnabled(false);
                stopButton.setEnabled(true);
            }
        });
        
        stopButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                aniPane.stopThread();
                stopButton.setEnabled(false);
                startButton.setEnabled(true);
            }
        });
    }
}
