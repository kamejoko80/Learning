import java.util.Stack;


public class Chapter03_exercise21_PairBalancing
{
	public static void main(String[] args)
	{
		System.out.println(checkBalancing("begin this ( /* ( { hello ) world! } */ ) really end".split(" ")));
		System.out.println(checkBalancing("begin this ( /* ( { hello world! } */ ) really end".split(" ")));
		System.out.println(checkBalancing("begin this ( /* { hello world! } */ ) really end".split(" ")));
	}
	
	/**
     * check if keywords are paired up in the tokens list. if yes, return -1; else return the location
     * where there is an error
     *
     * @param tokens list of tokens
     * @param pairs  list of keyword paris
     * @return -1 if all keywords are paired up. else the location where there is an error
     */
	public static int checkBalancing(String[] tokens)
	{
		class StrLoc
		{
			String str;
            int loc;

            StrLoc(String s, int l)
            {
                str = s;
                loc = l;
            }
		}
		
		Stack<StrLoc> stack = new Stack<StrLoc>();
		for (int i = 0; i < tokens.length; i++)
		{
            String s = tokens[i];
            switch (s)
            {
            	case "/*":
            		stack.add(new StrLoc("/*", i));
            		break;
            	case "(":
            		stack.add(new StrLoc("(", i));
            		break;
            	case "{":
            		stack.add(new StrLoc("{", i));
            		break;
            	case "[":
            		stack.add(new StrLoc("[", i));
            		break;
            	case "*/":
            		if (stack.peek().str.equals("/*"))
            		{
            			stack.pop();
            		}
            		else 
            		{
            			return i;
            		}
            		break;
            	case ")":
            		if (stack.peek().str.equals("("))
            		{
            			stack.pop();
            		}
            		else
            		{
            			return i;
            		}
            		break;
            	case "]":
            		if (stack.peek().str.equals("["))
            		{
            			stack.pop();
            		}
            		else
            		{
            			return i;
            		}
            		break;
            	case "}":
            		if (stack.peek().str.equals("{"))
            		{
            			stack.pop();
            		}
            		else
            		{
            			return i;
            		}
            		break;
            }
		}
		return -1;
	}
}