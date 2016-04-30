import java.util.Stack;


public class Chapter03_exercise22_PostfixExpressionEvaluator
{
	public static void main(String[] args)
	{
		System.out.println(evaluate("6 5 2 3 + 8 * + 3 + *".split(" ")));
	}
	
	public static double evaluate(String[] tokens)
	{
        Stack<String> stack = new Stack<String>();
        for (String s : tokens)
        {
            if (Character.isDigit(s.charAt(0)))
                stack.push(s);
            else
            {
                try
                {
                    String a, b;
                    switch (s)
                    {
                        case "+":
                            b = stack.pop();
                            a = stack.pop();
                            stack.push(String.valueOf(Double.valueOf(a) + Double.valueOf(b)));
                            break;
                        case "-":
                            b = stack.pop();
                            a = stack.pop();
                            stack.push(String.valueOf(Double.valueOf(a) - Double.valueOf(b)));
                            break;
                        case "*":
                            b = stack.pop();
                            a = stack.pop();
                            stack.push(String.valueOf(Double.valueOf(a) * Double.valueOf(b)));
                            break;
                        case "/":
                            b = stack.pop();
                            a = stack.pop();
                            stack.push(String.valueOf(Double.valueOf(a) / Double.valueOf(b)));
                            break;
                    }
                }
                catch (Exception e)
                {
                    e.printStackTrace();
                }
            }
        }
        if (stack.size() != 1)
            throw new IllegalArgumentException();

        return Double.valueOf(stack.pop());
    }
}