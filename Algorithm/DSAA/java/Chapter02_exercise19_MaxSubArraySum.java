
import java.util.ArrayList;

public class Chapter02_exercise19_MaxSubArraySum
{
	public static void main(String[] args)
	{
		int[] arr = new int[]{-5, 1, 9, -2, 4, -2, 8, 2, -1, 3};
        System.out.print(maxSubArraySum(arr));
    
	}
	
	public static ArrayList<Integer> maxSubArraySum(int[] a)
	{
		int curSum = 0, startIndex = 0;
		int maxSum = 0, maxEndIndex = 0, maxStartIndex = 0;
		
		for (int i = 0; i < a.length; i++)
		{
			curSum += a[i];
			if (curSum > maxSum)
			{
				maxSum = curSum;
				maxEndIndex = i;
				maxStartIndex = startIndex;
			}
			else if (curSum <= 0)
			{
				curSum = 0;
				startIndex = i + 1;
			}
		}
		
		ArrayList<Integer> result = new ArrayList<>();
		result.add(maxSum);//sum
        result.add(maxStartIndex);//start index
        result.add(maxEndIndex);//end index

        return result;
	}
}