import java.util.Scanner;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;

/*******************************************************************************
The MileRedeemer encapsulates the logic for redeeming mileage.
*******************************************************************************/
public class MileRedeemer
{
    // Store array of destination objects
    private ArrayList<Destination> destList = new ArrayList<Destination>();
    private int remainMiles = 0;        // Remaining miles initiate to zero

    /***************************************************************************
     * Read data into a destination array
     * Parameters: fileScanner
     * Description: This method uses the Scanner object to read and parse the 
     * destination data into an array of Destination objects, which should then 
     * be sorted by their normal mileage.
     **************************************************************************/
    public void readDestination(Scanner fileScanner)
    {
        try
        {
            while(fileScanner.hasNext())
            {
                String destCity;
                int normalMiles, 
                    ssaverMiles, 
                    addMiles, 
                    startMonth, 
                    endMonth; 
                
                // Store one line of the fileScanner 
                String recordLine = fileScanner.nextLine();

                String[] data = recordLine.split(";");
                String[] month = data[4].split("-");

                destCity = data[0];
                normalMiles = Integer.parseInt(data[1]);
                ssaverMiles = Integer.parseInt(data[2]);
                addMiles = Integer.parseInt(data[3]);
                startMonth = Integer.parseInt(month[0]);
                endMonth = Integer.parseInt(month[1]);

                Destination destObj = new Destination(destCity,normalMiles,
                  ssaverMiles,addMiles,startMonth,endMonth);

                destList.add(destObj);
            }
        }
        catch (NumberFormatException e)
        {
            System.err.println("Incorrect format in input. "+e.getMessage());
            System.exit(0);
        }
    }

    /***************************************************************************
     * Get a list of destination city
     * Description:  This method loops through the array of Destination objects 
     * and create an array of String objects from the city names which are 
     * sorted in ascending order.
     **************************************************************************/
    public String[] getCityNames()
    {
        // Convert the destList Array List to a normal array of object
        Destination[] destArray = (Destination[]) destList.toArray(new 
          Destination[destList.size()]);

        String[] stringArray = new String[destArray.length];

        for (int i = 0; i < stringArray.length;i++)
          stringArray[i] = destArray[i].getDestCity();
        
        Arrays.sort(stringArray);
        return stringArray;
    }

    /***************************************************************************
     * Redeem miles from total available miles and the desired month.
     * Parameters: Total avalable miles, desired month
     * Description:  method call some other methods to accomplish its subtasks 
     * including XXXXXXXXX. This method returns an array of String objects 
     * containing descriptions of redeemed tickets to be printed out by the main 
     * program and saves the miles remaining after the tickets have been 
     * redeemed.
     **************************************************************************/
    public String[] redeemMiles(int miles,int month)
    {
        int availMiles = miles;     // Current available miles
        // List if planned destination
        ArrayList<Destination> planDestinations = new ArrayList<>(); 
        // List of planned trips
        ArrayList<String> trips = new ArrayList<>(); 

        // Sort the destination list in descend order of normal miles
        Collections.sort(destList,new MileComparator());
        
        for (Destination d: destList)
        {
            // If travel time is not in the supersaver months
            if(month < d.getStartMonth() || month > d.getEndMonth())
            {
                int m = d.getNormalMiles();
                if(availMiles > m)
                {
                    // Add the destination into the planned destination.
                    planDestinations.add(d);
                    // Decrease the current available miles
                    availMiles-= m;
                }
            }
            // Travel time is in the supersaver months
            else
            {
                int m = d.getSsaverMiles();
                // Check if the user has enough miles for supersaver ticket
                if(availMiles > m)
                {
                    // Add the destination into the planned destination.
                    planDestinations.add(d);
                    // Decrease the current available miles
                    availMiles-= m;
                }
            }
        }

        // Check for upgrade posibility and output trips
        for (Destination d: planDestinations)
        {
            int m = d.getAddMiles();
            String c = d.getDestCity();
            if (availMiles >= m)
            {
                trips.add("* A trip to "+c+", first class");
                availMiles -= m;
            }
            else trips.add("* A trip to "+c+", economy class");
        }

        remainMiles = availMiles;
        String[] tripDetails = new String[trips.size()];
        tripDetails = trips.toArray(tripDetails);
        
        return tripDetails;
    }

    /***************************************************************************
     * Method getRemainMiles: get the remaining mileage avalable.
     **************************************************************************/
    public int getRemainMiles()
    {
        return remainMiles;
    }
}

/*******************************************************************************
Class MileComparator is implemented to use in Collection. sort method to sort 
the destination list in descend order of normal miles
*******************************************************************************/
class MileComparator implements Comparator<Destination>
{
    // Modify sort criteria to normalMiles
    @Override
    public int compare(Destination d1, Destination d2)
    {
        if (d1.getNormalMiles() > d2.getNormalMiles()) return -1;
        else if (d1.getNormalMiles() < d2.getNormalMiles()) 
            {
                return 1;
            }
            else return 0;
    }
}