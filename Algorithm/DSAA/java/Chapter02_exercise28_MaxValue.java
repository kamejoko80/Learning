
public class Chapter02_exercise28_MaxValue
{
	public static void main(String... args)
	{
        double[] arr = new double[]{0.5, 1.0, 2.3, 1.2, 4.0, 0.5};
        maxValues(arr);
    }
	
	// written by relphite
    public static void maxValues(double[] arr)
    {
        double[] maxArr = new double[arr.length];
        double max = -1.0;
        for (int i = arr.length - 1; i >= 0; i--)
        {
            if (arr[i] > max)
            {
                max = arr[i];
            }
            maxArr[i] = max;
        }
        // After this operation, maxArr[] = {4.0, 4.0, 4.0, 4.0, 4.0, 0.5}
        
        double maxAdd = Double.MIN_VALUE;
        double maxSubtract = Double.MIN_VALUE;
        double maxMultiply = Double.MIN_VALUE;
        double maxDivide = Double.MIN_VALUE;
        for (int i = 0; i < arr.length; i++)
        {
            maxAdd = Math.max(maxAdd, maxArr[i] + arr[i]);
            maxSubtract = Math.max(maxSubtract, maxArr[i] - arr[i]);
            maxMultiply = Math.max(maxMultiply, maxArr[i] * arr[i]);
            maxDivide = Math.max(maxDivide, maxArr[i] / arr[i]);
        }

        System.out.println("maxAdd = " + maxAdd);
        System.out.println("maxSubtract = " + maxSubtract);
        System.out.println("maxMultiply = " + maxMultiply);
        System.out.println("maxDivide = " + maxDivide);
    }
}