Malloc
=======



Developers
------------
* Megan Murray
* Mush Kapadia

Important Things to Note
------------------------
* We did not use a static array.  Insead, malloc actually grabs real memory from the computer.
* We avoid Fragmentation by grouping nearby free blocks together.


Implementation
---------------
Our imlementation uses a doubly linked list, which consists of nodes with a next and prev pointer, the size of the contents, and a flag free to determine if this space is free. As space is malloc'd, the linked list contains nodes for all of the memory that is malloc'd and if it is currently free or not (able to be overwritten or not).  

When free is called, we search through the list and mark the node/block as free.  Free also checks if the previous or next nodes are free, so we can then merge these blocks together to keep larger chunks of memory and avoid fragmentation. We took the tradeoff of storing the prev pointer in the header to avoid searching through the linked list each time a free is called for the merge.

###Effciency 

It's really efficient!