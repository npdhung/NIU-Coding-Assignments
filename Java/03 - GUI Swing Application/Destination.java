/*******************************************************************************
Demonstrate the Destination class
It encapsulates information such as the name of the destination city; normal 
miles for a ticket; supersaver miles; additional miles for upgrading; start 
month of the supersaver program and end of the supersaver program.
*******************************************************************************/
public class Destination
{
    private String destCity;        // Destination city
    private int normalMiles,        // Normal miles for a ticket
            ssaverMiles,            // Supersaver milers
            addMiles,               // Addition miles for upgrading
            startMonth,             // Start month of supersaver program
            endMonth;               // End month of supersaver program

    /***************************************************************************
     * Constructor
     * Parameters: destCity, normalMiles, sSaverMiles, addMiles, startMonth, 
     * endMonth
     * Description: Initialize default values of the destination city, normal 
     * miles, supersaver miles, addition miles for upgrading, start month and 
     * end month of the supersaver program.
     **************************************************************************/
    public Destination(String destCity, int normalMiles, int ssaverMiles, 
      int addMiles, int startMonth, int endMonth)
    {
        this.destCity = destCity;
        this.normalMiles = normalMiles;
        this.ssaverMiles = ssaverMiles;
        this.addMiles = addMiles;
        this.startMonth = startMonth;
        this.endMonth = endMonth;
    }
    
    /***************************************************************************
     * Method setDestCity allow change to the destination city destCity
     * Parameters: destCity
     **************************************************************************/
    public void setDestCity(String destCity)
    {
        this.destCity = destCity;
    }
    
    /***************************************************************************
     * Method setNormalMiles allow change to the normalMiles attribute
     * Parameters: normalMiles
     **************************************************************************/
    public void setNormalMiles(int normalMiles)
    {
        this.normalMiles = normalMiles;
    }
    
    /***************************************************************************
     * Method setSsaverMiles allow change to the ssaverMiles attribute
     * Parameters: ssaverMiles
     **************************************************************************/
    public void setSsaverMiles(int ssaverMiles)
    {
        this.ssaverMiles = ssaverMiles;
    }
    
    /***************************************************************************
     * Method setAddMiles allow change to the addMiles attribute
     * Parameters: addMiles
     **************************************************************************/
    public void setAddMiles(int addMiles)
    {
        this.addMiles = addMiles;
    }
    
    /***************************************************************************
     * Method setStartMonth allow change to the startMonth attribute
     * Parameters: startMonth
     **************************************************************************/
    public void setStartMonth(int startMonth)
    {
        this.startMonth = startMonth;
    }
    
    /***************************************************************************
     * Method setEndMonth allow change to the endMonth attribute
     * Parameters: endMonth
     **************************************************************************/
    public void setEndMonth(int endMonth)
    {
        this.endMonth = endMonth;
    }
    
    /***************************************************************************
     * Method getDestCity: get the name of the destination city
     **************************************************************************/
    public String getDestCity()
    {
        return destCity;
    }
    
    /***************************************************************************
     * Method getNormalMiles: get the normal miles need for redeeming
     **************************************************************************/
    public int getNormalMiles()
    {
        return normalMiles;
    }
    
    /***************************************************************************
     * Method getSsaverMiles: get the supersaver mileage
     **************************************************************************/
    public int getSsaverMiles()
    {
        return ssaverMiles;
    }
    
    /***************************************************************************
     * Method getAddMiles: get the additional mileage needed for upgrading
     **************************************************************************/
    public int getAddMiles()
    {
        return addMiles;
    }
    
    /***************************************************************************
     * Method getStartMonth: get the start month of the supersaver program
     **************************************************************************/
    public int getStartMonth()
    {
        return startMonth;
    }
    
    /***************************************************************************
     * Method getEndMonth: get the end month of the supersaver program
     **************************************************************************/
    public int getEndMonth()
    {
        return endMonth;
    }
}