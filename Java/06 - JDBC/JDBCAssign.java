/*******************************************************************************
CSCI470 - Assignment 6 - Spring 2023

Programmer: Nguyen Le (z1940550) & Hung Nguyen (z1924897)
Section:    0001
Due Date:   05/05/2023

Name:       JDBC program

Purpose:    Connect to a mySQL/MariaDB database server installed in the CS 
            department and perform basic query such as display all record, add a
            record, update a record and delete a record.
*******************************************************************************/
import java.sql.*;
import java.util.Scanner;

/*******************************************************************************
Demonstrate the main class JDBC
This class connects to the database using the database name, username and 
password that user provides in the command line arguments. The class then 
interact with user to perform query base on user input for option.
*******************************************************************************/
public class JDBCAssign {
    private static Scanner input;
    
    /**
     * This is the main method for the class. It connects to the database and 
     * perform query from user until the user choose the option to exit the 
     * program.
     */
    public static void main(String args[])
    {
        // Connect to the database
        String url = "jdbc:mysql://courses:3306/";
        if (args.length == 1)  
            url = "jdbc:mysql://courses:3306/"+args[0];
        if (args.length == 3)
            url = "jdbc:mysql://courses:3306/"+args[0]+"?user="+args[1]+"&password="+args[2];
        
        String dbName = args[0];
        try
        {
            Connection con = DriverManager.getConnection(url);
            
            // call printIntro to print out introduction for the program
            printIntro(dbName);
            
            // ask user for action on database
            System.out.print("=> Choose the action: ");
            input = new Scanner(System.in);
            String inputStr = "0";
            int choiceNumber = 0;
            inputStr = input.nextLine();
            try
            {
                choiceNumber = Integer.parseInt(inputStr);
            }
            catch (NumberFormatException e)
            {
                choiceNumber = 0;
            }
            while (choiceNumber != 5)
            {
                switch (choiceNumber)
                {
                    case 1: 
                        addRecord(con);     // add a record to the table
                        break;
                    case 2: 
                        deleteRecord(con);  // delete record based on SSN input
                        break;
                    case 3: 
                        updateAddress(con); // update address for a record
                        break;
                    case 4: 
                        getAll(con);        // print out all record
                        break;
                    case 5:
                        break;
                    default: 
                        System.out.println("Invalid option. Choose again.");
                        // input.nextLine();
                        break;
                }
                
                inputStr = "0";
                System.out.print("\n=> Choose another action: ");
                inputStr = input.nextLine();
                try
                {
                    choiceNumber = Integer.parseInt(inputStr);
                }
                catch (NumberFormatException e)
                {
                    choiceNumber = 0;
                }
            }
            System.out.println("Exit the program ...");
            input.close();
            con.close();
        }
        catch(SQLException e)
        {
            System.out.println("Error. SQLException.\n"+e);
        }
        
    }
    
    /**
     * This methods prints out the introduction of the program
     */
    private static void printIntro(String dbName)
    {
        System.out.println("The program is now successfully connected to the database " + dbName + ".");
        System.out.println();
        System.out.println("  1. ADD");
        System.out.println("  2. DELETE");
        System.out.println("  3. UPDATE ADDRESS");
        System.out.println("  4. GETALL");
        System.out.println("  5. EXIT");
        System.out.println();
    }
    
    /**
     * This method adds a record from name, ssn, address, and code from user 
     * input.
     */
    private static void addRecord(Connection conn)
    {
        String addedName,addedSSN,addedAddress;
        int addedCode;
        
        System.out.print("Please enter[name;ssn;address;code]:");
        String inputStr = input.nextLine();
        String[] data = inputStr.split(";");
        
        addedName = data[0];
        addedSSN = data[1];
        addedAddress = data[2];
        try
        {
            addedCode = Integer.parseInt(data[3]);
            String sqlStr = "INSERT INTO cust VALUES (?,?,?,?)";
            PreparedStatement pstmt = conn.prepareStatement(sqlStr);
            pstmt.setString(1, addedName);
            pstmt.setString(2, addedSSN);
            pstmt.setString(3, addedAddress);
            pstmt.setInt(4, addedCode);
            try
            {
                int returnCode = pstmt.executeUpdate();
                
                if (returnCode > 0)
                {
                    System.out.println("The customer is successfully inserted into the table.");
                }
                else if (returnCode == 0)
                    System.out.println("No record is inserted");
                else System.out.println("Error!");
                pstmt.close();
            }
            catch (SQLException e)
            {
                System.out.println("Error. SQLException.\n"+e);
            }
        }
        catch (NumberFormatException e)
        {
            System.out.println("String does not contain valid integer.\n"+e);
        }
        catch (SQLException e)
        {
            System.out.println("Error. SQLException.\n"+e);
        }
    }
    
    /**
     * This method deletes a record based on the ssn provided by user input.
     */
    private static void deleteRecord(Connection conn)
    {
        System.out.print("Please enter[ssn]:");
        String deletedSSN = input.nextLine();
        String sqlStr = "DELETE FROM cust WHERE ssn = ?";
        try
        {
            PreparedStatement pstmt = conn.prepareStatement(sqlStr);
            pstmt.setString(1, deletedSSN);
            int returnCode = pstmt.executeUpdate();
            
            if (returnCode > 0)
                System.out.println("The customer with ssn SSN is deleted.");
            else if (returnCode == 0)
            System.out.println("The customer is not found for deletion.");
            else System.out.println("Error when deleting record.");
            pstmt.close();
        }
        catch (SQLException e)
        {
            System.out.println("Error. SQLException.\n"+e);
        }
    }
    
    /**
     * This method updates the address for the corresponding ssn. The user will
     * provide the ssn and new addresss from keyboard input.
     */
    private static void updateAddress(Connection conn)
    {
        System.out.print("Please enter[ssn;address]:");
        String inputStr = input.nextLine();
        String[] data = inputStr.split(";");
        String chosenSSN = data[0];
        String newAddress = data[1];
        String sqlStr = "UPDATE cust SET address = ? WHERE ssn = ?";
        try
        {
            PreparedStatement pstmt = conn.prepareStatement(sqlStr);
            pstmt.setString(1, newAddress);
            pstmt.setString(2, chosenSSN);
            int returnCode;
            returnCode = pstmt.executeUpdate();
            if (returnCode > 0)
            {
                System.out.println("The customer's address has been updated.");
            }
            else if (returnCode == 0)
                System.out.println("There is no record matched the SSN input");
            else System.out.println("Error!");
            pstmt.close();
        }
        catch (SQLException e)
        {
            System.out.println("Error. SQLException.\n"+e);
        }
    }
    
    /**
     * This method prints out all the record in the table cust of the database.
     */
    private static void getAll(Connection conn)
    {
        try
        {
            Statement stmt = conn.createStatement();
            String request = "SELECT * FROM cust";
            ResultSet rs = stmt.executeQuery(request);
            String name,ssn,address;
            int code;
            while (rs.next())
            {
                name = rs.getString("name");
                ssn = rs.getString("ssn");
                address = rs.getString("address");
                code = rs.getInt("code");
                System.out.println(name+';'+ssn+';'+address+';'+code);
            }
            rs.close();
            stmt.close();    
        }
        catch (SQLException e)
        {
            System.out.println("Error. SQLException.\n"+e);
        }
    }
}
