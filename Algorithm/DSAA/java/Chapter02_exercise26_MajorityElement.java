
public class Chapter02_exercise26_MajorityElement
{
	public static void main(String... args)
	{
        int[] arr = new int[]{-3, 4, 2, 5, 2, 2, 1, 0, 2, 2, 7, 2, 3, 2, 2};
        int[] arr1 = new int[]{1, 3, 2, 3, 4, 3, 5, 3, 4, 3, 4, 3, 4, 3, 4, 3, 3};

    	// TODO: the Algorithm offered by the book is not correct for the array like this
        int[] arr2 = new int[]{2, 3, 4, 3, 5, 3, 6, 3, 7, 3};
        
        System.out.println(findMajorityElement(arr));
        System.out.println(findMajorityElement(arr1));
        System.out.println(findMajorityElement(arr2));
    }
	
	// written by relphite
    public static int findMajorityElement(int[] arr)
    {
        return findMajorityElement0(arr, 0, arr.length);
    }

    private static int findMajorityElement0(int[] arr, int startIndex, int endIndex)
    {
        if (endIndex <= startIndex)
            throw new IllegalArgumentException("illegal input array");
        if (endIndex - startIndex == 1)
            return arr[startIndex];
        
        if ((endIndex - startIndex) % 2 == 0)
        {
            int cur = 0;
            for (int i = startIndex; i < endIndex; i += 2)
            {
                if (arr[i] == arr[i + 1])
                    arr[cur++] = arr[i];
            }
            return findMajorityElement0(arr, 0, cur);
        }
        else
        {
            int last = arr[endIndex - 1];
            int lastCount = 1;
            int cur = 0;
            for (int i = startIndex; i < endIndex - 1; i += 2)
            {
                if (arr[i] == last) lastCount++;
                if (arr[i + 1] == last) lastCount++;
                if (arr[i] == arr[i + 1])
                    arr[cur++] = arr[i];
            }
            if (lastCount > (endIndex - startIndex) / 2)
                return last;
            return findMajorityElement0(arr, 0, cur);
        }
    }
}