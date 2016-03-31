
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Random;
import java.util.Arrays;

public class Chapter02_exercise15_SearchInSortedArray
{
    private static final int N = 10000;
    private static Random random = new Random();
    private static final int TIMES = 1000;
    
    public static void main(String[] args)
    {
    	Chapter02_exercise15_SearchInSortedArray solution = new Chapter02_exercise15_SearchInSortedArray();
    	int[] array = generateRandomArray(N);
    	//int[] array = {-1, 1, 3};
    	
    	boolean result = false;
    	for (Method m : solution.getClass().getDeclaredMethods())
    	{
    		if (m.getName().startsWith("a"))
    		{
    			long a = System.currentTimeMillis();
    			
    			try
    			{
    				for (int i = 1; i < TIMES; i++)
    					m.invoke(solution, array);
    				result = (boolean) m.invoke(solution, array);
    			}
    			catch (IllegalAccessException | InvocationTargetException e)
    			{
    				e.printStackTrace();
    			}
    			
    			long b = System.currentTimeMillis();
    			System.out.println(result + "\n" + m.getName() + ":" + String.valueOf(b - a));
    		}
    	}
    }
    
    public static int[] generateRandomArray(int n)
    {
    	int[] a = new int[n];
        for (int i = 0; i < n; i++) 
        {
            boolean diff = true;
            int r = -1;
            while (diff) 
            {
                diff = false;
                r = random.nextInt(10 * n);
                for (int j = 0; j < i; j++) 
                {
                    if (a[j] == r) diff = true;
                }
            }
            a[i] = r;
        }
    	Arrays.sort(a);
    	return a;
    }
    
    public boolean algorithm_1(int[] a)
    {
    	return recurtionForAlgorithm_1(a, 0, a.length - 1);
    }
    
    public boolean recurtionForAlgorithm_1(int[] a, int left, int right)
    {
    	boolean ifExist = false;
    	if (left == right)
    		return (a[left] == left);
    	
    	int half = left + (right - left) / 2;
    	if (a[half] < half)
    		ifExist = recurtionForAlgorithm_1(a, half + 1, right);
    	else if (a[half] > half)
    		ifExist = recurtionForAlgorithm_1(a, left, half);
    	else
    		ifExist = true;
    	return ifExist;
    }
    
    public boolean algorithm_2(int[] a)
    {
    	int left = 0, right = a.length - 1;
    	boolean ifExist = false;
    	
    	while (left != right)
    	{
    		int half = left + (right - left) / 2;
    		if (a[half] < half)
    			left = half + 1;
    		else if (a[half] > half)
    			right = half;
    		else
    		{
    			ifExist = true;
    			break;
    		}
    	}
    	
    	if (left == right)
    		if (a[left] == left)
    			ifExist = true;
    	
    	return ifExist;
    }
    
}
