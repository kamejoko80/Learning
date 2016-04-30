
public class Chapter03_exercise11_LinkedNode
{
	// relphite's solution
	class LinkedNode<E> {
	    E data;
	    LinkedNode<E> next;
	}

	public class MyLinkedList<E> 
	{
	    private LinkedNode<E> head;

	    public int size() 
	    {
	        LinkedNode<E> cur = head;
	        int size = 0;
	        while (cur != null)
	            size++;
	        return size;
	    }

	    public void print() 
	    {
	        LinkedNode<E> cur = head;
	        while (cur != null) 
	        {
	            System.out.print(cur.data);
	            if (cur.next != null)
	                System.out.print("->");
	        }
	    }

	    public boolean contains(E x)
	    {
	        LinkedNode<E> cur = head;
	        if (x == null)
	        {
	            while (cur != null)
	            {
	                if (cur.data == null)
	                    return true;
	                cur = cur.next;
	            }
	        } 
	        else
	        {
	            while (cur != null) 
	            {
	                if (x.equals(cur.data))
	                {
	                    return true;
	                }
	                cur = cur.next;
	            }
	        }
	        return false;
	    }

	    public boolean add(E x)
	    {
	        LinkedNode<E> dummy = new LinkedNode<>();
	        dummy.next = head;
	        boolean found = false;
	        if (x == null)
	        {
	            while (dummy.next != null)
	            {
	                if (dummy.next.data == null) 
	                {
	                    found = true;
	                    break;
	                }
	                dummy = dummy.next;
	            }
	        } 
	        else
	        {
	            while (dummy.next != null)
	            {
	                if (x.equals(dummy.next.data)) 
	                {
	                    found = true;
	                    break;
	                }
	                dummy = dummy.next;
	            }
	        }
	        if (found)
	            return false;
	        else 
	        {
	            dummy.next = new LinkedNode<>();
	            dummy.next.data = x;
	            return true;
	        }
	    }

	    /**
	     * remove element x from list. if found and removed return true.
	     * else return false
	     *
	     * @param x element to remove
	     * @return if found return true. else return false
	     */
	    public boolean remove(E x) {
	        LinkedNode<E> dummy = new LinkedNode<>();
	        dummy.next = head;
	        if (x == null) {
	            while (dummy.next != null) {
	                if (dummy.next.data == null) {
	                    //remove
	                    dummy.next = dummy.next.next;
	                    return true;
	                }
	                dummy = dummy.next;
	            }
	        } else {
	            while (dummy.next != null) {
	                if (x.equals(dummy.next.data)) {
	                    dummy.next = dummy.next.next;
	                    return true;
	                }
	                dummy = dummy.next;
	            }
	        }
	        return false;
	    }
	}
}