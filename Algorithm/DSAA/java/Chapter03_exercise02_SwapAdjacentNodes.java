
public class Chapter03_exercise02_SwapAdjacentNodes
{
	public static void main(String[] args)
	{
		{
            SinglyLinkNode head = new SinglyLinkNode("a");
            head.addNode("b");
            head.addNode("c");
            head.addNode("d");
            head.addNode("e");
            SinglyLinkNode.printLink(head);
            SinglyLinkNode.printLink(swapAdjacentNodes(head));
        }
        {
        	DoublyLinkNode head = new DoublyLinkNode("a");
        	head.addNode("b");
        	head.addNode("c");
        	head.addNode("d");
        	head.addNode("e");
            DoublyLinkNode.printLink(head);
            DoublyLinkNode.printLink(swapAdjacentNodes(head));
        }
	}
	
	public static SinglyLinkNode swapAdjacentNodes(SinglyLinkNode head)
	{
        if (head == null || head.next == null) return head;
        SinglyLinkNode dummy = new SinglyLinkNode("");
        SinglyLinkNode cur = head;
        dummy.next = cur;
        SinglyLinkNode before = dummy, after;
        while (cur != null && cur.next != null)
        {
            after = cur.next.next;
            before.next = cur.next;
            cur.next.next = cur;
            cur.next = after;
            before = cur;
            cur = cur.next;
        }

        return dummy.next;
    }

    public static DoublyLinkNode swapAdjacentNodes(DoublyLinkNode head)
    {
        if (head == null || head.next == null) return head;
        DoublyLinkNode dummy = new DoublyLinkNode("");
        DoublyLinkNode cur = head;
        dummy.next = cur;
        cur.prev = dummy;
        DoublyLinkNode before, first, second, after;
        while (cur != null && cur.next != null)
        {
            after = cur.next.next;
            first = cur;
            second = cur.next;
            before = cur.prev;
            if (before != null)
                before.next = second;
            second.prev = before;
            second.next = first;
            first.prev = second;
            first.next = after;
            if (after != null)
                after.prev = first;
            cur = after;
        }
        dummy.next.prev = null;
        return dummy.next;
    }
	
	
	static class SinglyLinkNode
	{
		String data;
		SinglyLinkNode next;
		
		SinglyLinkNode(String data)
		{
			this.data = data;
			this.next = null;
		}
		
		public void addNode(String data)
		{
			SinglyLinkNode newNode = new SinglyLinkNode(data);
			SinglyLinkNode tail = this;
			while (tail.next != null)
			{
				tail = tail.next;
			}
			tail.next = newNode;
		}
		
		@Override
		public String toString()
		{
			return this.data;
		}
		
		public static void printLink(SinglyLinkNode head)
		{
			while (head != null)
			{
				if (head.next != null)
	                System.out.print(head + "->");
	            else
	                System.out.print(head);
	            head = head.next;
			}
			System.out.print('\n');
		}
	}
	
	static class DoublyLinkNode
	{
		String data;
		DoublyLinkNode next, prev;
		
		DoublyLinkNode(String data)
		{
			this.data = data;
			this.next = null;
			this.prev = null;
		}
		
		@Override
		public String toString()
		{
			return this.data;
		}
		
		public static void printLink(DoublyLinkNode head)
		{
	        while (head != null)
	        {
	            if (head.prev != null)
	                System.out.print("<-");
	            System.out.print(head);
	            if (head.next != null)
	                System.out.print("->");
	            head = head.next;
	        }
	        System.out.print('\n');
	    }
		
		public void addNode(String data)
		{
			DoublyLinkNode newNode = new DoublyLinkNode(data);
			DoublyLinkNode tail = this;
			while (tail.next != null)
			{
				tail = tail.next;
			}
			tail.next = newNode;
			newNode.prev = tail;
		}
	}
}
