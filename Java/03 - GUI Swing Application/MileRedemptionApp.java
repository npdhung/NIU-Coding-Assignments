/*******************************************************************************
CSCI470 - Assignment 3 - Spring 2023

Programmer: Nguyen Le (z1940550) & Hung Nguyen (z1924897)
Section:    0001
Due Date:   03/20/2023

Name:       Mile Redemption GUI App.

Purpose:    Re-implement the Air Ticket Redeemer as a Java Swing Application.
*******************************************************************************/
import java.util.Scanner;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JSpinner;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.SpinnerListModel;
import javax.swing.SwingConstants;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.Dimension;

import java.util.Date;
import java.util.Calendar;
import java.text.SimpleDateFormat;
import java.text.ParseException;
import java.util.Locale;

import java.io.File; 
import java.io.IOException; 

/*******************************************************************************
Demonstrate the main class MileRedemption App
This class answerains the loop for user interaction. It gets file name from user 
input, then print out the avalaible destination information. The class then 
reads the data of available miles and travel month, then provide the list of 
tickets that may be redeemed and the remaining miles. 
*******************************************************************************/
public class MileRedemptionApp extends JFrame
{
    private static Scanner fileScanner = null;
    private static String[] destinationCity = null;
    private static String[] listOfTicket = null;
    private static String ticket = "";

    private static MileRedeemer mileRedeemer = new MileRedeemer();
    Destination destination = null;

    final JLabel comment = new JLabel("\nYou can redeem the following tickets:");
    final JLabel remMilesLabel = new JLabel("\nYour Remaining Miles");
    JTextArea ticketArea = new JTextArea();
    JList listBox;
    JButton button;
    JTextField text1, text2, text3, text4;
    JTextField remMiles = new JTextField("", 10);

    /**
     * Constructor for the MileRedemption class
     */
    public MileRedemptionApp()
    {
        // set the container's properties
        this.setSize(700, 360);
        this.setVisible(true);
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setLayout(new GridLayout(0, 2));
        this.setBackground(Color.LIGHT_GRAY);
        this.setResizable(false);

        // create left and right parts
        JPanel leftPane = new JPanel();
        JPanel rightPane = new JPanel();
        this.add(leftPane);
        this.add(rightPane);
        leftPane.setBackground(Color.GRAY);
        rightPane.setBackground(Color.GRAY);
        leftPane.setLayout(new GridLayout(2,0,20,5));
        rightPane.setLayout(new GridLayout(2,0,20,5));

        // set titles for the left and right panels
        leftPane.setBorder(BorderFactory.createTitledBorder("List of Destination Cities"));
        rightPane.setBorder(BorderFactory.createTitledBorder("Redeem Tickets"));

        // create 4 panels to display the required information
        final JPanel leftTopPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        leftTopPanel.setBackground(Color.LIGHT_GRAY);
        leftPane.add(leftTopPanel);
        final JPanel leftBottomPanel = new JPanel(new GridLayout(0, 2));
        leftBottomPanel.setBackground(Color.LIGHT_GRAY);
        leftPane.add(leftBottomPanel);
        final JPanel rightTopPanel = new JPanel(new FlowLayout(FlowLayout.CENTER));
        rightTopPanel.setBackground(Color.LIGHT_GRAY);
        rightPane.add(rightTopPanel);
        final JPanel rightBottomPanel = new JPanel(new FlowLayout(FlowLayout.CENTER));
        rightBottomPanel.setBackground(Color.PINK);
        rightPane.add(rightBottomPanel);

        // create a new list box to display destination cities
        final JList<String> listBox = new JList<>(destinationCity);
        listBox.setFixedCellWidth(330);
        leftTopPanel.add(listBox);

        // create panels for label and text fields for the left panel
        JPanel labelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT,0,13));
        labelPanel.setBackground(Color.LIGHT_GRAY);
        leftBottomPanel.add(labelPanel);
        JPanel textFieldPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT,0,5));
        textFieldPanel.setBackground(Color.LIGHT_GRAY);
        leftBottomPanel.add(textFieldPanel);

        // create additional labels and text fields and set font and size
        JLabel label1 = new JLabel("Required Miles");
        text1 = new JTextField("", 14);
        JLabel label2 = new JLabel("Miles for upgrading");
        text2 = new JTextField("", 14);
        JLabel label3 = new JLabel("Miles for SuperSaver");
        text3 = new JTextField("", 14);
        JLabel label4 = new JLabel("Months for SuperSaver");
        text4 = new JTextField("", 14);

        // set text field to non editable
        text1.setEditable(false);
        text2.setEditable(false);
        text3.setEditable(false);
        text4.setEditable(false);

        // add labels and text fields to the panels
        labelPanel.add(label1);
        textFieldPanel.add(text1);
        labelPanel.add(label2);
        textFieldPanel.add(text2);
        labelPanel.add(label3);
        textFieldPanel.add(text3);
        labelPanel.add(label4);
        textFieldPanel.add(text4);

        listBox.addListSelectionListener(new ListSelectionListener() {
            // this method is invoked when a city in the list is chosen
            public void valueChanged(ListSelectionEvent event) {
                // return if the value is changed
                if (event.getValueIsAdjusting()) return;
                // get miles information of a city
                destination = 
                    mileRedeemer.getDestInfo(listBox.getSelectedValue().toString());
                // set the content in the text fields accordingly
                text1.setText(" "+String.valueOf(destination.getNormalMiles()));
                text2.setText(" "+String.valueOf(destination.getAddMiles()));
                text3.setText(" "+String.valueOf(destination.getSsaverMiles()));
                text4.setText(" "+getMonthName(destination.getStartMonth())
                    +" to "+getMonthName(destination.getEndMonth()));
            }
        });

        // set the label and text field for the accumulated miles
        JLabel accumulatedMiles = new JLabel("Your Accumulated Miles");
        final JTextField accumulatedMilesText = new JTextField("", 10);

        // set the label and spinner for departure month
        JLabel departuredMonth = new JLabel("Month of Departure");
        SpinnerListModel monthModel = new SpinnerListModel(getMonthStrings());
        final JSpinner spinner = new JSpinner(monthModel);
        spinner.setPreferredSize(new Dimension(130, 20));

        // create button that invokes the redeemMiles method
        JButton button = new JButton("Redeem Tickets >>>");

        // add fields the panel
        rightTopPanel.add(accumulatedMiles);
        rightTopPanel.add(accumulatedMilesText);
        rightTopPanel.add(departuredMonth);
        rightTopPanel.add(spinner);
        rightTopPanel.add(button);
        
        button.addActionListener(new ActionListener() {
            // method that is invoked when the button is clicked
            public void actionPerformed(ActionEvent arg0) {
                // add comment section to the panel
                rightBottomPanel.add(comment);
                rightBottomPanel.add(ticketArea);
                // set the redeemable tickets to null
                ticket = "";
                // initialize the visibility of elements to false
                comment.setVisible(false);
                ticketArea.setVisible(false);
                remMilesLabel.setVisible(false);
                remMiles.setVisible(false);
                // get month value in number from spinner
                int month = getMonthNumber((String) spinner.getValue());
                // get miles from the text field anf check its validity
                String milesValue = accumulatedMilesText.getText();
                // validMiles need to be number with at least one charater
                boolean validMiles = milesValue.matches("\\d+");
                // continue if it is a number
                if (validMiles) 
                {
                    // convert mile values to int
                    int accumulatedMiles = Integer.parseInt(milesValue);
                    // get the list of redeemed tickets
                    listOfTicket = mileRedeemer.redeemMiles(accumulatedMiles, month);
                    // set comment ection to visible
                    comment.setVisible(true);
                    // print all the redeemable tickets
                    if (listOfTicket.length > 0)
                    {
                        for (String t:listOfTicket)
                            if (t != null) ticket = ticket.concat(t+"\n");
                        ticket = ticket.concat("Enjoy your trip!");
                        remMiles.setVisible(true);
                        remMilesLabel.setVisible(true);
                    }
                    else
                    {
                        comment.setVisible(false);
                        remMilesLabel.setVisible(false);
                        ticket = "You need more miles to redeem a ticket.";
                    }    
                    // set the text area with the ticket information
                    ticketArea.setText(ticket);
                    // add ticketArea to the panel and set it to be visible
                    ticketArea.setVisible(true);
                    // add Rem miles label to the panel and set it to be visible
                    rightBottomPanel.add(remMilesLabel);
                    // set remMiles text to visible and non-editable and add it in
                    remMiles.setText(""+ mileRedeemer.getRemainMiles());
                    remMiles.setEditable(false);
                    rightBottomPanel.add(remMiles);
                }
                else // if mile value is not a valid number, prompt user to re-enter
                {
                    comment.setVisible(false);
                    remMilesLabel.setVisible(false);
                    ticket = "Please enter a number of Accumulated Miles.";
                    ticketArea.setText(ticket);
                    ticketArea.setVisible(true);
                }
            }
        });
    }
    
    public static void main(String[] args) throws IOException
    {
        // check the number of arguments passed to the method
        if (args.length > 0)
            // store input file
            fileScanner = new Scanner(new File(args[0]));
        else
        {
            System.err.println("Please provide the name of the input file.");
            System.exit(0);
        }
        
        mileRedeemer.readDestination(fileScanner);
        destinationCity = mileRedeemer.getCityNames();
        // Swing Utility method to synchronizee with user's action
        javax.swing.SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                // create an instance of the test application
                new MileRedemptionApp();
            }
        });
    }
        
    /**
     * This method reads the month name and returns its corresponding number.
     */
    public int getMonthNumber(String monthName) 
    {
        Date date = null;
        try 
        {
            date = new SimpleDateFormat("MMM", Locale.ENGLISH)
            .parse(monthName);
        } 
        catch (ParseException e) 
        {
            System.out.println(e.getMessage());
        }
        Calendar cal = Calendar.getInstance();
        cal.setTime(date);
        int month = cal.get(Calendar.MONTH) + 1;
        return month;
    }

    /**
     * This method populate the month Strings for the Spinner
     */
    protected String[] getMonthStrings()
    {
        String[] months = new java.text.DateFormatSymbols().getMonths();
        int lastIndex = months.length - 1;
        if (months[lastIndex]==null || months[lastIndex].length()<=0)
        {   // last item is empty
            String[] monthStrings = new String[lastIndex];
            System.arraycopy(months, 0, monthStrings, 0, lastIndex);
            return monthStrings;
        }
        else    // last item is not empty
            return months;
    }
    
    /**
     * This method reads in the month number and return the corresponding month name
     */
    public String getMonthName(int month) 
    {
        String[] monthNameArr = {"January", "February", "March", "April", "May", 
        "June", "July", "August", "September", "October", 
        "November", "Devember"};
        String monthName = "Invalid Month";
        if ((month >= 1)&&(month <= 12)) monthName = monthNameArr[month-1];
        return monthName;
    }
}