class Solution
{
public:
    string reverseString(string s)
    {
        for (basic_string<char>::iterator left = s.begin(), right = s.end() - 1; left <= right; ++left, --right)
        {
            swap(*left, *right);
        }
        return s;
    }
};
