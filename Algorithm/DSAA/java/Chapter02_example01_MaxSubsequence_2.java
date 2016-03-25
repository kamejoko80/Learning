
public class Chapter02_example01_MaxSubsequence_2
{
	public static void main(String[] args)
	{
		int[] a = {4, -3, 5, -2, -1, 2, 6, -1};
		System.out.print(maxSubSum(a));
	}
	
	// Quadratic maximum contiguous subsequence sum algorithm.
	public static int maxSubSum( int [] a)
	{
		int maxSum = 0;
		
		for (int i = 0; i < a.length; i++)
		{
			int thisSum = 0;
			for (int j = i; j < a.length; j++)
			{
				thisSum += a[j];

				if (thisSum > maxSum)
					maxSum = thisSum;
			}
		}

		return maxSum;
	}
}