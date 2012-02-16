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
  $view->name = 'Similar Entries';
  $view->description = t('Provides links to similar content based on relevancy scores using in FULLTEXT searches.');
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Similar Entries';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->is_cacheable = '1';

  // Master display.
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Similar Entries';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'time';
  $handler->display->display_options['cache']['results_lifespan'] = 21600;
  $handler->display->display_options['cache']['output_lifespan'] = 21600;
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = 8;

  // Sets the Similar entries sort handler.
  $handler->display->display_options['sorts']['similarity']['id'] = 'similarity';
  $handler->display->display_options['sorts']['similarity']['table'] = 'similar_entries';
  $handler->display->display_options['sorts']['similarity']['field'] = 'similarity';
  $handler->display->display_options['sorts']['similarity']['order'] = 'DESC';

  // Sets the Similar entries node ID argument.
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'similar_entries';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['nid']['summary']['sort_order'] = 'desc';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['override'] = TRUE;
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = 8;

  // Enables additional validation of the node ID argument.
  $handler->override_option('arguments', array(
    'nid' => array(
      'id' => 'nid',
      'table' => 'similar_entries',
      'field' => 'nid',
      'title' => '',
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => '',
      'wildcard_substitution' => '',
      'breadcrumb' => '',
      'break_phrase' => 0,
      'not' => 0,
      'override' => array('button' => 'Override'),
      'relationship' => 'none',
      'default_argument_type' => 'node',
      'default_argument' => '',
      'default_options_div_prefix' => '',
      'default_argument_fixed' => '',
      'default_argument_user' => 0,
      'default_argument_php' => '',
      'validate_type' => 'node',
      'validate_fail' => 'not found',
      'validate_user_argument_type' => 'nid',
      'validate_user_roles' => array(
        '2' => 0,
        '3' => 0,
        '4' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_node_type' => array(
        'blog' => 0,
        'product' => 0,
        'presenter' => 0,
        'playlist_video' => 0,
        'standalone_video' => 0,
        'page' => 0,
        'article' => 0,
      ),
      'validate_argument_vocabulary' => array(
        '1' => 0,
        '2' => 0,
        '3' => 0,
        '4' => 0,
      ),
      'validate_argument_type' => 'nid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_node_flag_name' => '*relationship',
      'validate_argument_node_flag_test' => 'flaggable',
      'validate_argument_node_flag_id_type' => 'id',
      'validate_argument_user_flag_name' => '*relationship',
      'validate_argument_user_flag_test' => 'flaggable',
      'validate_argument_user_flag_id_type' => 'id',
      'validate_argument_php' => '',
    ),
  ));

  // Sets a node title field that links to the node.
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;

  // Provides node->moderated and node->published filters.  
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  // Filter by similarity score.
  $handler->display->display_options['filters']['similarity']['id'] = 'similarity';
  $handler->display->display_options['filters']['similarity']['table'] = 'similar_entries';
  $handler->display->display_options['filters']['similarity']['field'] = 'similarity';
  $handler->display->display_options['filters']['similarity']['value'] = '1';
  $handler->display->display_options['filters']['similarity']['operator'] = '>';
  $handler->display->display_options['filters']['similarity']['group'] = 0;
  $handler->display->display_options['filters']['similarity']['expose']['operator'] = FALSE;

  // Add a block display.
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['block_caching'] = 4;

  return array($view->name => $view);
}
