


public class Chapter03_exercise12_SortedLinkedNode
{
	// relphite's solution
	public class MySortedLinkedList<E extends Comparable<? super E>> {
	    private LinkedNode head;

	    public int size() {
	        LinkedNode cur = head;
	        int size = 0;
	        while (cur != null) {
	            cur = cur.next;
	            size++;
	        }
	        return size;
	    }

	    public void print() {
	        LinkedNode cur = head;
	        while (cur != null) {
	            System.out.print(cur.data);
	            if (cur.next != null)
	                System.out.print("->");
	        }
	    }

	    public boolean contains(E x) {
	        if (x == null) return false;
	        LinkedNode cur = head;
	        while (cur != null) {
	            if (cur.data.compareTo(x) == 0) return true;
	            if (cur.data.compareTo(x) > 0) return false;
	            cur = cur.next;
	        }
	        return false;
	    }

	    public boolean add(E x) {
	        if (x == null) return false; // null is not allowed
	        if (head == null) {
	            head = new LinkedNode();
	            head.data = x;
	            return true;
	        }
	        LinkedNode dummy = new LinkedNode();
	        dummy.next = head;
	        while (dummy.next != null) {
	            if (dummy.next.data.equals(x)) return false;
	            if (dummy.next.data.compareTo(x) > 0) {
	                //add after dummy
	                LinkedNode t = dummy.next;
	                LinkedNode n = new LinkedNode();
	                n.data = x;
	                dummy.next = n;
	                n.next = t;
	                if (t == head) {
	                    head = n;
	                }
	                return true;
	            }
	            dummy = dummy.next;
	        }
	        return false;
	    }

	    public boolean remove(E x) {
	        if (x == null) return false;
	        LinkedNode dummy = new LinkedNode();
	        dummy.next = head;
	        while (dummy.next != null) {
	            if (dummy.next.data.compareTo(x) < 0)
	                dummy = dummy.next;
	            else if (dummy.next.data.compareTo(x) == 0) {
	                LinkedNode t = dummy.next;
	                dummy.next = dummy.next.next;
	                if (t == head)
	                    head = dummy.next;
	                return true;
	            } else {
	                assert dummy.next.data.compareTo(x) > 0;
	                return false;
	            }
	        }
	        return false;
	    }

	    class LinkedNode {
	        E data;
	        LinkedNode next;
	    }
	}

}