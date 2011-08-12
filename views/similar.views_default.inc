<?php

/**
 * @file
 * Provides the default view for Similar entries module.
 */

/**
 * Implements hook_views_default_views().
 */
function similar_views_default_views() {
  $view = new view;
  $view->name = 'similar_entries';
  $view->description = t('Provides links to similar content based on FULLTEXT searches by Similar entries module.');
  $view->tag = t('default');
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = '1';
  $view->api_version = 2;
  $view->disabled = FALSE;
  $handler = $view->new_display('default', 'Default', 'default');

  // Sets a node title field that links to the node.
  $handler->override_option('fields', array(
    'title' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_lenth' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
      'override' => array('button' => 'Override'),
    ),
  ));

  // Sets the Similar entries sort handler.
  $handler->override_option('sorts', array(
    'similarity' => array(
      'order' => 'DESC',
      'id' => 'similarity',
      'table' => 'similar_entries',
      'field' => 'similarity',
      'relationship' => 'none',
      'override' => array('button' => 'Override'),
    ),
  ));

  // Sets the Similar entries node ID argument.
  $handler->override_option('arguments', array(
    'nid' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => '',
      'wildcard_substitution' => '',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'node',
      'default_argument' => '',
      'validate_type' => 'node',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'nid',
      'table' => 'similar_entries',
      'field' => 'nid',
      'validate_user_argument_type' => 'nid',
      'validate_user_roles' => array(
        '2' => 0,
        '3' => 0,
        '4' => 0,
      ),
      'override' => array('button' => 'Override'),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_fixed' => '',
      'default_argument_user' => 0,
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'blog' => 0,
        'product' => 0,
        'presenter' => 0,
        'playlist_video' => 0,
        'standalone_video' => 0,
        'page' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '1' => 0,
        '2' => 0,
        '3' => 0,
        '4' => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_node_flag_name' => '*relationship',
      'validate_argument_node_flag_test' => 'flaggable',
      'validate_argument_node_flag_id_tyoe' => 'id',
      'validate_argument_user_flag_name' => '*relationship',
      'validate_argument_user_flag_test' => 'flaggable',
      'validate_argument_user_flag_id_type' => 'id',
      'validate_argument_php' => '',
    ),
  ));

  // Provides node->moderated and node->published filters.
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'override' => array('button' => 'Override'),
      'relationship' => 'none',
    ),
    'similarity' => array(
      'operator' => '>',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => TRUE,
        'label' => '',
      ),
      'id' => 'similarity',
      'table' => 'similar_entries',
      'field' => 'similarity',
      'relationship' => 'none',
    ),
  ));

  // Set a few final items.
  $handler->override_option('access', array('type' => 'none'));
  $handler->override_option('cache', array('type' => 'none'));
  $handler->override_option('title', 'Similar entries');
  $handler->override_option('items_per_page', 8);

  // Add the similar entries block.
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->override_option('block_description', '');
  $handler->override_option('block_caching', '4');

  return array($view->name => $view);
}
