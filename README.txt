Description:
------------

A module that displays a block with the 5 most similar nodes to the currently viewed one, based on the title and body fields.

This module uses MySQL?s FULLTEXT indexing, and requires you to add a fulltext index on the node table, like this:
ALTER TABLE node ADD FULLTEXT(title, body);