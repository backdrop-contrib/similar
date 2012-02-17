<?php

/**
 * @file
 * Defines the Similar entries node ID argument.
 */

/**
 * Defines the similar entries View node ID argument.
 */
class similar_handler_argument_nid extends views_handler_argument_numeric {

  /**
   * Defines default values for argument settings.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['default_action'] = array('default' => 'default');
    $options['default_argument_type'] = array('default' => 'node');
    $options['validate']['contains']['type'] = array('default' => 'node');
    $options['validate']['contains']['fail'] = array('default' => 'not found');
    $options['increase_title'] = array('default' => 1);
    $options['include_fields'] = array('default' => 0);
    return $options;
  }

  /**
   * Defines the options form.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['not']);
    $form['default_argument_type']['#disabled'] = TRUE;

    $form['increase_title'] = array(
      '#type' => 'checkbox',
      '#title' => t('Increase the weight of node titles'),
      '#default_value' => !empty($this->options['increase_title']),
      '#description' => t('Increase the relevance of words in the title of the node being viewed.'),
    );
    if (module_exists('field')) {
      $form['include_fields'] = array(
        '#type' => 'checkbox',
        '#title' => t('Include content fields in matching'),
        '#default_value' => !empty($this->options['include_fields']) ? $this->options['include_fields'] : 0,
        '#description' => t('Enable FULLTEXT queries on fields defined with Field module.'),
      );
    }
  }

  /**
   * Overrides argument's default actions list to ensure that node ID
   * from URL is provided.
   */
  public function default_actions($which = NULL) {
    $defaults = parent::default_actions($which);
    unset($defaults['ignore'], $defaults['not found'], $defaults['summary'], $defaults['empty']);

    if ($which) {
      if (!empty($defaults[$which])) {
        return $defaults[$which];
      }
    }
    else {
      return $defaults;
    }
  }

  /**
   * Validates that the argument works.
   */
  public function validate_arg($arg) {
    if (!parent::validate_arg($arg)) {
      return FALSE;
    }

    if (!empty($this->options['break_phrase'])) {
      views_break_phrase($this->argument, $this);
    }
    else {
      $this->value = array($this->argument);
    }
    $this->view->nids = $this->value;
    return TRUE;
  }
  
  /**
   * Override the behavior of title(). Get the title of the node.
   */
  function title_query() {
    $titles = array();
  
    $result = db_query("SELECT n.title FROM {node} n WHERE n.nid IN (:nids)", array(':nids' => $this->value));
    foreach ($result as $term) {
      $titles[] = check_plain($term->title);
    }
    return $titles;
  }

  /**
   * Builds the query.
   */
  public function query() {

    // Since the view could have multiple nid arguments, load each node
    // and populate the $text variable with node titles and bodies.
    $text = '';
    foreach ($this->value as $nid) {
      $node = node_load($nid);
      if (isset($node->title) && isset($node->body) && isset($node->body[$node->language])) {
        // Remove punctuation from the title.
        $title = preg_replace('/[^a-z0-9 _-]+/i', '', $node->title);

        // Increase the relevancy of words in the node title if option is selected.
        if (!empty($this->options['increase_title'])) {
          $text .= ">". implode(" >", explode(' ', $title));
        }
        else {
          $text .= $title;
        }
        $text .= " {$node->body[$node->language][0]['value']} ";
      }
    }
    $text = trim(addslashes(strip_tags($text)));

    $this->ensure_my_table();

    $selects = array();
    if (db_table_exists('field_data_body')) {
      $body_table = $this->query->add_table('field_data_body');
      $selects[] = "MATCH ($body_table.body_value) AGAINST ('$text' IN BOOLEAN MODE)";
    }

    // @TODO: Currently the fields query runs successfully, but because
    // there are multiple FULLTEXT searches occuring on different indexes,
    // I haven't been able to get an accurate score from the results. When
    // multiple FULLTEXT searches are executed it switches to boolean mode.
    // @TODO: Consider supporting other types of entities?

    // Add more MATCH select statements for extra fields if enabled.
    if (module_exists('field') && $this->options['include_fields'] == 1) {
      foreach (similar_get_indices() as $table => $indexed) {
        if (!empty($indexed) && db_table_exists($table)) {
          $alias = $this->query->add_table($table);
          if (count($indexed) > 1) {
            $selects[] = "MATCH ($alias." . implode(", $alias.", $indexed) . ") AGAINST ('$text' IN BOOLEAN MODE)";
          }
          elseif (count($indexed) == 1) {
            $field = array_pop($indexed);
            $selects[] = "MATCH ($alias.$field) AGAINST ('$text' IN BOOLEAN MODE)";
          }
        }
      }
    }

    // An empty string is passed for the table to bypass the table alias.
    $query = count($selects) > 1 ? implode(" OR ", $selects) : array_pop($selects);
    $this->query->add_field('', $query, 'score');

    // Exclude the current node(s).
    if (count($this->value) > 1) {
      $this->query->add_where(0, 'node.nid', $this->value, 'NOT IN');
    }
    else {
      $this->query->add_where(0, 'node.nid', $this->value[0], '<>');
    }

    // Add a node_access tag to prevent conflict with node_access module.
    $this->query->add_tag('node_access');

    // Add a Similar Entries tag to indicate that filters and sort handlers are valid.
    $this->query->add_tag('similar_entries');
  }

}
