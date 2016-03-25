
public class Chapter02_example01_MaxSubsequence_1
{
	public static void main(String[] args)
	{
		int[] a = {4, -3, 5, -2, -1, 2, 6, -1};
		System.out.print(maxSubSum(a));
	}
	
	// Cubic maximum contiguous subsequence sum algorithm.
	public static int maxSubSum( int [] a)
	{
		int maxSum = 0;
		
		for (int i = 0; i < a.length; i++)
			for (int j = i; j < a.length; j++)
			{
				int thisSum = 0;

				for (int k = i; k <= j; k++)
					thisSum += a[k];

				if (thisSum > maxSum)
					maxSum = thisSum;
			}

		return maxSum;
	}
}