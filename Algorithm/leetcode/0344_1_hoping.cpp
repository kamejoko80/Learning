class Solution
{
public:
    string reverseString(string s)
    {
        char tmp;
        int length = s.length();
        for (int i = 0; i < length / 2; i++)
        {
            tmp = s[i];
            s[i] = s[length - 1 - i];
            s[length - 1 - i] = tmp;
        }
        return s;
    }
};
