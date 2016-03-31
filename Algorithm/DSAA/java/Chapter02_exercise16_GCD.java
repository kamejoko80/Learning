
public class Chapter02_exercise16_GCD
{
	public static void main(String[] args) 
	{
        long A = 156113136;
        long B = 15646314;
        
        long g1 = gcd1(A, B);
        long g2 = gcd2(A, B);
        System.out.println(A + "," + B + " : " + g1 + "," + g2);
    }

    public static long gcd1(long m, long n)
    {
        while (n > 0) 
        {
            long t = n;
            n = m % n;
            m = t;
        }
        return m;
    }

    public static long gcd2(long m, long n) 
    {
        if (m < n) return gcd2(n, m);

        if (m == n) return m;

        if (m % 2 == 0 && n % 2 == 0) 
            return 2 * gcd2(m / 2, n / 2);
        else if (m % 2 == 0 && n % 2 != 0) 
            return gcd2(m / 2, n);
        else if (m % 2 != 0 && n % 2 == 0)
            return gcd2(m, n / 2);
        else
            return gcd2((m + n) / 2, (m - n) / 2);
    }
}