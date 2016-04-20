import java.util.Collection;
import java.util.ConcurrentModificationException;
import java.util.Iterator;
import java.util.NoSuchElementException;

public class Chapter03_exercise09_MyArrayListAddAll<E> implements Iterable<E> {
    private Object[] elements;
    private int size;
    private int capacity;
    private static final int DEFAULT_CAPACITY = 10;
    private int modCount;

    // written by relphite
    public Chapter03_exercise09_MyArrayListAddAll() {
        size = 0;
        modCount = 0;
        capacity = DEFAULT_CAPACITY;
        elements = new Object[capacity];
    }

    public void add(E e) {
        if (size == capacity) {
            ensureCapacity(capacity + capacity >>> 1);
        }
        elements[size++] = e;
        modCount++;
    }

    public void addAll(Collection<E> items) {
        ensureCapacity(capacity + items.size());
        for (E e : items)
            add(e);
    }
    
    public void addAll(Iterable<? extends E> items) {
        Iterator<? extends E> iterator = items.iterator();
        while (iterator().hasNext()){
            add((E)iterator.next());
        }
    }

    private void ensureCapacity(int i) {
        if (i <= capacity) return;
        Object[] temp = new Object[i];
        System.arraycopy(elements, 0, temp, 0, size);
        elements = temp;
        capacity = i;
    }

    @SuppressWarnings("unchecked")
    public E get(int index) {
        if (index < 0 || index >= size)
            throw new ArrayIndexOutOfBoundsException();
        return (E) elements[index];
    }

    public void removeAt(int index) {
        if (index < 0 || index >= size)
            throw new ArrayIndexOutOfBoundsException();
        System.arraycopy(elements, index + 1, elements, index, size - index - 1);
        size--;
        modCount++;
    }

    public int size() {
        return size;
    }

    public boolean isEmpty() {
        return size == 0;
    }

    @Override
    public Iterator<E> iterator() {
        return new MyArrayListIterator();
    }

    private class MyArrayListIterator implements Iterator<E> {
        private int currentIndex = 0;
        private int expectedModCount = modCount;
        private boolean canRemove = false;

        @Override
        public boolean hasNext() {
            return currentIndex < size;
        }

        @SuppressWarnings("unchecked")
        @Override
        public E next() {
            if (!hasNext())
                throw new NoSuchElementException();
            if (expectedModCount != modCount)
                throw new ConcurrentModificationException();
            canRemove = true;
            return (E) elements[currentIndex++];
        }

        @Override
        public void remove() {
            if (expectedModCount != modCount)
                throw new ConcurrentModificationException();
            if (!canRemove)
                throw new IllegalStateException();
            removeAt(currentIndex - 1);
            expectedModCount++;
            canRemove = false;
        }
    }
}