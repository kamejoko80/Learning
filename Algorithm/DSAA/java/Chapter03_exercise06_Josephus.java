import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.LinkedList;

public class Chapter03_exercise06_Josephus
{
	public static void main(String[] args)
	{
		Chapter03_exercise06_Josephus solution = new Chapter03_exercise06_Josephus();
		for (Method m : solution.getClass().getDeclaredMethods())
		{
			if (m.getName().startsWith("j"))
			{
				long a = System.currentTimeMillis();
				try
				{
					m.invoke(solution, 10000, 156);
				}
				catch (IllegalAccessException | InvocationTargetException e)
				{
					e.printStackTrace();
				}
				
				long b = System.currentTimeMillis();
				System.out.println(m.getName() + ":" + String.valueOf(b - a));
			}
		}
		System.out.println(josephus(10000, 156));
		System.out.println(josephus_1(10000, 156));
		System.out.println();
	}
	
	public static int josephus(int n, int m)
	{
		LinkedList<Integer> circle = new LinkedList<Integer>();
		for (int i = 0; i < n; i++)
			circle.add(i + 1);
		
		int out = 0;
		for (int remain = n; remain > 1; remain--)
		{
			out = (m + out) % remain;
			circle.remove(out);
		}
		
		int output = Integer.valueOf(circle.toString().substring(1, circle.toString().length() - 1));
		return output;
	}
	
	// relphite's solusion
	private static class SinglyLinkedNode {
	    int data;
	    SinglyLinkedNode next;

	    SinglyLinkedNode(int i) {
	        data = i;
	        next = null;
	    }
	}
	
	public static int josephus_1(int n, int m)
	{
        assert m >= 0 : "m should not be negative";

        SinglyLinkedNode dummy = new SinglyLinkedNode(-1);
        SinglyLinkedNode cur = dummy;

        for (int i = 1; i <= n; i++) {
            cur.next = new SinglyLinkedNode(i);
            cur = cur.next;
        }
        cur.next = dummy.next;
        //now we have a circle
        assert cur.next.data == 1;

        while (cur.next != cur) {
            for (int i = 0; i < m; i++) {
                cur = cur.next;
            }
            cur.next = cur.next.next;
        }

        return cur.data;
    }
}