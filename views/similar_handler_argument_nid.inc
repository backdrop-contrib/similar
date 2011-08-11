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
    $options['include_fields'] = array('default' => 0);
    $options['minimum_similarity'] = array('default' => '0');
    return $options;
  }

  /**
   * Defines the options form.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['not']);

    $form['minimum_similarity'] = array(
      '#type' => 'select',
      '#title' => t('Minimum similarity score'),
      '#options' => array(
        '0' => '0',
        '1' => '1',
        '2' => '2',
        '3' => '3',
        '4' => '4',
        '5' => '5',
      ),
      '#default_value' => $this->options['minimum_similarity'],
      '#description' => t('Choosing a higher number will further limit the amount of results and provide only results with a higher similarity rating.'),
    );
    if (module_exists('field')) {
      $form['include_fields'] = array(
        '#type' => 'checkbox',
        '#title' => t('Include content fields in matching'),
        '#default_value' => !empty($this->options['include_fields']) ? $this->options['include_fields'] : 0,
        '#description' => t('Enable FULLTEXT queries on fields defined with Contend Contruction Kit (CCK).'),
      );
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
   * Builds the query.
   */
  public function query() {

    // Since the view could have multiple nid arguments, load each node
    // and populate the $text variable with node titles and bodies.
    $text = '';
    foreach ($this->value as $nid) {
      $node = node_load($nid);
      if (isset($node->title) && isset($node->body)) {
        $text .= "$node->title {$node->body[$node->language][0]['value']} ";
      }
    }
    $text = trim(addslashes(strip_tags($text)));

    $this->ensure_my_table();
    $nids = $this->nids;

    $selects = array();
    if (db_table_exists('field_data_body')) {
      $body_table = $this->query->add_table('field_data_body');
      $selects[] = "MATCH ($body_table.body_value) AGAINST ('$text')";
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
            $selects[] = "MATCH ($alias." . implode(", $alias.", $indexed) . ") AGAINST ('$text')";
          }
          elseif (count($indexed) == 1) {
            $field = array_pop($indexed);
            $selects[] = "MATCH ($alias.$field) AGAINST ('$text')";
          }
        }
      }
    }

    // An empty string is passed for the table to bypass the table alias.
    $query = count($selects) > 1 ? implode(" OR ", $selects) : array_pop($selects);
    $this->query->add_field('', $query, 'score');

    // Exclude the current node(s).
    if (count($this->value) > 1) {
      $placeholders = implode(', ', array_fill(0, count($this->value), '%d'));
      $this->query->add_where(0, 'node.nid', $this->value, 'NOT IN');
    }
    else {
      $this->query->add_where(0, 'node.nid', $this->value[0], '<>');
    }

    $this->query->add_having('nid', 'score', $this->options['minimum_similarity'], '>');
  }

}
