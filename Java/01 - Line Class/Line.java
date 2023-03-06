/*******************************************************************************
CSCI470 - Assignment 1 - Spring 2023

Programmer: Nguyen Le (z1940550) & Hung Nguyen (z1924897)
Section:    0001
Due Date:   02/05/2023

Name:       Line Class Enhancement.

Purpose:    Practice writting Java classes, adding variables and methods to the
            class, as well as exception throwing and handling.
*******************************************************************************/
import java.io.*;
import java.lang.Math;

/*******************************************************************************
Demonstrate a simple "Line" class, which is defined with its properties and 
interface
*******************************************************************************/
public class Line
{
    private int x1,y1,x2,y2,z1;
    private String c;
    
    /***************************************************************************
     * constructor
     * parameters: left, top, botttom, right, Thick, color
     * description: receives 4 integers which are the Line's start and end 
     * points.
     **************************************************************************/
    public Line(int left, int top, int bottom, int right, int Thick, String 
      color) throws IllegalAccessException 
    {
        setLeft (left);
        setTop (top);
        setRight(right);
        setBottom(bottom);
        setThick(Thick);
        setColor(color);
    }
    
    /***************************************************************************
     * constructor for 2 2-D points
     * parameters: TwoDPoint object1, TwoDPoint object1, Thick, color
     **************************************************************************/
    public Line(TwoDPoint obj1, TwoDPoint obj2,int Thick,String color) throws 
    IllegalAccessException 
    {
        this(obj1.x,obj1.y,obj2.x,obj2.y,Thick,color);
    }
    
    /***************************************************************************
     * method draw() call another method called drawline()
     * which is assumed to be a graphic primitive on the system.
     **************************************************************************/
    public void draw()
    {
        drawLine(x1,y1,x2,y2,z1,c);
    }
    
    /***************************************************************************
     * method drawLine() simulates drawing of a line for console mode
     * parameters: x1, y1, x2, y2, z1, c
     **************************************************************************/
    private void drawLine(int x1, int y1, int x2, int y2,int z1,String c)
    {
        System.out.println("\nDraw a line from x of "+ x1 +" and y of " + y1);
        System.out.println("To x of " + x2 + " and y of " + y2);
        System.out.println("Line width= " + z1);
        System.out.println("Line color= "+ c);
    }
    
    /***************************************************************************
     * method setline allow change to existing line
     * parameters: left, top, bottom, right, Thick, color
     **************************************************************************/
    public void setLine (int left, int top, int bottom, int right,int Thick, 
      String color) throws IllegalAccessException 
    {
        setLeft(left);
        setTop(top);
        setRight(right);
        setBottom(bottom);
        setThick(Thick);
        setColor(color);
    }
    
    /***************************************************************************
     * calculate the length of a line
     * parameters: x1, x2, y1, y2
     **************************************************************************/
    private double Calculator(int x1, int x2, int y1, int y2)
    {
        return Math.sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    }
    
    /***************************************************************************
     * get the length of a line
     **************************************************************************/
    public void getLength()
    {
        System.out.println("Length = "+ Calculator(x1,x2,y1,y2));
    }
    
    
    /***************************************************************************
     * set the left x-coordination of a line
     * parameters: left
     **************************************************************************/
    public void setLeft (int left) throws IllegalAccessException 
    {
        if (left < 0 || left >639)
            throw new IllegalAccessException("Invalid Argument");
        else
            this.x1= left;
    }
    
    /***************************************************************************
     * set the top y-coordination of a line
     * parameters: top
     **************************************************************************/
    public void setTop(int top) throws IllegalAccessException {
        if (top < 0|| top > 479)
            throw new IllegalAccessException("Invalid Argument");
        else
            y1 = top;
    }
    
    /***************************************************************************
     * set the bottom y-coordination of a line
     * parameters: bottom
     **************************************************************************/
    public void setBottom(int bottom) throws IllegalAccessException {
        if(bottom >479|| bottom <0)
            throw new IllegalAccessException("Invalid Argument");
        else
            y2= bottom;
    }
    
    /***************************************************************************
     * set the right x-coordination of a line
     * parameters: right
     **************************************************************************/
    public void setRight(int right) throws IllegalAccessException {
        if (right >639 || right <0)
            throw new IllegalAccessException("Invalid Argument");
        else
            x2=right;
    }
    
    /***************************************************************************
     * set the color of a line
     * parameters: color
     **************************************************************************/
    public void setColor(String color)
    {
        c=color;
    }
    
    /***************************************************************************
     * set the thickness of a line
     * parameters: Thick
     **************************************************************************/
    public void setThick(int Thick)
    {
        z1 = Thick;
    }
    
    /***************************************************************************
     * get the top x-coordination of a line
     **************************************************************************/
    public int getTop()
    {
        return y1;
    }
    
    /***************************************************************************
     * get the bottom y-coordination of a line
     **************************************************************************/
    public int getBottom()
    {
        return y2;
    }
    
    /***************************************************************************
     * get the left x-coordination of a line
     **************************************************************************/
    public int getLeft()
    {
        return x1;
    }
    
    /***************************************************************************
     * get the right x-coordination of a line
     **************************************************************************/
    public int getRight()
    {
        return x2;
    }
    
    /***************************************************************************
     * get the thickness of a line
     **************************************************************************/
    public int getThick() { return z1;}
    
    /***************************************************************************
     * get the color of a line
     **************************************************************************/
    public String getColor() { return c;}
}

