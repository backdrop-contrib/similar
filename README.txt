DESCRIPTION:
------------

A module that displays a block with similar published nodes to the
currently viewed one, based on the title and body fields.

This module uses MySQL's FULLTEXT indexing for MyISAM tables, and
requires you to add a fulltext index on the node table, like this:

ALTER TABLE node_revisions ADD FULLTEXT(title, body);

Configure the number of similar entries you want the block to
search for at (default is 5): admin/block/configure/similar/0

UPGRADING FROM OLDER THAN DRUPAL 4.7
------------------------------------

Execute the above fulltext index query. The Drupal update script
takes care of the old index for you (by deleting the old columns).

CONTRIBUTORS
------------
Arnab Nandi http://arnab.org/

David Kent Norman <deekayen at: deekayen {dot} net>
http://deekayen.net/