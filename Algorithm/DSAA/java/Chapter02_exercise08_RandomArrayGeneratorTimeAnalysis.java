

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Random;

public class Chapter02_exercise08_RandomArrayGeneratorTimeAnalysis 
{
    private static final int TIMES = 10;
    private Random random = new Random();

    public static void main(String... args) 
    {
    	Chapter02_exercise08_RandomArrayGeneratorTimeAnalysis solution = new Chapter02_exercise08_RandomArrayGeneratorTimeAnalysis();
        try 
        {
            for (int n : new int[]{250, 500, 1000, 2000})
                solution.check(solution.getClass().getMethod("f1", new Class[]{int.class}), n);

            for (int n : new int[]{25000, 50000, 100000, 200000, 400000, 800000})
                solution.check(solution.getClass().getMethod("f2", new Class[]{int.class}), n);

            for (int n : new int[]{100000, 200000, 400000, 800000, 1600000, 3200000, 6400000})
                solution.check(solution.getClass().getMethod("f3", new Class[]{int.class}), n);

        } 
        catch (NoSuchMethodException e) 
        {
            e.printStackTrace();
        }

    }

    private void check(Method m, int n) 
    {
        try 
        {
            long a = System.currentTimeMillis();
            for (int i = 0; i < TIMES; i++)
                m.invoke(this, n);
            long b = System.currentTimeMillis();
            System.out.println(m.getName() + "(n=" + n + ") : " + (b - a));
        } 
        catch (IllegalAccessException | InvocationTargetException e) 
        {
            e.printStackTrace();
        }
    }

    public void f1(int n) 
    {
        int[] a = new int[n];
        for (int i = 0; i < n; i++) 
        {
            boolean diff = true;
            int r = -1;
            while (diff) 
            {
                diff = false;
                r = random.nextInt(n) + 1;
                for (int j = 0; j < i; j++) 
                {
                    if (a[j] == r) diff = true;
                }
            }
            a[i] = r;
        }
    }

    public void f2(int n) 
    {
        int[] a = new int[n];
        boolean[] used = new boolean[n];
        for (int i = 0; i < n; i++)
        {
            boolean diff = true;
            int r = -1;
            while (diff) 
            {
                diff = false;
                r = random.nextInt(n) + 1;
                if (used[r - 1])
                    diff = true;
            }
            a[i] = r;
            used[r - 1] = true;
        }
    }

    public void f3(int n) 
    {
        int a[] = new int[n];
        for (int i = 0; i < n; i++)
        {
            a[i] = i + 1;
        }
        for (int i = 1; i < n; i++) 
        {
            int r = random.nextInt(i);
            int t = a[i];
            a[i] = a[r];
            a[r] = t;
        }
    }
}