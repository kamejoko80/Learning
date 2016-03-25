
public class Chapter02_example03_gcd
{
	public static void main(String[] args)
	{
		System.out.print(gcd(100156,183505));
	}
	
	// Euclid's algorithm.
	public static long gcd(long m, long n)
	{
		while (n != 0)
		{
			long rem = m % n;
			m = n;
			n = rem;
		}
		return m;
	}
}