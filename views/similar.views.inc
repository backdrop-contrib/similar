<?php

/**
 * @file
 * View hook implementations for Similar entries module.
 */

/**
 * Implements hook_views_data().
 *
 * Since fields are now a part of core in Drupal 7, we don't have
 * to create explicit relationships between node tables and field
 * tables as Views already knows about those relationships.
 *
 * @TODO: Consider supporting other types of entities?
 */
function similar_views_data() {
  // We namespace the data keys to prevent conflicts with 'similar'.
  $data['similar_entries']['table']['group'] = t('Similar entries');
  $data['similar_entries']['table']['join'] = array('#global' => array());

  // Adds the similarity argument handler.
  $data['similar_entries']['similar_entries'] = array(
    'title' => t('Similarity'),
    'help' => t('The rating given to the related content given as an argument.'),
    'field' => array(
      'handler' => 'similar_handler_field_similar',
      'click sortable' => TRUE,
    ),
    'sort' => array('handler' => 'similar_handler_sort_similar'),
  );
  $data['similar_entries']['nid'] = array(
    'title' => t('Nid'),
    'help' => t('ID of content item(s). Passes content IDs to Similar entries.'),
    'argument' => array(
      'handler' => 'similar_handler_argument_nid',
      'parent' => 'views_handler_argument_numeric',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function similar_views_handlers() {
  return array(
    'info' => array('path' => drupal_get_path('module', 'similar') . '/views'),
    'handlers' => array(
      'similar_handler_field_similar' => array(
        'parent' => 'views_handler_field',
      ),
      'similar_handler_sort_similar' => array(
        'parent' => 'views_handler_sort',
      ),
      'similar_handler_argument_nid' => array(
        'parent' => 'views_handler_argument_numeric',
      ),
    ),
  );
}
