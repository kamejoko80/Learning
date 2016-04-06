
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;

public class Chapter03_exercise05_ListUnion
{
	public static void main(String[] args)
	{
		ArrayList<Integer> L1 = new ArrayList<>(Arrays.asList(1, 3, 4, 5, 5, 5, 7, 9, 10));
        ArrayList<Integer> L2 = new ArrayList<>(Arrays.asList(2, 3, 5, 5, 8, 9, 10));
        System.out.print(union(L1, L2));
	}
	
	public static List<Integer> union(List<Integer> L1, List<Integer> L2)
	{
		List<Integer> L3 = new ArrayList<>();
		Iterator<Integer> iterator1 = L1.iterator();
		Iterator<Integer> iterator2 = L2.iterator();
		
		if (!iterator2.hasNext()) return L3; 
	    
        int int1, int2;
        int2 = iterator2.next();
        while (iterator1.hasNext())
        {
            int1 = iterator1.next();
            L3.add(int1);
            
            if (int1 == int2)
            {
                if (iterator2.hasNext())
                    int2 = iterator2.next();
            }
            else if (int1 > int2)
            {
                while (int1 > int2 && iterator2.hasNext())
                {
                	L3.add(int2);
                    int2 = iterator2.next();
                }
                if (int1 == int2)
                {
                    if (iterator2.hasNext())
                        int2 = iterator2.next();
                }
            }
        } // end of while

		return L3;
	}
}