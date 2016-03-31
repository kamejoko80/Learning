import java.util.Arrays;


public class Chapter02_exercise32_TwoComparisonsBinSearch
{
	public static void main(String[] args)
	{
		Double[] arr = new Double[]{0.5, 1.0, 2.3, 1.2, 4.0, 0.5};
        Arrays.sort(arr);
        System.out.println(twoComparisonsbinarySearch(arr, 1.0));
	}
	
	
	public static <T extends Comparable<? super T>> int twoComparisonsbinarySearch(T[] a, T x)
	{
		int low = 0, high = a.length - 1;
		
		while (low < high)
		{
			int mid = (low + high) / 2;
			
			if (a[mid].compareTo(x) < 0)
			{
				low = mid + 1;
			}
			else
			{
				high = mid;
			}
		}
		
		// !!! written like "a[low] == x" is false
		if (a[low].compareTo(x) == 0)
			return low;
		else
			return -1;
	}
}