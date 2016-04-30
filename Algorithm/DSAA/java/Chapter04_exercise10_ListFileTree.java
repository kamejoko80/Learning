
import java.io.File;

public class Chapter04_exercise10_ListFileTree
{
	private static final String rootFolder = ".";
	
	public static void main(String[] args)
	{
		listFileTree(0, new File(rootFolder));
	}
	
	public static void listFileTree(int indentation, File folder)
	{
		assert folder != null;
		for (File f : folder.listFiles())
		{
			if (f.isDirectory())
			{
				System.out.println(f.getPath());
				listFileTree(indentation + 1, f);
			}
			else
			{
                for (int i = 0; i < indentation; i++)
                {
                    System.out.print(" |--");
                }
                System.out.println(f.getName() + " : " + f.length());
            }
		}
	}
}