
public class Chapter02_exercise27_PartiallySortedMatrixSearch
{
	// written by relphite 
	public static void main(String... args)
	{
        int[][] matrix = new int[][]{
                {1, 2, 3, 4, 5},
                {2, 3, 4, 5, 6},
                {3, 5, 6, 8, 9},
                {6, 7, 8, 10, 15},
                {100, 200, 201, 999, 63452}
        };
        for (int n : new int[]{-1, 1, 2, 100, 202, 99, 10, 999, 88888, 63452})
            System.out.println(n + " : " + new Chapter02_exercise27_PartiallySortedMatrixSearch().checkExists(matrix, n));
    }

    /**
     * find if an int exists in a partially sorted 2-d int array
     * <p/>
     * T(N)=3T(N/2)+O(1) => O(N)
     *
     * @param matrix sorted every row and every column
     * @param target int to search
     * @return exists or not
     */
    public boolean checkExists(int[][] matrix, int target)
    {
        return !(matrix.length == 0 || matrix[0].length == 0)
                && checkExists0(matrix, 0, 0, matrix.length - 1, matrix[0].length - 1, target);
    }

    private boolean checkExists0(int[][] matrix, int x1, int y1, int x2, int y2, int target)
    {
        if (x1 > x2 || y1 > y2) return false;
        if (x1 == x2 && y1 == y2) return matrix[x1][y1] == target;
        if (x2 - x1 == 1 && y2 - y1 == 1)
            return matrix[x1][y1] == target || matrix[x2][y1] == target
                    || matrix[x1][y2] == target || matrix[x2][y2] == target;
        int midX = (x1 + x2) / 2;
        int midY = (y1 + y2) / 2;
        int midValue = matrix[midX][midY];
        if (midValue == target) return true;
        else if (midValue > target)
        {
            return checkExists0(matrix, x1, y1, midX, midY, target)
                    || checkExists0(matrix, x1, midY, midX, y2, target)
                    || checkExists0(matrix, midX, y1, x2, midY, target);
        }
        else
        {
            return checkExists0(matrix, midX, midY, x2, y2, target)
                    || checkExists0(matrix, x1, midY, midX, y2, target)
                    || checkExists0(matrix, midX, y1, x2, midY, target);
        }
    }
}