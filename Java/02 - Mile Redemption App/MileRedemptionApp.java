/*******************************************************************************
CSCI470 - Assignment 2 - Spring 2023

Programmer: Nguyen Le (z1940550) & Hung Nguyen (z1924897)
Section:    0001
Due Date:   02/19/2023

Name:       Mile Redemption App.

Purpose:    Write a console-based Java application to help an airline frequent 
            flyer member find out good ways of redeeming their accumulated 
            mileage into air tickets.
*******************************************************************************/
import java.util.Scanner; 
import java.io.File; 
import java.io.IOException; 

/*******************************************************************************
Demonstrate the main class MileRedemption App
This class answerains the loop for user interaction. It gets file name from user 
input, then print out the avalaible destination information. The class then 
reads the data of available miles and travel month, then provide the list of 
tickets that may be redeemed and the remaining miles. Finally, it prompts the
user input of whether to answerinue with the app.
*******************************************************************************/
public class MileRedemptionApp
{
    public static void main(String[] args) throws IOException
    {
        int miles,  // Avalaible Miles entered by the user
            month;  // Month of departure
        char answer = 'y'; 
        Scanner inputScan = new Scanner(System.in);

        // A loop for user interaction
        do 
        {
            try
            {
                answer = 'n';
                System.out.println("Enter the name of the input file:");
                String input = inputScan.next();

                Scanner fileInput = new Scanner(new File(input));
                MileRedeemer redeemObj = new MileRedeemer();

                redeemObj.readDestination(fileInput);
                System.out.println("\n---------------------------------------------");
                System.out.println("List of destination cities you can travel to: ");
                
                // Print the city names
                for (String str: redeemObj.getCityNames()) 
                {
                    System.out.println(str);
                }
                
                System.out.println("\n---------------------------------------------");
                System.out.print("\nPlease input your total accumulated miles: ");
                miles = inputScan.nextInt();

                System.out.print("\nPlease input your month of departure (1-12): ");
                month = inputScan.nextInt();
                System.out.println("");
                
                //Print values returned by redeemMiles method
                for(String str:redeemObj.redeemMiles(miles, month))
                {
                    System.out.println(str);
                }
                
                System.out.println("\nYour remaining miles: "
                  +redeemObj.getRemainMiles());
                
                // Close fileInput object
                fileInput.close();
            }
            catch(Exception e)
            {
                System.err.println("\nEncountered wrong input "+e.getMessage());
            }
            finally
            {
                System.out.print("\nDo you want to continue:(y/n)?");
                String str = inputScan.next();
                answer = str.charAt(0);
            }

        } while (answer=='y');
        
        System.out.println("\nExit the app!");
        
        // Close inputScan object
        inputScan.close();
    }
}