public class TestLine 
{
    public static void main(String args[]) throws IllegalAccessException 
    {
        Line l1 = null;
        Line l2 = null;
        Line L3 = null;
        Line L4 = null;
        TwoDPoint obj1 = null;
        TwoDPoint obj2 = null;

        l1 = new Line(10, 10, 100, 100,5, "red");
        l1.draw();
        l1.setLine(5, 5, l1.getRight(), l1.getBottom(),5,"yellow");
        l1.draw();
        l1.getLength();
        //l1.setLeft(3000);
        l1.draw();
        l2 = new Line(100, 100, 400, 400,5, "blue");
        l2.draw();
        l2.setBottom(479);
        l2.draw();
        
        //new class twodpoint
        L3 = new Line (new TwoDPoint(10,10), new TwoDPoint(100,100), 5, "red");
        L3.draw();

        System.out.println("Altering line with Invalid values: ");
        try 
        {
            l1.setLeft(5000);
        }
        catch (Exception ex)
        {
            System.out.println("Exception catched : "+ ex.getMessage());

        }
        System.out.println("Altering line with Invalid values: ");
        try 
        {
            L3.setTop(500);
        }
        catch (Exception ex)
        {
            System.out.println("Exception catched : "+ ex.getMessage());

        }
        try
        {
            Line newLine = new Line(new TwoDPoint(1000,200), 
              new TwoDPoint(3000,300),10,"blue");

        }
        catch (Exception ex)
        {
            System.out.println("Exception catched while creating line : " + 
              ex.getMessage());
            System.exit(88);
        }
    }
}

