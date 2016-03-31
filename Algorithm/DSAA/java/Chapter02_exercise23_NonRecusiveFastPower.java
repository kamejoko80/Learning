import java.math.BigInteger;
import java.util.ArrayList;

public class Chapter02_exercise23_NonRecusiveFastPower
{
	public static void main(String[] args)
	{
		System.out.println(fastPower(2, 50));
		System.out.println(exponentiationNonRecursive(BigInteger.valueOf(2), BigInteger.valueOf(50)));
	}
	
	public static long fastPower(long a, long n)
	{
		long result = a;
		ArrayList<Long> remain = new ArrayList<Long>();
		
		while(n != 1)
		{
			if (n % 2 == 0)
			{
				result *= result;
			}
			else
			{
				remain.add(result);
				result *= result;
			}
			n = n / 2;
		}
		
		for (long re : remain)
		{
			result *= re;
		}
		return result;
	}
	
	// written by ralphite
	public static BigInteger exponentiationNonRecursive(BigInteger x, BigInteger n)
	{
        assert n.compareTo(BigInteger.ZERO) >= 0;

        if (n.compareTo(BigInteger.ZERO) == 0) return BigInteger.ONE;
        if (n.compareTo(BigInteger.ONE) == 0) return x;

        ArrayList<BigInteger> list = new ArrayList<BigInteger>();
        list.add(BigInteger.ONE);
        list.add(x);
        BigInteger p = BigInteger.ONE;
        for (int i = 2; p.compareTo(n) <= 0; i++)
        {
            BigInteger prev = list.get(i - 1);
            list.add(prev.multiply(prev));
            p = p.multiply(BigInteger.valueOf(2));
        }

        BigInteger result = BigInteger.ONE;

        int i = 1;
        while (n.compareTo(BigInteger.ZERO) > 0)
        {
            if (n.mod(BigInteger.valueOf(2)).compareTo(BigInteger.ZERO) != 0)
                result = result.multiply(list.get(i));
            n = n.divide(BigInteger.valueOf(2));
            i++;
        }
        return result;
    }
}