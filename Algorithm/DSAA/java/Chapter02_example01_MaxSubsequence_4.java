
public class Chapter02_example01_MaxSubsequence_4
{
	public static void main(String[] args)
	{
		int[] a = {4, -3, 5, -2, -1, 2, 6, -1};
		System.out.print(maxSubSum(a));
	}
	
	// Linear maximum contiguous subsequence sum algorithm.
	public static int maxSubSum( int [] a)
	{
		int maxSum = 0, thisSum = 0;
		
		for (int i = 0; i < a.length; i++)
		{
			thisSum += a[i];

			if (thisSum > maxSum)
				maxSum = thisSum;
			else if (thisSum < 0)
				thisSum = 0;
		}

		return maxSum;
	}
}