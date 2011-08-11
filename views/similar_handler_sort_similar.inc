<?php

/**
 * @file
 * Defines the Similar entries sort handler.
 */

/**
 * Defines the similar entries sort handler.
 */
class similar_handler_sort_similar extends views_handler_sort {

  /**
   * Defines default options.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['order'] = array('default' => 'DESC');
    return $options;
  }

  /**
   * Defines query elements.
   */
  public function query() {
    $this->query->add_field(NULL, 'COUNT(node.nid)', 'node_count', array('aggregate' => TRUE));
    $this->query->add_orderby(NULL, NULL, $this->options['order'], 'score');
  }

}
