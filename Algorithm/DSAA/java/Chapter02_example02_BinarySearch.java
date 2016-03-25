
public class Chapter02_example02_BinarySearch
{
	public static void main(String[] args)
	{
		Integer[] a = {0, 1, 2, 4, 6, 8, 16, 32};
		System.out.print(binarySearch(a, 16));
	}
	
	// Perform the standard binary search.
	// @return index where item is found, or -1 if not found.
	static int NOT_FOUND = -1;
	
	public static <AnyType extends Comparable<? super AnyType>> 
	int binarySearch( AnyType [] a, AnyType x)
	{
		int low = 0, high = a.length - 1;
		
		while (low <= high)
		{
			int mid = (low + high) / 2;
			
			if (a[mid].compareTo(x) < 0)
				low = mid + 1;
			else if (a[mid].compareTo(x) > 0)
				high = mid - 1;
			else
				return mid; // Found
		}

		return NOT_FOUND;
	}
}