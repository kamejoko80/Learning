
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class Chapter02_exercise07_TimeAnalysis
{
    private static final int N = 2000;

    public static void main(String... args) 
    {
    	Chapter02_exercise07_TimeAnalysis solution = new Chapter02_exercise07_TimeAnalysis();
        for (Method m : solution.getClass().getDeclaredMethods()) 
        {
            if (m.getName().startsWith("f")) 
            {
                long a = System.currentTimeMillis();
                
                try 
                {
                    m.invoke(solution, N);
                } 
                catch (IllegalAccessException | InvocationTargetException e) 
                {
                    e.printStackTrace();
                }
                
                long b = System.currentTimeMillis();
                System.out.println(m.getName() + ":" + String.valueOf(b - a));
            }
        }
    }

    public void f1_N(int n) 
    {
        int sum = 0;
        for (int i = 0; i < n; i++)
            sum++;
    }

    public void f2_N2(int n) 
    {
        int sum = 0;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                sum++;
    }

    public void f3_N3(int n) 
    {
        int sum = 0;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n * n; j++)
                sum++;
    }

    public void f4_N2(int n) 
    {
        int sum = 0;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < i; j++)
                sum++;
    }

    public void f5_N4(int n) 
    {
        int sum = 0;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < i * i; j++)
                for (int k = 0; k < j; k++)
                    sum++;
    }

    public void f6_N3(int n) 
    {
        int sum = 0;
        for (int i = 0; i < n; i++)
            for (int j = 1; j < i * i; j++)
                if (j % i == 0)
                    for (int k = 0; k < j; k++)
                        sum++;
    }
}
