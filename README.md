Malloc
=======



### Developers

* Megan Murray
* Mush Kapadia

Important Things to Note
------------------------
* We did not use a static array.  Insead, malloc actually grabs real memory from the computer.
* We avoid Fragmentation by grouping nearby free blocks together (See Below).


Implementation
---------------
Our imlementation uses a doubly linked list, which consists of nodes with a next and prev pointer, the size of the contents, and a flag free to determine if this space is free. As space is malloc'd, the linked list contains nodes for all of the memory that is malloc'd and if it is currently free or not (able to be overwritten or not).  

When free is called, we search through the list and mark the node/block as free.  Free also checks if the previous or next nodes are free, so we can then merge these blocks together to keep larger chunks of memory and avoid fragmentation. We took the tradeoff of storing the prev pointer in the header to avoid searching through the linked list each time a free is called for the merge.

###Effciency 

Malloc runs in O(n) time.  There are no nested while loops.

### Error Checking

* free()ing pointers that were never allocated.
The free() method begins by making sure the pointer is valid, meaning it is within our linked list.  If the pointer is not found in the linked list, it was never malloc'd, and we error out.

* free()ing pointers to dynamic memory that were not returned from malloc().
The free() method begins by making sure the pointer is valid, meaning it is within our linked list.  If the pointer is not found in the linked list, it was never malloc'd, and we error out.

* redundant free()ing of the same pointer.
The free() method begins by making sure the pointer is valid and within our linked list.  This loop also contains "&& temp->free == 0", meaning we simultaneously check if the pointer we are looking for is already free.  If so, we error out.

* Saturation
Dueu to dynamically grabbing memory, saturation is a very rare if not non-existent problem.  However, get_memory will return null on a bad memory grab (saturation, no memory left) and the program will catch this and error out, saying "memory exhausted."

* Fragmentation
When memory is freed, we check the two adjacent blocks (next and prev). If either is free, it can be combined with hte current blovk being freed.  The MergeBlock method adds two blocks of memory together.






