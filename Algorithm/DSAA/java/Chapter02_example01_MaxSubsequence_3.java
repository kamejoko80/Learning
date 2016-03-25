
public class Chapter02_example01_MaxSubsequence_3
{
	public static void main(String[] args)
	{
		int[] a = {4, -3, 5, -2, -1, 2, 6, -1};
		System.out.print(maxSubSum(a));
	}
	
	
	// Recursive maximum contiguous subsequence sum algorithm.
	public static int maxSumRec( int [] a, int left, int right)
	{
		if (left ==right)  // base case
			if (a[left] > 0)
				return a[left];
			else
				return 0;
		
		int center = (left + right) / 2;
		int maxLeftSum = maxSumRec(a, left, center);
		int maxRightSum = maxSumRec(a, center + 1, right);
		
		int maxLeftBorderSum = 0, leftBorderSum = 0;
		for (int i = center; i >= left; i--)
		{
			leftBorderSum += a[i];
			if (maxLeftBorderSum < leftBorderSum)
				maxLeftBorderSum  =  leftBorderSum;
		}
		
		int maxRightBorderSum = 0, rightBorderSum = 0;
		for (int i = center + 1; i <= right; i++)
		{
			rightBorderSum += a[i];
			if (maxRightBorderSum < rightBorderSum)
				maxRightBorderSum = rightBorderSum;
		}
		
		return max3(maxLeftSum, maxRightSum, maxLeftBorderSum + maxRightBorderSum);
	}
	
	// Driver for 
	public static int maxSubSum(int[] a)
	{
		return maxSumRec(a, 0, a.length -1);
	}
	
	public static int max3(int a, int b, int c)
	{
		if (a > b)
			if (a > c)
				return a;
			else
				return c;
		else
			if (b > c)
				return b;
			else
				return c;		
	}
}