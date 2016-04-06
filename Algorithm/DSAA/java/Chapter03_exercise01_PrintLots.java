import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Iterator;


public class Chapter03_exercise01_PrintLots
{
	public static void main(String[] args)
	{
		Collection<Integer> L = new ArrayList<Integer>(Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
        Collection<Integer> P = new ArrayList<Integer>(Arrays.asList(1, 3, 4, 6, 9, 10));
		printLots_1(L, P);
		printLots_2(L, P);
	}
	
	public static void printLots_1(Collection<Integer> L, Collection<Integer> P)
	{
		int counter = 0;
		for (int index : P)
		{
			for (int content : L)
			{
				counter++;
				if (counter == index)
				{
					System.out.print(content + ", ");
					counter = 0;
					break;
				}
			}
		}
	}
	
	public static void printLots_2(Collection<Integer> L, Collection<Integer> P)
	{
		Iterator<Integer> iteratorL = L.iterator();
        Iterator<Integer> iteratorP = P.iterator();
        int i = 0;
        while (iteratorP.hasNext())
        {
            int p = iteratorP.next();
            while (i < p - 1)
            {
                if (!iteratorL.hasNext())
                    throw new IllegalArgumentException("illegal input list");
                iteratorL.next();
                i++;
            }
            if (!iteratorL.hasNext())
                throw new IllegalArgumentException("illegal input list");
            System.out.print(iteratorL.next() + ", ");
            i++;
        }
	}
	
	
	
}